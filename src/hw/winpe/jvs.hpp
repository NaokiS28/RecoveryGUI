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
    Notes:
        WinPE does not support serial ports, JVS cannot be used
*/

#pragma once
#include "common/iohandler.hpp"

class JVSHandler : public InputHandler
{
public:
    JVSHandler(const char* port = nullptr) { }
    int init() { return 0; }
    int isReady(){ return 0; }
    int update() { return 0; }
    //int getInputs(DeviceInputs &dev);
};