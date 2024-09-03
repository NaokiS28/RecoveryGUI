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
#include <memory>

#include "postbox.hpp"
#include "inputdefs.hpp"
#include "devhandler.hpp"

/*
    Virtual Output
*/

/*
    Todo:
    [ ] - The lot

*/

namespace vOutput
{
    class Context {
    public:
        post::PostBox* _outBox;
        post::PostBox* _inBox;
        
        Context(post::PostBox *outBox) : _outBox(outBox){}
    };
    
}