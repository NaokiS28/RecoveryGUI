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

#include "common/util/hash.hpp"
#include "common/util/log.hpp"
#include "common/args.hpp"
#include "common/gpu.hpp"
#include "main/app/app.hpp"
#include "main/uibase.hpp"

int main(int argc, const char **argv) {
	util::initZipCRC32();

	args::MainArgs args;

	for (; argc > 1; argc--)
		args.parseArgument(*(++argv));

	if (!args.resourcePath) {
		LOG_APP("required arguments missing");
		return 1;
	}

	auto gpuCtx = new gpu::Context(args.screenWidth, args.screenHeight);
	auto uiCtx  = new ui::Context(*gpuCtx);
	auto app    = new App(*uiCtx);

	app->run(args.resourcePath);

	delete app;
	delete uiCtx;
	delete gpuCtx;
	return 0;
}
