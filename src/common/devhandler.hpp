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

#include <vector>
#include <functional>

#include "virtualio.hpp"

enum InputDeviceStatus : int
{
    INPUT_DEVICE_OK,
    INPUT_DEVICE_BASE = 100,
    INPUT_DEVICE_NOTREADY,
    INPUT_DEVICE_INIT,
    INPUT_DEVICE_NOTATTACHED,
    INPUT_DEVICE_NOTAVAILABLE,
};

enum DeviceClasses
{
    INPUT_CLASS_JOYSTICK,
    INPUT_CLASS_KEYBOARD,
    INPUT_CLASS_MOUSE,
    INPUT_CLASS_LIGHTGUN,
    TOTAL_INPUT_CLASSES
};

class DeviceHandler
{
public:
    virtual ~DeviceHandler() = default;         // Any expansion classes are constructed during the IO handler construction.
    virtual int init() { return 0; }            // Override if a handler needs to be init'd after the app has started.
    virtual int update() = 0;                   // Update should be used to poll any devices
    virtual int reload() = 0;                   // Reload is called if the IO system needs to be reset
    virtual const char *getClassName() = 0;     // Return only the class name in CAPS. (i.e. "JAMMA")
    virtual int getClassType() = 0;             // Return the class code/closest type
    virtual int getDeviceCount() = 0;           // Return how many devices this class handler current recognizes

    virtual bool getSwitch(uint32_t code) = 0;  // Get boolean value of switch from input code
    virtual int getAnalog(uint32_t code) = 0;   // Get analog (int16_t) value of input from input code
    virtual int getRelative(uint32_t code) = 0; // Get relative (int16_t) value of input from input code

    virtual int getSwitchCount(uint32_t code) { return 0; }
    virtual int getAnalogCount(uint32_t code) { return 0; }
    virtual int getRelativeCount(uint32_t code) { return 0; }
};

using DeviceHandlerFactory = std::function<DeviceHandler *()>;

inline std::vector<DeviceHandlerFactory> &getDeviceHandlerFactories()
{
    static std::vector<DeviceHandlerFactory> factories;
    return factories;
}

#define REGISTER_DEVICE_HANDLER(ClassName)                                                                  \
    namespace                                                                                               \
    {                                                                                                       \
        struct ClassName##Register                                                                          \
        {                                                                                                   \
            ClassName##Register()                                                                           \
            {                                                                                               \
                getDeviceHandlerFactories().push_back([]() -> DeviceHandler * { return new ClassName(); }); \
            }                                                                                               \
        };                                                                                                  \
        static ClassName##Register global_##ClassName##Register;                                            \
    }
