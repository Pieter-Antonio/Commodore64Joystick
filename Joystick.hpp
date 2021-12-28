#include <stdio.h>
#include "pico/stdlib.h"
#include <map>
#include <string>
#include "bsp/board.h"
#include "tusb.h"

class Joystick {
    typedef int8_t Direction;
    enum Axis {x, y};
    enum Button {Fire, ExtraButton1, ExtraButton2, ExtraButton3, ExtraButton4};
    struct Status {
        Direction X;
        Direction Y;
        uint32_t Buttons;
    };

    public:
        Joystick(uint8_t DB9Pin1, uint8_t DB9Pin2, uint8_t DB9Pin3, uint8_t DB9Pin4, uint8_t DB9Pin6, uint8_t ExtraPin1, uint8_t ExtraPin2, uint8_t ExtraPin3, uint8_t ExtraPin4);
        void ResetAll();
        bool CheckDirection(Axis AxisLetter, Direction DirectionNumber);
        bool CheckButton(Button ButtonNumber);
        void ReadInputs();
        Status GetStatus();
        void USBUpdate();
        std::map<std::string, uint8_t> Pins;

    private:
        void ResetAxis(Axis AxisLetter);
        void ResetButton(Button ButtonNumber);
        void SetAxis(Axis AxisLetter, Direction DirectionNumber);
        void SetButton(Button ButtonNumber);
        const Direction Up = -127;
        const Direction Down = 127;
        const Direction Left = -127;
        const Direction Right = 127;
        Direction _x_axis = 0;
        Direction _y_axis = 0;
        uint32_t _buttons = 0x00000000;
};