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
#include <cstdio>
#include <cstring>
#include <vector>
#include <memory>

/*
    Todo:
    [ ] - AnalogInput - Add hystereis
    [ ] - AnalogInput - Clean up digital input section?
    [ ] - Condense "button" held into Input classes?
    [ ] - Is XOR grouped inputs even useful?
    [ ] - General clean up
*/

namespace VirtualIO
{
    enum InputType : int
    {
        INPUT_TYPE_ERROR,
        INPUT_TYPE_DIGITAL,
        INPUT_TYPE_ANALOG,
        INPUT_TYPE_ROTARY,
        INPUT_TYPE_KEYCODE
    };

    enum InputState : bool
    {
        INPUT_STATE_INACTIVE = false,
        INPUT_STATE_ACTIVE = true
    };

    enum InputSource
    {
        INPUT_SRC_NONE,     // Do not use, this is for code clarity
        INPUT_SRC_KEYBOARD, 
        INPUT_SRC_MOUSE,    
        INPUT_SRC_LIGHTGUN, 
        INPUT_SRC_JOYSTICK, 
        INPUT_SRC_JVS,      
        INPUT_SRC_JAMMA,    
        INPUT_SRC_ABSOLUTE,  // Input can only come from a specific device hash
        INPUT_SRC_GROUP     // Input is combined input using input group
    };

    inline const uint8_t INPUT_SRC_ALL = 0xFF;
    // Input name: Get the encoded input name for IO functions. ID is the device hash if src is ABSOLUTE
    inline constexpr uint32_t getInputCode(uint8_t src, uint8_t id, uint8_t inputType, uint8_t inputIdx){
        return (0 | (src << 24) | (id << 16) | (inputType << 8) | (inputIdx << 0));
    }
    inline constexpr uint8_t getInputSrcType(uint32_t code) { return ((code & 0xFF000000) >> 24); }
    inline constexpr uint8_t getInputDevId(uint32_t code) { return ((code & 0x00FF0000) >> 16); }
    inline constexpr uint8_t getInputType(uint32_t code) { return ((code & 0x0000FF00) >> 8); }
    inline constexpr uint8_t getInputIdx(uint32_t code) { return ((code & 0x000000FF) >> 0); }

    inline constexpr const char* getInputName(uint32_t code){
        uint8_t devId = getInputDevId(code) + 1;
        //uint8_t idx = getInputIdx(code);

        char srcName[32];
        switch (getInputSrcType(code)){
            case INPUT_SRC_MOUSE: snprintf(srcName, 32, "%s%d_%s", "MOUSE", devId, ""); break;
            case INPUT_SRC_KEYBOARD: snprintf(srcName, 32, "%s_%s", "KEY", ""); break;
            case INPUT_SRC_JOYSTICK: snprintf(srcName, 32, "%s%d_%s", "JOY", devId, ""); break;
            case INPUT_SRC_LIGHTGUN: snprintf(srcName, 32, "%s%d_%s", "LIGHTGUN", devId, ""); break;
            case INPUT_SRC_JVS: snprintf(srcName, 32, "%s%d_%s", "JVS", devId, ""); break;
            case INPUT_SRC_JAMMA: snprintf(srcName, 32, "%s_%s", "JAMMA", ""); break;
            case INPUT_SRC_ABSOLUTE: snprintf(srcName, 32, "%s-%X_%s", "DEV", devId, ""); break;
            default: return "INVALID_SRC";
        }
    }

    /*
        Mapping system uses 32-bit codes and flags to set processing instructions
        An input map must start with GROUP_START(INPUT) and end with GROUP_END(INPUT).
        Nesting is allowed, this is done with GROUP_START() and terminated with
        GROUP_END(). This allows the end user to create complex maps for
        multi-function buttons and combos.
    */

    enum MapModifiers
    {
        IO_MAP_GRP_START, // Map group to combine similar inputs start flag
        IO_MAP_GRP_END,   // Map group end flag
        IO_MAP_OR,        // Any group inputs can be active to trigger output
        IO_MAP_AND,       // Group inputs must all be active to trigger output
        IO_MAP_XOR,       // Any group inputs can be active, but not all of them to trigger output
        IO_MAP_NAND,      // Group inputs must be all inactive to trigger output
        IO_MAP_NOR        // (NOT for sigular) Any group inputs inactive will trigger output
    };

    // Input group allows you to group more than one Input into a single digital signal
    class InputGroup //: public Input
    {
        // Todo: Export array to mapping names for saving to file
        //std::vector<std::unique_ptr<Input>> inputArr;
        uint8_t arrSize = 0;
        uint8_t groupMode = IO_MAP_OR;

    public:
        //void addInput(Input &i);
        //void removeInput(Input &i);
        // int getMapping()
        bool getState();
        inline int16_t getValue() { return (getState() ? 0x0000 : 0xFFFF); }
    };

    class KeyMap
    {
    public:
        KeyMap(uint32_t b)
        {
            button = b;
        }

        uint32_t button = 0x00;
    };
}