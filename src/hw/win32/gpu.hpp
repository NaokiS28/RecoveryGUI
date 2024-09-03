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

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <array>
#include <windows.h>
#include <wingdi.h>

#include "targetver.h"
#include "common/util/vesa.hpp"
#include "common/util/misc.hpp"
#include "common/util/log.hpp"

namespace gpu
{

	/* Types */

	using Color = uint32_t;

	enum ScaleFactor
	{
		GUI_SCALE_1X = 1,
		GUI_SCALE_2X,
		GUI_SCALE_3X,
		GUI_SCALE_4X
	};

	enum BlendMode
	{
		GP0_BLEND_BITMASK = 3,
		GP0_BLEND_SEMITRANS = 0,
		GP0_BLEND_ADD = 1,
		GP0_BLEND_SUBTRACT = 2,
		GP0_BLEND_DIV4_ADD = 3
	};

	enum ColorDepth
	{
		GP0_COLOR_BITMASK = 3,
		GP0_COLOR_4BPP = 0,
		GP0_COLOR_8BPP = 1,
		GP0_COLOR_16BPP = 2
	};

	struct Rect
	{
	public:
		int16_t x1, y1, x2, y2;
	};

	struct RectWH
	{
	public:
		int16_t x, y, w, h;
	};

	struct RectRB
	{
	public:
		int16_t x, y, r, b;
	};

	static inline uint32_t rgb(uint8_t r, uint8_t g, uint8_t b)
	{
		return 0xFF000000 | ((r & 0xff) << 0) | ((g & 0xff) << 8) | ((b & 0xff) << 16);
	}

