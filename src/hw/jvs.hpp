#pragma once

// Add other architectures below

#if (defined(_WIN32) && !defined(_WINPE))
#include "hw/win32/jvs.hpp"
#elif (defined(_WIN32) && defined(_WINPE))
#include "hw/winpe/jvs.hpp"
#else
#error Unsupported target hardware! 
#endif