# IO System Overview

BemaniUX is designed to be modular and adaptable to most systems where a screen, input and half decent CPU are present. Part of this includes a modular I/O framework designed to allow the same app to work across multiple devices by simply changing the hardware definition. To support this goal, the application is expected to read I/O using the vInput and vOutput classes primarially, with vJoy, vMouse, vLight and vKeyboard being in place to support their namesakes.

## vInput
All input/output devices that are supported by BemaniUX are grouped into four main device classes. These are:
* Joystick (vJoy)
* Keyboard (vKey)
* Mouse (vMouse)
* Lightgun (vLight)

vInput is the main input handler sub system which co-ordinates the classes and serves as the API for the main application.

Each device in these classes is given an index number on a first-come-first-served basis. In the context of the program, whatever joystick device happens to be init'd first will be the first in the `Joystick` class, for example. In another example, if your hardware had a JAMMA input and the device handler for JAMMA is loaded first, it will be assumed that it is loaded into the first device index.

It should be noted, however, that because this order might change even between compiles, you shouldn't program the application to expect a single device to always assume this position, instead you should read the device's name/sub-class to determine the device type.

Whilst it's expected you will use the virtual inputs provided to receive a standardised input layout, if you wanted a specific device to read, based on a device ID, sub-class or similar, then you would call `JoyAbs(hash, 5)`, which would only return button 5 of that specific device, if it exists on the system. otherwise requesting `Joystick(1, 5)` would use the first device in the Joystick class and return button 5, whatever device the 1st joystick is.

In contrast, reading from a player's inputs, `GetPlayer(playerNumber)` will return a preformatted array of input data which inturn has come from the device/devices specified in the mapping file.

**Code mapping:**
* Bits 0-7    = Input Index (255 inputs max)
* Bits 8-9    = Input Type (Digital, Analogue, Rotary, Keypad)
* Bits 12-15  = Return type (Digital, Analogue, Keycode)
* Bits 16-23  = Device ID (within sub-category)
* Bits 24-27  = Device type sub-category (e.g. JOYSTICK class, sub-class 2, devID 1)
* Bits 28-31  = Device type category

#### Reading inputs
vInput can be polled as with a traditional input system, however for efficiency, you can use `getMessages()` to retrieve any updates that need to be processed. This means the application only needs to handle input when it actually has changed, i.e. when a controller has been removed.

The following messages exist:
* `IM_NONE` - No updates to process.
* `IM_INPUT` - An input updated state, read the returned `uint32_t code` to see which virtual input changed†.
* `IM_DEVICE_DISCONNECT` - A controller that was in use has been removed. Read the `uint32_t code` to see which device was removed.
* `IM_DEVICE_CONNECT` - A new controller has been detected and is ready for use.
* `IM_DEVICE_ERROR` - A controller in use has had multiple read errors. Read the `uint32_t code` to see which device was affected.

**†** - `IM_INPUT` is only added to the message buffer once between input reads as to not overrun the app with updates. 

#### Input structs
When reading from the vInput system, the following structs will be used to convey input data.

**JoystickState**
The `JoystickState` struct has three elements:
* `uint32_t digital` - A 32-bit integer holding up to 32 inputs in boolean form. The mapping is below.
* `int16_t analog` - An array of signed integers for the analog inputs. Has as many elements as `Input::MAX_ANALOG` specifies.
* `int16_t relative` - An array of signed integers for the relative/rotary delta inputs since last read. Has as many elements as `Input::MAX_RELATIVE` specifies.

**KeyState**
The `KeyState` struct has the following elements:
* `uin16_t codePoint` - UTF8 character

**MouseState**
The `MouseState` struct has the following elements:
* `int16_t x, y` - X/Y delta values for movement since last read.
* `int16_t sY, sX` - Scroll X/Y delta values since last read.
* `uint8_t buttons` - Mouse's current button states. Mapping is below.

**LightgunState**
The `LightgunState` struct has the following elements:
* `int16_t x, y` - X/Y absolute values for movement since last read.
* `uint8_t buttons` - Lightgun's current button states. Mapping is below.

#### Button Mappings
The following lists the mappings for the digital button states from vInput.
* Joystick:
  * Cabinet (Player 0):
    * 0: Test
    * 1: Tilt 1
    * 2: Tilt 2
    * 3: Tilt 3
    * 4: Service Up †
    * 5: Service Down †
    * 6: Service Left †
    * 5: Service Right †
  * Player 1 and up
    * 0: Start
    * 1: Service
    * 2: Up
    * 3: Down
    * 4: Left
    * 5: Right
    * 6: Button 1
    * 7: Button 2
    * -> 31: Button 26
