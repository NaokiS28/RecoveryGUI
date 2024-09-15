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
#include <string>

namespace Input
{
    constexpr const int MAX_DIGITAL = 32;   // I can't think of a time a device will have more than 32 inputs
    constexpr const int MAX_ANALOG = 6;
    constexpr const int MAX_RELATIVE = 6;

    enum InputMessage {
        IM_NONE,
        IM_DEVICE_CONNECT,
        IM_DEVICE_DISCONNECT,
        IM_INPUT,
        IM_INPUT_DIGITAL,
        IM_INPUT_ANALOG,
        IM_INPUT_RELATIVE,
        IM_MOUSE,
        IM_MOUSE_DIGITAL,
        IM_MOUSE_AXIS,
        IM_LIGHTGUN
    };

    // Input name: Get the encoded input name for IO functions. ID is the device hash if src is ABSOLUTE
    inline constexpr uint32_t getInputCode(uint8_t id, uint8_t inputType, uint8_t inputIdx)
    {
        return (0 | (id << 16) | (inputType << 8) | (inputIdx << 0));
    }
    inline constexpr uint32_t getInputCode(uint8_t inputType, uint8_t inputIdx){
        return getInputCode(0, inputType, inputIdx);
    }
    
    inline constexpr uint8_t getInputSrcSubClass(uint32_t code) { return ((code & 0x0F000000) >> 24); }
    inline constexpr uint8_t getInputSrcType(uint32_t code) { return ((code & 0xF0000000) >> 28); }
    inline constexpr uint8_t getInputDevId(uint32_t code) { return ((code & 0x00FF0000) >> 16); }
    inline constexpr uint8_t getReturnType(uint32_t code) { return ((code & 0x0000F000) >> 12); }
    inline constexpr uint8_t getInputType(uint32_t code) { return ((code & 0x00000F00) >> 8); }
    inline constexpr uint8_t getInputIdx(uint32_t code) { return ((code & 0x000000FF) >> 0); }

    inline constexpr uint8_t getInputClass(uint32_t code) { return getInputType(code); }

    // Inline code generators
    /*
    inline constexpr uint32_t Joystick(int idx, int btn) { 
        return (uint32_t)((Device::JoystickClass << 28) | (idx << 24) | btn);
    }

    inline constexpr uint32_t JoyAbs(int hash, int btn) { 
        return (uint32_t)((Device::JoystickClass << 28) | (hash << 24) | btn);
    }

    inline constexpr uint32_t Keyboard(int idx, int btn) { 
        return (uint32_t)((Device::KeyboardClass << 28) | (idx << 24) | btn);
    }

    inline constexpr uint32_t Mouse(int idx, int btn) { 
        return (uint32_t)((Device::MouseClass << 28) | (idx << 24) | btn);
    }

    inline constexpr uint32_t Lightgun(int idx, int btn) { 
        return (uint32_t)((Device::LightgunClass << 28) | (idx << 24) | btn);
    }
    */

    constexpr const char* CabinetTestName = "CABINET_TEST";
    constexpr const char* CabinetTiltName = "CABINET_TILT_%d";
    constexpr const char* CabinetCoinName = "CABINET_COIN_%d";

    constexpr const char* PlayerButtonNames[] =
    {
        "PLAYER%d_START",
        "PLAYER%d_SRV",
        "PLAYER%d_UP",
        "PLAYER%d_DN",
        "PLAYER%d_LF",
        "PLAYER%d_RT",
        "PLAYER%d_BUTTON_%d"
    };

    constexpr const char* PlayerAxisNames[] =
    {
        "PLAYER%d_X_AXIS",
        "PLAYER%d_Y_AXIS",
        "PLAYER%d_Z_AXIS",
        "PLAYER%d_RX_AXIS",
        "PLAYER%d_RY_AXIS",
        "PLAYER%d_RZ_AXIS",
        "PLAYER%d_%c_AXIS"
    };

    constexpr const char* PlayerDriveNames[] =
    {
        "PLAYER%d_STEERING_WHEEL",
        "PLAYER%d_ACCELERATOR",
        "PLAYER%d_BRAKE",
        "PLAYER%d_CLUTCH",
        "PLAYER%d_PADDLE_L",
        "PLAYER%d_PADDLE_R",
        "PLAYER%d_%c_AXIS"
    };

    constexpr const char* PlayerFlightNames[] =
    {
        "PLAYER%d_YOKE_X",
        "PLAYER%d_YOKE_Y",
        "PLAYER%d_RUDDER",
        "PLAYER%d_THROTTLE_1",
        "PLAYER%d_THROTTLE_2",
        "PLAYER%d_%c_AXIS"
    };

    constexpr const char* PlayerRelativeNames[] =
    {
        "PLAYER%d_SPINNER_%d"
    };

    typedef enum {
        None,
        Digital,
        Analog,
        Relative
    } InputType;

