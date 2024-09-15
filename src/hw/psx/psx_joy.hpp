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
    PSX Joypad class
    ===============================
    NaokiS28

    Notes:
        This is just a skeleton driver to show how to make a multi-device handler 
        as the PSX would have. The general gist is that this class should handle
        all PSX pad reading duties, differentiate between installed pad types
        and format the data as appropriate.

        When the controller changes, you should send a DM_DISCONNECT and then
        DM_CONNECT message so that the input handler knows to query connected
        devices.
*/

#pragma once

#include <vector>
#include "common/io/devhandler.hpp"
#include "common/io/inputdefs.hpp"

using namespace Device;

namespace PSX {
    enum PadTypes {
        None,
        Digital,
        DualAnalog,
        DualShock,
        Flightstick,
        Mouse,
        Keyboard,
        PocketStation,
        MemoryCard,
        GunCon,
        NeGcon,
        Jogcon,
        Multitap,
        TypeMax = Multitap
    };

    constexpr const char* const PadNames[] = {
        "None",
        "Digital",
        "DualAnalog",
        "DualShock",
        "Flightstick",  // Yes yes, "Sony Analog Joystick"... it's a flight stick.
        "Mouse",
        "Keyboard",
        "PocketStation" // Not an input, but since I imgine someone will want to know what memory card is inserted
        "MemoryCard"
        "GunCon",
        "NeGcon",
        "Jogcon",
        "Multitap"      // Similar to above, not strictly an input device, but could be utilised.
    };
}

class PSXJoy : public JoystickHandler, public MouseHandler, public LightgunHandler
{
public:
    int init() override { return 0; }
    void update() override {}
    int reload() override {}

    const char *getDriverName() override { return "PlayStation"; }
    const char *getDeviceName(int idx) override {}
    int getDeviceCount() override { return 0; }

    // Joypads
    bool getSwitch(uint32_t code) override {}
    uint32_t getDigital(uint32_t code) override {}
    int16_t getAnalog(uint32_t code) override {}
    int16_t getRelative(uint32_t code) override { return 0; }

    int getSwitchCount(uint32_t code) override { return 0; }
    int getAnalogCount(uint32_t code) override { return 0; }
    int getRelativeCount(uint32_t code) override { return 0; }

    // Mouse
    int getMouseType(int idx){ return Mouse::Relative; }

    // Keyboard
    int getKeyboardType(int idx){ return Keyboard::AlphaNumeric; }

    // Lightgun
    int getLightgunType(int idx){ return Lightgun::RasterScan; }
};

REGISTER_DEVICE_HANDLER(PSXJoy)