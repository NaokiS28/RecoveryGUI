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

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include "common/file/file.hpp"
#include "common/file/misc.hpp"
#include "common/util/hash.hpp"
#include "common/util/log.hpp"
#include "common/util/templates.hpp"

namespace file {

/* Host file and directory classes */

size_t HostFile::read(void *output, size_t length) {
	return fread(output, 1, length, _fd);
}

size_t HostFile::write(const void *input, size_t length) {
	return fwrite(input, 1, length, _fd);
}

uint64_t HostFile::seek(uint64_t offset) {
	fseek(_fd, offset, SEEK_SET);
	return ftell(_fd);
}

uint64_t HostFile::tell(void) const {
	return ftell(_fd);
}

void HostFile::close(void) {
	fclose(_fd);
}

bool HostDirectory::getEntry(FileInfo &output) {
	// TODO: implement
	return false;
}

/* Host filesystem provider */

bool HostProvider::init(void) {
	return true;
}

bool HostProvider::getFileInfo(FileInfo &output, const char *path) {
	// TODO: implement
	return false;
}

Directory *HostProvider::openDirectory(const char *path) {
	// TODO: implement
	return nullptr;
}

bool HostProvider::createDirectory(const char *path) {
	// TODO: implement
	return false;
}

File *HostProvider::openFile(const char *path, uint32_t flags) {
	const char *mode;

	switch (flags) {
		case READ:
			mode = "rb";
			break;

		case WRITE:
		case WRITE | FORCE_CREATE:
			mode = "wb";
			break;

		case WRITE | ALLOW_CREATE:
			mode = "ab";
			break;

		case READ | WRITE | FORCE_CREATE:
			mode = "w+b";
			break;

		case READ | WRITE:
		case READ | WRITE | ALLOW_CREATE:
			mode = "r+b";
			break;

		default:
			return nullptr;
	}

	auto fd = fopen(path, mode);

	if (!fd) {
		LOG_FS("failed to open %s", path);
		return nullptr;
	}

	fseek(fd, 0, SEEK_END);

	auto file  = new HostFile();
	file->_fd  = fd;
	file->size = ftell(fd);

	fseek(fd, 0, SEEK_SET);
	return file;
}

/* Virtual filesystem driver */

VFSMountPoint *VFSProvider::_getMounted(const char *path) {
	auto hash = util::hash(path, VFS_PREFIX_SEPARATOR);

	for (auto &mp : _mountPoints) {
		if (mp.prefix == hash)
			return &mp;
	}

	LOG_FS("unknown device: %s", path);
	return nullptr;
}

bool VFSProvider::mount(const char *prefix, Provider *provider, bool force) {
	auto hash = util::hash(prefix, VFS_PREFIX_SEPARATOR);

	VFSMountPoint *freeMP = nullptr;

	for (auto &mp : _mountPoints) {
		if (!mp.prefix) {
			freeMP = &mp;
		} else if (mp.prefix == hash) {
			if (force) {
				freeMP = &mp;
				break;
			}

			LOG_FS("%s was already mapped", prefix);
			return false;
		}
	}

	if (!freeMP) {
		LOG_FS("no mount points left for %s", prefix);
		return false;
	}

	freeMP->prefix     = hash;
	freeMP->pathOffset =
		(__builtin_strchr(prefix, VFS_PREFIX_SEPARATOR) - prefix) + 1;
	freeMP->provider   = provider;

	LOG_FS("mapped %s", prefix);
	return true;
}

bool VFSProvider::unmount(const char *prefix) {
	auto hash = util::hash(prefix, VFS_PREFIX_SEPARATOR);

	for (auto &mp : _mountPoints) {
		if (mp.prefix != hash)
			continue;

		mp.prefix     = 0;
		mp.pathOffset = 0;
		mp.provider   = nullptr;

		LOG_FS("unmapped %s", prefix);
		return true;
	}

	LOG_FS("%s was not mapped", prefix);
	return false;
}

bool VFSProvider::getFileInfo(FileInfo &output, const char *path) {
	auto mp = _getMounted(path);

	if (!mp)
		return false;

	return mp->provider->getFileInfo(output, &path[mp->pathOffset]);
}

bool VFSProvider::getFileFragments(
	FileFragmentTable &output, const char *path
) {
	auto mp = _getMounted(path);

	if (!mp)
		return false;

	return mp->provider->getFileFragments(output, &path[mp->pathOffset]);
}

Directory *VFSProvider::openDirectory(const char *path) {
	auto mp = _getMounted(path);

	if (!mp)
		return nullptr;

	return mp->provider->openDirectory(&path[mp->pathOffset]);
}

bool VFSProvider::createDirectory(const char *path) {
	auto mp = _getMounted(path);

	if (!mp)
		return false;

	return mp->provider->createDirectory(&path[mp->pathOffset]);
}

File *VFSProvider::openFile(const char *path, uint32_t flags) {
	auto mp = _getMounted(path);

	if (!mp)
		return nullptr;

	return mp->provider->openFile(&path[mp->pathOffset], flags);
}

size_t VFSProvider::loadData(util::Data &output, const char *path) {
	auto mp = _getMounted(path);

	if (!mp)
		return 0;

	return mp->provider->loadData(output, &path[mp->pathOffset]);
}

size_t VFSProvider::loadData(void *output, size_t length, const char *path) {
	auto mp = _getMounted(path);

	if (!mp)
		return 0;

	return mp->provider->loadData(output, length, &path[mp->pathOffset]);
}

size_t VFSProvider::saveData(
	const void *input, size_t length, const char *path
) {
	auto mp = _getMounted(path);

	if (!mp)
		return 0;

	return mp->provider->saveData(input, length, &path[mp->pathOffset]);
}

}
