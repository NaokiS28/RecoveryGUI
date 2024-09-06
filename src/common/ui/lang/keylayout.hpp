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

namespace locale
{ 
    enum LocaleKeyType
    {
        Key_Size = 100,
        Key_0_25U,
        Key_0_5U,
        Key_0_75U,
        Key_1U = 110,
        Key_1_25U,
        Key_1_5U,
        Key_1_75U,
        Key_2U = 120,
        Key_2_25U,
        Key_2_5U,
        Key_2_75U,
        Key_3U = 130,
        Key_3_25U,
        Key_3_5U,
        Key_3_75U,
        Key_4U = 140,
        Key_5U = 150,
        Key_6U = 160,
        Key_7U = 170,
        Key_UnitMaxSize,
        Key_Vert_Size = 200,
        Key_Vert_1U = 210,
        Key_Vert_1_25U,
        Key_Vert_1_5U,
        Key_Vert_1_75U,
        Key_Vert_2U = 220,
        Key_Vert_2_25U,
        Key_Vert_2_5U,
        Key_Vert_2_75U,
        Key_VertMaxSize,
        Key_Enter = 300,
        Key_EnterSpan,
        Key_EnterReverseSpan,
        Key_EnterSqaure,
        Key_EnterNumPad,
        Key_Enter_SecondRect,
        Key_EnterMaxSize,
        Key_ModKey = 400,
        CursorUp,
        CursorDown,
        CursorLeft,
        CursorRight,
        Tab,
        Enter,
        CapsLock,
        Space,
        Backspace,
        Shift,
        Blank,
        Diacritics,
        Lang1,
        Lang2,
        Lang3,
        Key_ModKeyMaxSize,
        Array_Mode = 500,
        NextRow,
        End
    };

    enum LocaleModifiers {
        ModNone,
        ModShift = 0x01,
        ModAltGr = 0x02,
        ModLang1 = 0x04,
        ModLang2 = 0x08,
        ModLang3 = 0x10,
        ModifierEnd
    };

    struct KeyboardLayout {
        const int rows;
        const int cols;
        const std::vector<int> keyLayout;  // Remove the initializer

        // Constructor to initialize the members
        KeyboardLayout(int r, int c, std::vector<int> keys)
            : rows(r), cols(c), keyLayout(std::move(keys)) {}
    };

    static const KeyboardLayout KeyLayout_ANSI = {
        5, 15,
        {
            Key_1U, 13, Key_2U, Backspace, NextRow,
            Key_1_5U, Tab, Key_1U, 12, Key_1_5U, 1, NextRow,
            Key_1_75U, CapsLock, Key_1U, 11, Key_2_25U, Enter, NextRow,
            Key_2_25U, Shift, Key_1U, 10, Key_2_75U, Shift, NextRow,
            Key_2U, Diacritics, Key_1U, Blank, 2, Key_5U, Space, 
            Key_2U, Blank, Key_1U, CursorLeft, CursorDown, CursorUp, CursorRight,
            End
        }
    };
    
    static const KeyboardLayout KeyLayout_ANSI_Alt = {
        5, 15,
        {
        Key_1U, 14, Backspace, NextRow,
        Key_1_5U, Tab, Key_1U, 12, Key_EnterReverseSpan, Enter, NextRow,
        Key_1_75U, CapsLock, Key_1U, 11, NextRow,
        Key_2_25U, Shift, Key_1U, 10, Key_2_75U, Shift, NextRow,
        Key_2U, Diacritics, Key_1U, Blank, 2, Key_5U, Space, 
        Key_2U, Blank, Key_1U, CursorLeft, CursorDown, CursorUp, CursorRight,
        End}
    };
    
    static const KeyboardLayout KeyLayout_ISO = {
        5, 15,
        {Key_1U, 13, Key_2U, Backspace, NextRow,
        Key_1_5U, Tab, Key_1U, 12, Key_EnterSpan, Enter, NextRow,
        Key_1_75U, CapsLock, Key_1U, 12, NextRow,
        Key_1_25U, Shift, Key_1U, 11, Key_2_75U, Shift, NextRow,
        Key_2U, Diacritics, Key_1U, Blank, 2, Key_5U, Space, 
        Key_1U, Blank, 2, CursorLeft, CursorDown, CursorUp, CursorRight,
        End}
    };
    
    static const KeyboardLayout KeyLayout_KS = {
        5, 15,
        {Key_1U, 14, Backspace, NextRow,
        Key_1_5U, Tab, Key_1U, 12, Key_EnterReverseSpan, Enter, NextRow,
        Key_1_75U, CapsLock, Key_1U, 11, NextRow,
        Key_2_25U, Shift, Key_1U, 10, Key_3U, Shift, NextRow,
        Key_1_5U, Diacritics, Key_1U, Blank, 3, Key_1U, Lang1, Key_3_5U, Space, 
        Key_1U, Lang2, Key_1U, Blank, CursorLeft, CursorDown, CursorUp, CursorRight,
        End}
    };
    
    static const KeyboardLayout KeyLayout_ABNT = {
        5, 15,
        {Key_1U, 13, Key_2U, Backspace, NextRow,
        Key_1_5U, Tab, Key_1U, 12, Key_EnterSpan, Enter, NextRow,
        Key_1_75U, CapsLock, Key_1U, 12, NextRow,
        Key_1_25U, Shift, Key_1U, 12, Key_2U, Shift, NextRow,
        Key_1_5U, Diacritics, Key_1U, Blank, 3, Key_5U, Space, 
        Key_1_5U, Blank, Key_1U, CursorLeft, CursorDown, CursorUp, CursorRight,
        End}
    };
    
    static const KeyboardLayout KeyLayout_JIS = {
        5, 15, 
        {Key_1U, 14, Backspace, NextRow,
        Key_1_5U, Tab, Key_1U, 12, Key_EnterReverseSpan, Enter, NextRow,
        Key_1_75U, CapsLock, Key_1U, 11, NextRow,
        Key_2_25U, Shift, Key_1U, 10, Key_3U, Shift, NextRow,
        Key_1_5U, Diacritics, Key_1U, Blank, 3, Key_1U, Lang1, Key_3_5U, Space, 
        Key_1U, Lang2, Lang3, CursorLeft, CursorDown, CursorUp, CursorRight,
        End}
    };
    
    static const KeyboardLayout KeyLayout_NumPad = {
        //Key_1U, Space, Key_1U, 3, NextRow,    // NumLock, /, *, -
        4, 4,
        {Key_1U, 4, NextRow,
        Key_1U, 4, NextRow,
        Key_1U, 3, Key_Vert_2U, Enter, NextRow,
        Key_2U, 1, Key_1U, 1,
        End}
    };
}