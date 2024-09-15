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
    Windows (Win32) ACIO Class
    ===============================
    NaokiS28

    Notes:
        Skeleton driver for Konami ACIO devices on Bemani PCs.
*/

#pragma once

#include <vector>
#include <memory>
#include <string>
#include <windows.h>

#include "common/io/devhandler.hpp"
#include "common/io/inputdefs.hpp"

using namespace Device;

class JoystickDevice
{
    // Win32 Joystick API wrapper
public:
    JoystickDevice() {}

};

class Win32ACIO : public JoystickHandler
{
private:
    std::vector<JoystickDevice> _devList;

public:
    Win32ACIO() { _deviceClasses[0] = Device::JoystickClass; }
    ~Win32ACIO() {  }

    int init() override {};
    void update() override {};
    int reload() override {};

    const char *getDeviceName(int idx);
    int getDeviceCount() override { return _devList.size(); }

    bool getSwitch(uint32_t code) override;
    int getAnalog(uint32_t code) override;
    int getRelative(uint32_t code) override { return 0; }

    int getSwitchCount(uint32_t code) override { return 0; }
    int getAnalogCount(uint32_t code) override { return 0; }
    int getRelativeCount(uint32_t code) override { return 0; }
};

REGISTER_DEVICE_HANDLER(Win32ACIO)