/*
 * 573in1 - Copyright (C) 2022-2024 spicyjpeg
 *
 * 573in1 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * 573in1 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * 573in1. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace gpu {

/* Types */

using Color = uint32_t;

enum BlendMode {
	GP0_BLEND_BITMASK   = 3,
	GP0_BLEND_SEMITRANS = 0,
	GP0_BLEND_ADD       = 1,
	GP0_BLEND_SUBTRACT  = 2,
	GP0_BLEND_DIV4_ADD  = 3
};

enum ColorDepth {
	GP0_COLOR_BITMASK = 3,
	GP0_COLOR_4BPP    = 0,
	GP0_COLOR_8BPP    = 1,
	GP0_COLOR_16BPP   = 2
};

struct Rect {
public:
	int16_t x1, y1, x2, y2;
};

struct RectWH {
public:
	int16_t x, y, w, h;
};

struct RectRB {
public:
	int16_t x, y, r, b;
};

static inline uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
	return 0
		| ((r & 0xff) <<  0)
		| ((g & 0xff) <<  8)
		| ((b & 0xff) << 16);
}

/* Rendering context */

static constexpr size_t DISPLAY_LIST_SIZE = 0x4000;
static constexpr size_t LAYER_STACK_SIZE  = 16;

struct Buffer {
public:
	// TODO: implement
};

class Context {
private:
	Buffer _buffers[2];
	int    _currentBuffer;

	uint32_t _lastTexpage;

	inline Buffer &_drawBuffer(void) {
		return _buffers[_currentBuffer];
	}
	inline Buffer &_dispBuffer(void) {
		return _buffers[_currentBuffer ^ 1];
	}

public:
	int width, height, refreshRate;

	inline Context(int width, int height)
	: _lastTexpage(0) {
		setResolution(width, height);
	}

	inline void newLayer(int x, int y) {
		newLayer(x, y, width, height);
	}

	inline void drawRect(RectWH &rect, Color color, bool blend = false) {
		drawRect(rect.x, rect.y, rect.w, rect.h, color, blend);
	}
	inline void drawGradientRectH(
		RectWH &rect, Color left, Color right, bool blend = false
	) {
		drawGradientRectH(rect.x, rect.y, rect.w, rect.h, left, right, blend);
	}
	inline void drawGradientRectV(
		RectWH &rect, Color top, Color bottom, bool blend = false
	) {
		drawGradientRectV(rect.x, rect.y, rect.w, rect.h, top, bottom, blend);
	}
	inline void drawGradientRectD(
		RectWH &rect, Color top, Color middle, Color bottom, bool blend = false
	) {
		drawGradientRectD(
			rect.x, rect.y, rect.w, rect.h, top, middle, bottom, blend
		);
	}

	inline void drawBackdrop(Color color) {
		drawRect(0, 0, width, height, color);
	}
	inline void drawBackdrop(Color color, BlendMode blendMode) {
		setBlendMode(blendMode, true);
		drawRect(0, 0, width, height, color, true);
	}

	void setResolution(int width, int height);
	void flip(void);

	void newLayer(int x, int y, int drawWidth, int drawHeight);
	void setTexturePage(uint16_t page, bool dither = false);
	void setBlendMode(BlendMode blendMode, bool dither = false);

	void drawRect(
		int x, int y, int width, int height, Color color, bool blend = false
	);
	void drawGradientRectH(
		int x, int y, int width, int height, Color left, Color right,
		bool blend = false
	);
	void drawGradientRectV(
		int x, int y, int width, int height, Color top, Color bottom,
		bool blend = false
	);
	void drawGradientRectD(
		int x, int y, int width, int height, Color top, Color middle,
		Color bottom, bool blend = false
	);
};

/* Image class */

struct TIMHeader {
public:
	uint32_t magic, flags;
};

struct TIMSectionHeader {
public:
	uint32_t length;
	RectWH   vram;
};

class Image {
public:
	uint16_t u, v, width, height;

	inline Image(void)
	: width(0), height(0) {}

	void initFromVRAMRect(
		const RectWH &rect, ColorDepth colorDepth,
		BlendMode blendMode = GP0_BLEND_SEMITRANS
	);
	bool initFromTIMHeader(
		const TIMHeader *header, BlendMode blendMode = GP0_BLEND_SEMITRANS
	);
	void drawScaled(
		Context &ctx, int x, int y, int w, int h, bool blend = false
	) const;
	void draw(Context &ctx, int x, int y, bool blend = false) const;
};

}
