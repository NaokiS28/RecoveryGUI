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

#include "keylayout.hpp"
#include "keytext.hpp"

namespace locale
{
    struct LocaleStruct
    {
        const int supportedMods;
        const int localeIndex;
        const char *flag;               // Use only Unicode flag
        const char *langName;           // Native language name
        const char *keyboardName;       // e.g. en-GB
        const KeyboardLayout keyLayout; // One of the six keyboard layouts
        const char *const *keyText;     // String of keyboard character keys
    };

    enum LocaleIndex
    {
        US,
        GB,
        FR,
        DE,
        ES,
        HR,
        IT,
        JP,
        KR,
        LOCALE_END,
        NumberPad = 100
    };

    static LocaleStruct enUS = {
        (locale::ModShift | locale::ModAltGr),
        US,
        "🇺🇸", // This is actually the US flag emoji
        "English (US)",
        "en-US",
        locale::KeyLayout_ANSI,
        usText};

    static LocaleStruct enGB = {
        (locale::ModShift | locale::ModAltGr),
        GB,
        "🇬🇧",
        "English (GB)",
        "en-GB",
        locale::KeyLayout_ISO,
        gbText};

    static LocaleStruct itIT = {
        (locale::ModShift | locale::ModAltGr),
        IT,
        "🇮🇹",
        "Italiano",
        "it-IT",
        locale::KeyLayout_ISO,
        itText};

    static LocaleStruct jpJP = {
        (locale::ModShift | locale::ModAltGr),
        JP,
        "🇯🇵",
        "日本語",
        "jp-JP",
        locale::KeyLayout_JIS,
        nullptr};

    static LocaleStruct hrHR = {
        (locale::ModShift | locale::ModAltGr),
        HR,
        "🇭🇷",
        "Hrvatski",
        "hr-HR",
        locale::KeyLayout_ISO,
        nullptr};

    static LocaleStruct frFR = {
        (locale::ModShift | locale::ModAltGr),
        FR,
        "🇫🇷",
        "Français",
        "fr-FR",
        locale::KeyLayout_ISO,
        nullptr};

    static LocaleStruct deDE = {
        (locale::ModShift | locale::ModAltGr),
        DE,
        "🇩🇪",
        "Deutsch",
        "de-DE",
        locale::KeyLayout_ISO,
        nullptr};

    static LocaleStruct esES = {
        (locale::ModShift | locale::ModAltGr),
        ES,
        "🇪🇸",
        "Español",
        "es-ES",
        locale::KeyLayout_ISO,
        nullptr};

    static LocaleStruct krKR = {
        (locale::ModShift | locale::ModAltGr),
        KR,
        "🇰🇷",
        "한국어",
        "kr-KR",
        locale::KeyLayout_KS,
        nullptr};

    static LocaleStruct numPad = {
        locale::ModNone,
        NumberPad,
        "",
        "",
        "",
        locale::KeyLayout_NumPad,
        numPadText};
}