#include <stdio.h>
#include "pico/stdlib.h"
#include <map>
#include <string>
#include "bsp/board.h"
#include "tusb.h"

#define AXISMIN 127
#define AXISMAX -127
#define AXISCENTRE 0

class Joystick {
    public:
        struct Position {
            int8_t X;
            int8_t Y;
        };
        struct Status {
            Position Stick;
            uint32_t Buttons;
        };

        Joystick(uint8_t DB9Pin1, uint8_t DB9Pin2, uint8_t DB9Pin3, uint8_t DB9Pin4, uint8_t DB9Pin6, uint8_t ExtraPin1, uint8_t ExtraPin2, uint8_t ExtraPin3, uint8_t ExtraPin4);
        void ResetAll();
        void ReadInputs();
        Status GetStatus();
        void USBUpdate();

        const Position Up = {AXISCENTRE, AXISMAX};
        const Position Down = {AXISCENTRE, AXISMIN};
        const Position Left = {AXISMAX, AXISCENTRE};
        const Position Right = {AXISMIN, AXISCENTRE};
        const Position Centre = {AXISCENTRE, AXISCENTRE};
        const Position UpLeft = {AXISMAX, AXISMAX};
        const Position UpRight = {AXISMIN, AXISMAX};
        const Position DownLeft = {AXISMAX, AXISMIN};
        const Position DownRight = {AXISMIN, AXISMIN};


    private:
        enum Button {Fire, ExtraButton1, ExtraButton2, ExtraButton3, ExtraButton4};

        void SetPosition(Position PositionStruct);
        void SetButton(Button ButtonNumber);
        void ResetButton(Button ButtonNumber);

        Position CurrentPosition;
        typedef std::map<std::string, uint8_t> PinDict;
        PinDict Pins;
        typedef std::map<std::string, bool> PinValueDict;
        PinValueDict PinValues;
        int8_t _x_axis;
        int8_t _y_axis;
        uint32_t _buttons;
};