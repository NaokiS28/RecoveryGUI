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
#include <vector>
#include <functional>
#include <string>

#include "postbox.hpp"
#include "inputdefs.hpp"

namespace Device
{
    enum DeviceStatus : int
    {
        INPUT_DEVICE_OK,
        INPUT_DEVICE_BASE = 100,
        INPUT_DEVICE_NOTREADY,
        INPUT_DEVICE_INIT,
        INPUT_DEVICE_NOTATTACHED,
        INPUT_DEVICE_NOTAVAILABLE,
    };

    enum DeviceClass {
        NullClass,
        JoystickClass,
        KeyboardClass,
        MouseClass,
        LightgunClass,
        Class_Max = LightgunClass
    };

    constexpr const char* const DeviceClassNames[] = {
        "NullDevice"
        "Joystick",
        "Keyboard",
        "Mouse",
        "Lightgun"        
    };

    class DeviceHandler
    {
    protected:
        std::array<int, DeviceClass::Class_Max> _deviceClasses;
    public:
        post::PostBox* _outBox;
        
        DeviceHandler() :
            _deviceClasses{} {
            _deviceClasses.fill(DeviceClass::NullClass);
        }   
        virtual ~DeviceHandler() = default;
        
        int getClassType(int idx) {
            if (idx < 0 || (size_t)idx >= _deviceClasses.size()) {
                return DeviceClass::NullClass; // Handle out-of-bounds access safely
            }
            return _deviceClasses[idx];
        }
        const char *getClassName(int idx) { 
            if (idx < 0 || (size_t)idx >= _deviceClasses.size()) {
                return Device::DeviceClassNames[_deviceClasses[idx]]; 
            }
            return nullptr;
        }

        virtual const char *getSubClassName(int idx) { return nullptr; }
        
        virtual int init() { return 0; }    // Override if a handler needs to be init'd after the app has started.
        virtual int reload() = 0;           // Reload is called if the IO system needs to be reset
        virtual void update() = 0;           // Update should be used to poll any devices

        virtual int getDeviceCount() = 0;       // Return how many devices this class handler current recognizes
        virtual const char* getDeviceName(int idx) { return nullptr; }
    };

    class JoystickHandler : public virtual DeviceHandler
    {    
    public:
        virtual ~JoystickHandler() = default; // Any expansion classes are constructed during the IO handler construction.

        virtual bool getSwitch(uint32_t code) = 0;  // Get boolean value of switch from input code
        virtual int getAnalog(uint32_t code) = 0;   // Get analog (int16_t) value of input from input code
        virtual int getRelative(uint32_t code) = 0; // Get relative (int16_t) value of input from input code

        virtual int getSwitchCount(uint32_t code) { return 0; }
        virtual int getAnalogCount(uint32_t code) { return 0; }
        virtual int getRelativeCount(uint32_t code) { return 0; }
    };

    class MouseHandler : public virtual DeviceHandler
    {
    public:
        virtual ~MouseHandler() = default; // Any expansion classes are constructed during the IO handler construction.

        virtual int getMouseType(int idx) = 0;
    };

    class LightgunHandler : public virtual DeviceHandler
    {
    public:
        virtual ~LightgunHandler() = default; // Any expansion classes are constructed during the IO handler construction.

        //virtual int getGunType(int idx) = 0;
    };

    class KeyboardHandler : public virtual DeviceHandler
    {
    public:
        virtual ~KeyboardHandler() = default; // Any expansion classes are constructed during the IO handler construction.

        virtual int getKeyboardType(int idx) = 0;
    };

    class NullHandler : public JoystickHandler, public KeyboardHandler, public MouseHandler {
    public:
        ~NullHandler(){}
        void update(){} 
        int init(){ return 0; }
        int reload(){ return 0; }

        int getDeviceCount() { return 0; }

        int getMouseType(int idx) { return 0; }
        int getKeyboardType(int idx) { return 0; }
        bool getSwitch(uint32_t code) { return 0; }
        int getAnalog(uint32_t code) { return 0; }
        int getRelative(uint32_t code) { return 0; }
    };
}


using DeviceHandlerFactory = std::function<Device::DeviceHandler *()>;
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
