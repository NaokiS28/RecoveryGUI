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

/*
    Todo:
    [ ] - Interface with IO vKey
    [ ] - Code is a bit messy. Cleaning up would be nice
    [ ] - Asian language support
    [ ] - Connect up modifier keys
    [ ] - Cyrillic support
*/

#pragma once

#include <cstring>
#include <array>
#include <vector>

#include "hw/gpu.hpp"
#include "hw/gpufont.hpp"
#include "common/io/devhandler.hpp"
#include "common/util/tween.hpp"
#include "common/util/units.hpp"
#include "common/ui/lang/locale.hpp"
#include "common/ui/layerman.hpp"

using namespace Device;

namespace OSKeyboard
{
    constexpr const char *const OSKName = "vKey OSK";
}

class OSKeyDev : public KeyboardHandler
{
public:
    OSKeyDev()
    {
        _deviceClasses[0] = DeviceClass::KeyboardClass;
    }

    int init() override { return 0; }
    void update() override {}
    int reload() override { return 0; }

    void clearBuffer() {}
    void type(const char *c) {}

    int getDeviceCount() override { return 1; }
    int getKeyboardType(int idx) override { return Keyboard::AlphaNumeric; }
    const char *getDriverName() override { return OSKeyboard::OSKName; }
};

namespace OSKeyboard
{
    constexpr const int OSKPadding = 10;
    constexpr const int OSKSpacing = 3;
    constexpr const int OSKKeySize = 15;

    enum AnimState
    {
        Hidden,
        Opening,
        Open,
        Hiding
    };

    struct KeyMetric
    {
        int x, y, w, h, r, c;
        int type;
        const char *text = nullptr;
    };

    class OSKeyLayer : public layers::Layer
    {
    private:
        bool _enabled = false;
        int _selKeyRow = 0, _selKeyCol = 0; // Active/selected key (starts at 1,1, 0,0 means no key)

        AnimState _animationState = Hidden;
        util::Tween<int, util::QuadOutEasing> _popupAnim;

        int _keyModifiers = locale::ModNone;

        locale::LocaleStruct _inputLocale = locale::locales.front();
        std::vector<KeyMetric> _keyLayout;
        gpu::RectWH _keyboardRect; // BG and total size

        OSKeyDev *_oskDev = nullptr;

        inline bool _keySelected(int r, int c) const
        {
            return ((r + 1 == _selKeyRow) && (c + 1 == _selKeyCol));
        }

        inline void _calcKeyboardRect()
        {
            _keyboardRect.x = 0;
            _keyboardRect.y = 0;
            _keyboardRect.w = ((OSKPadding * 2) +
                               (OSKKeySize * _inputLocale.keyLayout.cols) +
                               (OSKSpacing * (_inputLocale.keyLayout.cols - 1)));
            _keyboardRect.h = ((OSKPadding * 2) +
                               (OSKKeySize * _inputLocale.keyLayout.rows) +
                               (OSKSpacing * (_inputLocale.keyLayout.rows - 1)));
        }
        int _getKeyIdx(int row, int col);
        void _setActiveKey(int row, int col);
        void _loadLayout();
        void _drawKey(
            gpu::Context &ctx, gpu::Font &font,
            gpu::Color *color,
            const KeyMetric &key,
            bool selected) const;
        void _type();

    public:
        OSKeyLayer(int x, int y, int w, int h) : Layer(x, y, w, h, layers::LayerType::Overlay, layers::LayerPriority::Top) {}
        void draw(gpu::Context &ctx, gpu::Font &font, gpu::Color *color, uint32_t time) const;
        void resize(int hRes, int vRes)
        {
            _calcKeyboardRect();
            x = (hRes / 2) - (_keyboardRect.w / 2);
            y = ((vRes - OSKPadding) - _keyboardRect.h);
        }
        
        void update(uint32_t time);
        void show();
        void hide();
        void onSelect();
        void onClick();
        void nextItem();
        void prevItem();
        void mouseMove(int x, int y);

        const char *getName() { return OSKName; }

        void setOSKDev(DeviceHandler *osk)
        {
            // This layer will only accept OSK devices
            if (!strcmp(OSKName, osk->getDriverName()))
            {
                _oskDev = dynamic_cast<OSKeyDev *>(osk);
            }
        }
    };
}

REGISTER_DEVICE_HANDLER(OSKeyDev)