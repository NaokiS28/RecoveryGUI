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
#include <map>

#include "postbox.hpp"
#include "hw/gpu.hpp"

#include "inputdefs.hpp"
#include "devhandler.hpp"
#include "common/ui/lang/locale.hpp"

/*
    Virtual Keyboard + Overlay for UI

*/

/*
    Todo:
    [ ] - The lot

*/

namespace vKey
{
    typedef struct{
        uint8_t keyboardId;
        uint8_t classDevIdx = 0;
        uint8_t subDevIdx = 0;
    } KeyDev;

    class Context {
    private:
        bool _enabled = false;
        bool _physicalPresent = false;
        int _locale = locale::US;
        std::vector<std::map<int, char>> _charBuffer;       // <Keyboard Number, character>
        std::vector<Device::KeyboardHandler*> _devices;
        std::vector<KeyDev> _keyboardDevs;

        void _checkPhysicalPresent();

    public:
        post::PostBox* _outBox;
        post::PostBox _inBox;

        Context(post::PostBox *outBox);

        int init();
        int reload();
        void update();

        void setLocale(locale::LocaleIndex locale) { _locale = locale; }
        void show() { _enabled = (!_physicalPresent && true); }
        void hide() { _enabled = false; }

        void addKeyboard() { }
        void addDevice(Device::KeyboardHandler *device) { 
            _devices.push_back(device); 
            _devices.back()->_outBox = &_inBox;
        }
        
        int getKeyboardCount() { return _keyboardDevs.size(); }

        Device::KeyboardHandler* getKeyboardPtr(const char* name) const {
            for (auto& dev : _devices){
                if(!strcmp(name, dev->getDriverName())){
                    return dev;
                }
            }
            return nullptr;
        }
    };
    
}