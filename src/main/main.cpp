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

#include "common/util/hash.hpp"
#include "common/util/log.hpp"
#include "common/util/vesa.hpp"
#include "common/args.hpp"
#include "main/app/app.hpp"
#include "main/uibase.hpp"
#include "hw/gpu.hpp"
#include "hw/windowproc.hpp"

int main(int argc, const char **argv)
{
	util::initZipCRC32();

	args::MainArgs args;

	for (; argc > 1; argc--)
		args.parseArgument(*(++argv));

	if (!args.resourcePath)
	{
		LOG_APP("required arguments missing");
		return 1;
	}

	LOG_APP("Resource path is: %s", args.resourcePath);
	LOG_APP("Screen resolution is: %dx%d", args.screenWidth, args.screenHeight);

	auto gpuCtx = new gpu::Context(args.screenWidth, args.screenHeight);
	//auto ioCtx = new io::Context();
	auto uiCtx = new ui::Context(*gpuCtx);
	auto app = new App(*uiCtx);
	
	#ifdef _WINDOWS_
	win::Window window(uiCtx);
	gpuCtx->windowHandle = window.CreateMainWindow(args.screenWidth, args.screenHeight);
	gpuCtx->setResolution(vesa::QVGA);
	gpuCtx->setScale(gpu::GUI_SCALE_1X);
	#endif

	app->run(args.resourcePath);

	delete app;
	delete uiCtx;
	delete gpuCtx;
	return 0;
}

