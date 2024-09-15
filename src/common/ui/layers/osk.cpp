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

#include "osk.hpp"

namespace OSKeyboard
{
    int _utfLength(unsigned char c)
    {
        // Check if it's a single-byte (ASCII) character
        if ((c & 0x80) == 0)
        {
            return 1; // ASCII (0xxxxxxx)
        }
        // Check if it's the start of a 2-byte sequence (110xxxxx)
        if ((c & 0xE0) == 0xC0)
        {
            return 2;
        }
        // Check if it's the start of a 3-byte sequence (1110xxxx)
        if ((c & 0xF0) == 0xE0)
        {
            return 3;
        }
        // Check if it's the start of a 4-byte sequence (11110xxx)
        if ((c & 0xF8) == 0xF0)
        {
            return 4;
        }
        // Otherwise, it's not the start of a UTF-8 character
        return 0;
    }

    int _getKeySize(int style)
    {
        // Get key unit size
        int size = 0;
        int qKey = (OSKKeySize / 4);
        int uSize = ((style % 100) / 10);
        int qSize = style % 10;
        // 1.5U = 15 + 3 + 6

        switch (qSize)
        {
        default:
            break;
        case 1:
            //.25
            size += qKey;
            break;
        case 2:
            //.5
            size += (qKey * 2);
            break;
        case 3:
            //.75
            size += (qKey * 3);
            break;
        }

        size += (OSKKeySize * uSize);

        int sSize = 0;
        if (uSize > 1)
            sSize = uSize - 1;
        if (qSize > 0)
            sSize++;

        size += (OSKSpacing * sSize);

        return size;
    }

    const char *_getModkeyText(int type)
    {
        switch (type)
        {
        case locale::Shift:
            return "⇧";
        case locale::Backspace:
            return "⌫";
        case locale::Diacritics:
            return "äÄ";
        case locale::CapsLock:
            return "⇧A";
        case locale::Tab:
            return "Tab";
        case locale::CursorUp:
            return "▴";
        case locale::CursorDown:
            return "▾";
        case locale::CursorLeft:
            return "◂";
        case locale::Enter:
            return "↵";
        case locale::CursorRight:
            return "▸";
        default:
            return nullptr;
        }
    }

    int OSKeyLayer::_getKeyIdx(int row, int col)
    {
        if (row > _inputLocale.keyLayout.rows || col > _inputLocale.keyLayout.cols)
        {
            return 0;
        }

        int idx = (col + (_inputLocale.keyLayout.cols * row));
        int loopCount = 0; // Prevents a hanging loop.
        while (loopCount < 10)
        {
            if (_keyLayout[idx].r == row)
            {
                if (_keyLayout[idx].c == col)
                {
                    // We should be at the right key now.
                    return idx;
                }
                else if (_keyLayout[idx].c < col)
                    idx++;
                else if (_keyLayout[idx].c > col)
                    idx--;
            }
            else if (_keyLayout[idx].r < row)
                idx++;
            else if (_keyLayout[idx].r > row)
                idx--;
            loopCount++;
        }
        return 0;
    }

    void OSKeyLayer::_drawKey(
        gpu::Context &ctx, gpu::Font &font,
        gpu::Color *color,
        const KeyMetric &key,
        bool selected) const
    {
        int16_t kTextW = (int16_t)font.getStringWidth(key.text);
        int16_t kTextH = (int16_t)font.getLineHeight();
        gpu::RectWH rect{
            (int16_t)key.x,
            (int16_t)key.y,
            (int16_t)key.w,
            (int16_t)key.h};

        ctx.drawRect(rect, gpu::rgb(0, 0, 0));

        if (selected)
        {
            ctx.drawGradientRectVVar(
                rect,
                gpu::rgba(0, 0, 0, 0),
                color[3],
                units::percentOf(35, 0, rect.h),
                units::percentOf(100, 0, rect.w));
        };

        if (key.text != nullptr)
        {
            rect.x += (rect.w / 2) - (kTextW / 2) + 1;
            rect.y += (rect.h / 2) - (kTextH / 2) + 1;
            font.draw(ctx, key.text, rect);
        }
    }

    void OSKeyLayer::_setActiveKey(int row, int col)
    {
        /*
        if(row > 0 && col > 0){
            sound.play(select)
        }
        */
        _selKeyCol = col;
        _selKeyRow = row;
    }

