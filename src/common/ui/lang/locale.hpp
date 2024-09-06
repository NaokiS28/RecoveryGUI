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

#include <array>
#include "langstructs.hpp"

/*
    Todo:
    [ ] - Add support for more keyboard locales
*/

namespace locale
{      
    static std::array<LocaleStruct, LOCALE_END> locales = {
        enUS, enGB, frFR, deDE, esES, hrHR, itIT, jpJP, krKR
    };

    inline constexpr bool supportsShift(const LocaleStruct &locale){
        return (locale.supportedMods & locale::ModShift);
    }

    inline constexpr bool supportsAltGr(const LocaleStruct &locale){
        return (locale.supportedMods & locale::ModAltGr);
    }

    inline constexpr bool supportsLang1(const LocaleStruct &locale){
        return (locale.supportedMods & locale::ModLang1);
    }

    inline constexpr bool supportsLang2(const LocaleStruct &locale){
        return (locale.supportedMods & locale::ModLang2);
    }

    inline constexpr bool supportsLang3(const LocaleStruct &locale){
        return (locale.supportedMods & locale::ModLang3);
    }

    inline const char *getKeyText(const locale::LocaleStruct &locale, int mod = 0 )
    {
        int row = 0;
        row += ((locale::supportsShift(locale) && (mod & locale::ModShift)) ? locale::ModShift : 0);
        row += ((locale::supportsAltGr(locale) && (mod & locale::ModAltGr)) ? locale::ModAltGr : 0);
        row += ((locale::supportsLang1(locale) && (mod & locale::ModLang1)) ? locale::ModLang1 : 0);
        row += ((locale::supportsLang2(locale) && (mod & locale::ModLang2)) ? locale::ModLang2 : 0);

        return locale.keyText[row];
    }
}