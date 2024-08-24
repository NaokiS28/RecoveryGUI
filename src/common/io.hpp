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

#include <stdint.h>
#include <windows.h>
#include <vector>
#include "file/file.hpp"
#include "util/log.hpp"
#include "virtualio.hpp"
#include "iohandler.hpp"
#include "hw/keyboard.hpp"
#include "hw/joystick.hpp"
#include "hw/mouse.hpp"
#include "hw/jvs.hpp"

/*
    IO overview:

    All input/output devices that are supported are grouped into categories. These
    are Mouse, Lightgun, Keyboard, Joystick e.t.c. In each of these categories
    the devices are sorted in a list. So requesting JOYSTICK_1_B5 would use the 
    first device in the Joystick list and return button 5.

    If you wanted a specific device to respond, based on a device ID or the like,
    then you would call ABSOLUTE_*hash*_B5, which would only return button 5 of
    that specific device, if it exists on the system.

    IO devices like JAMMMA and JVS are not included in the joystick list as they
    support more features than a HID joystick might typically support.

    Code mapping:
    Bits 0-7    = Input Index (255 inputs max)
    Bits 8-9    = Input Type (Digital, Analogue, Rotary, Keypad)
    Bits 12-15  = Return type (Digital, Analogue, Keycode)
    Bits 16-23  = Device ID (within category)
    Bits 24-31  = Device type category
*/

/* Todo list:
    [ ] - Support controller/keyboard map file/s
    [ ] - InputMapping - Export from app to file
    [ ] - Figure out whether to load IO DLLs? Or at least have a dynamic input list
    [ ] - General clean up
*/

namespace io
{
    enum PlayerNumbers {
        PLAYER_CABINET,
        PLAYER_1,
        PLAYER_2,
        PLAYER_3,
        PLAYER_4,
        PLAYER_5,
        PLAYER_6,
        PLAYER_7,
        PLAYER_8
    };
    
    /* Player Class
        This is the class that the application/game should use to tell
        the input system what requirements the game requires.
        Cabinet/Machine service inputs must be player 0.
        The maps store the 32-bit encoded input locator.
    */
    class Player
    {
    private:
        uint8_t number;
        uint8_t analogCount;
        uint8_t digitalCount;
        uint32_t *digitalMap;
        uint32_t *analogMap;    
    public:
        Player(){}
        Player(size_t number, size_t digital = 0, size_t analog = 0);
        ~Player(){
            delete digitalMap; delete analogMap;
        }

        void update(){}
        //int getInputs(){ return 0; }
        int getAnalogInputs(int idx, int count){ return 0; }
        int getDigitalInputs(int idx, int count){ return 0; }
    };

    class Context
    {
    private:
        std::vector<VirtualIO::KeyMap> _map;
        std::vector<Player*> _playerList;

    public:
        KeyboardHandler keyboard = KeyboardHandler();
        JoystickHandler joystick = JoystickHandler();
        MouseHandler mouse = MouseHandler();
        //LightgunHandler lightgun = LightGunHandler();
        //JVSHandler jvs = JVSHandler();
        //JAMMAHandler jamma = JAMMAHandler();

        Context();
        int init();
        int update();

        void addNewPlayer(Player* player){ _playerList.push_back(player); }
        size_t getPlayerListSize() const { return _playerList.size(); }

        bool getRawInputState(uint32_t _idx) const { return false; }
        int16_t getRawInputValue(uint32_t _idx) const { return 0; }

        int newMapping(uint32_t button, uint32_t *mapping, uint8_t size);
        int editMapping(uint32_t button, uint32_t *mapping, uint8_t size);
    
    };
}