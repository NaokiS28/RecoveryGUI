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

#include <windows.h>
#include <stdint.h>
#include "main/uibase.hpp"
#include "resources.h"

namespace win
{
    class Window
    {
    public:
    HWND CreateMainWindow(int hRes, int vRes);
        static LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            Window* window = nullptr;

        if (msg == WM_NCCREATE) {
            // Retrieve the instance pointer and store it in the user data
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            window = (Window*)cs->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
        } else {
            // Retrieve the instance pointer from the user data
            window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (window) {
            return window->MainWindowHandler(hwnd, msg, wParam, lParam);
        } else {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        }

        LRESULT MainWindowHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        Window(ui::Context* uiCtx) : uiCtx(uiCtx) {}

    private:
        ui::Context *uiCtx; // Store the context pointer here
    };
}