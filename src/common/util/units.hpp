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

#include <string>
#include <cstring>
#include "../../common/gpufont.hpp"
#include "../../common/gpu.hpp"

/* Todo list:
    [ ] - Get relative units. Need font size, object size etc.
*/

namespace units
{
    enum UnitType
    {
        // CSS Style, for spicy's pleasure.
        px,
        em,
        ch,
        rem,
        vw,
        vh,
        vmin,
        vmax,
        perc,
        // Total size
        UnitTypeCount,
        UnitInvalid
    };

    inline constexpr size_t percentToSizeT(uint8_t x, size_t out_max)
    {
        if (x > 100)
            x = 100;
        return x * out_max / 100;
    }

    inline constexpr size_t percentOf(uint8_t x, size_t start, size_t end)
    {
        if (x > 100)
            x = 100;
        return x * (end - start) / 100;
    }

    inline constexpr uint8_t percentToShort(uint8_t x)
    {
        return percentToSizeT(x, UINT8_MAX);
    }

    inline constexpr uint8_t percentToInt(uint8_t x)
    {
        return percentToSizeT(x, UINT16_MAX);
    }

    int unitStrToType(const char *str);
    size_t unitStrToPx(gpu::Context &ctx, const char *str, size_t dpi);
}