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

/*
    Windows (Win32) Joystick Class
    ===============================
    NaokiS28

    Notes:
        Win32 joysticks support a maximum of 32 buttons and 16 joypads.
        Joystick names will always be "Microsoft PC-joystick driver", it seems.
*/

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <windows.h>
#include "common/iohandler.hpp"
#include "common/virtualio.hpp"
#include "common/util/log.hpp"

class JoystickDevice
{
    // Win32 Joystick API wrapper
public:
    JoystickDevice() {}
    JoystickDevice(JOYINFOEX _i, JOYCAPS _c)
    {
        state = _i;
        device = _c;
    }
    uint32_t getInputs() { return state.dwButtons; }
    JOYINFOEX state;
    JOYCAPS device;
};

class JoystickHandler : public InputHandler
{
private:
    bool polledMode = false; // Joysticks can use messaging or polling.
    uint16_t joystickCount = 0;
    JoystickDevice *joystickList = nullptr;
    void _newJoystick(JoystickDevice &joy);

public:
    JoystickHandler(bool _poll = true) : polledMode(_poll) {}
    ~JoystickHandler() { delete joystickList; }

    int init();
    int update();
    
    int16_t getInput(uint32_t code);
};