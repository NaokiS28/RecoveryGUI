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

#include <stddef.h>
#include <stdio.h>
#include "common/file/file.hpp"
#include "common/file/misc.hpp"
#include "common/file/zip.hpp"
#include "common/util/log.hpp"
#include "common/util/misc.hpp"
#include "common/util/templates.hpp"
#include "common/ui/layers.hpp"
#include "common/defs.hpp"
#include "hw/gpu.hpp"
#include "main/app/app.hpp"

/* Filesystem manager class */

const char *const IDE_MOUNT_POINTS[]{"ide0:", "ide1:"};

FileIOManager::FileIOManager(void)
	: _resourceFile(nullptr)
{
	vfs.mount("resource:", &resource);
	vfs.mount("host:", &host);
}

bool FileIOManager::loadResourceFile(const char *path)
{
	closeResourceFile();

	if (path)
		_resourceFile = vfs.openFile(path, file::READ);

	if (_resourceFile)
	{
		if (resource.init(_resourceFile))
			return true;

		_resourceFile->close();
		delete _resourceFile;
		_resourceFile = nullptr;
	}

	// Fall back to the default in-memory resource archive in case of failure.
	// resource.init(resourcePtr, resourceLength);
	return false;
}

void FileIOManager::closeResourceFile(void)
{
	resource.close();

	if (_resourceFile)
	{
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
	  _ctx(ctx)
{
}

void App::_loadResources(void)
{
	auto &res = _fileIO.resource;
	res.loadImage(_background.tile, "assets/textures/background.png");
	res.loadStruct(_ctx.colors, "assets/palette.dat");
	res.loadImage(_ctx.font.image, "assets/textures/font.png");
	res.loadData(_ctx.font.metrics, "assets/textures/font.metrics");
	res.loadImage(_splashOverlay.image, "assets/textures/splash.png");
	res.loadData(_stringTable, "assets/lang/en.lang");
	//res.loadImage(_ctx.cursor.arrow, "assets/textures/cursor.png");
}

bool App::_createDataDirectory(void)
{
	file::FileInfo info;

	if (!_fileIO.vfs.getFileInfo(info, EXTERNAL_DATA_DIR))
		return _fileIO.vfs.createDirectory(EXTERNAL_DATA_DIR);
	if (info.attributes & file::DIRECTORY)
		return true;

	return false;
}

bool App::_getNumberedPath(
	char *output, size_t length, const char *path, int maxIndex)
{
	file::FileInfo info;

	// Perform a binary search in order to quickly find the first unused path.
	int low = 0;
	int high = maxIndex;

	while (low <= high)
	{
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

void App::_updateOverlays(void)
{
	// Date and time overlay
	static char dateString[24];
	util::Date date;

	date.fromCurrentTime();
	date.toString(dateString);

	_textOverlay.leftText = dateString;

	// Splash screen overlay
	int timeout = _ctx.gpuCtx.getRefreshRate() * _SPLASH_SCREEN_TIMEOUT;

	__atomic_signal_fence(__ATOMIC_ACQUIRE);

	if (_ctx.time > timeout)
		_splashOverlay.hide(_ctx);

#ifdef ENABLE_LOG_BUFFER
	// Log overlay
	if (
		_ctx.buttons.released(ui::BTN_DEBUG) &&
		!_ctx.buttons.longReleased(ui::BTN_DEBUG))
		_logOverlay.toggle(_ctx);
#endif
}

void App::run(const char *resourceFile)
{
#ifdef ENABLE_LOG_BUFFER
	util::logger.setLogBuffer(&_logBuffer);
#endif

	LOG_APP("build " VERSION_STRING " (" __DATE__ " " __TIME__ ")");
	LOG_APP("(C) 2022-2024 spicyjpeg, NaokiS");

	_ctx.init();

	_ctx.screenData = this;
	
	
	/*_ctx.layerMan.newLayer<uibg::TiledBackground>( 
		0, 0,
		_ctx.gpuCtx.getHorizontalRes(),
		_ctx.gpuCtx.getVerticalRes(),
		uibg::ScrollDirections::DownRight 
		);*/
	
	_ctx.layerMan.newLayer<vKeyboard::vKeyboard>( 
		0, 0,
		_ctx.gpuCtx.getHorizontalRes(),
		_ctx.gpuCtx.getVerticalRes()
		);
	
	vKeyboard::vKeyboard* oskptr = dynamic_cast<vKeyboard::vKeyboard*>(
		_ctx.layerMan.getPtrToLayer(vKeyboard::OSKName));
	Device::KeyboardHandler* oskdevptr = dynamic_cast<Device::KeyboardHandler*>(
		_ctx._ioCtx.getKeyDevPtr(vKeyboard::OSKName));
	if(oskptr != nullptr && oskdevptr != nullptr){
		oskptr->setOSKDev(oskdevptr);
	}

	_fileIO.loadResourceFile(resourceFile);
	_loadResources();

	_textOverlay.rightText = "v" VERSION_STRING;

	_ctx.backgrounds[0] = &_background;
	_ctx.backgrounds[1] = &_textOverlay;
	_ctx.overlays[0] = &_splashOverlay;
#ifdef ENABLE_LOG_BUFFER
	_ctx.overlays[2] = &_logOverlay;
#endif
	// Temporary input debugging
	_ctx.overlays[3] = &_inputDbgOverlay;

	_ctx.show(_mainMenuScreen);
	_splashOverlay.show(_ctx);
	//_ctx.sounds[ui::SOUND_STARTUP].play();

	_keepAppAlive = true;

	while (_keepAppAlive)
	{
		switch (_ctx.update())
		{
		case 1:
			_keepAppAlive = false;
			break;
		default:
			break;
		}
		// runUpdate is called every ~60hz
		if (_ctx.runUpdate)
		{
			_ctx.runUpdate = false;
			_updateOverlays();
			_ctx.tick();
			
			InvalidateRect(_ctx.gpuCtx.windowHandle, NULL, FALSE);
			//RedrawWindow(_ctx.gpuCtx.windowHandle, NULL, NULL, RDW_INTERNALPAINT);
		}
	}
}
