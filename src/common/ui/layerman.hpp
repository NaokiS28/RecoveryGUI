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

#include <stdint.h>
#include <vector>
#include <memory>
#include <functional>

#include "hw/gpu.hpp"
#include "hw/gpufont.hpp"

/*
	Layer Manager is a dynamic layer managment system for 
	handling updating and drawing the various layers added by
	the program. It supports adding any layer based on the
	layer template, and will track and handle mouse movements
	over objects.
*/

/*
	Todo:
	[ ] - Figure out how to dynamically load assets for layers
	[ ] - Figure out a better way to delete layers
*/

namespace layers
{
	// Sets at what priority this layer should be draw at.
	// 	Higher the priority, the closer to top it is draw at.
	enum LayerPriority
	{
		Default,
		Bottom,
		Top
	};

	enum class LayerType
	{
		Background,
		UI,
		Overlay
	};

	class Layer
	{
	protected:
		LayerType type;
		LayerPriority priority;

	public:
		int x, y, w, h;

		Layer(int x, int y, int w, int h, LayerType t, LayerPriority p) :
			type(t), priority(p), x(x), y(y), w(w), h(h) {}
		~Layer() = default;

		LayerType getType() const { return type; }
		LayerPriority getPriority() const { return priority; }

		virtual const char* getName() = 0;
		virtual void resize(int vRes, int hRes) {}
		virtual void draw(gpu::Context *ctx, gpu::Font *font, gpu::Color *color, uint32_t time) const {}
		virtual void update(uint32_t time) {}
		virtual void onSelect(){}
		virtual void onClick(int clickCode) {}
		virtual void onScroll(int clickCode) {}
		virtual void onKey(int clickCode) {}
		virtual void mouseMove(int x, int y) {}
		virtual void prevItem(){}
		virtual void nextItem(){}
	};

	class LayerManager
	{
	private:
		gpu::Context *gpu = nullptr;
		gpu::Font *font = nullptr;
		gpu::Color *color = nullptr;
		uint32_t currentTime = 0;
		int _backgroundCount = 0;
		int _uilayerCount = 0;
		int _overlayCount = 0;
		std::vector<std::unique_ptr<Layer>> _uxLayers;

		int _getNewLayerIndex(Layer* layer);
		inline void _setOffset(int x, int y) const
		{
			gpu->setOffset(x, y);
		};

	public:
		LayerManager(gpu::Context *ctx, gpu::Font *font, gpu::Color *color) : gpu(ctx), font(font), color(color){}
		~LayerManager() {}

		int getLayerCount() { return _uxLayers.size(); }

		void render();
		void update(uint32_t time);

		void deleteLayer(LayerType type, int idx){
			// This needs to be better really.
			switch(type){
				case LayerType::Background: break;
				case LayerType::UI: idx += _backgroundCount; break;
				case LayerType::Overlay: idx += (_backgroundCount + _uilayerCount); break;
			}
			_uxLayers.erase(_uxLayers.begin() + idx);
		}

		Layer* getPtrToLayer(const char* name){
			for (auto& layer : _uxLayers){
				if(!strcmp(name, layer.get()->getName()))
					return layer.get();
			}
			return nullptr;
		}

		template <typename T, typename... Args>
		void newLayer(Args &&...args)
		{
			// I have ChatGPT to thank for this entire idea
			static_assert(std::is_base_of<layers::Layer, T>::value, "T must be derived from layers::Layer");

			std::unique_ptr<T> layer = std::make_unique<T>(std::forward<Args>(args)...);

			int idx = _getNewLayerIndex(layer.get());
			switch (layer->getType())
			{
			case LayerType::Background:
				_backgroundCount++;
				break;
			case LayerType::UI:
				_uilayerCount++;
				break;
			case LayerType::Overlay:
				_overlayCount++;
				break;
			default:
				break;
			}
			layer->resize(gpu->getHorizontalRes(), gpu->getVerticalRes());
			_uxLayers.insert(_uxLayers.begin() + idx, std::move(layer));
		};
	};

}