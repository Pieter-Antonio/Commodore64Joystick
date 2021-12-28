# Commodore64Joystick

This repository contains a C/C++ program to use the Raspberry Pi Pico as an adapter for the original Commodore 64 Joystick. It uses it's digital inputs to read the position of the joystick and sends the information over USB to a computer. Four extra buttons were added for more flexibility.

The device manifests itself as a HID joystick for compatibility in games.

## Compiling on Linux
Make sure the Pico C/C++ SDK is installed and available in your PATH

    mkdir build
    cd build
    cmake ..
    make -j4

Finally, copy the generated UF2 file to the Raspberry Pi Pico in BOOTSEL mode.