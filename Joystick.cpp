#include "Joystick.hpp"

Joystick::Joystick(uint8_t DB9Pin1, uint8_t DB9Pin2, uint8_t DB9Pin3, uint8_t DB9Pin4, uint8_t DB9Pin6, uint8_t ExtraPin1, uint8_t ExtraPin2, uint8_t ExtraPin3, uint8_t ExtraPin4) {
    printf("Starting USB HID Device");
    board_init();
    tusb_init();
    printf("HID Device Initialised!");

    Pins = {
        {(std::string)"UP", DB9Pin1},
        {(std::string)"DOWN", DB9Pin2},
        {(std::string)"LEFT", DB9Pin3},
        {(std::string)"RIGHT", DB9Pin4},
        {(std::string)"FIRE", DB9Pin6},
        {(std::string)"EXTRA1", ExtraPin1},
        {(std::string)"EXTRA2", ExtraPin2},
        {(std::string)"EXTRA3", ExtraPin3},
        {(std::string)"EXTRA4", ExtraPin4}
    };

    for (auto const& [PinName, Pin] : Pins) {
        gpio_init(Pin);
        gpio_set_dir(Pin, GPIO_IN);
        gpio_pull_up(Pin);
    }

    printf("GPIO Initialised!");

    ResetAll();
    USBUpdate();
}

void Joystick::ResetAll() {
    _buttons = 0x0;

    _x_axis = 0;
    _y_axis = 0;
}

void Joystick::ResetAxis(Joystick::Axis AxisLetter) {
    if (AxisLetter == x) {
        _x_axis = 0;
    }
    else if (AxisLetter == y) {
        _y_axis = 0;
    }
}

void Joystick::ResetButton(Joystick::Button ButtonNumber) {
    uint32_t bit = 1;
    bit = bit << ButtonNumber;
    _buttons = _buttons & (~bit);
}

void Joystick::SetAxis(Joystick::Axis AxisLetter, Joystick::Direction DirectionNumber) {
    if (AxisLetter == x) {
        _x_axis = DirectionNumber;
    }
    else if (AxisLetter == y) {
        _y_axis = DirectionNumber;
    }
}

void Joystick::SetButton(Joystick::Button ButtonNumber) {
    uint32_t bit = 1;
    bit = bit << ButtonNumber;
    _buttons = _buttons | bit;
}

bool Joystick::CheckDirection(Joystick::Axis AxisLetter, Joystick::Direction DirectionNumber) {
    if (AxisLetter == x) {
        if (_x_axis == DirectionNumber) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (AxisLetter == y) {
        if (_y_axis == DirectionNumber) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool Joystick::CheckButton(Joystick::Button ButtonNumber) {
    uint32_t bit = 1;
    bit = bit << ButtonNumber;

    if (_buttons & bit) {
        return true;
    }

    else {
        return false;
    }
}

Joystick::Status Joystick::GetStatus() {
    Status output;
    output.X = _x_axis;
    output.Y = _y_axis;
    output.Buttons = _buttons;

    return output;
}

void Joystick::ReadInputs() {
    // If the stick is pressed, but was not
    if ( ( !gpio_get(Pins["UP"]) ) && ( !CheckDirection(y, Up) ) ) {
        SetAxis(y, Up);
    }

    else if ( ( !gpio_get(Pins["DOWN"]) ) && ( !CheckDirection(y, Down) ) ) {
        SetAxis(y, Down);
    }

    if ( ( !gpio_get(Pins["LEFT"]) ) && ( !CheckDirection(x, Left) ) ) {
        SetAxis(x, Left);
    }

    else if ( ( !gpio_get(Pins["RIGHT"]) ) && ( !CheckDirection(x, Right) ) ) {
        SetAxis(x, Right);
    }

    // If the button is pressed, but was not
    if ( ( !gpio_get(Pins["FIRE"]) ) && ( !CheckButton(Fire) ) ) {
        SetButton(Fire);
    }

    if ( ( !gpio_get(Pins["EXTRA1"]) ) && ( !CheckButton(ExtraButton1) ) ) {
        SetButton(ExtraButton1);
    }

    if ( ( !gpio_get(Pins["EXTRA2"]) ) && ( !CheckButton(ExtraButton2) ) ) {
        SetButton(ExtraButton2);
    }

    if ( ( !gpio_get(Pins["EXTRA3"]) ) && ( !CheckButton(ExtraButton3) ) ) {
        SetButton(ExtraButton3);
    }

    if ( ( !gpio_get(Pins["EXTRA4"]) ) && ( !CheckButton(ExtraButton4) ) ) {
        SetButton(ExtraButton4);
    }

    // If the stick is not pressed, but was previously

    if ( ( ( gpio_get(Pins["UP"]) ) && ( CheckDirection(y, Up) ) ) || ( ( gpio_get(Pins["DOWN"]) ) && ( CheckDirection(y, Down) ) ) ) {
        ResetAxis(y);
    }

    if ( ( ( gpio_get(Pins["LEFT"]) ) && ( CheckDirection(x, Left) ) ) || ( ( gpio_get(Pins["RIGHT"]) ) && ( CheckDirection(x, Right) ) ) ) {
        ResetAxis(x);
    }

    // If the button is not pressed, but was previously
    if ( ( gpio_get(Pins["FIRE"]) ) && ( CheckButton(Fire) ) ) {
        ResetButton(Fire);
    }

    if ( ( gpio_get(Pins["EXTRA1"]) ) && ( CheckButton(ExtraButton1) ) ) {
        ResetButton(ExtraButton1);
    }

    if ( ( gpio_get(Pins["EXTRA2"]) ) && ( CheckButton(ExtraButton2) ) ) {
        ResetButton(ExtraButton2);
    }

    if ( ( gpio_get(Pins["EXTRA3"]) ) && ( CheckButton(ExtraButton3) ) ) {
        ResetButton(ExtraButton3);
    }

    if ( ( gpio_get(Pins["EXTRA4"]) ) && ( CheckButton(ExtraButton4) ) ) {
        ResetButton(ExtraButton4);
    }
}

void Joystick::USBUpdate() {
    if (tud_suspended()) {
        tud_remote_wakeup();
    }

    if (tud_hid_ready()) {
        tud_hid_gamepad_report(1, _x_axis, _y_axis, 0, 0, 0, 0, 0, _buttons);
    }

    tud_task();
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
    // TODO not Implemented
    (void) itf;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
    // This example doesn't use multiple report and report ID
    (void) itf;
    (void) report_id;
    (void) report_type;

    // echo back anything we received from host
    tud_hid_report(0, buffer, bufsize);
}
