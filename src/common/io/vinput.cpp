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

#include "vinput.hpp"

namespace vInput
{
    Context::Context()
    {
        for (const auto &factory : getDeviceHandlerFactories())
        {
            Device::DeviceHandler *device = factory();
            _devices.push_back(device);
            for (int c = 0; c < Device::DeviceClass::Class_Max; c++)
            {
                int devClass = device->getClassType(c);
                switch (devClass)
                {
                case Device::DeviceClass::JoystickClass:
                    _joyClass.addDevice(dynamic_cast<Device::JoystickHandler *>(device));
                    continue;
                case Device::DeviceClass::MouseClass:
                    _mouseClass.addDevice(dynamic_cast<Device::MouseHandler *>(device));
                    continue;
                case Device::DeviceClass::KeyboardClass:
                    _keyboardClass.addDevice(dynamic_cast<Device::KeyboardHandler *>(device));
                    continue;
                case Device::DeviceClass::LightgunClass:
                    _lightgunClass.addDevice(dynamic_cast<Device::LightgunHandler *>(device));
                    continue;
                case Device::DeviceClass::OSClass:
                    continue;
                case Device::DeviceClass::NullClass:
                    continue;
                default:
                    LOG_APP("Error: Unhandled device class: 0x%x", devClass);
                    break;
                }
            }
        }
    }

    int Context::init()
    {
        int result = 0;
        result += _joyClass.init();
        result += _keyboardClass.init();
        result += _mouseClass.init();
        result += _lightgunClass.init();
        return result;
    }

    void Context::update()
    {
        _joyClass.update();
        _keyboardClass.update();
        _mouseClass.update();
        _lightgunClass.update();
        if (_inBox.available())
        {
            _processMessage();
        }
    }

    int Context::reload()
    {
        int result = 0;
        result += _joyClass.reload();
        result += _keyboardClass.reload();
        result += _mouseClass.reload();
        result += _lightgunClass.init();
        return result;
    }

    void Context::_processMessage()
    {
        post::InputMsgStruct msg;
        if (_inBox.getMessage(&msg))
        {
            switch (msg.classCode)
            {
            case Device::JoystickClass:
                break;
            case Device::MouseClass:
                break;
            case Device::LightgunClass:
                break;
            case Device::KeyboardClass:
                break;
            default:
                break;
            }
        }
    }
}