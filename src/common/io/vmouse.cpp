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

#include "vmouse.hpp"

namespace vMouse
{
    Context::Context(post::PostBox *outBox) : _outBox(outBox)
    {
        _physicalPresent = false;
    }

    void Context::addMouse()
    {
    }

    void Context::removeMouse()
    {
    }

    void Context::_checkPhysicalPresent()
    {
        bool physicalDevice = false;
        for (auto &dev : _devices)
        {
            int type = dev->getMouseType(0);
            if (type != Mouse::None && type != Mouse::Virtual)
            {
                physicalDevice = true;
            }
        }
        _physicalPresent = physicalDevice;
    }

    void Cursor::draw(gpu::Context &ctx)
    {
        cursor.draw(ctx, _x, _y);
    }

    void Cursor::click(int button)
    {
        if (_visible && _onClick != nullptr)
        {
            if (button <= CURSOR_MAX_BUTTON && button != CURSOR_NULL)
            {
                _onClick(button);
            }
        }
    }

    void Cursor::scroll(int delta)
    {
        if (_visible && _onClick != nullptr)
        {
            _onScroll(
                (delta > 0) ? CURSOR_SCROLL_UP : CURSOR_SCROLL_DOWN,
                delta);
        }
    }
}