#pragma once

// Add other architectures below

#ifdef _WIN32
#include "hw/win32/keyboard.hpp"
#else
#error Unsupported target hardware! 
#endif