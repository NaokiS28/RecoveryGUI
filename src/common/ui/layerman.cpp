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

#include "layerman.hpp"

namespace layers
{
    void LayerManager::update(uint32_t time){
        currentTime = time;
        for(auto& layer : _uxLayers){
            layer->update(currentTime);
        }
    }

    void LayerManager::render(){
        for(auto& layer : _uxLayers){
            gpu.setOffset(layer->x, layer->y);
            layer->draw(gpu, font, color, currentTime);
        }
    }

    int LayerManager::_getNewLayerIndex(Layer* layer)
    {
        int idx;
        int lowCount;
        int topCount;
        switch (layer->getType())
        {
        case LayerType::Background:
            lowCount = 0;
            topCount = _backgroundCount;
            break;
        default:
        case LayerType::UI:
            lowCount = _backgroundCount;
            topCount = _uilayerCount;
            break;
        case LayerType::Overlay:
            lowCount = _backgroundCount + _uilayerCount;
            topCount = _overlayCount;
            break;
        }

        if (layer->getPriority() == LayerPriority::Top)
            idx = (lowCount + topCount);
        else if (layer->getPriority() == LayerPriority::Default)
        {
            idx = (lowCount + topCount);
            for (; idx > lowCount; idx--)
            {
                if (_uxLayers[idx]->getPriority() != LayerPriority::Top)
                    break;
            }
        }
        else if (layer->getPriority() == LayerPriority::Bottom)
        {
            idx = lowCount;
        }

        return idx;
    }

}

