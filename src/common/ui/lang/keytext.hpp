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

/*
    Todo:
    [ ] - Add support for more keyboard locales
    [ ] - Convert into a loaded file
*/

namespace locale
{    
    constexpr const char *numPadText[] = {
        "789+456-1230."
    };

    constexpr const char *konamiNumPadText[] = {
        "789456123000"
    };
    
    constexpr const char *gbText[] = {
        "`1234567890-=qwertyuiop[]asdfghjkl;'#\\zxcvbnm,./",    // Normal
        "¬!\"£$%^&*()_+QWERTYUIOP{}ASDFGHJKL:@~|ZXCVBNM<>?",    // Shift
        "`123€567890-=qwertyuiop[]asdfghjkl;'#\\zxcvbnm,./",    // AltGr
        "¬!\"€$%^&*()_+QWERTYUIOP{}ASDFGHJKL:@~|ZXCVBNM<>?"     // AltGr + Shift
    };

    constexpr const char *usText[] = {
        "`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./",
        "~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?",
        "`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./",
        "~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?" 
    };

    constexpr const char *itText[] = {
        "\\1234567890`ìqwertyuiopè+asdfghjklòàù<zxcvbnm,.-", 
        "|!\"£$%&/()?^QWERTYUIOPé*ASDFGHJKLç#§>ZXCVBNM;:_",
        "\\1234€67890`ìqw€rtyuiop[]asdfghjkl@#ù<zxcvbnm,.-",
        "|!\"£$€&/()?^QW€RTYUIOP{}ASDFGHJKLç#§>ZXCVBNM;:_"
    };

    constexpr const char *CtrlKeyText = "Ctrl";
    constexpr const char *AltKeyText = "Alt";
    constexpr const char *OSKeyText = "⊞\0⌘\0❖";
    
}