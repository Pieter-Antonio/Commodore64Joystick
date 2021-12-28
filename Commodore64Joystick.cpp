#include "Joystick.hpp"

const uint8_t DB9Pin1 = 10;
const uint8_t DB9Pin2 = 11;
const uint8_t DB9Pin3 = 12;
const uint8_t DB9Pin4 = 13;
const uint8_t DB9Pin6 = 15;
const uint8_t ExtraPin1 = 21;
const uint8_t ExtraPin2 = 20;
const uint8_t ExtraPin3 = 19;
const uint8_t ExtraPin4 = 18;

int main() {
    stdio_init_all();
    sleep_ms(3000);

    Joystick C64Joy(DB9Pin1, DB9Pin2, DB9Pin3, DB9Pin4, DB9Pin6, ExtraPin1, ExtraPin2, ExtraPin3, ExtraPin4);

    while (true) {
        C64Joy.ReadInputs();
        C64Joy.USBUpdate();
    }

    return 0;
}