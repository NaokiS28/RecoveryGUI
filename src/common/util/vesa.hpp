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

#pragma once

#include <array>

namespace vesa {
    constexpr std::array<int, 2> QVGA = { 320, 240 };
    constexpr std::array<int, 2> WQVGA = { 384, 240 };
    constexpr std::array<int, 2> FWQVGA = { 432, 240 };
    constexpr std::array<int, 2> HVGA = { 480, 320 };
    constexpr std::array<int, 2> VGA = { 640, 480 };
    constexpr std::array<int, 2> WVGA = { 768, 480 };
    constexpr std::array<int, 2> FWVGA = { 848, 480 };
    constexpr std::array<int, 2> SVGA = { 800, 600 };
    constexpr std::array<int, 2> WSVGA = { 1024, 600 };
    constexpr std::array<int, 2> XHD = { 960, 720 };
    constexpr std::array<int, 2> HD = { 1280, 720 };
    constexpr std::array<int, 2> XGA = { 1024, 768 };
    constexpr std::array<int, 2> WXGA = { 1280, 768 };
    constexpr std::array<int, 2> FWXGA = { 1366, 768 };
    constexpr std::array<int, 2> SXGAP = { 1400, 1050 };
    constexpr std::array<int, 2> WSXGAP = { 1680, 1050 };
    constexpr std::array<int, 2> XFHD = { 1440, 1080 };
    constexpr std::array<int, 2> FHD = { 1920, 1080 };
}