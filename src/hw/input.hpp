#pragma once

// Add other architectures below

#if (defined(_WIN32))
#include "hw/win32/joystick.hpp"
#include "hw/win32/jvs.hpp"
#include "hw/win32/keyboard.hpp"
#include "hw/win32/mouse.hpp"
#include "hw/win32/sound.hpp"
#include "hw/win32/windowproc.hpp"
#include "hw/win32/resources.h"
#else
#error Unsupported target hardware! 
#endif