	static inline uint32_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		return 0 | ((r & 0xff) << 0) | ((g & 0xff) << 8) | ((b & 0xff) << 16) | ((a & 0xff) << 24);
	}

	static inline uint8_t GetRedValue(Color c)
	{
		return 0 | ((c & 0x000000FF) >> 0);
	}

	static inline uint8_t GetGreenValue(Color c)
	{
		return 0 | ((c & 0x0000FF00) >> 8);
	}

	static inline uint8_t GetBlueValue(Color c)
	{
		return 0 | ((c & 0x00FF0000) >> 16);
	}

	static inline uint8_t GetAlphaValue(Color c)
	{
		return 0 | ((c & 0xFF000000) >> 24);
	}

	/* Rendering context */

	static constexpr size_t DISPLAY_LIST_SIZE = 0x4000;
	static constexpr size_t LAYER_STACK_SIZE = 16;

	enum GPUGradientMode : uint32_t
	{
		GPU_GRADIENT_H = 0,
		GPU_GRADIENT_V = 1,
		GPU_GRADIENT_D
	};

	class Context
	{
	private:
		PAINTSTRUCT ps = {};
		HDC gpuHdc = NULL;
		uint32_t _lastTexpage;
		int hRes, vRes, refreshRate, dpi;
		int xOffset = 0, yOffset = 0;
		uint8_t scaleFactor = GUI_SCALE_1X;

	public:
		int width, height;
		HDC bufferHdc = NULL;
		HBITMAP bufferBitmap = NULL;
		HWND windowHandle = NULL;

		inline Context(int width, int height)
		{
			this->hRes = width;
			this->vRes = height;
		}

		inline Context();

		inline void setOffset(int x, int y)
		{
			return newLayer(x, y, width, height);
		}

		inline void drawRect(RectWH &rect, Color color, uint8_t a = 255)
		{
			drawRect(rect.x, rect.y, rect.w, rect.h, color, a);
		}
		inline void drawGradientRectH(RectWH &rect, Color left, Color right, uint8_t a = 255)
		{
			drawGradientRectH(rect.x, rect.y, rect.w, rect.h, left, right, a);
		}
		inline void drawGradientRectV(RectWH &rect, Color top, Color bottom, uint8_t a = 255)
		{
			drawGradientRectV(rect.x, rect.y, rect.w, rect.h, top, bottom, a);
		}
		inline void drawGradientRectD(RectWH &rect, Color top, Color middle, Color bottom, uint8_t a = 255)
		{
			drawGradientRectD(rect.x, rect.y, rect.w, rect.h, top, middle, bottom, a);
		}

		inline void drawGradientRectHVar(RectWH &rect, Color left, Color right, int startPoint, int endPoint, uint8_t a)
		{
			drawGradientRectHVar(rect.x, rect.y, rect.w, rect.h, left, right, startPoint, endPoint, a);
		}
		inline void drawGradientRectVVar(RectWH &rect, Color top, Color bottom, int startPoint, int endPoint, uint8_t a)
		{
			drawGradientRectVVar(rect.x, rect.y, rect.w, rect.h, top, bottom, startPoint, endPoint, a);
		}

		inline void drawBackdrop(Color color)
		{
			drawRect(0, 0, width, height, color);
		}
		inline void drawBackdrop(Color color, BlendMode blendMode)
		{
			// setBlendMode(blendMode, true);
			drawRect(0, 0, width, height, color, true);
		}

		~Context()
		{
			DeleteObject(bufferBitmap);
			DeleteDC(bufferHdc);
		}

		void init();

		inline void setResolution(const std::array<int, 2> &resolution, bool updateWindow = true)
		{
			setResolution(resolution[0], resolution[1], updateWindow);
		}

		void setResolution(int _width, int _height, bool updateWindow = true);
		inline int getHorizontalRes() { return hRes; }
		inline int getVerticalRes() { return vRes; }
		inline int getRefreshRate() { return refreshRate; }
		void beginDraw();
		void endDraw();

		void drawAlpha(HDC hdc, int x, int y, int w, int h, int sx, int sy, uint8_t a) const;

		void newLayer(int x, int y, int drawWidth, int drawHeight);
		void setTexturePage(uint16_t page, bool dither = false);
		void setBlendMode(BlendMode blendMode, bool dither = false);

		void drawRect(int x, int y, int width, int height, Color color, uint8_t a = 255);
		void drawGradientRectH(
			int x, int y, int w, int h, Color left, Color right,
			uint8_t a = 255);
		void drawGradientRectV(
			int x, int y, int w, int h, Color top, Color bottom,
			uint8_t a = 255);
		void drawGradientRectD(
			int x, int y, int w, int h, Color top, Color middle,
			Color bottom, uint8_t a = 255);

		std::array<int, 2> getResolution() { return std::array<int, 2> { hRes, vRes }; }

		void setScale(int scale);
		void drawGradientRect(int x, int y, int w, int h, GRADIENT_RECT *r, TRIVERTEX *v, GPUGradientMode m, uint8_t a = 255);
		void drawGradientRectHVar(int x, int y, int w, int h, Color left, Color right, int startPoint, int endPoint, uint8_t a = 255);
		void drawGradientRectVVar(int x, int y, int w, int h, Color top, Color bottom, int startPoint, int endPoint, uint8_t a = 255);
		void updateWindow();
	};

	/* Image class */

	struct TIMHeader
	{
	public:
		uint32_t magic, flags;
	};

	struct TIMSectionHeader
	{
	public:
		uint32_t length;
		RectWH vram;
	};

	class Image
	{
	public:
		bool valid = false;
		char *name;
		uint16_t u, v, width, height;
		HDC imgHdc = NULL;
		HBITMAP imgBitmap = NULL;

		inline Image(void)
			: width(0), height(0) {}

		void initFromVRAMRect(
			const RectWH &rect, ColorDepth colorDepth,
			BlendMode blendMode = GP0_BLEND_SEMITRANS);
		bool initFromTIMHeader(
			const TIMHeader *header, BlendMode blendMode = GP0_BLEND_SEMITRANS);
		void drawScaled(
			Context &ctx, int x, int y, int w, int h, bool blend = false) const;
		void draw(Context &ctx, int x, int y, int alpha = 255) const;
		void draw(Context &ctx, int x, int y, int sx, int sy, int w, int h, int a = 255) const;
	};

	class AnimatedImage
	{
	public:
		char *name;
		uint16_t u, v, width, height;
		HDC imgHdc = NULL;
		HBITMAP imgBitmap = NULL;

		inline AnimatedImage(void)
			: width(0), height(0) {}

		//void initFromVRAMRect(
		//	const RectWH &rect, ColorDepth colorDepth,
		//	BlendMode blendMode = GP0_BLEND_SEMITRANS);
		//bool initFromTIMHeader(
		//	const TIMHeader *header, BlendMode blendMode = GP0_BLEND_SEMITRANS);
		//void drawScaled(
		//	Context &ctx, int x, int y, int w, int h, bool blend = false) const;
		//void draw(Context &ctx, int x, int y, int alpha = 255) const;
		//void draw(Context &ctx, int x, int y, int sx, int sy, int w, int h, int a = 255) const;
	};

}
