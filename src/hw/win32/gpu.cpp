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

#include "gpu.hpp"

namespace gpu
{

	/* Rendering context */
	void Context::init()
	{
		// This should be dealt with a little better but it works.
		// Setup and use a double buffer. We draw to "hdc" as gpu::Context expects this - Pretty this up.
		if (windowHandle != NULL)
		{
			gpuHdc = GetDC(windowHandle);
			bufferHdc = CreateCompatibleDC(gpuHdc); // Creates a compatible HDC akin to the original
			bufferBitmap = CreateCompatibleBitmap(gpuHdc, hRes, vRes);

#if (WINVER >= _WIN32_WINNT_WINBLUE)
			// Only in Windows 10, ver 1607 (Redstone 1)
			dpi = GetDpiForWindow(windowHandle);
#else
			dpi = 96;
#endif

			SelectObject(bufferHdc, bufferBitmap);
			ReleaseDC(windowHandle, gpuHdc);
		}
	}

	void Context::updateWindow(){

	}

	void Context::beginDraw()
	{
		ps = {};
		gpuHdc = BeginPaint(windowHandle, &ps);
		SelectObject(bufferHdc, bufferBitmap);
		SetMapMode(bufferHdc, MM_ANISOTROPIC);
		SetWindowExtEx(bufferHdc, hRes, vRes, NULL);
		SetViewportExtEx(bufferHdc, hRes, vRes, NULL);
	}

	void Context::endDraw()
	{
		// Post drawing
		BitBlt(gpuHdc, 0, 0, hRes, vRes, bufferHdc, 0, 0, SRCCOPY);
		EndPaint(windowHandle, &ps); // Inform Windows we are done
	}

	void Context::setScale(int scale)
	{
		scaleFactor = scale;
		// if (windowHandle != NULL)
		//	setResolution(hRes * scaleFactor, vRes * scaleFactor);
	}

