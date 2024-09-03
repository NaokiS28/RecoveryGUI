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

#include "bg.hpp"

namespace uibg
{
    TiledBackground::TiledBackground(
        int x, int y, int w, int h,
        ScrollDirections d) : Layer(x, y, w, h,
                                    LayerType::Background,
                                    LayerPriority::Bottom),
                              _scrollDirection(d)
    {

    }

    void TiledBackground::draw(gpu::Context &ctx, uint32_t time) const
    {
        ctx.drawBackdrop(gpu::rgb(0,0,0));
        if (tile.valid)
        {
            int offsetX = uint32_t(time / 2) % tile.width;
            int offsetY = uint32_t(time / 3) % tile.height;

            for (int x = -offsetX; x < w; x += tile.width)
            {
                for (int y = -offsetY; y < h; y += tile.height)
                    tile.draw(ctx, x, y);
            }
        }
    }
}