* Mouse:
  * 0: Left
  * 1: Right
  * 2: Middle
  * 4: X-Left
  * 5: x-Right
  * 6-7: No use
* Lightgun:
  * 0: Trigger
  * 1: Reload/Pedal
  * 2: Action Button
  * 3: Switch weapon button
  * 4-7: Extra buttons †

**†** - These are for compatibility with some cabinets, please do not rely on these to be present.

#### Default Mappings
Despite the JoystickState having a defined layout, JoystickHandlers are not expected to return the button values in this layout, but rather as a boolean representation of the buttons at a given position. In some cases, such as with the Windows HID Joypad API, there is no default layout and as such controllers can have start or other buttons in any logical location. In another example, some controllers such as dance pads will see the X/Y axis for the directions but also use the digital buttons as the X/Y axis cannot report both U&D or L&R held at the same time. This makes it virtually impossible to make a "one-size fits all" type of mapping and so has to be manually adjusted in the mapping file.

On the other side, when running on a system with a known input scheme, for example the PS1 or 573, the inputs will have a set position at compile and run time. To facilitate the mapping of these controls to the respective inputs in vInput, a device handler can specify an ideal map for devices so that vInput will use that mapping scheme instead, if possible. Furthermore, if the device is related to a specific player (JAMMA Player 1 and Player 2, for example) then you can also specify this in the ideal map so that vInput can try to associate those controls with that player. If, however, the ideal map specifies Player 1 and it cannot map that control to vInput's Player 1, it will use the next available player slot.

If an ideal mapping layout is not given, vInput will use it's own generic layout to map device input to player controls.

### DeviceHandler
Device Handlers are the drivers of the input system. They are responsible for handling the hardware devices and converting the given information into a standard format. They can be single or multi-class capable, i.e. PS1 controller port driver for handling joypads and mice, or JVS.

The base class for all device handlers is the `DeviceHandler` class. However it is expected that a driver will use either the `JoystickHandler`, `KeyboardHandler`, `MouseHandler` or `Lightgun` derrived classes as their base class.

The `DeviceHandler` class expects the following functions to be overriden:
* `int init()` - (*Optional*) Use this if handler needs to be init'd after the app has started.
* `int reload()` - Reload is called if the IO system needs to be reset
* `void update()` - Update should be used to poll any devices
* `int getDeviceCount()` - Return how many devices this class handler current recognizes
* `const char* getDeviceName(int idx)` - (*Optional, but recomended*) Get's the given device index's null-terminated name.

Further to these, the specific device classes have further functions that need to be handled:

**JoystickHandler**
* `bool getSwitch(uint32_t code)` - Return a digital switch state using the given input code.
* `int getAnalog(uint32_t code)` - Return an analog using the given input code.
* `int getRelative(uint32_t code)` - Return a relative value (i.e. rotary delta) using the given input code.
* `int getSwitchCount(uint32_t code)` - Return the total amount of digital inputs.
* `int getAnalogCount(uint32_t code)` - Return the total amount of analog inputs.
* `int getRelativeCount(uint32_t code)` - Return the total amount of relative inputs.

**KeyboardHandler**
* `int getKeyboardType(int idx)` - Return the `Keyboard::KeyboardType` for the device at the given index.

**MouseHandler**
* `int getMouseType(int idx)` - Return the `Mouse::MouseType` for the device at the given index.

### Subsystem Overview
Below is a quick summary of the members that run alongside vInput.

#### vJoy
vJoy is the overall joystick handler for the vInput system. It reads from all of the connected and present joystick devices and will track their current state. Any changes will cause a `IM_INPUT` message to be sent to the vInput system. vJoy is not supposed to be directly interfaced with, use vInput for interaction or reading of input devices.

#### vKeyboard
vKeyboard is the overall keyboard manager for the vInput system, as well as serving as a virtual on-screen keyboard for systems that lack a physical keyboard. When using standard BemaniUX functions, vKeyboard will automatically appear on it's own layer when required. There shouldn't be a need to directly interface with vKeyboard.

#### vMouse
Like vKeyboard, vMouse is the mouse manager for handling mouse input in the UI. It supports multiple mice and cursors and will use it's own mouse cursor layer to display the cursor. When a mouse is clicked, the BemaniUX layering system will automatically determine what element was clicked on. **Note:** The first cursor will always be given priority over any actions.

#### vLightgun
vLightgun follows in the last two systems, having it's own layer for displaying lightgun crosshairs if required. The input is stored as an absolute X/Y system in the vInput system, with the buttons being given standard digital inputs.

## vOutput
vOutput is the output managment system. It is designed to abstract the outputs provided by existing hardware to a standard set of APIs.
As outputs to items like joypads are expected to be mapped to a given player's input device, programmers can get the associated output device's ID and handle to the input of the player's controls.