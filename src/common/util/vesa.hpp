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