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

#pragma once

#include <stddef.h>
#include "common/file/file.hpp"
#include "common/file/misc.hpp"
#include "common/file/zip.hpp"
#include "common/util/log.hpp"
#include "common/util/templates.hpp"
#include "main/app/mainmenu.hpp"
#include "main/uibase.hpp"
#include "modals.hpp"

/* Filesystem manager class */

extern const char *const IDE_MOUNT_POINTS[];

class FileIOManager {
private:
	file::File *_resourceFile;

public:
	file::ZIPProvider  resource;
	file::HostProvider host;
	file::VFSProvider  vfs;

	inline ~FileIOManager(void) {
		closeResourceFile();
	}

	FileIOManager(void);

	bool loadResourceFile(const char *path);
	void closeResourceFile(void);
};

/* App class */

class App {
	friend class MainMenuScreen;

	friend class MessageScreen;
	friend class ConfirmScreen;
	friend class FilePickerScreen;
	friend class FileBrowserScreen;

private:
	// mainmenu.cpp
	MainMenuScreen _mainMenuScreen;

	// modals.cpp
	MessageScreen     _messageScreen;
	ConfirmScreen     _confirmScreen;
	FilePickerScreen  _filePickerScreen;
	FileBrowserScreen _fileBrowserScreen;

	ui::TiledBackground   _background;
	ui::TextOverlay       _textOverlay;
	ui::SplashOverlay     _splashOverlay;
#ifdef ENABLE_LOG_BUFFER
	util::LogBuffer       _logBuffer;
	ui::LogOverlay        _logOverlay;
#endif

	ui::Context       &_ctx;
	file::StringTable _stringTable;
	FileIOManager     _fileIO;

	void _loadResources(void);
	bool _createDataDirectory(void);
	bool _getNumberedPath(
		char *output, size_t length, const char *path, int maxIndex = 9999
	);
	void _updateOverlays(void);
	void _runWorker(
		bool (App::*func)(void), ui::Screen &next, bool goBack = false,
		bool playSound = false
	);

public:
	App(ui::Context &ctx);

	[[noreturn]] void run(const char *resourceFile);
};

#define APP      (reinterpret_cast<App *>(ctx.screenData))
#define STR(id)  (APP->_stringTable.get(id ## _h))
#define STRH(id) (APP->_stringTable.get(id))

#define WSTR(id)  (_stringTable.get(id ## _h))
#define WSTRH(id) (_stringTable.get(id))
