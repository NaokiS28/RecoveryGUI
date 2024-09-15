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
        OSClass,
        Class_Max = OSClass
    };

    constexpr const char* const DeviceClassNames[] = {
        "NullDevice"
        "Joystick",
        "Keyboard",
        "Mouse",
        "Lightgun",
        "OS"
    };

    class DeviceHandler
    {
    protected:
        std::array<int, DeviceClass::Class_Max> _deviceClasses;
        bool _polledMode = false;

        void setPolledMode(bool state){ _polledMode = state; }

        void postMessage(uint32_t classCode, uint32_t classIdx, uint32_t messageCode, uint32_t param){
            if(_outBox != nullptr){
                _outBox->postMessage(classCode, classIdx, messageCode, param);
            }
        }
        
    public:
        int _classCode = 0;     // Do not modify
        int _classIdx = 0;      // Do not modify
        post::PostBox* _outBox = nullptr;   // Do not modify

        DeviceHandler() :
            _deviceClasses{} {
            _deviceClasses.fill(DeviceClass::NullClass);
        }   
        virtual ~DeviceHandler() = default;

        bool isPollable(){ return _polledMode; }
        
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

        
        virtual int init() { return 0; }    // Override if a handler needs to be init'd after the app has started.
        virtual int reload() = 0;           // Reload is called if the IO system needs to be reset
        virtual void update() = 0;           // Update should be used to poll any devices

        virtual int getDeviceCount() = 0;       // Return how many devices this class handler current recognizes
        virtual const char* getDeviceName(int idx) { return nullptr; }
        virtual const char *getDriverName() = 0;
    };

    class JoystickHandler : public virtual DeviceHandler
    {    
    public:
        virtual ~JoystickHandler() = default; // Any expansion classes are constructed during the IO handler construction.

        void postMessage(uint32_t messageCode, uint32_t param){
            if(_outBox != nullptr){
                _outBox->postMessage(DeviceClass::JoystickClass, _classIdx, messageCode, param);
            }
        }

        virtual bool getSwitch(uint32_t code) = 0;  // Get boolean value of switch from input code
        virtual uint32_t getDigital(uint32_t code) = 0; // Get all 32 digital states
        virtual int16_t getAnalog(uint32_t code) = 0;   // Get analog (int16_t) value of input from input code
        virtual int16_t getRelative(uint32_t code) = 0; // Get relative (int16_t) value of input from input code

        virtual int getSwitchCount(uint32_t code) { return 0; }
        virtual int getAnalogCount(uint32_t code) { return 0; }
        virtual int getRelativeCount(uint32_t code) { return 0; }

        virtual const char *getDriverName() { return "Joystick"; }
    };

    class MouseHandler : public virtual DeviceHandler
    {
    public:
        virtual ~MouseHandler() = default; // Any expansion classes are constructed during the IO handler construction.

        void postMessage(uint32_t messageCode, uint32_t param){
            if(_outBox != nullptr){
                _outBox->postMessage(DeviceClass::MouseClass, _classIdx, messageCode, param);
            }
        }

        virtual int getMouseType(int idx) = 0;

        virtual const char *getDriverName() { return "Mouse"; }
    };

    class LightgunHandler : public virtual DeviceHandler
    {
    public:
        virtual ~LightgunHandler() = default; // Any expansion classes are constructed during the IO handler construction.

        void postMessage(uint32_t messageCode, uint32_t param){
            if(_outBox != nullptr){
                _outBox->postMessage(DeviceClass::LightgunClass, _classIdx, messageCode, param);
            }
        }

        virtual int getLightgunType(int idx) = 0;

        virtual const char *getDriverName() override { return "Lightgun"; }
    };

    class KeyboardHandler : public virtual DeviceHandler
    {
    public:
        virtual ~KeyboardHandler() = default; // Any expansion classes are constructed during the IO handler construction.

        void postMessage(uint32_t messageCode, uint32_t param){
            if(_outBox != nullptr){
                _outBox->postMessage(DeviceClass::KeyboardClass, _classIdx, messageCode, param);
            }
        }

        virtual int getKeyboardType(int idx) = 0;
        virtual void type(const char *c) = 0;

        virtual const char *getDriverName() { return "Keyboard"; }
    };

    /*
        OS Handler is for any un-accounted for inputs/systems that
        has need to be handled, such as Windows API messages.
    */ 

    class OSHandler : public virtual DeviceHandler
    {
    public:
        virtual ~OSHandler() = default; // Any expansion classes are constructed during the IO handler construction.

        void postMessage(int devIdx, int messageCode){
            if(_outBox != nullptr){
                _outBox->postMessage(DeviceClass::KeyboardClass, _classIdx, devIdx, messageCode);
            }
        }
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
        int16_t getAnalog(uint32_t code) { return 0; }
        int16_t getRelative(uint32_t code) { return 0; }

        const char *getDriverName() override { return "null"; }
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
