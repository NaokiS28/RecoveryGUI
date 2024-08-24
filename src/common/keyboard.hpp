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
#include "iohandler.hpp"

/*
    Todo:
    [ ] - Process all important messages
    [ ] - Create virtual inputs from keyboard
    [ ] - Multiple keyboard support? (Requires Raw Input, unlikely to happen)
*/

class KeyboardHandler : public InputHandler
{
private:


public:
    KeyboardHandler(){}
    void processKeyDown(WPARAM p);
    void processKeyUp(WPARAM p);
    int init();
    int update();
    int getInputs(DeviceInputs &dev);
    int processMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};