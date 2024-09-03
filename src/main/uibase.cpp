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

#include <stdint.h>
#include "common/util/log.hpp"
#include "common/util/templates.hpp"
#include "common/util/tween.hpp"
#include "common/util/units.hpp"
#include "hw/gpu.hpp"
#include "hw/gpufont.hpp"
#include "main/uibase.hpp"

#include "common/io/inputdefs.hpp"

namespace ui
{

	/* Button state manager */

	ButtonState::ButtonState(void)
		: _held(0), _prevHeld(0), _longHeld(0), _prevLongHeld(0), _pressed(0),
		  _released(0), _longPressed(0), _longReleased(0), _repeatTimer(0) {}

	uint8_t ButtonState::_getHeld(void) const
	{
		// TODO: implement
#if 0
	auto    inputs = io::getJAMMAInputs();
	auto    map    = _BUTTON_MAPPINGS[_buttonMap];
	uint8_t held   = 0;

	for (int i = 0; i < NUM_BUTTONS; i++) {
		if (inputs & map[i])
			held |= 1 << i;
	}

	return held;
#else
		return 0;
#endif
	}

	void ButtonState::reset(void)
	{
		_held = _getHeld();
		_prevHeld = _held;
		_longHeld = 0;
		_prevLongHeld = 0;

		_pressed = 0;
		_released = 0;
		_longPressed = 0;
		_longReleased = 0;
		_repeatTimer = 0;
	}

	void ButtonState::update(void)
	{
		_prevHeld = _held;
		_prevLongHeld = _longHeld;
		_held = _getHeld();

		uint32_t changed = _prevHeld ^ _held;

		if (changed & _held)
			_repeatTimer = 1;
		else if (changed & _prevHeld)
			_repeatTimer = 0;
		else if (_held && _repeatTimer)
			_repeatTimer++;

		_pressed = (changed & _held) & ~_pressed;
		_released = (changed & _prevHeld) & ~_released;
		_longHeld = (_repeatTimer >= REPEAT_DELAY) ? _held : 0;

		changed = _prevLongHeld ^ _longHeld;

		_longPressed = (changed & _longHeld) & ~_longPressed;
		_longReleased = (changed & _prevLongHeld) & ~_longReleased;
	}

	/* UI context */
	Context::Context(gpu::Context &gpuCtx, void *screenData)
		: _currentScreen(0), gpuCtx(gpuCtx), time(0), screenData(screenData)
	{
		util::clear(_screens);
		util::clear(backgrounds);
		util::clear(overlays);
	}

	void Context::init()
	{
		gpuCtx.init();
		//ioCtx.init();
		_ioCtx.init();

		SetTimer(gpuCtx.windowHandle, UI_REFRESH_TIMER, UI_REFRESH_INTERVAL, NULL);
	}

	void Context::show(Screen &screen, bool goBack, bool playSound)
	{
		auto oldScreen = getCurrentScreen();

		if (oldScreen)
			oldScreen->hide(*this, goBack);

		_currentScreen ^= 1;
		_screens[_currentScreen] = &screen;

		// if (playSound)
		// sounds[goBack ? SOUND_EXIT : SOUND_ENTER].play();

		screen.show(*this, goBack);
	}

	void Context::draw(void)
	{
		gpuCtx.beginDraw();

		auto oldScreen = getInactiveScreen();
		auto newScreen = getCurrentScreen();


		// Background layers (tiled background, time/version)
		for (auto layer : backgrounds)
		{
			if (layer)
				layer->draw(*this);
		}

		// Last screen that was drawn
		if (oldScreen)
			oldScreen->draw(*this, false);
		// Current screen to draw
		if (newScreen)
			newScreen->draw(*this, true);

		// Any overlays, such as the debug log or the splash screen
		for (auto layer : overlays)
		{
			if (layer)
				layer->draw(*this);
		}

		layerMan.render();

		gpuCtx.endDraw();
	}

	int Context::update(void)
	{
		// A lot of this is just handling Windows Messages

		_ioCtx.update();
		layerMan.update(time);

		MSG msg = {};
		BOOL bRet;
		if ((bRet = GetMessage(&msg, gpuCtx.windowHandle, 0, 0)) != 0)
		{
			if (bRet == -1)
			{
				// handle the error and possibly exit
				LOG_APP("Application error: bRet: %d", bRet);
				bRet = 0;
				return 1;
			}
			else if (bRet == 0)
			{
				return 1;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				// Existing code:
				buttons.update();

				auto screen = getCurrentScreen();

				if (screen)
					screen->update(*this);

				return 0;
			}
		}
		return bRet;
	}

	/* Layer classes */

