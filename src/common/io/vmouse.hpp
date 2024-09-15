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

#include <vector>
#include <functional>

#include "postbox.hpp"
#include "devhandler.hpp"
#include "common/ui/layers/vmouse.hpp"

/*
    Virtual Mouse/Cursor class for UI
*/

/*
    Todo:
    [ ] - Map virtual mouse to physical mice

*/

namespace vMouse
{
    enum MouseActions
    {
        CURSOR_NULL,
        CURSOR_LEFT_CLICK,
        CURSOR_RIGHT_CLICK,
        CURSOR_MIDDLE_CLICK,
        CURSOR_MAX_BUTTON = CURSOR_MIDDLE_CLICK,
        CURSOR_SCROLL_DOWN,
        CURSOR_SCROLL_UP,
        CURSOR_MAX_SCROLL = CURSOR_SCROLL_UP,
    };

    class Cursor
    {
    private:
        bool _visible = false;
        int _x = 0, _y = 0;
        std::function<void(int)> _onClick = nullptr;
        std::function<void(int, int)> _onScroll = nullptr;

    public:
        gpu::Image cursor;

        void show() { _visible = true; }  // Show the cursor. Also enables mouse functions
        void hide() { _visible = false; } // Hide the cursor. Note that this effectively disables the cursor.

        // Specify the function called when cursor left/right clicks (int = click)
        void setClickFunc(void (*ptr)(int)) { _onClick = ptr; }
        // Specify the function called when cursor wheel is scrolled (int scrollUp/down, int delta)
        void setScrollFunc(void (*ptr)(int, int)) { _onScroll = ptr; }

        void click(int button);
        void scroll(int delta);
        
        void draw(gpu::Context &ctx);
    };

    typedef struct{
        uint8_t mouseId;
        uint8_t classDevIdx = 0;
        uint8_t subDevIdx = 0;
        Cursor cursor;
    } MouseDev;

    class Context
    {
    private:
        bool _physicalPresent = false;
        std::vector<Device::MouseHandler*> _devices;
        std::vector<MouseDev> _mouseDevs;
        
        void _checkPhysicalPresent();

    public:
        post::PostBox* _outBox;
        post::PostBox _inBox;

        Context(post::PostBox *outBox);

        int init();
        int reload();
        void update();

        void addMouse();
        void removeMouse();
        void addDevice(Device::MouseHandler *device) { 
            _devices.push_back(device);
            _devices.back()->_outBox = &_inBox;
        }

        int getMouseCount() { return _mouseDevs.size(); }

        Device::MouseHandler* getMousePtr(const char* name) const {
            for (auto& dev : _devices){
                if(!strcmp(name, dev->getDriverName())){
                    return dev;
                }
            }
            return nullptr;
        }
    };

}