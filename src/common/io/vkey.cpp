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

#include "vkey.hpp"

namespace vKey
{

    Context::Context(post::PostBox *outBox) : _outBox(outBox)
    {
        _enabled = false;
        _physicalPresent = false;
        _locale = locale::US;
    }

    void Context::_checkPhysicalPresent(){
        bool physicalDevice = false;
        for (auto& dev : _devices){
            int type = dev->getKeyboardType(0);
            if(type != Mouse::None && type != Mouse::Virtual){
                physicalDevice = true;
            }
        }
        _physicalPresent = physicalDevice;
    }

    void Context::update(){
        for (auto& dev : _devices){
            dev->update();
        }
        if(_inBox.available()){
            post::InputMsgStruct msg;
            _inBox.getMessage(&msg);
        }
    }

    int Context::init(){
        int result = 0;
        for (auto &dev : _devices)
        {
            result += dev->init();
        }
        _checkPhysicalPresent();
        return result;
    }

    int Context::reload(){
        int result = 0;
        for (auto &dev : _devices)
        {
            result += dev->reload();
        }
        return result;
    }
}