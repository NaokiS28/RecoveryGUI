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
#include "common/virtualio.hpp"
#include "common/devhandler.hpp"

using namespace VirtualIO;

/*
    Todo:
    [ ] - Process all important messages
    [ ] - Create virtual inputs from mouse
    [ ] - Multiple mouse support? (Requires Raw Input)
*/

class MouseHandler : public DeviceHandler
{
private:
    //uint8_t mouseCount = 0;
    /*
    AnalogInput mouseX;
    AnalogInput mouseY;
    AnalogInput mouseScroll;
    DigitalInput mouseL;
    DigitalInput mouseM;
    DigitalInput mouseR;
    */
    int mouseCount = 0;
    bool isCursorHidden = false;

public:
    MouseHandler(){}
    inline bool getCursorHide() const { return isCursorHidden; }
    int init();
    int update();
    int reload() { return 0; }

    const char *getClassName() { return "MOUSE"; }
    int getDeviceCount() { return mouseCount; }
    
    int getClassType() { return INPUT_CLASS_MOUSE; }

    bool getSwitch(uint32_t code) { return false; }
    int getAnalog(uint32_t code) { return 0; }
    int getRelative(uint32_t code) { return 0; }
    int processMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

REGISTER_DEVICE_HANDLER(MouseHandler)