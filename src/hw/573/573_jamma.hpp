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
    573 JAMMA class
    ===============================
    NaokiS28

    Notes:
        This is just a skeleton driver to show how to make an input class for
        the 573 JAMMA port. The general gist is that this class should handle
        all cabinet controls and in a way that will map from different cabinet
        wiring schemes to a standard interface.

        If the cab type changes, you should send a DM_DISCONNECT and then
        DM_CONNECT message so that the input handler knows to reload the input
        map if needed.
*/

#pragma once

#include <vector>
#include "common/io/devhandler.hpp"
#include "common/io/inputdefs.hpp"

using namespace Device;

namespace Arcade {
    enum CabTypes {
        JAMMA,
        DDR,
        DDRSolo,
        GuitarFreaks,
        DrumMania,
        GunMania,
        MaxType = GunMania
    };

    constexpr const char* const CabinetNames[] = {
        "JAMMA",
        "DDR",
        "DDR Solo",
        "GuitarFreaks",
        "DrumMania",
        "GunMania"
    };
}

class JAMMA573 : public JoystickHandler //, public LightgunHandler
{
private:
    int _cabType = Arcade::JAMMA;

public:
    int init() override { 
        _outBox->postMessage(Input::IM_DEVICE_CONNECT, 1);
        _outBox->postMessage(Input::IM_DEVICE_CONNECT, 2);
        return 0; 
    }
    void update() override {}
    int reload() override { return 0; }

    const char *getDeviceName(int idx) override { return Arcade::CabinetNames[_cabType]; }
    int getDeviceCount() override { return 1; }

    // JAMMA
    bool getSwitch(uint32_t code) override {}
    int getAnalog(uint32_t code) override {}
    int getRelative(uint32_t code) override { return 0; }

    int getSwitchCount(uint32_t code) override { return 0; }
    int getAnalogCount(uint32_t code) override { return 0; }
    int getRelativeCount(uint32_t code) override { return 0; }

};

REGISTER_DEVICE_HANDLER(JAMMA573)