	void Layer::_setOffset(Context &ctx, int x, int y) const
	{
		ctx.gpuCtx.setOffset(x, y);
	}

	void Layer::_setTexturePage(Context &ctx, uint16_t texpage, bool dither) const
	{
		ctx.gpuCtx.setTexturePage(texpage, dither);
	}

	void Layer::_setBlendMode(
		Context &ctx, gpu::BlendMode blendMode, bool dither) const
	{
		ctx.gpuCtx.setBlendMode(blendMode, dither);
	}

	/*
		TiledBackground::TiledBackground(Context &ctx)
		{
			ui = &ctx;
			LayerCallback layer = {
				LayerType::Background,
				LayerPriority::Bottom,
				{0, 0,
				 ui->gpuCtx.getHorizontalRes(),
				 ui->gpuCtx.getVerticalRes()},
				[this](){ draw(*ui); }
			};
			newLayer(layer);
		}
	*/

	void TiledBackground::draw(Context &ctx, bool active) const
	{
		_setOffset(ctx, 0, 0);
		//_setTexturePage(ctx, tile.texpage);

		int offsetX = uint32_t(ctx.time / 2) % tile.width;
		int offsetY = uint32_t(ctx.time / 3) % tile.height;

		for (int x = -offsetX; x < ctx.gpuCtx.width; x += tile.width)
		{
			for (int y = -offsetY; y < ctx.gpuCtx.height; y += tile.height)
				tile.draw(ctx.gpuCtx, x, y);
		}
	}

	void TextOverlay::draw(Context &ctx, bool active) const
	{
		_setOffset(ctx, 0, 0);
		int lineHeight = ctx.font.getLineHeight();

		gpu::RectWH rect;

		rect.y = ctx.gpuCtx.height - (8 + lineHeight);
		rect.h = lineHeight;

		if (leftText)
		{
			rect.x = 8;
			rect.w = ctx.gpuCtx.width - 16;
			ctx.font.draw(ctx.gpuCtx, leftText, rect, ctx.colors[COLOR_TEXT2]);
		}
		if (rightText)
		{
			int width = ctx.font.getStringWidth(rightText);

			rect.x = ctx.gpuCtx.width - (8 + width);
			rect.w = width;
			ctx.font.draw(ctx.gpuCtx, rightText, rect, ctx.colors[COLOR_TEXT2]);
		}
	}

	void SplashOverlay::draw(Context &ctx, bool active) const
	{
		int brightness = _fadeAnim.getValue(ctx.time);
		int imgAlpha = _imgFadeAnim.getValue(ctx.time);

		if (!brightness)
			return;

		// Backdrop
		_setOffset(ctx, 0, 0);
		ctx.gpuCtx.drawBackdrop(
			gpu::rgba(0, 0, 0, brightness));

		// Image
		// GDI Bitmaps have origin to top-left
		int x = (ctx.gpuCtx.width - image.width) / 2;
		int y = (ctx.gpuCtx.height - image.height) / 2;

		image.draw(ctx.gpuCtx, x, y, imgAlpha);
	}

	void SplashOverlay::show(Context &ctx)
	{
		if (!_fadeAnim.getTargetValue())
		{
			_fadeAnim.setValue(ctx.time, 0, 0xff, SPEED_SLOWEST);
		}
		if (!_imgFadeAnim.getTargetValue())
		{
			_imgFadeAnim.setValue(ctx.time, 0, 0xff, SPEED_SLOWEST);
		}
	}

	void SplashOverlay::hide(Context &ctx)
	{
		if (_fadeAnim.getTargetValue())
		{
			_fadeAnim.setValue(ctx.time, 0xff, 0, SPEED_SLOWEST);
		}
		if (_imgFadeAnim.getTargetValue())
		{
			_imgFadeAnim.setValue(ctx.time, 0xff, 0, SPEED_SLOWEST);
		}
	}

	void LogOverlay::draw(Context &ctx, bool active) const
	{
		int offset = _slideAnim.getValue(ctx.time);

		if (!offset)
			return;

		// Backdrop
		_setOffset(
			ctx, 0, offset - ctx.gpuCtx.height);
		ctx.gpuCtx.drawBackdrop(ctx.colors[COLOR_BACKDROP], gpu::GP0_BLEND_SUBTRACT);

		// Text
		int screenHeight = ctx.gpuCtx.height - SCREEN_MIN_MARGIN_Y * 2;
		int lineHeight = ctx.font.getLineHeight();

		gpu::Rect rect;

		rect.x1 = SCREEN_MIN_MARGIN_X;
		rect.y1 = SCREEN_MIN_MARGIN_Y;
		rect.x2 = ctx.gpuCtx.width - SCREEN_MIN_MARGIN_X;
		rect.y2 = SCREEN_MIN_MARGIN_Y + lineHeight;

		for (int i = (screenHeight / lineHeight) - 1; i >= 0; i--)
		{
			ctx.font.draw(
				ctx.gpuCtx, _buffer.getLine(i), rect, ctx.colors[COLOR_TEXT1]);

			rect.y1 = rect.y2;
			rect.y2 += lineHeight;
		}
	}

