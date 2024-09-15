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

#include "vjoy.hpp"

namespace vJoy
{
    int Context::init()
    {
        int result = 0;
        for (auto &dev : _devices)
        {
            result += dev->init();
        }
        return result;
    }

    int Context::reload()
    {
        int result = 0;
        for (auto &dev : _devices)
        {
            result += dev->reload();
        }
        return result;
    }

    void Context::update()
    {
        for (auto &dev : _devices)
        {
            dev->update();
        }

        while (_inBox.available())
        {
            _processMessage();
        }

        for (auto &dev : _devices)
        {
            if (dev->isPollable())
            {
                // do poll?
            }
        }
    }

    void Context::_processMessage()
    {
        post::InputMsgStruct msg;
        _inBox.getMessage(&msg);

        if (msg.message == InputMessage::IM_DEVICE_CONNECT ||
            msg.message == Input::IM_DEVICE_DISCONNECT)
        {
            switch (msg.message)
            {
            case InputMessage::IM_DEVICE_DISCONNECT:
            {
                // Toast message that device disconnected
                int devIdx = getInputCode(msg.param, 0, 0);
                int joyIdx = _getVJoyDevIdx(msg.classIdx, devIdx);
                _joystickDevs.erase(_joystickDevs.begin() + joyIdx);
            }
            break;
            case InputMessage::IM_DEVICE_CONNECT:
            {
                int devIdx = getInputCode(msg.param, 0, 0);
                vJoyDev joy = {
                    _joyDevCount++,
                    (uint8_t)msg.classIdx,
                    (uint8_t)msg.param,
                    _devices[msg.classIdx]->getSwitchCount(devIdx),
                    _devices[msg.classIdx]->getAnalogCount(devIdx),
                    _devices[msg.classIdx]->getRelativeCount(devIdx),
                    {}};
                _joystickDevs.push_back(joy);
            }
            break;
            }
        }
        else
        {
            int inpIdx = Input::getInputIdx(msg.param);
            int devIdx = Input::getInputDevId(msg.param);
            int vdevIdx = _getVJoyDevIdx(msg.classIdx, devIdx);
            if (vdevIdx != 255)
            {
                switch (msg.message)
                {
                case InputMessage::IM_INPUT:
                    // Do inputy stuff
                    {
                        int type = 0; // getChangedType();
                        switch (type)
                        {
                        case InputType::Digital:
                            break;
                        case InputType::Analog:
                            break;
                        case InputType::Relative:
                            break;
                        }
                    }
                    break;
                case InputMessage::IM_INPUT_DIGITAL:
                {
                    if (inpIdx == 255)
                        _joystickDevs[vdevIdx].state.digital = _devices[msg.classIdx]->getDigital(msg.param);
                    else
                    {
                        bool btn = _devices[msg.classIdx]->getSwitch(msg.param);
                        uint32_t mask = (uint32_t) ~(1 << inpIdx);
                        _joystickDevs[vdevIdx].state.digital = ((_joystickDevs[vdevIdx].state.digital & mask) |
                                                                (uint32_t)(btn << inpIdx));
                    }
                }
                break;
                case InputMessage::IM_INPUT_ANALOG:
                    _joystickDevs[vdevIdx].state.analog[inpIdx] = _devices[msg.classIdx]->getAnalog(msg.param);
                    break;
                case InputMessage::IM_INPUT_RELATIVE:
                    _joystickDevs[vdevIdx].state.relative[inpIdx] = _devices[msg.classIdx]->getRelative(msg.param);
                    break;
                default:
                    break;
                }
            }
        }
    }

    int Context::_getVJoyDevIdx(int classIdx, int devIdx)
    {
        int idx = 0;
        for (auto &dev : _joystickDevs)
        {
            if (dev.classDevIdx == classIdx &&
                dev.subDevIdx == devIdx)
                return idx;
            else
                idx++;
        }
        return 255;
    }

    void Context::addDevice(Device::JoystickHandler *device)
    {
        _devices.push_back(device);
        auto itr = _devices.end() - 1;
        auto idx = std::distance(_devices.begin(), itr);
        _devices[idx]->_outBox = &_inBox;
        _devices[idx]->_classIdx = idx;
    }

    uint32_t Context::getSwitch(uint8_t idx)
    {
        if (idx > _joystickDevs.size())
            return 0;
        return _joystickDevs[idx].state.digital;
    }

    int16_t Context::getAnalog(uint8_t idx, uint8_t channel)
    {
        if (idx > _joystickDevs.size())
            return 0;
        if (channel > _joystickDevs[idx].analogCount)
            return 0;
        return _joystickDevs[idx].state.analog[channel];
    }

    int16_t Context::getRelative(uint8_t idx, uint8_t channel)
    {
        if (idx > _joystickDevs.size())
            return 0;
        if (channel > _joystickDevs[idx].relativeCount)
            return 0;
        return _joystickDevs[idx].state.relative[channel];
    }

    uint8_t Context::getSwitchCount(uint8_t idx)
    {
        if (idx > _joystickDevs.size())
            return 0;
        return _joystickDevs[idx].switchCount;
    }

    uint8_t Context::getAnalogCount(uint8_t idx)
    {
        if (idx > _joystickDevs.size())
            return 0;
        return _joystickDevs[idx].analogCount;
    }

    uint8_t Context::getRelativeCount(uint8_t idx)
    {
        if (idx > _joystickDevs.size())
            return 0;
        return _joystickDevs[idx].relativeCount;
    }

    // Gets the name of the drive at given class index
    const char *Context::getDriverName(uint8_t idx)
    {
        if (idx > _devices.size())
            return nullptr;
        const char *ptr = nullptr;
        ptr = _devices[idx]->getDriverName();
        return ptr;
    }

    // Gets the name of the joystick at given device index
    const char *Context::getJoystickName(uint8_t idx)
    {
        if (idx > _joystickDevs.size())
            return nullptr;
        const char *ptr = nullptr;
        int classIdx = _joystickDevs[idx].classDevIdx;
        int devIdx = _joystickDevs[idx].subDevIdx;
        ptr = _devices[classIdx]->getDeviceName(
            _joystickDevs[devIdx].subDevIdx);
        return ptr;
    }

}