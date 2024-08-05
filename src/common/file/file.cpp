/*
 * 573in1 - Copyright (C) 2022-2024 spicyjpeg
 *
 * 573in1 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * 573in1 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * 573in1. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "common/file/file.hpp"
#include "common/util/hash.hpp"
#include "common/util/templates.hpp"
#include "common/gpu.hpp"

namespace file {

/* File fragment table */

uint64_t FileFragment::getLBA(uint64_t sector, size_t tableLength) const {
	auto fragment = this;

	for (; tableLength; tableLength--, fragment++) {
		if (sector < fragment->length)
			return fragment->lba + sector;

		sector -= fragment->length;
	}

	return 0;
}

/* Base file and directory classes */

File::~File(void) {
	close();
}

Directory::~Directory(void) {
	close();
}

/* Base file and asset provider classes */

Provider::~Provider(void) {
	close();
}

size_t Provider::loadData(util::Data &output, const char *path) {
	auto _file = openFile(path, READ);

	if (!_file)
		return 0;

	//assert(_file->size <= SIZE_MAX);
	if (!output.allocate(size_t(_file->size))) {
		_file->close();
		delete _file;
		return 0;
	}

	size_t actualLength = _file->read(output.ptr, output.length);

	_file->close();
	delete _file;
	return actualLength;
}

size_t Provider::loadData(void *output, size_t length, const char *path) {
	auto _file = openFile(path, READ);

	if (!_file)
		return 0;

	//assert(file->size >= length);
	size_t actualLength = _file->read(output, length);

	_file->close();
	delete _file;
	return actualLength;
}

size_t Provider::saveData(const void *input, size_t length, const char *path) {
	auto _file = openFile(path, WRITE | ALLOW_CREATE);

	if (!_file)
		return 0;

	size_t actualLength = _file->write(input, length);

	_file->close();
	delete _file;
	return actualLength;
}

size_t Provider::loadTIM(gpu::Image &output, const char *path) {
	util::Data data;

	if (!loadData(data, path))
		return 0;

	auto header  = data.as<const gpu::TIMHeader>();
	auto section = reinterpret_cast<const uint8_t *>(&header[1]);

	if (!output.initFromTIMHeader(header)) {
		data.destroy();
		return 0;
	}
	if (header->flags & (1 << 3)) {
		auto clut = reinterpret_cast<const gpu::TIMSectionHeader *>(section);

		// TODO: implement
		//gpu::upload(clut->vram, &clut[1], true);
		section += clut->length;
	}

	auto image = reinterpret_cast<const gpu::TIMSectionHeader *>(section);

	//gpu::upload(image->vram, &image[1], true);

	data.destroy();
	return data.length;
}

/* String table parser */

static const char _ERROR_STRING[]{ "missingno" };

const char *StringTable::get(util::Hash id) const {
	if (!ptr)
		return _ERROR_STRING;

	auto blob  = as<const char>();
	auto table = as<const StringTableEntry>();
	auto index = id % STRING_TABLE_BUCKET_COUNT;

	do {
		auto entry = &table[index];
		index      = entry->chained;

		if (entry->hash == id)
			return &blob[entry->offset];
	} while (index);

	return _ERROR_STRING;
}

size_t StringTable::format(
	char *buffer, size_t length, util::Hash id, ...
) const {
	va_list ap;

	va_start(ap, id);
	size_t outLength = vsnprintf(buffer, length, get(id), ap);
	va_end(ap);

	return outLength;
}

}
