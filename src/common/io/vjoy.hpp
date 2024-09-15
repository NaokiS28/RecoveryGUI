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

#include <map>
#include <vector>
#include <cstring>

#include "postbox.hpp"
#include "inputdefs.hpp"
#include "devhandler.hpp"

/*
    Virtual Joystick for UI
    =======================
    
*/

/*
    Todo:
    [ ] - The lot

*/

namespace vJoy
{
    using namespace Input;
    
    typedef struct {
        uint8_t joyNumber = 0;
        uint8_t classDevIdx = 0;
        uint8_t subDevIdx = 0;
        uint8_t switchCount = 0;
        uint8_t analogCount = 0;
        uint8_t relativeCount = 0;
        Joystick::JoystickState state;
        //Joystick::JoystickState lastState;
    } vJoyDev;

    class Context {
    private:
        //std::map<int, 
        std::vector<Device::JoystickHandler*> _devices;
        std::vector<vJoyDev> _joystickDevs;
        uint8_t _joyDevCount = 0;       // This is deliberately seperate and does not decrement.

        int _getVJoyDevIdx(int classIdx, int devIdx);
        void _processMessage();

    public:
        post::PostBox* _outBox;
        post::PostBox _inBox;
        Context(post::PostBox *outBox) : _outBox(outBox){}

        int init();
        int reload();
        void update();

        void addDevice(Device::JoystickHandler *device);
        int getJoystickCount() { return _joystickDevs.size(); }
        
        uint8_t getSwitchCount(uint8_t idx);
        uint8_t getAnalogCount(uint8_t idx);
        uint8_t getRelativeCount(uint8_t idx);
        uint32_t getSwitch(uint8_t idx);
        int16_t getAnalog(uint8_t idx, uint8_t channel);
        int16_t getRelative(uint8_t idx, uint8_t channel);
        const char *getDriverName(uint8_t idx);
        const char *getJoystickName(uint8_t idx);

        Device::JoystickHandler* getJoyPtr(const char* name) const {
            for (auto& dev : _devices){
                if(!strcmp(name, dev->getDriverName())){
                    return dev;
                }
            }
            return nullptr;
        }
    };
    
}