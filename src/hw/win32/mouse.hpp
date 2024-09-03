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
//#include "common/io/virtualio.hpp"
#include "common/io/devhandler.hpp"

using namespace Device;

/*
    Todo:
    [ ] - Process all important messages
    [ ] - Create virtual inputs from mouse
    [ ] - Multiple mouse support? (Requires Raw Input)
*/

class Win32Mouse : public MouseHandler
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
    Win32Mouse() { _deviceClasses[0] = DeviceClass::MouseClass; }
    inline bool getCursorHide() const { return isCursorHidden; }

    int init();
    void update();
    int reload(){ return 0; }
    
    int getDeviceCount() { return mouseCount; }
    int getMouseType(int idx);

    int processMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

REGISTER_DEVICE_HANDLER(Win32Mouse)