#include "Joystick.hpp"

Joystick::Joystick(uint8_t DB9Pin1, uint8_t DB9Pin2, uint8_t DB9Pin3, uint8_t DB9Pin4, uint8_t DB9Pin6, uint8_t ExtraPin1, uint8_t ExtraPin2, uint8_t ExtraPin3, uint8_t ExtraPin4) {
    printf("Starting USB HID Device");
    board_init();
    tusb_init();
    printf("HID Device Initialised!");

    const PinDict Pins = {
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

void Joystick::ResetButton(Joystick::Button ButtonNumber) {
    uint32_t bit = 1;
    if (ButtonNumber < 32) {
        bit = bit << ButtonNumber;
        _buttons = _buttons & (~bit);
    }
}

void Joystick::SetButton(Joystick::Button ButtonNumber) {
    uint32_t bit = 1;
    if (ButtonNumber < 32) {
        bit = bit << ButtonNumber;
        _buttons = _buttons | bit;
    }
}

void Joystick::SetPosition(Joystick::Position PositionStruct) {
    _x_axis = PositionStruct.X;
    _y_axis = PositionStruct.Y;
}

Joystick::Status Joystick::GetStatus() {
    Status output;
    output.Stick.X = _x_axis;
    output.Stick.Y = _y_axis;
    output.Buttons = _buttons;

    return output;
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

void Joystick::ReadInputs() {
    PinValues.clear();

    uint32_t Reading = gpio_get_all();

    for (auto const& [PinName, Pin] : Pins) {
        bool value = 0;
        uint32_t bit = 1;
        bit = bit << Pin;
        value = (bit & Reading);
        PinValues.emplace(PinName, value);
    }

    if (PinValues["UP"]) {
        if (PinValues["LEFT"]) {
            CurrentPosition = UpLeft;
        }
        else if (PinValues["RIGHT"]) {
            CurrentPosition = UpRight;
        }
        else {
            CurrentPosition = Up;
        }
    }
    else if (PinValues["DOWN"]) {
        if (PinValues["LEFT"]) {
            CurrentPosition = DownLeft;
        }
        else if (PinValues["RIGHT"]) {
            CurrentPosition = DownRight;
        }
        else {
            CurrentPosition = Down;
        }
    }
    else if (PinValues["LEFT"]) {
        CurrentPosition = Left;
    }
    else if (PinValues["RIGHT"]) {
        CurrentPosition = Right;
    }
    else {
        CurrentPosition = Centre;
    }

    SetPosition(CurrentPosition);

    if (PinValues["FIRE"]) {
        SetButton(Fire);
    }
    else {
        ResetButton(Fire);
    }

    if (PinValues["EXTRA1"]) {
        SetButton(ExtraButton1);
    }
    else {
        ResetButton(ExtraButton1);
    }

    if (PinValues["EXTRA2"]) {
        SetButton(ExtraButton2);
    }
    else {
        ResetButton(ExtraButton2);
    }

    if (PinValues["EXTRA3"]) {
        SetButton(ExtraButton3);
    }
    else {
        ResetButton(ExtraButton3);
    }

    if (PinValues["EXTRA4"]) {
        SetButton(ExtraButton4);
    }
    else {
        ResetButton(ExtraButton4);
    }
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
