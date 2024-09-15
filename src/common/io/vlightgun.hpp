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

#include <memory>
#include <vector>
#include <functional>

#include "postbox.hpp"
#include "devhandler.hpp"
#include "hw/gpu.hpp"
//#include "common/ui/layers/vlightgun.hpp"

/*
    Virtual Lightgun class for UI
*/

/*
    Todo:
    [ ] - The lot

*/

namespace vLightgun
{
    class Crosshair
    {
    private:
        bool _visible = false;
        int _x = 0, _y = 0;

    public:
        gpu::Image cursor;

        void show() { _visible = true; }  // Show the cursor. Also enables mouse functions
        void hide() { _visible = false; } // Hide the cursor. Note that this effectively disables the cursor.
        
        void draw(gpu::Context &ctx){}
    };

    class Context
    {
    private:
        std::vector<std::unique_ptr<Crosshair>> _crosshairList;
        std::vector<Device::LightgunHandler*> _devices;

    public:
        post::PostBox* _outBox;
        post::PostBox _inBox;

        Context(post::PostBox *outBox);

        int init();
        int reload();
        void update();

        void addLightgun();
        void removeLightgun();
        
        void addDevice(Device::LightgunHandler *device) { 
            _devices.push_back(device);
            _devices.back()->_outBox = &_inBox;
        }
    };

}