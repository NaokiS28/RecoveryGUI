# 573in1 - Copyright (C) 2022-2024 spicyjpeg
#
# 573in1 is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# 573in1 is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# 573in1. If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.25)

project(
	RecoveryGUI
	LANGUAGES    C CXX
	VERSION      1.0.0
	DESCRIPTION  ""
	HOMEPAGE_URL "https://github.com/NaokiS28/RecoveryGUI"
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

## Main executable

# Always generate compile_commands.json when building. This allows some IDEs and
# tools (such as clangd) to automatically configure include directories and
# other options.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
	main
	src/common/file/file.cpp
	src/common/file/misc.cpp
	src/common/file/zip.cpp
	src/common/util/hash.cpp
	src/common/util/log.cpp
	src/common/util/misc.cpp
	src/common/util/string.cpp
	src/common/util/tween.cpp
	src/common/args.cpp
	src/common/gpu.cpp
	src/common/gpufont.cpp
	src/main/app/app.cpp
	src/main/main.cpp
	src/main/uibase.cpp
	src/main/uicommon.cpp
	src/main/uimodals.cpp
	src/vendor/miniz.c
	src/vendor/qrcodegen.c
)
target_include_directories(
	main PRIVATE
	src
)
target_compile_options(
	main PRIVATE
	-Wall
	-Wextra
	-Wno-unused-parameter
	$<$<COMPILE_LANGUAGE:CXX>:
		-Wno-pmf-conversions
	>
)
target_compile_definitions(
	main PRIVATE
	VERSION="${PROJECT_VERSION}"
	EXTERNAL_DATA_DIR="hdd:/${PROJECT_NAME}"
)

## Resource archive

file(GLOB_RECURSE assetList RELATIVE "${PROJECT_SOURCE_DIR}" assets/*)

function(addResourceArchive configFile outputFile)
	configure_file(${configFile} ${configFile} ESCAPE_QUOTES)

	add_custom_command(
		COMMAND
			"${Python3_EXECUTABLE}"
			"${PROJECT_SOURCE_DIR}/tools/buildResourceArchive.py"
			${configFile}
			${outputFile}
		OUTPUT  ${outputFile}
		DEPENDS
			${configFile}
			${assetList}
		COMMENT "Building ${outputFile}"
		VERBATIM
	)
endfunction()

configure_file(assets/about.txt about.txt NEWLINE_STYLE LF)
addResourceArchive(resources.json resources.zip)
