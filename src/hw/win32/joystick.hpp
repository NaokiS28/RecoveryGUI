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

#include <vector>
#include <memory>
#include <string>
#include <windows.h>

#include "common/util/log.hpp"
#include "common/io/devhandler.hpp"
#include "common/io/inputdefs.hpp"

using namespace Device;

class JoystickDevice
{
    // Win32 Joystick API wrapper
public:
    JoystickDevice() {}
    JoystickDevice(int _id, JOYINFOEX _i, JOYCAPS _c)//, DeviceMeta _m)
    {
        id = _id;
        state = _i;
        device = _c;
        //meta = _m;
    }
    uint32_t getInputs() { return state.dwButtons; }
    int id = 0;
    JOYINFOEX state;
    JOYCAPS device;
    //DeviceMeta meta;
};

class Win32Joy : public JoystickHandler
{
private:
    bool polledMode = true; // Joysticks can use messaging or polling.
    std::vector<JoystickDevice> joystickList;
    void _newJoystick(JoystickDevice &joy);
    void _reset();

public:
    Win32Joy() { _deviceClasses[0] = Device::JoystickClass; }
    ~Win32Joy() { _reset(); }

    int init() override;
    void update() override;
    int reload() override;

    const char *getDeviceName(int idx);
    int getDeviceCount() override { return joystickList.size(); }

    bool getSwitch(uint32_t code) override;
    int getAnalog(uint32_t code) override;
    int getRelative(uint32_t code) override { return 0; }

    int getSwitchCount(uint32_t code) override { return joystickList[Input::getInputDevId(code)].device.wNumButtons; }
    int getAnalogCount(uint32_t code) override { return joystickList[Input::getInputDevId(code)].device.wNumAxes; }
    int getRelativeCount(uint32_t code) override { return 0; }
};

REGISTER_DEVICE_HANDLER(Win32Joy)