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

#pragma once
#include <vector>
#include "virtualio.hpp"

enum InputDeviceStatus : int {
    INPUT_DEVICE_OK,
    INPUT_DEVICE_BASE = 100,
    INPUT_DEVICE_NOTREADY,
    INPUT_DEVICE_INIT,
    INPUT_DEVICE_NOTATTACHED,
    INPUT_DEVICE_NOTAVAILABLE,
};

struct DeviceInputs {
    const char *devName = nullptr;
    uint8_t devID = 0;
    /*
    std::vector<VirtualIO::AnalogInput*> analogArray;
    std::vector<VirtualIO::DigitalInput*> digitalArray;
    size_t getNumDigitalInputs() const {
        return digitalArray.size();
    }

    size_t getNumAnalogInputs() const {
        return analogArray.size();
    }
    */
};

class InputHandler
    {
    public:
        virtual int init() { return 0; }
        virtual int update() { return 0; }
        virtual int reload() { return 0; }
        virtual int getInputs(DeviceInputs &dev) { return INPUT_DEVICE_NOTAVAILABLE; }
    };