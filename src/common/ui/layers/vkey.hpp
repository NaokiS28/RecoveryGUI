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
    [ ] - Load and draw graphics
    [ ] - Interface with IO vKey
*/

#pragma once

#include <cstring>

#include "hw/gpu.hpp"
#include "hw/gpufont.hpp"
#include "common/io/devhandler.hpp"
#include "common/util/tween.hpp"
#include "common/util/units.hpp"
#include "common/util/locale.hpp"
#include "common/ui/layerman.hpp"

using namespace Device;

namespace vKeyboard
{
    constexpr const char *const OSKName = "vKey OSK";
}

class OSKeyboard : public KeyboardHandler
{
public:
    OSKeyboard()
    {
        _deviceClasses[0] = DeviceClass::KeyboardClass;
    }

    int init() { return 0; }
    void update() {}
    int reload() { return 0; }

    void clearBuffer(){}
    void type(uint16_t codePoint){}

    int getDeviceCount() { return 1; }
    int getKeyboardType(int idx) { return Keyboard::AlphaNumeric; }
    const char *getSubClassName(int idx) { return vKeyboard::OSKName; }
};

namespace vKeyboard
{
    constexpr const int OSKHeight = 110;
    constexpr const int OSKWidth = 300;
    constexpr const int OSKPadding = 10;
    constexpr const int OSKSpacing = 5;
    constexpr const int OSK_KeySize = 15;

    enum KeyType {
        NextRow,
        Single,
        WideSingle,
        DoubleWide,
        Enter,
        Space,
        Backspace,
        Shift,
        HalfBlank,
        Blank,
        Cursor,
        Diacritics,
        End
    };

    constexpr const int OSKLayout[][14] = {
        {Single, 12, Backspace, 1, NextRow},
        {Single, 12, Enter, 1, NextRow},
        {Single, 12, NextRow},
        {Shift, 1, Single, 11, Shift, 1, NextRow},
        {Diacritics, 1, Blank, 1, HalfBlank, 1, Space, 1, Blank, 1, Cursor, 4, End}
    };

    constexpr const char *OSKModifiers[] = { "Ä▲►▼◄↵⌫⇧" };

    class vKeyboard : public layers::Layer
    {
    private:
        bool _enabled = true;
        int keyX = 0, keyY = 0;     // Active/selected key
        uint8_t _inputLocale = locale::IT;
        
        util::Tween<int, util::QuadOutEasing> _popupAnim;
        OSKeyboard *_oskDev = nullptr;

    public:
        vKeyboard(int x, int y, int w, int h) : Layer(x, y, w, h, layers::LayerType::Overlay, layers::LayerPriority::Top) {}
        void draw(gpu::Context *ctx, gpu::Font *font, gpu::Color *color, uint32_t time) const;
        void resize(int hRes, int vRes)
        {
            w = hRes;
            h = vRes;
        }
        void show();
        void hide();
        void nextItem() {}

        const char* getName(){ return OSKName; }

        void setOSKDev(DeviceHandler *osk)
        {
            // This layer will only accept OSK devices
            if (!strcmp(OSKName, osk->getSubClassName(0))){
                _oskDev = dynamic_cast<OSKeyboard *>(osk);
            }
        }
    };
}

REGISTER_DEVICE_HANDLER(OSKeyboard)