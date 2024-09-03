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
*/

namespace locale
{
    enum LocaleIndex
    {
        US,
        GB,
        IT,
        LOCALE_END
    };

    enum LocaleShift : bool {
        KeyNormal,
        KeyShift
    };

    constexpr const char *us[4][2] = {
        {"1234567890-=", "!@#$%^&*()_+"},
        {"qwertyuiop[]", "QWERTYUIOP{}"},
        {"asdfghjkl;'\\", "ASDFGHJKL:\"|"},
        {"\\zxcvbnm,./", "|ZXCVBNM<>?"}};

    constexpr const char *gb[4][2] = {
        {"1234567890-=", "!\"£$%^&*()_+"},
        {"qwertyuiop[]", "QWERTYUIOP{}"},
        {"asdfghjkl;'#", "ASDFGHJKL:@~"},
        {"\\zxcvbnm,./", "|ZXCVBNM<>?"}};
    
    constexpr const char *it[4][2] = {
        {"\\1234567890ì", "\\!\"£$%/()=+?^"},
        {"qwertyuiopè+", "QWERTYUIOPé*"},
        {"asdfghjklòàù", "ASDFGHJKLç#§"},
        {"<zxcvbnm,.-", ">ZXCVBNM;:_"}};
    

    constexpr const char* const (*locales[])[2] = {
        us, gb, it
    };

    constexpr const char *getKeyRow(int row, LocaleIndex locale, LocaleShift shift){
        if(row < 4)
            return locales[locale][row][shift];
        else
            return nullptr;
    }
}