# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/usr/share/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Commodore64Joystick C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Commodore64Joystick
        Commodore64Joystick.cpp
        usb_descriptors.cpp
        Joystick.cpp )

pico_set_program_name(Commodore64Joystick "Commodore64Joystick")
pico_set_program_version(Commodore64Joystick "2.1")

pico_enable_stdio_uart(Commodore64Joystick 1)
pico_enable_stdio_usb(Commodore64Joystick 0)

pico_add_extra_outputs(Commodore64Joystick)

target_include_directories(Commodore64Joystick PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Add the standard library to the build
target_link_libraries(Commodore64Joystick PRIVATE pico_stdlib tinyusb_device tinyusb_board)

