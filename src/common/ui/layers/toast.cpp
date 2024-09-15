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

#include "toast.hpp"

namespace toast
{
    int ToastBar::_drawMessage(gpu::Context &gpuCtx, gpu::Font &font, gpu::Color *color, notif::PushMessage &msg, int x, int y, int w, int h) const
    {
        int lineHeight = font.getLineHeight();
        int iconWidth = (font.getCharacterWidth('/') * 3);

        gpu::RectWH rect = {
            (int16_t)x,
            (int16_t)y,
            (int16_t)w,
            (int16_t)h
        };
        gpuCtx.setOffset(x, y);
        gpuCtx.drawRect(rect, color[5], percentToShort(75));

        // Icon
        rect.y = y + ((rect.h / 2) - (lineHeight / 2));
        rect.x = x + ((rect.h / 2) - (iconWidth / 2));
        font.draw(gpuCtx, "❕", rect);

        // Text
        int msgHeight = font.getStringHeight(msg.message, w - rect.h);
        rect.y = y + ((rect.h / 2) - (msgHeight / 2));
        rect.x = x + rect.h;
        font.draw(gpuCtx, msg.message, rect);

        return rect.h;
    }

    void ToastBar::draw(gpu::Context &ctx, gpu::Font &font, gpu::Color *color, uint32_t time) const
    {
        int msgCount = msgBar.getLength();
        int msgX = x;
        int msgY = y;
        bool waitForAnim = false;   // If a toast notif is opening, don't draw any more till it's done.
        for(; msgCount > 0; msgCount--){
            notif::PushMessage msg;
            if(msg.state != notif::Opened || msg.state != notif::Closing){
                waitForAnim = true;
            }
            msgBar.getMessage(msgCount - 1, msg);
            msgY += _drawMessage(ctx, font, color, msg, msgX, msgY, w, h);
        }
    }
}