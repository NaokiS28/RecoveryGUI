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

#include "units.hpp"

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

    int unitStrToType(const char *str)
    {
        // Array of strings corresponding to enum values
        const char *unitStrings[UnitTypeCount] = {
           "px", "em", "ch", "rem", "vw", "vh", "vmin", "vmax", "%"};

        for (int i = 0; i < UnitTypeCount; ++i)
        {
            if (std::strcmp(str, unitStrings[i]) == 0)
            {
                return static_cast<UnitType>(i);
            }
        }

        // If not found, handle the error as needed. Here, we return UnitTypeCount (invalid).
        return UnitInvalid;
    }

    size_t unitStrToPx(gpu::Context &ctx, const char *str, size_t dpi)
    {
        int hRes = ctx.getHorizontalRes();
        int vRes = ctx.getVerticalRes();
        float input = std::stof(str);
        size_t unitPx = 0;
        int type = units::unitStrToType(str);
        switch (type)
        {
        case px:
            unitPx = static_cast<int>(input);
            break;
        case em:
            break;
        case ch:
            break;
        case vw:
            unitPx = percentOf(1, 0, hRes);
            break;
        case vh:
            unitPx = percentOf(1, 0, vRes);
            break;
        case vmin:
        {
            if (vRes < hRes)
                unitPx = percentOf(1, 0, vRes);
            else
                unitPx = percentOf(1, 0, hRes);
        }
        break;
        case vmax:
        {
            if (vRes > hRes)
                unitPx = percentOf(1, 0, vRes);
            else
                unitPx = percentOf(1, 0, hRes);
        }
        break;
        case perc:
            //val = percentOf(val, 0, )
            break;
        default:
            return 0;
        }
        return unitPx;
    }
}