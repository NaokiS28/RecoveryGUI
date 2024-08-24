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

#include <stdint.h>
#include <vector>
#include "file/file.hpp"
#include "util/log.hpp"
#include "common/io.hpp"
#include "hw/keyboard.hpp"
#include "hw/joystick.hpp"
#include "hw/mouse.hpp"
#include "hw/jvs.hpp"

namespace io
{
    Player::Player(size_t number, size_t digital, size_t analog)
    {
        number = number;
        digitalCount = digital;
        analogCount = analog;
        if(digitalCount) 
            digitalMap = new uint32_t[digitalCount];
        if(analogCount)
            analogMap = new uint32_t[analogCount];
    }

    Context::Context()
    {
        LOG_APP("Init IO system");
        mouse.init();
        keyboard.init();
        joystick.init();
        // lightgun.init();
        // jvs.init();
        // jamma.init();
    }

    int Context::init()
    {
        // int result;
        //DeviceInputs device;
        /*if((result = mouse->getInputs(device)) == INPUT_DEVICE_OK){
            for(uint16_t a = 0; a < device.getNumAnalogInputs(); a++)
                _inputList.push_back(device.analogArray[a]);
            for(uint16_t d = 0; d < device.getNumDigitalInputs(); d++)
                _inputList.push_back(device.digitalArray[d]);
        }*/

        return 0;
    }

    int Context::update()
    {
        joystick.update();
        mouse.update();
        // jvs->update();
        return 0;
    }

    /*
    bool Context::getRawInputState(uint32_t _idx) const {
        return _inputList[_idx]->getState();
    }

    int16_t Context::getRawInputValue(uint32_t _idx) const {
        return _inputList[_idx]->getValue();
    }
    */

    int Context::newMapping(uint32_t button, uint32_t *mapping, uint8_t size)
    {
        VirtualIO::KeyMap map(button);
        for (; size > 0; size--)
        {
            // uint8_t modifier = (((uint32_t)mapping[size] >> 24) & 0xFF);
            /*
            switch (modifier)
            {
            case IO_MAP_OR:

                break;
            default:
                break;
            }
            */
        }
        //_map.push_back(map);
        return 0;
    }

    int Context::editMapping(uint32_t button, uint32_t *mapping, uint8_t size)
    {
        return 0;
    }
}
