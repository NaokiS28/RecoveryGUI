#pragma once

// Add other architectures below

#ifdef _WIN32
#include "hw/win32/mouse.hpp"
#else
#error Unsupported target hardware! 
#endif