    enum PlayerNumbers
    {
        PLAYER_CABINET,
        PLAYER_1,
        PLAYER_2,
        PLAYER_3,
        PLAYER_4,
        PLAYER_5,
        PLAYER_6,
        PLAYER_7,
        PLAYER_8
    };

    enum MachineSwitch
    {
        MACHINE_TEST,
        MACHINE_TILT_1,
        MACHINE_TILT_2,
        MACHINE_TILT_3,
        MACHINE_TILT_MAX,
        MACHINE_COIN_1,
        MACHINE_COIN_2,
        MACHINE_COIN_3,
        MACHINE_COIN_4,
        MACHINE_COIN_MAX,
    };

    enum PlayerButtons
    {
        PLAYER_BTN_ST,
        PLAYER_BTN_SRV,
        PLAYER_BTN_UP,
        PLAYER_BTN_DN,
        PLAYER_BTN_LF,
        PLAYER_BTN_RT,
        PLAYER_BTN_1,
        PLAYER_BTN_2,
        PLAYER_BTN_3,
        PLAYER_BTN_4,
        PLAYER_BTN_5,
        PLAYER_BTN_6,
        PLAYER_BTN_7,
        PLAYER_BTN_8,
        PLAYER_BTN_9,
        PLAYER_BTN_10,
        PLAYER_BTN_11,
        PLAYER_BTN_12,
        PLAYER_BTN_13,
        PLAYER_BTN_14,
        PLAYER_BTN_15,
        PLAYER_BTN_16,
        PLAYER_BTN_17,
        PLAYER_BTN_18,
        PLAYER_BTN_19,
        PLAYER_BTN_20,
        PLAYER_BTN_21,
        PLAYER_BTN_22,
        PLAYER_BTN_23,
        PLAYER_BTN_24,
        PLAYER_BTN_25,
        PLAYER_BTN_26,
        PLAYER_BTN_27,
        PLAYER_BTN_28,
        PLAYER_BTN_29,
        PLAYER_BTN_30,
        PLAYER_BTN_31,
        PLAYER_BTN_32,
        PLAYER_BUTTON_MAX,
        PLAYER_BUTTON_MIN = PLAYER_BTN_1
    };

    enum PlayerAxis
    {
        PLAYER_X_AXIS,
        PLAYER_Y_AXIS,
        PLAYER_Z_AXIS,
        PLAYER_RX_AXIS,
        PLAYER_RY_AXIS,
        PLAYER_RZ_AXIS,
        PLAYER_AXIS_MAX
    };

    enum PlayerDriving
    {
        PLAYER_STEERING_WH,
        PLAYER_ACCELERATOR,
        PLAYER_BRAKE,
        PLAYER_CLUTCH,
        PLAYER_PADDLE_L,
        PLAYER_PADDLE_R,
        PLAYER_DRIVING_MAX
    };

    enum PlayerFlight
    {
        PLAYER_YOKE_X,
        PLAYER_YOKE_Y,
        PLAYER_RUDDER,
        PLAYER_THROTTLE_1,
        PLAYER_THROTTLE_2,
        PLAYER_FLIGHT_MAX
    };

    enum PlayerRelative {
        PLAYER_RELATIVE_MAX
    };

    enum DefaultDigitalMap : uint32_t {

    };

    enum DefaultAnalogMap : uint32_t {

    };

    enum DefaultRelativeMap : uint32_t {

    };
}

namespace Joystick {
    struct JoystickState {
        uint32_t digital;
        int16_t analog[Input::MAX_ANALOG];
        int16_t relative[Input::MAX_RELATIVE];
    };
}

namespace Mouse {
    enum MouseType{
        None,
        Virtual,            // Virtual mouse, movement is simulated
        Relative,           // Relative input, IE standard x/y mouse
        Absolute,           // Absolute input, IE tablet/touchscreen
        MaxType = Absolute
    };

    struct MouseMeta {
        MouseType type;
    };

    struct MouseState {
        int16_t x = 0;
        int16_t y = 0;
        int16_t sX = 0;
        int16_t sY = 0;
        uint8_t buttons = 0x00;
    };
}

namespace Keyboard {
    enum KeyboardType{
        None,
        Virtual,            // Virtual, onscreen keyboard
        Majong,
        Numeric,            // Numeric keypad, IE code entry keypad
        AlphaNumeric,       // Standard keyboard input
        MaxType = AlphaNumeric
    };

    struct KeyboardMeta {
        KeyboardType type;
    };

    struct KeyState {
        uint16_t codePoint = 0x0000;
    };
}

namespace Lightgun {
    typedef enum {
        None,
        XY,
        RasterScan,
        LightSensor,
        Touchscreen
    } LightgunType;

    struct LightgunState {
        int16_t x = 0;
        int16_t y = 0;
        uint8_t buttons = 0x00;
    };
}