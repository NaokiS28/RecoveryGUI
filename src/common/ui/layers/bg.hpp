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
#include "common/ui/layerman.hpp"

namespace uibg
{
    using namespace layers;
    
    enum ScrollDirections {
        Static,
        Up,
        UpRight,
        Right,
        DownRight,
        Down,
        DownLeft,
        Left,
        UpLeft
    };

    class TiledBackground : public Layer
    {
    private:
        uint32_t offsetStep = 0;
        ScrollDirections _scrollDirection;

    public:
        gpu::Image tile;
        TiledBackground(int x, int y, int w, int h, ScrollDirections d);
        void draw(gpu::Context &ctx, uint32_t time) const;
        void resize(int hRes, int vRes){
            w = hRes;
            h = vRes;
        }
    };
}