    void OSKeyLayer::draw(gpu::Context &ctx, gpu::Font &font, gpu::Color *color, uint32_t time) const
    {
        if (_animationState != Hidden && _oskDev != nullptr)
        {
            // Backdrop
            ctx.setOffset(x, y);
            ctx.drawRect(
                _keyboardRect.x,
                _keyboardRect.y,
                _keyboardRect.w,
                _keyboardRect.h, color[5], units::percentToShort(75));

            // Key loop
            bool kSelect; // Is this key selected
            int charCount = 0;
            char text[5] = {'\0'};
            const char *line = locale::getKeyText(_inputLocale, _keyModifiers);

            for (auto key : _keyLayout)
            {
                // Add key text from locale - This is inefficient, but its also keeping the text up to date
                if (key.type == 0)
                {
                    int charlen = _utfLength(line[charCount]);
                    memset(text, '\0', 5);
                    strncpy(text, line + (charCount++), charlen);
                    key.text = text;
                }

                kSelect = _keySelected(key.r, key.c);
                if (key.type != locale::Blank)
                {
                    _drawKey(
                        ctx, font, color,
                        key, kSelect);
                }
            }
        }
    }

    void OSKeyLayer::_loadLayout()
    {
        if (_inputLocale.keyLayout.keyLayout.capacity() == 0)
        {
            return;
        }

        int keyUnit = _getKeySize(locale::Key_1U);
        int keyCount = 0;
        int keyType = 0;    // 0 = Character, anything else is local::LocaleKeyType
        _keyLayout.clear();

        // Key loop
        bool done = false;
        int layoutIdx = 0;  // Index of current layout element
        int loopCount = 0;  // anti-lockup
        KeyMetric key = {OSKPadding, OSKPadding, OSKKeySize, OSKKeySize, 0, 0, keyType, nullptr};

        while (!done && loopCount < 200)    // 200 is way overkill for a keyboard
        {
            if (layoutIdx >= (int)_inputLocale.keyLayout.keyLayout.size())
            {
                break;
            }

            int layoutCode = _inputLocale.keyLayout.keyLayout[layoutIdx++]; // Get current code

            // 1U Key, 2.5U key etc
            if (layoutCode > locale::Key_Size && layoutCode < locale::Key_ModKey)
                keyUnit = layoutCode;
            // Character key
            else if (layoutCode < locale::Key_Size)
                keyCount = layoutCode;
            // Non-character key and not enter key size def
            else if (layoutCode > locale::Key_Enter && layoutCode < locale::Array_Mode)
            {
                key.type = layoutCode;
                int idx = _inputLocale.keyLayout.keyLayout[layoutIdx];
                if (idx < locale::Key_Size)
                {
                    // More than one mod/special key
                    keyCount = idx;
                    layoutIdx++;
                }
                else
                    keyCount = 1;
            }
            // Array code most likely
            else
            {
                switch (layoutCode)
                {
                case locale::NextRow:
                    key.r++;
                    key.c = 0;
                    key.x = OSKPadding;
                    key.y += (OSKKeySize + OSKSpacing);
                    break;
                default:
                case locale::End:
                    done = true;
                    break;
                }
            }

            switch (keyType)
            {
            case locale::Key_Enter:
                // ANSI (2.5u)
                key.w = _getKeySize(locale::Key_2_5U);
                _keyLayout.push_back(key);
                break;

            case locale::Key_EnterSpan:
                // ISO/ABNT/JIS (Top Heavy)
                key.w = _getKeySize(locale::Key_1_5U);
                _keyLayout.push_back(key);
                // Second to instruct drawing code to make a second rect
                _keyLayout.push_back({((x + key.w) - (_getKeySize(locale::Key_1_5U))),
                                      (key.y + OSKKeySize),
                                      key.r, key.c,
                                      _getKeySize(locale::Key_1_25U),
                                      (OSKKeySize + OSKSpacing),
                                      locale::Key_Enter_SecondRect});
                break;

            case locale::Key_EnterReverseSpan:
                // ANSI Alt/KS (Bottom Heavy)
                key.w = _getKeySize(locale::Key_1_5U);
                key.h += OSKSpacing;
                _keyLayout.push_back(key); // Top part
                _keyLayout.push_back({((x + key.w) - (_getKeySize(locale::Key_2_25U))),
                                      (y + OSKKeySize + OSKSpacing),
                                      key.r, key.c,
                                      _getKeySize(locale::Key_2_25U),
                                      OSKKeySize,
                                      locale::Key_Enter_SecondRect});
                break;

            case locale::Key_EnterNumPad:
                // Numpad
                key.h = _getKeySize(locale::Key_2U);
                _keyLayout.push_back(key);
                break;

            default:
                // Anything else
                for (int c = 0; c < keyCount; c++)
                {
                    // Add text to modifier keys
                    if (key.type != 0)
                    {
                        key.text = _getModkeyText(key.type);
                    }

                    // Add to vector
                    if (keyUnit < locale::Key_Vert_Size)
                    {
                        key.w = _getKeySize(keyUnit);
                        _keyLayout.push_back(key); // w, h, type
                    }
                    else
                    {
                        key.h = _getKeySize(keyUnit);
                        _keyLayout.push_back(key); // w, h, type
                    }

                    key.x += (key.w + OSKSpacing);
                    key.c++;
                }
                keyCount = 0;
                key.type = 0;
                break;
            }
        }

        if (done)
        {
            _calcKeyboardRect();
        }
        mouseMove(200, 170);
    }

