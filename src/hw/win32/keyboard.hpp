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
#include "common/io/devhandler.hpp"

using namespace Device;

/*
    Todo:
    [ ] - Process all important messages
    [ ] - Create virtual inputs from keyboard
    [ ] - Multiple keyboard support? (Requires Raw Input, unlikely to happen)
*/

class Win32Keyboard : public KeyboardHandler
{
private:
    int keyboardCount = 0;


public:
    Win32Keyboard() { _deviceClasses[0] = DeviceClass::KeyboardClass; }
    void processKeyDown(WPARAM p);
    void processKeyUp(WPARAM p);

    int init();
    void update();
    int reload() { return 0; }

    void type(const char *c){}

    int getDeviceCount() { return keyboardCount; }

    int getKeyboardType(int idx) { return Keyboard::AlphaNumeric; }

    int processMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

REGISTER_DEVICE_HANDLER(Win32Keyboard)