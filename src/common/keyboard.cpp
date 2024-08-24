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
#include "io.hpp"
#include "keyboard.hpp"

int KeyboardHandler::init()
{
    return 0;
}

int KeyboardHandler::update()
{
    return 0;
}

int KeyboardHandler::getInputs(DeviceInputs &dev)
{
    return 0;
}

void KeyboardHandler::processKeyDown(WPARAM p)
{
    // int doSomething;
}

void KeyboardHandler::processKeyUp(WPARAM p)
{
}

int KeyboardHandler::processMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_KEYUP:
    {
        return 0;
    }
    case WM_KEYDOWN:
    {
        return 0;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}