	// Change the current GUI resolution
	void Context::setResolution(int _width, int _height, bool updateWindow)
	{
		// util::CriticalSection sec;
		RECT rect = {0, 0, _width, _height};
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		hRes = _width;
		vRes = _height;
		width = hRes;
		height = vRes;
		refreshRate = 60;

		// Sets the main app resolution only, does not reposition window.
		if (updateWindow)
		{
			gpuHdc = GetDC(windowHandle);
			if (bufferBitmap)
				DeleteObject(bufferBitmap);
			bufferBitmap = CreateCompatibleBitmap(gpuHdc, hRes, vRes);
			SelectObject(bufferHdc, bufferBitmap);
			ReleaseDC(windowHandle, gpuHdc);
			SetWindowPos(windowHandle, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
		}
	}

	void Context::newLayer(int x, int y, int drawWidth, int drawHeight)
	{
		// SetViewportOrgEx(bufferHdc, x, y, NULL);
		xOffset = x;
		yOffset = y;
		width = drawWidth;
		height = drawHeight;
	}

	void Context::setTexturePage(uint16_t page, bool dither)
	{
		// TODO: implement
	}

	void Context::setBlendMode(BlendMode blendMode, bool dither)
	{
		// TODO: implement
	}

	void Context::drawRect(
		int x, int y, int w, int h, Color color, uint8_t a)
	{
		// 🎵 If it's hacky but it works, commit the code
		drawGradientRectH(x, y, w, h, color, color, a);
	}

	// Draw rectangle with gradient from left to right
	void Context::drawGradientRectH(int x, int y, int w, int h, Color left, Color right, uint8_t a)
	{
		TRIVERTEX vertex[2];
		vertex[0].x = 0;
		vertex[0].y = 0;
		vertex[0].Red = gpu::GetRedValue(left) << 8;
		vertex[0].Green = gpu::GetGreenValue(left) << 8;
		vertex[0].Blue = gpu::GetBlueValue(left) << 8;
		vertex[0].Alpha = gpu::GetAlphaValue(left) << 8;

		vertex[1].x = w;
		vertex[1].y = h;
		vertex[1].Red = gpu::GetRedValue(right) << 8;
		vertex[1].Green = gpu::GetGreenValue(right) << 8;
		vertex[1].Blue = gpu::GetBlueValue(right) << 8;
		vertex[1].Alpha = gpu::GetAlphaValue(right) << 8;

		GRADIENT_RECT gRect;
		gRect.UpperLeft = 0;
		gRect.LowerRight = 1;

		drawGradientRect(x, y, w, h, &gRect, vertex, GPU_GRADIENT_H, a);
	}

	// Draw a gradient with variable start/end point
	void Context::drawGradientRectHVar(int x, int y, int w, int h, Color left, Color right, int startPoint, int endPoint, uint8_t a)
	{
		// Draw the first part of the rect
		if (startPoint > 0)
			drawRect(x, y, startPoint, h, left, a);

		// Draw the gradient
		TRIVERTEX vertex[2];
		vertex[0].x = 0;
		vertex[0].y = 0;
		vertex[0].Red = gpu::GetRedValue(left) << 8;
		vertex[0].Green = gpu::GetGreenValue(left) << 8;
		vertex[0].Blue = gpu::GetBlueValue(left) << 8;
		vertex[0].Alpha = gpu::GetAlphaValue(left) << 8;

		vertex[1].x = (endPoint - startPoint);
		vertex[1].y = h;
		vertex[1].Red = gpu::GetRedValue(right) << 8;
		vertex[1].Green = gpu::GetGreenValue(right) << 8;
		vertex[1].Blue = gpu::GetBlueValue(right) << 8;
		vertex[1].Alpha = gpu::GetAlphaValue(right) << 8;

		GRADIENT_RECT gRect;
		gRect.UpperLeft = 0;
		gRect.LowerRight = 1;

		drawGradientRect(x + startPoint, y, (endPoint - startPoint), h, &gRect, vertex, GPU_GRADIENT_H, a);

		// Draw the last part of the rect
		if (endPoint < w)
			drawRect(x + endPoint, y, (w - endPoint), h, right, a);
	}

	// Draw a rectangle with gradient from top to bottom
	void Context::drawGradientRectV(int x, int y, int w, int h, Color top, Color bottom, uint8_t a)
	{
		TRIVERTEX vertex[2];
		vertex[0].x = 0;
		vertex[0].y = 0;
		vertex[0].Red = gpu::GetRedValue(top) << 8;
		vertex[0].Green = gpu::GetGreenValue(top) << 8;
		vertex[0].Blue = gpu::GetBlueValue(top) << 8;
		vertex[0].Alpha = gpu::GetAlphaValue(top) << 8;

		vertex[1].x = w;
		vertex[1].y = h;
		vertex[1].Red = gpu::GetRedValue(bottom) << 8;
		vertex[1].Green = gpu::GetGreenValue(bottom) << 8;
		vertex[1].Blue = gpu::GetBlueValue(bottom) << 8;
		vertex[1].Alpha = gpu::GetAlphaValue(bottom) << 8;

		GRADIENT_RECT gRect;
		gRect.UpperLeft = 0;
		gRect.LowerRight = 1;

		drawGradientRect(x, y, w, h, &gRect, vertex, GPU_GRADIENT_V, a);
	}

	// Draw a rectangle with gradient from top to bottom with variable gradient
	void Context::drawGradientRectVVar(int x, int y, int w, int h, Color top, Color bottom, int startPoint, int endPoint, uint8_t a)
	{
		// Draw the first part of the rect
		if (startPoint > 0)
			drawRect(x, y, w, startPoint, top, a);

		// Draw gradient part
		TRIVERTEX vertex[2];
		vertex[0].x = 0;
		vertex[0].y = 0;
		vertex[0].Red = gpu::GetRedValue(top) << 8;
		vertex[0].Green = gpu::GetGreenValue(top) << 8;
		vertex[0].Blue = gpu::GetBlueValue(top) << 8;
		vertex[0].Alpha = gpu::GetAlphaValue(top) << 8;

		vertex[1].x = w;
		vertex[1].y = (endPoint - startPoint);
		vertex[1].Red = gpu::GetRedValue(bottom) << 8;
		vertex[1].Green = gpu::GetGreenValue(bottom) << 8;
		vertex[1].Blue = gpu::GetBlueValue(bottom) << 8;
		vertex[1].Alpha = gpu::GetAlphaValue(bottom) << 8;

		GRADIENT_RECT gRect;
		gRect.UpperLeft = 0;
		gRect.LowerRight = 1;

		drawGradientRect(x, y + startPoint, w, (endPoint - startPoint), &gRect, vertex, GPU_GRADIENT_V, a);

		// Draw the rest of the rect
		if (endPoint < w)
			drawRect(x, y + endPoint, w, (h - startPoint), top, a);
	}

	void Context::drawGradientRectD(
		int x, int y, int w, int h, Color top, Color middle, Color bottom,
		uint8_t a)
	{
		TRIVERTEX vertex[3];
		vertex[0].x = 0;
		vertex[0].y = 0;
		vertex[0].Red = gpu::GetRedValue(top) << 8;
		vertex[0].Green = gpu::GetBlueValue(top) << 8;
		vertex[0].Blue = gpu::GetGreenValue(top) << 8;
		vertex[0].Alpha = gpu::GetAlphaValue(top) << 8;

		vertex[1].x = (w / 2);
		vertex[1].y = (h / 2);
		vertex[1].Red = gpu::GetRedValue(middle) << 8;
		vertex[1].Green = gpu::GetGreenValue(middle) << 8;
		vertex[1].Blue = gpu::GetBlueValue(middle) << 8;
		vertex[1].Alpha = gpu::GetAlphaValue(middle) << 8;

		vertex[2].x = w;
		vertex[2].y = h;
		vertex[2].Red = gpu::GetRedValue(bottom) << 8;
		vertex[2].Green = gpu::GetBlueValue(bottom) << 8;
		vertex[2].Blue = gpu::GetGreenValue(bottom) << 8;
		vertex[2].Alpha = gpu::GetAlphaValue(bottom) << 8;

		// Create a GRADIENT_RECT structure that
		// references the TRIVERTEX vertices.
		GRADIENT_RECT gRect[2];
		gRect[0].UpperLeft = 0;
		gRect[0].LowerRight = 1;
		gRect[1].UpperLeft = 1;
		gRect[1].LowerRight = 2;

		drawGradientRect(x, y, w, h, gRect, vertex, GPU_GRADIENT_D, a);
	}

	void Context::drawGradientRect(int x, int y, int w, int h, GRADIENT_RECT *r, TRIVERTEX *v, GPUGradientMode m, uint8_t a)
	{
		// Create a 32-bit DIB section to ensure alpha channel support
		BITMAPINFO bmi = {};
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = w;
		bmi.bmiHeader.biHeight = -h; // Top-down
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32; // 32-bit
		bmi.bmiHeader.biCompression = BI_RGB;

		void *pBits = nullptr;
		HDC hdc = CreateCompatibleDC(bufferHdc);
		HBITMAP hBitmap = CreateDIBSection(bufferHdc, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
		SelectObject(hdc, hBitmap);

		if (m != GPU_GRADIENT_D)
		{
			GradientFill(hdc, v, 2, r, 1, m);
		}
		else
		{
			// Diagonal
		}

		drawAlpha(hdc, x, y, w, h, 0, 0, a);

		DeleteDC(hdc);
		DeleteObject(hBitmap);
	}

	void Context::drawAlpha(HDC hdc, int x, int y, int w, int h, int sx, int sy, uint8_t a) const
	{
		BLENDFUNCTION blend;
		blend.BlendOp = AC_SRC_OVER;
		blend.AlphaFormat = AC_SRC_ALPHA;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = static_cast<uint8_t>(a);
		BOOL result = AlphaBlend(
			bufferHdc,
			x + xOffset, y + yOffset,
			w * scaleFactor, h * scaleFactor,
			hdc,
			sx, sy,
			w, h,
			blend);

		if (!result)
		{
			DWORD error = GetLastError();
			LOG_APP("AlphaBlend error: 0x%x", error);
		}
	}

	/* Image class */

	void Image::initFromVRAMRect(
		const RectWH &rect, ColorDepth colorDepth, BlendMode blendMode)
	{
		int shift = 2 - int(colorDepth);

		u = (rect.x & 0x3f) << shift;
		v = rect.y & 0xff;
		width = rect.w << shift;
		height = rect.h;
		// TODO: implement
	}

	bool Image::initFromTIMHeader(const TIMHeader *header, BlendMode blendMode)
	{
		if (header->magic != 0x10)
			return false;

		auto ptr = reinterpret_cast<const uint8_t *>(&header[1]);

		if (header->flags & (1 << 3))
		{
			auto clut = reinterpret_cast<const TIMSectionHeader *>(ptr);

			// TODO: implement
			ptr += clut->length;
		}

		auto image = reinterpret_cast<const TIMSectionHeader *>(ptr);

		initFromVRAMRect(image->vram, ColorDepth(header->flags & 3), blendMode);
		return true;
	}

	void Image::drawScaled(
		Context &ctx, int x, int y, int w, int h, bool blend) const
	{
		// TODO: implement
	}

	void Image::draw(Context &ctx, int x, int y, int a) const
	{
		draw(ctx, x, y, 0, 0, width, height, a);
	}

	void Image::draw(Context &ctx, int x, int y, int sx, int sy, int w, int h, int a) const
	{
		if (imgHdc && imgBitmap)
		{
			ctx.drawAlpha(imgHdc, x, y, w, h, sx, sy, a);
		}
	}
}