	void LogOverlay::toggle(Context &ctx)
	{
		bool shown = !_slideAnim.getTargetValue();

		_slideAnim.setValue(ctx.time, shown ? ctx.gpuCtx.height : 0, SPEED_SLOW);
		// ctx.sounds[shown ? SOUND_ENTER : SOUND_EXIT].play();
	}

	void ScreenshotOverlay::draw(Context &ctx, bool active) const
	{
		int brightness = _flashAnim.getValue(ctx.time);

		if (!brightness)
			return;

		_setOffset(ctx, 0, 0);
		ctx.gpuCtx.drawBackdrop(
			gpu::rgb(brightness, brightness, brightness), gpu::GP0_BLEND_ADD);
	}

	void ScreenshotOverlay::animate(Context &ctx)
	{
		_flashAnim.setValue(ctx.time, 0xff, 0, SPEED_SLOW);
		// ctx.sounds[ui::SOUND_SCREENSHOT].play();
	}

	void InputDebugOverlay::draw(Context &ctx, bool active) const
	{
		_setOffset(ctx, 0, 0);
		int lineHeight = ctx.font.getLineHeight();
		int lineWidth = ctx.font.getLineHeight() * 15;
		int16_t originX = static_cast<int16_t>(ctx.gpuCtx.width - lineWidth);
		int16_t originY = 0;

		gpu::RectWH bg = {originX, originY,
						  static_cast<int16_t>(lineWidth),
						  static_cast<int16_t>(ctx.gpuCtx.height)};

		ctx.gpuCtx.drawGradientRectHVar(
			bg, gpu::rgba(0, 0, 0, 0), gpu::rgba(0, 0, 0, 255),
			0, 20, units::percentToShort(50));

		gpu::Rect text;
		text.x1 = originX + 20 + SCREEN_MIN_MARGIN_X;
		text.y1 = originY + SCREEN_MIN_MARGIN_Y;
		text.x2 = originX + ctx.gpuCtx.width / 3;
		text.y2 = originY + SCREEN_MIN_MARGIN_Y + lineHeight;

		ctx.font.draw(
			ctx.gpuCtx, "Input Debug:", text, ctx.colors[COLOR_TEXT1]);
		text.y1 = text.y2 + lineHeight;
		text.y2 = text.y1 + lineHeight;

		/*
		int joyStickListLength = ctx.ioCtx.getDeviceCount(vInput::JoystickClass);
		char textBuffer[32];

		for (int i = 0; i < joyStickListLength; i++)
		{
			textBuffer[0] = '\0';
			snprintf(
				textBuffer, 32, "%s:", ctx.ioCtx.getDeviceName(vInput::JoystickClass, i));

			ctx.font.draw(
				ctx.gpuCtx, textBuffer, text, ctx.colors[COLOR_TEXT1]);

			text.y1 = text.y2;
			text.y2 += lineHeight;

			int analogCount = ctx.ioCtx.getRawInputCount(
				Input::getInputCode(vInput::JoystickClass, i, VirtualIO::INPUT_TYPE_ANALOG, 0));
			for (int a = 0; a < analogCount; a++)
			{
				textBuffer[0] = '\0';
				int code = Input::getInputCode(vInput::JoystickClass, i, VirtualIO::INPUT_TYPE_DIGITAL, a);
				int16_t value = ctx.ioCtx.getRawInputInt(code);
				snprintf(
					textBuffer, 32, "%d: %s - %x", a + 1, "n/a", // ctx.ioCtx.getRawInputName(i),
					value);

				ctx.font.draw(
					ctx.gpuCtx, textBuffer, text, ctx.colors[COLOR_TEXT1]);

				text.y1 = text.y2;
				text.y2 += lineHeight;
			}

			int switchCount = ctx.ioCtx.getRawInputCount(
				Input::getInputCode(vInput::JoystickClass, i, VirtualIO::INPUT_TYPE_DIGITAL, 0));
			for (int s = 0; s < switchCount; s++)
			{
				textBuffer[0] = '\0';
				int code = Input::getInputCode(vInput::JoystickClass, i, VirtualIO::INPUT_TYPE_DIGITAL, s);
				bool state = ctx.ioCtx.getRawInputState(code);
				snprintf(
					textBuffer, 32, "%d: %s - %s", s + 1, "n/a", // ctx.ioCtx.getRawInputName(i),
					(state ? "ON" : "OFF"));

				ctx.font.draw(
					ctx.gpuCtx, textBuffer, text, ctx.colors[COLOR_TEXT1]);

				text.y1 = text.y2;
				text.y2 += lineHeight;
			}
		}
		*/
	}