    void OSKeyLayer::mouseMove(int x, int y)
    {
        // mouseMove is only called when the cursor moves over the layer
        // so x & y will only be as low or high as the extents of the w & h values.
        // x and y account for the position of the layer, so 0,0 is top left of the layer
        int keyR = 0, keyC = 0;
        int idx = 0;
        int tempX = 0;
        int tempY = 0;

        // Quickly skip past rows/colums we know it cant be
        for (; keyR <= _inputLocale.keyLayout.rows; keyR++)
        {
            tempY = (this->y + ((OSKKeySize + OSKSpacing) * (keyR + 1)));
            if (y < (tempY + OSKKeySize))
            {
                break;
            }
        }
        for (; keyC <= _inputLocale.keyLayout.cols; keyC++)
        {
            tempX = (this->x + ((OSKKeySize + OSKSpacing) * (keyC + 1)));
            if (x < (tempX + OSKKeySize))
            {
                break;
            }
        }

        // Get starting index
        idx = _getKeyIdx(keyR, keyC);
        tempX = (_keyLayout[idx].x + this->x);
        tempY = (_keyLayout[idx].y + this->y);
        if (x >= tempX && x <= (tempX + _keyLayout[idx].w) &&
            y >= tempY && y <= (tempY + _keyLayout[idx].h))
        {
            // Mouse over key, (+1 as 0,0 is no key)
            _setActiveKey(_keyLayout[idx].r + 1, _keyLayout[idx].c + 1);
        }
        else
        {
            // Mouse not over a key
            _setActiveKey(0, 0);
        }
    }

    void OSKeyLayer::prevItem()
    {
        if (_selKeyCol - 1 >= 0)
            _setActiveKey(_selKeyRow, _selKeyCol - 1);
        else
        {
            if (_selKeyRow - 1 >= 0)
                _setActiveKey(_selKeyRow - 1, _inputLocale.keyLayout.cols);
            else
                _setActiveKey(_inputLocale.keyLayout.rows, _inputLocale.keyLayout.cols);
        }
    }

    void OSKeyLayer::nextItem()
    {
        if (_selKeyCol + 1 <= _inputLocale.keyLayout.cols)
            _setActiveKey(_selKeyRow, _selKeyCol + 1);
        else
        {
            if (_selKeyRow + 1 <= _inputLocale.keyLayout.rows)
                _setActiveKey(_selKeyRow + 1, _selKeyCol + 1);
            else
                _setActiveKey(1, _selKeyCol + 1);
        }
    }

    void OSKeyLayer::update(uint32_t time)
    {
        if (!_popupAnim.isDone(time))
        {
            y = _popupAnim.getValue(time);
        }
        else
        {
            if (_enabled && _animationState == Hiding)
            {
                _animationState = Hidden;
                _enabled = false;
            }
            else if (!_enabled && _animationState == Opening)
            {
                _animationState = Open;
                _enabled = true;
            }
        }
    }

    void OSKeyLayer::_type()
    {
        if (_selKeyCol > 0 && _selKeyRow > 0)
        {
            // Todo
            _oskDev->type(0);
        }
    }

    void OSKeyLayer::onSelect()
    {
        _type();
    }

    void OSKeyLayer::onClick()
    {
        _type();
    }

    void OSKeyLayer::show()
    {
        if (_oskDev != nullptr && !_enabled)
        {
            _loadLayout();
            _calcKeyboardRect();
            _popupAnim.setValue(0, y + _keyboardRect.h, y, 20);
            _animationState = Opening;
        }
    }

    void OSKeyLayer::hide()
    {
        if (_oskDev != nullptr && _enabled)
        {
            _calcKeyboardRect();
            _popupAnim.setValue(0, y, y + _keyboardRect.h, 20);
            _animationState = Hiding;
        }
    }
}