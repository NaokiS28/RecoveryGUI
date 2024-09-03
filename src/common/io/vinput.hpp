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
#include <queue>
#include <memory>
#include <map>

#include "vjoy.hpp"
#include "vmouse.hpp"
#include "vkey.hpp"
#include "vlightgun.hpp"

#include "postbox.hpp"
#include "inputdefs.hpp"
#include "devhandler.hpp"

/*
    Virtual Input

    Collates the Device Handlers and oversees the various sub systems
    that make up the input system.
*/

/*
    Todo:
    [ ] - The lot, still very much a skeleton driver.

*/

namespace vInput
{
    constexpr const int GenericMap[] = {
        Input::PLAYER_BTN_UP,
        Input::PLAYER_BTN_DN,
        Input::PLAYER_BTN_LF,
        Input::PLAYER_BTN_RT,
        0,
    };

    class Context
    {
    private:
        post::PostBox _inBox;
        std::vector<Device::DeviceHandler *> _devices;

        vJoy::Context _joyClass = vJoy::Context(&_inBox);
        vMouse::Context _mouseClass = vMouse::Context(&_inBox);
        vKey::Context _keyboardClass = vKey::Context(&_inBox);
        vLightgun::Context _lightgunClass = vLightgun::Context(&_inBox);

        void _processMessage();

    public:
        Context();
        ~Context() {}

        int init();
        int reload();
        void update();

        Device::KeyboardHandler* getKeyDevPtr(const char* name){
            return _keyboardClass.getKeyboardPtr(name);
        }

        friend class vJoy::Context;
        friend class vMouse::Context;
        friend class vKey::Context;
        friend class vLightgun::Context;
    };

}