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

/*
    Todo:
    [ ] - Load and draw graphics
    [ ] - Interface with IO vMouse
*/

#pragma once

#include "hw/gpu.hpp"
#include "common/ui/layerman.hpp"

namespace vMouse
{
    class vMouse : public layers::Layer
    {
    private:
        bool    _enabled = false;
        uint8_t _inputLocal;
        uint32_t offsetStep = 0;

    public:
        //gpu::Image tile;
        vMouse(int x, int y, int w, int h):
            Layer(x, y, w, h, layers::LayerType::Overlay, layers::LayerPriority::Top){}
        void draw(gpu::Context *ctx, gpu::Font *font, gpu::Color *color, uint32_t time) const;
        void resize(int hRes, int vRes){
            w = hRes;
            h = vRes;
        }
        void show(){ _enabled = true; }
        void hide(){ _enabled = false; }
    };
}