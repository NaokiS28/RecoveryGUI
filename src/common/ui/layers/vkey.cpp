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

#include "vkey.hpp"

namespace vKeyboard
{
    void vKeyboard::draw(gpu::Context *ctx, gpu::Font *font, gpu::Color *color, uint32_t time) const
    {
        if (_enabled && _oskDev != nullptr)
        {
            ctx->setOffset(
                (ctx->getHorizontalRes() / 2) - (OSKWidth / 2),
                ctx->getVerticalRes() - (OSKHeight + OSKPadding));
            gpu::RectWH rect{
                0, 0,
                OSKWidth, OSKHeight};
            ctx->drawRect(rect, color[5], units::percentToShort(75));

            int layoutRow = 0;
            int layoutCol = 0;
            int currentKey = 0;
            int keyX = 14;
            int keyY = 8;
            int keyTextCol = 0;
            int cursorCount = 0;
            bool done = false;
            bool keySelected = true;
            while (!done)
            {
                currentKey = OSKLayout[layoutRow][layoutCol];
                if (currentKey == KeyType::NextRow)
                {
                    keyX = 14;
                    keyY += (OSK_KeySize + OSKSpacing);
                    layoutRow++;
                    layoutCol = 0;
                    keyTextCol = 0;
                    cursorCount = 0;
                    continue;
                }
                else if (currentKey == KeyType::End)
                {
                    done = true;
                    continue;
                }

                const char *rowText = locale::getKeyRow(layoutRow, (locale::LocaleIndex)_inputLocale, locale::KeyNormal);
                int count = OSKLayout[layoutRow][layoutCol + 1];
                char text[] = {0, 0, 0, 0, 0};

                int keyWidth = 0;
                int keyHeight = OSK_KeySize;
                switch (currentKey)
                {
                case KeyType::HalfBlank:
                    keyWidth += (OSK_KeySize / 2);
                    break;
                case KeyType::Cursor:
                case KeyType::Blank:
                case KeyType::Single:
                    keyWidth += OSK_KeySize;
                    break;
                case KeyType::Diacritics:
                    strcpy(text, "äÄ");
                    keyWidth += (OSK_KeySize + (OSK_KeySize / 2));
                    break;
                case KeyType::Shift:
                    strcpy(text, "⇧");
                    keyWidth += (OSK_KeySize + (OSK_KeySize / 2));
                    break;
                case KeyType::WideSingle:
                    keyWidth += (OSK_KeySize + (OSK_KeySize / 2));
                    break;
                case KeyType::Backspace:
                    strcpy(text, "⌫");
                    keyWidth += ((OSK_KeySize * 2) + OSKSpacing);
                    break;
                case KeyType::DoubleWide:
                    keyWidth += ((OSK_KeySize * 2) + OSKSpacing);
                    break;
                case KeyType::Enter:
                    keyWidth += ((OSK_KeySize * 2) + OSKSpacing);
                    keyHeight += (OSK_KeySize + OSKSpacing);
                    strcpy(text, "↵");
                    break;
                case KeyType::Space:
                    keyWidth += ((OSK_KeySize + OSKSpacing) * 5) + OSK_KeySize;
                    break;
                }

                for (; count > 0; count--)
                {
                    if (currentKey != KeyType::Blank && currentKey != KeyType::HalfBlank)
                    {
                        ctx->drawRect(
                            keyX,
                            keyY,
                            keyWidth, keyHeight,
                            gpu::rgb(0, 0, 0));

                        if (currentKey == KeyType::Single && rowText != nullptr)
                        {
                            char kText[] = {0, 0, 0, 0, 0};
                            kText[0] = rowText[keyTextCol++];
                            if (kText[0] < 0) // UTF shenanigans
                                kText[1] = rowText[keyTextCol++];
                            strcpy(text, kText);
                        }
                        else if (currentKey == KeyType::Cursor)
                        {
                            switch (cursorCount++)
                            {
                            case 0:
                                strcpy(text, "◂");
                                break;
                            case 1:
                                strcpy(text, "▴");
                                break;
                            case 2:
                                strcpy(text, "▾");
                                break;
                            case 3:
                                strcpy(text, "▸");
                                break;
                            default:
                                break;
                            }
                        }
                        int16_t kTextW = (int16_t)font->getStringWidth(text);
                        int16_t kTextH = (int16_t)font->getStringHeight(text, kTextW);
                        gpu::RectWH keyRect = {
                            (int16_t)(keyX + ((OSK_KeySize / 2) - (kTextW / 2)) + 1),
                            (int16_t)(keyY + ((OSK_KeySize / 2) - (kTextH / 2)) + 1),
                            kTextW, kTextH};
                        font->draw(
                            *ctx, text,
                            keyRect);

                        if (keySelected)
                        {
                            ctx->drawGradientRectV(
                                keyX,
                                keyY,
                                keyWidth, keyHeight,
                                gpu::rgba(0, 0, 0, 0),
                                color[3]);
                            keySelected = false;
                        }
                    }
                    keyX += keyWidth + OSKSpacing;
                }

                layoutCol += 2;
            }
        }
    }

    void vKeyboard::show()
    {
        if (_oskDev != nullptr && !_enabled)
        {
            //_popupAnim.setValue(0, goBack ? (-width) : width, 0, 20);
            _enabled = true;
        }
    }

    void vKeyboard::hide()
    {
        if (_oskDev != nullptr && _enabled)
        {
            _enabled = false;
        }
    }
}