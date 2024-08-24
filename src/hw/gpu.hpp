#pragma once

// Add other architectures below

#if (defined(_WIN32))
#include "hw/win32/gpu.hpp"
#else
#error Unsupported target hardware! 
#endif