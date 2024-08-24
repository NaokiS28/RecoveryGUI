# BemaniUX - Copyright (C) 2022-2024 spicyjpeg, NaokiS
#
# BemaniUX is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# BemaniUX is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# BemaniUX. If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.25)

project(
	BemaniUX
	LANGUAGES    C CXX
	VERSION      1.0.0
	DESCRIPTION  ""
	HOMEPAGE_URL "https://github.com/NaokiS28/BemaniUX"
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

## Main executable

# Always generate compile_commands.json when building. This allows some IDEs and
# tools (such as clangd) to automatically configure include directories and
# other options.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_SOURCE_DIR}/src/hw/win32/icon.rc")
	set(APP_INFO_RESOURCE_WINDOWS "${CMAKE_SOURCE_DIR}/src/hw/win32/info.rc")
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
		src/hw/win32/gpu.cpp
		src/hw/win32/gpufont.cpp
		src/common/io.cpp
		src/hw/win32/joystick.cpp
		src/hw/win32/keyboard.cpp
		src/hw/win32/mouse.cpp
		src/hw/win32/jvs.cpp
		src/common/io.cpp
		src/common/virtualio.cpp
		src/hw/win32/windowproc.cpp
		src/main/app/app.cpp
		src/main/app/mainmenu.cpp
		src/main/app/modals.cpp
		src/main/main.cpp
		src/main/uibase.cpp
		src/main/uicommon.cpp
		src/main/uimodals.cpp
		src/vendor/miniz.c
		src/vendor/qrcodegen.c
		${APP_ICON_RESOURCE_WINDOWS}
	)
else()
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
		src/common/io.cpp
		src/common/joystick.cpp
		src/common/keyboard.cpp
		src/common/mouse.cpp
		src/common/jvs.cpp
		src/common/io.cpp
		src/common/virtualio.cpp
		src/common/windowproc.cpp
		src/main/app/app.cpp
		src/main/app/mainmenu.cpp
		src/main/app/modals.cpp
		src/main/main.cpp
		src/main/uibase.cpp
		src/main/uicommon.cpp
		src/main/uimodals.cpp
		src/vendor/miniz.c
		src/vendor/qrcodegen.c
	)
endif()

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
target_compile_features(
	main PRIVATE
	cxx_std_20
)
target_compile_definitions(
	main PRIVATE
	VERSION="${PROJECT_VERSION}"
	EXTERNAL_DATA_DIR="hdd:/${PROJECT_NAME}"
	ENABLE_APP_LOGGING=1
	ENABLE_FS_LOGGING=1
	ENABLE_LOG_BUFFER=1
)

if(MINGW)
    target_link_libraries(main PRIVATE msimg32 Winmm User32)
else()
    target_link_libraries(main PRIVATE Msimg32 Winmm User32)
endif()

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

# Ensure the resource archive actually gets built.
add_custom_target(resources ALL DEPENDS resources.zip)
