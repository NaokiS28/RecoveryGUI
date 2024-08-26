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

#include <stddef.h>
#include <stdint.h>
#include <windows.h>
#include "common/devhandler.hpp"

/*
    Todo:
    [ ] - Process all important messages
    [ ] - Create virtual inputs from keyboard
    [ ] - Multiple keyboard support? (Requires Raw Input, unlikely to happen)
*/

class KeyboardHandler : public DeviceHandler
{
private:
    int keyboardCount = 0;


public:
    KeyboardHandler(){}
    void processKeyDown(WPARAM p);
    void processKeyUp(WPARAM p);
    int init();
    int update();
    int reload() { return 0; }

    const char *getClassName() { return "KEYBOARD"; }
    int getDeviceCount() { return keyboardCount; }
    
    int getClassType() { return INPUT_CLASS_KEYBOARD; }

    bool getSwitch(uint32_t code) { return false; }
    int getAnalog(uint32_t code) { return 0; }
    int getRelative(uint32_t code) { return 0; }
    int processMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

REGISTER_DEVICE_HANDLER(KeyboardHandler)