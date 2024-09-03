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
        //LOG_APP("No joystick driver installed.");
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
            
            LOG_APP("Found joystick: %s (%d axis, %d buttons)",
                    joystickDevice.device.szPname, 
                    joystickDevice.device.wNumAxes, 
                    joystickDevice.device.wNumButtons);
            _newJoystick(joystickDevice);
            
        }
    } 
    return 0;
}

void Win32Joy::update()
{
    //uint16_t result = JOYERR_NOERROR;

    if (polledMode)
    {
        for (auto &joy : joystickList)
        {
            //uint16_t status = 
            joyGetPosEx(joy.id, &joy.state);
            /*if (status > result)
                result = (joy.id * 10) + status;
                */
        }
    }

    //return result;
}

void Win32Joy::_reset(){ 
    for(auto& dev : joystickList){
        _outBox->postMessage(Input::IM_DEVICE_DISCONNECT, dev.id);
    }
    joystickList.erase(joystickList.begin(), joystickList.end());
}

int Win32Joy::reload()
{
    _reset();
    init();
    return 0;
}

const char* Win32Joy::getDeviceName(int idx){
    if((size_t)idx < joystickList.size()){
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

int Win32Joy::getAnalog(uint32_t code)
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

void Win32Joy::_newJoystick(JoystickDevice &joy){
    joystickList.push_back(joy);

}