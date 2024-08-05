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
#include <stdlib.h>
#include "common/util/hash.hpp"
#include "common/util/templates.hpp"
#include "common/args.hpp"

namespace args {

/* Command line argument parsers */

static constexpr char _VALUE_SEPARATOR = '=';

static constexpr int _DEFAULT_SCREEN_WIDTH  = 320;
static constexpr int _DEFAULT_SCREEN_HEIGHT = 240;

static const char _DEFAULT_RESOURCE_PATH[]{ "host:./resources.zip" };

MainArgs::MainArgs(void)
: screenWidth(_DEFAULT_SCREEN_WIDTH), screenHeight(_DEFAULT_SCREEN_HEIGHT),
resourcePath(_DEFAULT_RESOURCE_PATH) {}

bool MainArgs::parseArgument(const char *arg) {
	if (!arg)
		return false;

	switch (util::hash(arg, _VALUE_SEPARATOR)) {
		case "screen.width"_h:
			screenWidth = int(strtol(&arg[13], nullptr, 0));
			return true;

		case "screen.height"_h:
			screenHeight = int(strtol(&arg[14], nullptr, 0));
			return true;

		case "resource.path"_h:
			resourcePath = &arg[14];
			return true;

		default:
			return false;
	}
}

}
