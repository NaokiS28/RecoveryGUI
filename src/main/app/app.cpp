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
#include <stdio.h>
#include "common/file/file.hpp"
#include "common/file/misc.hpp"
#include "common/file/zip.hpp"
#include "common/util/log.hpp"
#include "common/util/misc.hpp"
#include "common/util/templates.hpp"
#include "common/defs.hpp"
#include "common/gpu.hpp"
#include "main/app/app.hpp"
#include "main/uibase.hpp"

/* Filesystem manager class */

const char *const IDE_MOUNT_POINTS[]{ "ide0:", "ide1:" };

FileIOManager::FileIOManager(void)
: _resourceFile(nullptr) {
	vfs.mount("resource:", &resource);
	vfs.mount("host:",     &host);
}

bool FileIOManager::loadResourceFile(const char *path) {
	closeResourceFile();

	if (path)
		_resourceFile = vfs.openFile(path, file::READ);

	if (_resourceFile) {
		if (resource.init(_resourceFile))
			return true;

		_resourceFile->close();
		delete _resourceFile;
		_resourceFile = nullptr;
	}

	// Fall back to the default in-memory resource archive in case of failure.
	//resource.init(resourcePtr, resourceLength);
	return false;
}

void FileIOManager::closeResourceFile(void) {
	resource.close();

	if (_resourceFile) {
		_resourceFile->close();
		delete _resourceFile;
		_resourceFile = nullptr;
	}
}

/* App class */

static constexpr int _SPLASH_SCREEN_TIMEOUT = 5;

App::App(ui::Context &ctx)
#ifdef ENABLE_LOG_BUFFER
: _logOverlay(_logBuffer),
#else
:
#endif
_ctx(ctx) {}

void App::_loadResources(void) {
	auto &res = _fileIO.resource;

	res.loadStruct(_ctx.colors,       "assets/palette.dat");
	res.loadTIM(_background.tile,     "assets/textures/background.tim");
	res.loadTIM(_ctx.font.image,      "assets/textures/font.tim");
	res.loadData(_ctx.font.metrics,   "assets/textures/font.metrics");
	res.loadTIM(_splashOverlay.image, "assets/textures/splash.tim");
	res.loadData(_stringTable,        "assets/lang/en.lang");
}

bool App::_createDataDirectory(void) {
	file::FileInfo info;

	if (!_fileIO.vfs.getFileInfo(info, EXTERNAL_DATA_DIR))
		return _fileIO.vfs.createDirectory(EXTERNAL_DATA_DIR);
	if (info.attributes & file::DIRECTORY)
		return true;

	return false;
}

bool App::_getNumberedPath(
	char *output, size_t length, const char *path, int maxIndex
) {
	file::FileInfo info;

	// Perform a binary search in order to quickly find the first unused path.
	int low  = 0;
	int high = maxIndex;

	while (low <= high) {
		int index = low + (high - low) / 2;

		snprintf(output, length, path, index);

		if (_fileIO.vfs.getFileInfo(info, output))
			low = index + 1;
		else
			high = index - 1;
	}

	if (low > maxIndex)
		return false;

	snprintf(output, length, path, low);
	return true;
}

void App::_updateOverlays(void) {
	// Date and time overlay
	static char dateString[24];
	util::Date  date;

	date.fromCurrentTime();
	date.toString(dateString);

	_textOverlay.leftText = dateString;

	// Splash screen overlay
	int timeout = _ctx.gpuCtx.refreshRate * _SPLASH_SCREEN_TIMEOUT;

	__atomic_signal_fence(__ATOMIC_ACQUIRE);

	if (_ctx.time > timeout)
		_splashOverlay.hide(_ctx);

#ifdef ENABLE_LOG_BUFFER
	// Log overlay
	if (
		_ctx.buttons.released(ui::BTN_DEBUG) &&
		!_ctx.buttons.longReleased(ui::BTN_DEBUG)
	)
		_logOverlay.toggle(_ctx);
#endif
}

[[noreturn]] void App::run(const char *resourceFile) {
#ifdef ENABLE_LOG_BUFFER
	util::logger.setLogBuffer(&_logBuffer);
#endif

	LOG_APP("build " VERSION_STRING " (" __DATE__ " " __TIME__ ")");
	LOG_APP("(C) 2022-2024 spicyjpeg");

	_ctx.screenData = this;

	_fileIO.loadResourceFile(resourceFile);
	_loadResources();

	_textOverlay.rightText = "v" VERSION_STRING;

	_ctx.backgrounds[0] = &_background;
	_ctx.backgrounds[1] = &_textOverlay;
	_ctx.overlays[0]    = &_splashOverlay;
#ifdef ENABLE_LOG_BUFFER
	_ctx.overlays[1]    = &_logOverlay;
#endif

	_ctx.show(_mainMenuScreen);
	_splashOverlay.show(_ctx);
	//_ctx.sounds[ui::SOUND_STARTUP].play();

	for (;;) {
		_ctx.update();
		_updateOverlays();

		_ctx.draw();
		// TODO: wait for vsync here
		_ctx.tick();
		_ctx.gpuCtx.flip();
	}
}
