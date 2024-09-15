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

#include "hw/gpu.hpp"
#include "common/util/units.hpp"
#include "common/util/tween.hpp"
#include "common/ui/layerman.hpp"
#include "common/ui/notifd.hpp"

namespace toast
{
    using namespace layers;
    using namespace units;

    constexpr const int ToastBar_Padding = 10;
    constexpr const int ToastBar_LineHeight = 2;
    
    class ToastBar : public Layer
    {
    private:
        uint32_t offsetStep;
        notif::Context &msgBar;
        util::Tween<int, util::QuadOutEasing> _toastAnim;

        int _drawMessage(
            gpu::Context &gpuCtx, gpu::Font &font, 
            gpu::Color *color, notif::PushMessage &msg, 
            int x, int y, int w, int h) const;

    public:
        gpu::Image tile;
        ToastBar(notif::Context &ctx, int x, int y, int w, int h): Layer(x, y, w, h, layers::LayerType::Overlay, layers::LayerPriority::Top), msgBar(ctx) {}
        const char *getName(){ return "ToastBar"; }
        void draw(gpu::Context &ctx, gpu::Font &font, gpu::Color *color, uint32_t time) const;
        void resize(int hRes, int vRes){
            w = hRes;
            h = vRes;
        }
    };
}