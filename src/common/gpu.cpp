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

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "common/util/misc.hpp"
#include "common/gpu.hpp"

namespace gpu {

/* Rendering context */

void Context::flip(void) {
	auto &oldBuffer = _drawBuffer(), &newBuffer = _dispBuffer();

	// TODO: implement
}

void Context::setResolution(int _width, int _height) {
	util::CriticalSection sec;

	width       = _width;
	height      = _height;
	refreshRate = 60;

	// TODO: implement

	_currentBuffer = 0;
	flip();
}

void Context::newLayer(int x, int y, int drawWidth, int drawHeight) {
	// TODO: implement
}

void Context::setTexturePage(uint16_t page, bool dither) {
	// TODO: implement
}

void Context::setBlendMode(BlendMode blendMode, bool dither) {
	// TODO: implement
}

void Context::drawRect(
	int x, int y, int width, int height, Color color, bool blend
) {
	// TODO: implement
}

void Context::drawGradientRectH(
	int x, int y, int width, int height, Color left, Color right, bool blend
) {
	// TODO: implement
}

void Context::drawGradientRectV(
	int x, int y, int width, int height, Color top, Color bottom, bool blend
) {
	// TODO: implement
}

void Context::drawGradientRectD(
	int x, int y, int width, int height, Color top, Color middle, Color bottom,
	bool blend
) {
	// TODO: implement
}

/* Image class */

void Image::initFromVRAMRect(
	const RectWH &rect, ColorDepth colorDepth, BlendMode blendMode
) {
	int shift = 2 - int(colorDepth);

	u       = (rect.x & 0x3f) << shift;
	v       = rect.y & 0xff;
	width   = rect.w << shift;
	height  = rect.h;
	// TODO: implement
}

bool Image::initFromTIMHeader(const TIMHeader *header, BlendMode blendMode) {
	if (header->magic != 0x10)
		return false;

	auto ptr = reinterpret_cast<const uint8_t *>(&header[1]);

	if (header->flags & (1 << 3)) {
		auto clut = reinterpret_cast<const TIMSectionHeader *>(ptr);

		// TODO: implement
		ptr += clut->length;
	}

	auto image = reinterpret_cast<const TIMSectionHeader *>(ptr);

	initFromVRAMRect(image->vram, ColorDepth(header->flags & 3), blendMode);
	return true;
}

void Image::drawScaled(
	Context &ctx, int x, int y, int w, int h, bool blend
) const {
	// TODO: implement
}

void Image::draw(Context &ctx, int x, int y, bool blend) const {
	// TODO: implement
}

}
