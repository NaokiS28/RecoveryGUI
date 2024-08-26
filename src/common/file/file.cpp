/*
 * BemaniUX - Copyright (C) 2022-2024 spicyjpeg, NaokiS
 *
 * BemaniUX is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * BemaniUX is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * BemaniUX. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <shlwapi.h>
#include "common/file/file.hpp"
#include "common/util/hash.hpp"
#include "common/util/templates.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "common/util/stb_image.h"

namespace file
{

	/* File fragment table */

	uint64_t FileFragment::getLBA(uint64_t sector, size_t tableLength) const
	{
		auto fragment = this;

		for (; tableLength; tableLength--, fragment++)
		{
			if (sector < fragment->length)
				return fragment->lba + sector;

			sector -= fragment->length;
		}

		return 0;
	}

	/* Base file and directory classes */

	File::~File(void)
	{
		close();
	}

	Directory::~Directory(void)
	{
		close();
	}

	/* Base file and asset provider classes */

	Provider::~Provider(void)
	{
		close();
	}

	size_t Provider::loadData(util::Data &output, const char *path)
	{
		auto _file = openFile(path, READ);

		if (!_file)
			return 0;

		// assert(_file->size <= SIZE_MAX);
		if (!output.allocate(size_t(_file->size)))
		{
			_file->close();
			delete _file;
			return 0;
		}

		size_t actualLength = _file->read(output.ptr, output.length);

		_file->close();
		delete _file;
		return actualLength;
	}

	size_t Provider::loadData(void *output, size_t length, const char *path)
	{
		auto _file = openFile(path, READ);

		if (!_file)
			return 0;

		// assert(file->size >= length);
		size_t actualLength = _file->read(output, length);

		_file->close();
		delete _file;
		return actualLength;
	}

	size_t Provider::saveData(const void *input, size_t length, const char *path)
	{
		auto _file = openFile(path, WRITE | ALLOW_CREATE);

		if (!_file)
			return 0;

		size_t actualLength = _file->write(input, length);

		_file->close();
		delete _file;
		return actualLength;
	}

	// Load an image from file, using file extension as selector
	size_t Provider::loadImage(gpu::Image &output, const char *path)
	{
		// Fetch and copy the path string to find extension handler
		unsigned long pathLen = strlen(path);
		char *pathStr = new char(pathLen);
		strcpy(pathStr, path);

		// Copy filename to the image struct
		char *nameToken = strtok(pathStr, "/");
		char *fileName;
		while (nameToken != NULL)
		{
			fileName = nameToken;
			nameToken = strtok(NULL, "/");
		}
		output.name = new char[strlen(fileName)];
		strcpy(output.name, fileName);

		strcpy(pathStr, path);
		char *pathToken = strtok(pathStr, ".");
		pathToken = strtok(NULL, "."); // Skip the first part of the file name.

		char cwd[256];
		GetCurrentDirectory(256, cwd);

		while (pathToken != NULL)
		{
			if (strstr(pathToken, "png") != NULL)
			{
				// PNG file
				return loadPNG(output, path);
			}
			else if (strstr(pathToken, "bmp") != NULL)
			{
				// BMP file
				return loadBMP(output, path);
			}
			else if (strstr(pathToken, "tim") != NULL)
			{
				// TIM file
				return loadTIM(output, path);
			}

			pathToken = strtok(NULL, ".");
		}

		delete pathStr;
		return 0;
	}

	size_t Provider::loadBMP(gpu::Image &output, const char *path)
	{
		// Todo: do it
		return 0;
	}

	size_t Provider::loadPNG(gpu::Image &output, const char *path)
	{
		int width, height, channels;
		util::Data data;

		if (!loadData(data, path))
			return 0;

		// Load the PNG image using stb_image
		unsigned char *img = stbi_load_from_memory((unsigned char *)data.ptr, data.length, &width, &height, &channels, 4); // Force 32-bit RGBA

		if (img == nullptr)
		{
			return 0; // Failed to load image
		}

		bool alphaPresent = false;
		for (int i = 0; i < width * height; i++)
		{
			unsigned char *pixel = img + i * 4;
			unsigned char temp = pixel[0];
			pixel[0] = pixel[2]; // Swap red and blue channels for BGR(A) format
			pixel[2] = temp;
			if (pixel[3] < 255)
				alphaPresent = true;
		}
		if (alphaPresent)
			LOG_FS("%s has alpha.", path);

		// Set up the bitmap info
		BITMAPINFO bmi = {};
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = width;
		bmi.bmiHeader.biHeight = -height; // Negative to indicate a top-down DIB
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32; // We force 32-bit to ensure alpha channel is supported
		bmi.bmiHeader.biCompression = BI_RGB;

		// Create a DIB section and copy image data
		HDC hdc = GetDC(NULL);
		void *pBits = nullptr; // Pointer to receive the DIB section memory
		output.imgHdc = CreateCompatibleDC(hdc);
		output.imgBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);

		if (output.imgBitmap && pBits)
		{
			// Copy the pixel data directly into the DIB section
			memcpy(pBits, img, width * height * 4); // Copying RGBA data
			SelectObject(output.imgHdc, output.imgBitmap);
		}

		output.width = width;
		output.height = height;

		// Cleanup
		ReleaseDC(NULL, hdc);
		stbi_image_free(img);

		return bmi.bmiHeader.biSize;
	}

	size_t Provider::loadTIM(gpu::Image &output, const char *path)
	{
		util::Data data;

		if (!loadData(data, path))
			return 0;

		auto header = data.as<const gpu::TIMHeader>();
		auto section = reinterpret_cast<const uint8_t *>(&header[1]);

		if (!output.initFromTIMHeader(header))
		{
			data.destroy();
			return 0;
		}
		if (header->flags & (1 << 3))
		{
			auto clut = reinterpret_cast<const gpu::TIMSectionHeader *>(section);

			// TODO: implement
			// gpu::upload(clut->vram, &clut[1], true);
			section += clut->length;
		}

		// auto image = reinterpret_cast<const gpu::TIMSectionHeader *>(section);

		// gpu::upload(image->vram, &image[1], true);

		data.destroy();
		return data.length;
	}

	/*size_t Provider::loadKeymap(io::KeyboardHandler &output, const char *path){
		util::Data data;

		if (!loadData(data, path))
			return 0;

		// Todo: This

		data.destroy();
		return data.length;
	}*/

	/* String table parser */

	static const char _ERROR_STRING[]{"missingno"};

	const char *StringTable::get(util::Hash id) const
	{
		if (!ptr)
			return _ERROR_STRING;

		auto blob = as<const char>();
		auto table = as<const StringTableEntry>();
		auto index = id % STRING_TABLE_BUCKET_COUNT;

		do
		{
			auto entry = &table[index];
			index = entry->chained;

			if (entry->hash == id)
				return &blob[entry->offset];
		} while (index);

		return _ERROR_STRING;
	}

	size_t StringTable::format(
		char *buffer, size_t length, util::Hash id, ...) const
	{
		va_list ap;

		va_start(ap, id);
		size_t outLength = vsnprintf(buffer, length, get(id), ap);
		va_end(ap);

		return outLength;
	}

}
