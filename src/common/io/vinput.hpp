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
    ===============
    Collates the Device Handlers and oversees the various sub systems
    that make up the input system. It is designed to be an interposing
    layer that will handle the input system for the main app.
*/

/*
    Todo:
    [ ] - The lot, still very much a skeleton.

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

        inline uint8_t getJoystickCount() { return _joyClass.getJoystickCount(); }
        inline const char* getJoystickName(uint8_t idx) { return _joyClass.getJoystickName(idx); }
        inline uint8_t getJoystickSwitch(uint8_t idx) { return _joyClass.getSwitchCount(idx); }
        inline bool getJoystickSwitch(uint8_t idx, uint8_t channel) { 
            return (_joyClass.getSwitch(idx) >> channel) & 0b1; 
        }
        inline int16_t getJoystickAnalog(uint8_t idx, uint8_t channel) { return _joyClass.getAnalog(idx, channel); }
        inline int16_t getJoystickRelative(uint8_t idx, uint8_t channel) { return _joyClass.getRelative(idx, channel); }
        inline uint8_t getJoystickSwitchCount(uint8_t idx) { return _joyClass.getSwitchCount(idx); }
        inline uint8_t getJoystickAnalogCount(uint8_t idx) { return _joyClass.getAnalogCount(idx); }
        inline uint8_t getJoystickRelativeCount(uint8_t idx) { return _joyClass.getRelativeCount(idx); }
        
        inline uint8_t getKeyboardCount() { return _keyboardClass.getKeyboardCount(); }

        inline uint8_t getMouseCount() { return _mouseClass.getMouseCount(); }

        inline Device::JoystickHandler* getJoyDevPtr(const char* name) const {
            return _joyClass.getJoyPtr(name);
        }

        inline Device::KeyboardHandler* getKeyDevPtr(const char* name) const {
            return _keyboardClass.getKeyboardPtr(name);
        }

        inline Device::MouseHandler* getMouseDevPtr(const char* name) const {
            return _mouseClass.getMousePtr(name);
        }

        friend class vJoy::Context;
        friend class vMouse::Context;
        friend class vKey::Context;
        friend class vLightgun::Context;
    };

}