	/* Base screen classes */

	void AnimatedScreen::_setOffset(
		Context &ctx, int x, int y, int width, int height) const
	{
		Screen::_setOffset(ctx, x + _slideAnim.getValue(ctx.time), y);
	}

	void AnimatedScreen::show(Context &ctx, bool goBack)
	{
		int width = ctx.gpuCtx.width;
		_slideAnim.setValue(ctx.time, goBack ? (-width) : width, 0, SPEED_SLOW);
	}

	void AnimatedScreen::hide(Context &ctx, bool goBack)
	{
		int width = ctx.gpuCtx.width;
		_slideAnim.setValue(ctx.time, 0, goBack ? width : (-width), SPEED_SLOW);
	}

	void BackdropScreen::show(Context &ctx, bool goBack)
	{
		if (!_fadeAnim.getTargetValue())
			_fadeAnim.setValue(ctx.time, 0, 0x50, SPEED_FAST);
	}

	void BackdropScreen::hide(Context &ctx, bool goBack)
	{
		if (_fadeAnim.getTargetValue())
			_fadeAnim.setValue(ctx.time, 0x50, 0, SPEED_FAST);
	}

	void BackdropScreen::draw(Context &ctx, bool active) const
	{
		int brightness = _fadeAnim.getValue(ctx.time);

		if (!brightness)
			return;

		_setOffset(ctx, 0, 0);
		ctx.gpuCtx.drawBackdrop(
			gpu::rgb(brightness, brightness, brightness), gpu::GP0_BLEND_ADD);
	}

	ModalScreen::ModalScreen(int width, int height)
		: _width(width), _height(height), _title(nullptr), _body(nullptr) {}

	void ModalScreen::show(Context &ctx, bool goBack)
	{
		BackdropScreen::show(ctx, goBack);

		_titleBarAnim.setValue(ctx.time, 0, _width, SPEED_SLOW);
	}

	void ModalScreen::draw(Context &ctx, bool active) const
	{
		_setOffset(ctx, 0, 0);
		BackdropScreen::draw(ctx, active);

		if (active)
		{
			int windowHeight = TITLE_BAR_HEIGHT + _height;

			_setOffset(
				ctx, (ctx.gpuCtx.width - _width) / 2,
				(ctx.gpuCtx.height - windowHeight) / 2);

			//_setBlendMode(ctx, gpu::GP0_BLEND_SEMITRANS, true);

			// Window
			ctx.gpuCtx.drawGradientRectD(
				0, 0, _width, windowHeight, ctx.colors[COLOR_WINDOW1],
				ctx.colors[COLOR_WINDOW2], ctx.colors[COLOR_WINDOW3]);
			ctx.gpuCtx.drawGradientRectH(
				0, 0, _titleBarAnim.getValue(ctx.time), TITLE_BAR_HEIGHT,
				ctx.colors[COLOR_ACCENT1], ctx.colors[COLOR_ACCENT2]);
			ctx.gpuCtx.drawRect(
				_width, SHADOW_OFFSET, SHADOW_OFFSET, windowHeight,
				ctx.colors[COLOR_SHADOW]);
			ctx.gpuCtx.drawRect(
				SHADOW_OFFSET, windowHeight, _width - SHADOW_OFFSET, SHADOW_OFFSET,
				ctx.colors[COLOR_SHADOW]);

			// Text
			gpu::Rect rect;

			rect.x1 = TITLE_BAR_PADDING;
			rect.y1 = TITLE_BAR_PADDING;
			rect.x2 = _width - TITLE_BAR_PADDING;
			rect.y2 = TITLE_BAR_PADDING + ctx.font.getLineHeight();
			// rect.y2 = TITLE_BAR_HEIGHT - TITLE_BAR_PADDING;
			ctx.font.draw(ctx.gpuCtx, _title, rect, ctx.colors[COLOR_TITLE]);

			rect.y1 = TITLE_BAR_HEIGHT + MODAL_PADDING;
			rect.y2 = _height - MODAL_PADDING;
			ctx.font.draw(ctx.gpuCtx, _body, rect, ctx.colors[COLOR_TEXT1], true);
		}
	}
}
