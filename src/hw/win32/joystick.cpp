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

#include "joystick.hpp"

int Win32Joy::init()
{
    // Note: Windows reports how many joysticks the system can currently support,
    //          not the total connected joysticks.
    uint16_t joyReport = joyGetNumDevs();

    if (!joyReport)
    {
        // LOG_APP("No joystick driver installed.");
        return 1;
    }

    uint16_t joy = 0;
    JOYINFOEX joyState;
    JOYCAPS joyDeviceInfo;

    joyState.dwSize = sizeof(JOYINFOEX);
    joyState.dwFlags = JOY_RETURNALL;

    for (uint16_t j = 0; j < joyReport; j++)
    {
        if ((joy = joyGetPosEx(j, &joyState)) == JOYERR_NOERROR)
        {
            joyGetDevCaps(j, &joyDeviceInfo, sizeof(JOYCAPS));

            JoystickDevice joystickDevice(j, joyState, joyDeviceInfo);

            LOG_APP("Joystick %d: %d axis, %d button joystick",
                    j + 1,
                    joystickDevice.device.wNumAxes,
                    joystickDevice.device.wNumButtons);
            _newJoystick(joystickDevice);
        }
    }
    return 0;
}

void Win32Joy::update()
{
    for (auto &joy : joystickList)
    {
        int joyResult = joyGetPosEx(joy.id, &joy.state);
        if (joyResult == JOYERR_NOERROR)
        {
            size_t result = memcmp(&joy.state, &joy.lastState, sizeof(joy.state));
            if (result != 0)
            {
                if (joy.state.dwButtons != joy.lastState.dwButtons)
                {
                    postMessage(Input::InputMessage::IM_INPUT_DIGITAL,
                                Input::getInputCode(joy.id, Input::InputType::Digital, 255));
                }

                for (int x = 0; x < 6; x++)
                {
                    uint32_t curr, last;
                    switch (x)
                    {
                    case 0:
                        curr = joy.state.dwXpos;
                        last = joy.lastState.dwXpos;
                        break;
                    case 1:
                        curr = joy.state.dwYpos;
                        last = joy.lastState.dwYpos;
                        break;
                    case 2:
                        curr = joy.state.dwZpos;
                        last = joy.lastState.dwZpos;
                        break;
                    case 3:
                        curr = joy.state.dwRpos;
                        last = joy.lastState.dwRpos;
                        break;
                    case 4:
                        curr = joy.state.dwUpos;
                        last = joy.lastState.dwUpos;
                        break;
                    case 5:
                        curr = joy.state.dwVpos;
                        last = joy.lastState.dwVpos;
                        break;
                    }
                    if (curr != last)
                    {
                        uint32_t code = Input::getInputCode(
                            joy.id, Input::InputType::Analog, x);
                        postMessage(Input::InputMessage::IM_INPUT_ANALOG, code); // Plus axis
                    }
                }

                memcpy(&joy.lastState, &joy.state, sizeof(joy.state));
            }
        }
        else
        {
            switch (joyResult)
            {
            case JOYERR_UNPLUGGED:
                _removeJoystick(joy);
                continue;
            default:
                continue;
            }
        }
    }
}

void Win32Joy::_reset()
{
    for (auto &dev : joystickList)
    {
        postMessage(Input::InputMessage::IM_DEVICE_DISCONNECT, dev.id);
    }
    joystickList.erase(joystickList.begin(), joystickList.end());
}

int Win32Joy::reload()
{
    _reset();
    init();
    return 0;
}

const char *Win32Joy::getDeviceName(int idx)
{
    if ((size_t)idx < joystickList.size())
    {
        return joystickList[idx].device.szPname;
    }
    return nullptr;
}

bool Win32Joy::getSwitch(uint32_t code)
{
    uint8_t id = Input::getInputDevId(code);
    uint8_t inputIdx = Input::getInputIdx(code);
    if (id < joystickList.size() && inputIdx < joystickList[id].device.wNumButtons)
    {
        return ((joystickList[id].getInputs() >> inputIdx) & 0x1);
    }
    else
    {
        return false;
    }
}

uint32_t Win32Joy::getDigital(uint32_t code)
{
    int devIdx = Input::getInputDevId(code);
    return joystickList[devIdx].state.dwButtons;
}

int16_t Win32Joy::getAnalog(uint32_t code)
{
    uint8_t id = Input::getInputDevId(code);
    uint8_t inputIdx = Input::getInputIdx(code);
    int16_t val = 0x0000;

    if (id < joystickList.size() && inputIdx < joystickList[id].device.wNumAxes)
    {
        switch (inputIdx)
        {
        case 0:
            val = joystickList[id].state.dwXpos;
            break;
        case 1:
            val = joystickList[id].state.dwYpos;
            break;
        case 2:
            val = joystickList[id].state.dwZpos;
            break;
        case 3:
            val = joystickList[id].state.dwRpos;
            break;
        case 4:
            val = joystickList[id].state.dwUpos;
            break;
        case 5:
            val = joystickList[id].state.dwVpos;
            break;
        default:
            break;
        }
    }
    return val;
}

void Win32Joy::_newJoystick(JoystickDevice &joy)
{
    joystickList.push_back(joy);
    auto itr = joystickList.end() - 1;
    auto idx = std::distance(joystickList.begin(), itr);
    joy.id = idx;
    postMessage(Input::InputMessage::IM_DEVICE_CONNECT, joy.id);
}

void Win32Joy::_removeJoystick(JoystickDevice &joy)
{
    for (size_t i = 0; i < joystickList.size(); i++)
    {
        auto &dev = joystickList.at(i);
        if (dev.id == joy.id)
        {
            postMessage(Input::InputMessage::IM_DEVICE_DISCONNECT, dev.id);
            joystickList.erase(joystickList.begin() + i);
            break;
        }
    }
}