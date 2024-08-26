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

#include "hw/gpu.hpp"

/*
    Virtual Mouse/Cursor class for UI
*/

/*
    Todo:
    [ ] - Map virtual mouse to physical mice

*/

namespace VirtualMouse
{
    enum MouseActions{
        CURSOR_NULL,
        CURSOR_LEFT_CLICK,
        CURSOR_RIGHT_CLICK,
        CURSOR_MIDDLE_CLICK,
        CURSOR_MAX_BUTTON = CURSOR_MIDDLE_CLICK,
        CURSOR_SCROLL_DOWN,
        CURSOR_SCROLL_UP,
        CURSOR_MAX_SCROLL = CURSOR_SCROLL_UP,
    };

    class Cursor {
    private:
        bool _visible = false;
        int _x = 0, _y = 0;
        void (*_onClick)(int) = nullptr;
        void (*_onScroll)(int, int) = nullptr;
    public:
        gpu::Image cursor;

        void show(){ _visible = true; }         // Show the cursor. Also enables mouse functions
        void hide(){ _visible = false; }        // Hide the cursor. Note that this effectively disables the cursor.
        // Specify the function called when cursor left/right clicks (int = click)
        void setClickFunc(void (*ptr)(int)){ _onClick = ptr; }
        // Specify the function called when cursor wheel is scrolled (int scrollUp/down, int delta)
        void setScrollFunc(void (*ptr)(int, int)){ _onScroll = ptr; }

        void click(int button);
        void scroll(int delta);

        void update();
        void draw(gpu::Context &ctx);
    };
    
}