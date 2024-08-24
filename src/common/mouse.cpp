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

#include <stddef.h>
#include <stdint.h>
#include <windows.h>
#include "mouse.hpp"

int MouseHandler::init()
{
    //mouseX = AnalogInput("MOUSE_X_AXIS", INPUT_SRC_MOUSE, 0);
    //mouseY = AnalogInput("MOUSE_Y_AXIS", INPUT_SRC_MOUSE, 0);
    //mouseScroll = AnalogInput("MOUSE_SCROLL_AXIS", INPUT_SRC_MOUSE, 0);
    //mouseL = DigitalInput("MOUSE_LEFT_CLICK", INPUT_SRC_MOUSE, 0);
    //mouseM = DigitalInput("MOUSE_MIDDLE_CLICK", INPUT_SRC_MOUSE, 0);
    //mouseR = DigitalInput("MOUSE_RIGHT_CLICK", INPUT_SRC_MOUSE, 0);
    return 0;
}

int MouseHandler::update()
{
    
    return 0;
}

int MouseHandler::getInputs(DeviceInputs &dev)
{
    /*
    dev.devName = "MOUSE";
    dev.devID = 0;
    dev.analogArray.push_back(&mouseX);
    dev.analogArray.push_back(&mouseY);
    dev.analogArray.push_back(&mouseScroll);
    dev.digitalArray.push_back(&mouseL);
    dev.digitalArray.push_back(&mouseR);
    dev.digitalArray.push_back(&mouseM);
    */
    return INPUT_DEVICE_OK;
}

int MouseHandler::processMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SETCURSOR:
    {
        if (LOWORD(lParam) == HTCLIENT)
        {
            // Cursor is in the client area; hide the cursor if not already hidden
            if (!isCursorHidden)
            {
                ShowCursor(FALSE);
                isCursorHidden = true;
            }
        }
        else
        {
            // Cursor is in a non-client area (e.g., title bar); show the cursor
            if (isCursorHidden)
            {
                SetCursor(LoadCursor(NULL, IDC_ARROW));
                ShowCursor(TRUE);
                isCursorHidden = false;
            }
        }
        return 0;
    }
    case WM_MOUSEWHEEL:
    {
        int16_t val = static_cast<int16_t>(HIWORD(wParam));
        int16_t scrollVal = 0; //mouseScroll.getValue();
        int32_t newScrollVal = static_cast<int32_t>(scrollVal) + val;

        if (newScrollVal > INT16_MAX)
        {
            //mouseScroll.setValue(INT16_MAX);
        }
        else if (newScrollVal < INT16_MIN)
        {
            //mouseScroll.setValue(INT16_MIN);
        }
        else
        {
            //mouseScroll.setValue(static_cast<int16_t>(newScrollVal));
        }

        return 0;
    }
    case WM_MOUSEMOVE:
    {
        //mouseX.setValue((int16_t)lParam & 0xFFFF);
        //mouseY.setValue((int16_t)(lParam >> 16) & 0xFFFF);
        return 0;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
