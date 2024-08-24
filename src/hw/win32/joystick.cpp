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

int JoystickHandler::init()
{
    LOG_APP("JoystickHandler init");

    // Note: Windows reports how many joysticks the system can currently support,
    //          not the total connected joysticks.
    uint16_t joyReport = joyGetNumDevs();

    if (!joyReport)
    {
        LOG_APP("No joystick driver installed.");
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
            JoystickDevice joystickDevice(joyState, joyDeviceInfo);
            _newJoystick(joystickDevice);
        }
    }
    return joystickCount;
}

int JoystickHandler::update()
{
    uint16_t result = JOYERR_NOERROR;

    if (polledMode)
    {
        for (uint8_t i = 0; i < joystickCount; i++)
        {
            uint16_t joy = joyGetPosEx(i, &joystickList[i].state);
            if (joy > result)
                result = joy;
        }
    }

    return result;
}

int16_t JoystickHandler::getInput(uint32_t code)
{
    // Todo
    uint8_t id = VirtualIO::getInputDevId(code);
    uint8_t InputType = VirtualIO::getInputType(code);
    uint8_t inputIdx = VirtualIO::getInputIdx(code);

    if(id > (joystickCount - 1)){
        return VirtualIO::INPUT_STATE_INACTIVE;
    }

    if(InputType == VirtualIO::INPUT_TYPE_ANALOG){
        int16_t val = 0x0000;
        switch(inputIdx){
            case 0: val = joystickList[id].state.dwXpos; break;
            case 1: val = joystickList[id].state.dwYpos; break;
            case 2: val = joystickList[id].state.dwZpos; break;
            case 3: val = joystickList[id].state.dwRpos; break;
            case 4: val = joystickList[id].state.dwUpos; break;
            case 5: val = joystickList[id].state.dwVpos; break;
            default: break;
        }
        return val;
    }
    else if(InputType == VirtualIO::INPUT_TYPE_DIGITAL){
        return ((joystickList[id].getInputs() >> inputIdx) & 0x1 ? 0x7FFF : 0x0000);
    }
    return 0;
}

void JoystickHandler::_newJoystick(JoystickDevice &joy)
{
    JoystickDevice *newJoystickList = new JoystickDevice[joystickCount + 1];
    for (uint8_t i = 0; i < joystickCount; i++)
    {
        newJoystickList[i] = joystickList[i];
    }
    newJoystickList[joystickCount] = joy;
    delete[] joystickList;
    joystickList = newJoystickList;
    joystickCount++;
    LOG_APP("Found joystick: %s (%d axis, %d buttons)",
            joy.device.szPname, joy.device.wNumAxes, joy.device.wNumButtons);
}