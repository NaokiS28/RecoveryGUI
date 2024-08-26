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

#include "common/io.hpp"

namespace io
{
    Player::Player(size_t number, size_t digital, size_t analog)
    {
        number = number;
        digitalCount = digital;
        analogCount = analog;
        if (digitalCount)
            digitalMap = new uint32_t[digitalCount];
        if (analogCount)
            analogMap = new uint32_t[analogCount];
    }

    Context::Context()
    {
        // Registers all device classes that have been included in the build
        for (const auto &factory : getDeviceHandlerFactories())
        {
            DeviceHandler *device = factory();
            _devices.push_back(device);
        }
    }

    Context::~Context()
    {
        // Delete all device classes init'd during construction
        for (auto *device : _devices)
        {
            delete device;
        }
    }

    int Context::init()
    {
        // Init all handlers. Returns how many handlers were sucessfully init'd.
        // If any handlers fail to init, INIT_BASE is set in result, and the last
        //  handler index that failed is returned.
        int count = 0;
        int result = 0;
        for (auto &dev : _devices)
        {
            if (dev->init() == 0)
            {
                result++;
            }
            else
            {
                result += (INIT_BASE + (count << 4));
            }
            count++;
        }
        return result;
    }

    int Context::update()
    {
        // Run through the handler updates
        for (auto &dev : _devices)
        {
            dev->update();
        }
        return 0;
    }

    size_t Context::getDeviceCount(uint32_t classCode)
    {
        size_t count = 0;
        for(auto& dev : _devices){
            if(dev->getClassType() == classCode){
                count += dev->getDeviceCount();
            }
        }
        return count;
    }

    int Context::getRawInputCount(uint32_t code){
        int type = VirtualIO::getInputType(code);
        int classCode = VirtualIO::getInputSrcType(code);
        for(auto& dev : _devices){
            if(dev->getClassType() == classCode){
                switch(type){
                    case VirtualIO::INPUT_TYPE_ANALOG: return dev->getAnalogCount(code);
                    case VirtualIO::INPUT_TYPE_DIGITAL: return dev->getSwitchCount(code);
                    case VirtualIO::INPUT_TYPE_ROTARY: return dev->getRelativeCount(code);
                    default: return 0;
                }
            }
        }
        return 0;
    }

    bool Context::getRawInputState(uint32_t code){
        int classCode = VirtualIO::getInputSrcType(code);
        for(auto& dev : _devices){
            if(dev->getClassType() == classCode){
                return dev->getSwitch(code);
            }
        }
        return 0;
    }

    int16_t Context::getRawInputValue(uint32_t code){
        int classCode = VirtualIO::getInputSrcType(code);
        for(auto& dev : _devices){
            if(dev->getClassType() == classCode){
                return dev->getAnalog(code);
            }
        }
        return 0;
    }

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
