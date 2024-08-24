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

#include <stdint.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include <memory>
#include "virtualio.hpp"

namespace VirtualIO
{

    /*
    void InputGroup::addInput(Input &i)
    {
        //inputArr.push_back(std::make_unique<Input>(i));
    }

    void InputGroup::removeInput(Input &i)
    {
    }
    */

    bool InputGroup::getState()
    {
        bool val = false;
        if (!arrSize)
            return false;

        int8_t xorCount = 0;

        if (groupMode == IO_MAP_OR || groupMode == IO_MAP_AND || groupMode == IO_MAP_NAND || groupMode == IO_MAP_NOR)
        {
            for (uint8_t i = 0; i < arrSize; i++)
            {
                if (groupMode == IO_MAP_XOR)
                {
                    //if (inputArr[i]->getValue() != INPUT_STATE_INACTIVE)
                    //    xorCount++;
                }
                else
                {
                    //if (inputArr[i]->getValue() != INPUT_STATE_INACTIVE)
                    //    val = true;
                    //else if (groupMode == IO_MAP_AND)
                    //    val = false;
                }
            }
            if (groupMode == IO_MAP_NAND || groupMode == IO_MAP_NOR)
            {
                val = !val;
            }
            else if (groupMode == IO_MAP_XOR)
            {
                return ((xorCount > 0) && (xorCount < arrSize));
            }
        }

        return val;
    }
}