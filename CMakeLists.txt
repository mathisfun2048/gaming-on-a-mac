cmake_minimum_required(Version 3.13)
# this makes sure CMake is version 3.13+ to build this project
# CMake is a build system generator -- it makes the build files (like makefiles) needed to compile a project
# Build files are teh instructions that tell your compiler how to turn source code into binary -> for ARM this is teh final .uf2 files

include($ENV(PICO_SDK_PATH)/external/pico_sdk_import.cmake)
# this imports the pico SDK build system by pulling the SDK's CMake Configuration from PICO_SDK_PATH
# PICO_SDK_PATH is a variable you set up when you instal the pico sdk
# this imports all of the SDK libaries that sets up ARM compilation 

project(mac-gaming C CXX ASM)
# this declairs teh project name as "mac-gaming" and enables 3 languages: C, C++, Assembly
# declaring the langauges is needed to tell CMake which cross-compilers to configure
# in this case, the 3 are arm-none-eabi-gcc (C), arm-none-ebai-g++ (C++), arm-none-ebai-as (assembly)
# these are cross-copilers because the code runs on the pc but produces binary code for ARM
# eabi stands for embedded application binary interface

set(CMAKE_C_Standard 11)
set(CMAKE_CXX_STANDARD 17)
# These set the "version" of c and c++

pico_sdk_init()
# this initializes the pico sdk! it sets up teh rp2040 build environment, configures compiler flags for Cortex-M0+, makes all teh SDK libraries avalible
# this has to be under the project files and before defining targets

add_executable(mac-gaming
    src/main.c
    src/matrix.c
    src/encoder.c
    src/oled.c
    src/usb_hid.c
    src/usb_descriptors.c
    )
# this tells CMake to compile these C files, the binary will be in mac-gaming.uf2

target_include_directores(mac-gaming PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)
# this tells CMake where to find the header files 
# PRIVATE means that the mac-gaming executables will only use the headers that this points to 
# ${CMAKE_C_CURRENT_LIST_DIR}/include points to the directory where teh .h files are
# this is technichally not absolutly needed, but it allows us to just do #include "matrix.h" instead of #include ".. /include/matrix.h"

pico_enable_stdio_usb(mac-gaming 0)
pico_enable_stdio_uart(mac-gaming 0)
# this disables both stdio output methods -> thsi means theres no debug console output for serial, this makes sense because we are using USB HID instead of CDC serial
# CDC (communications device class) creates a virtual serial port, showing a seperate USB device for communication
# USB HID (human interface device) is a usb protocol for keyboards, mice, ect... This allows this to send keypresses directly to computer

target_link_libraries(mac-gaming
    pico_stdlib
    hardware_gpio
    hardware-irq
    tinyusb_device
)
# this links these libraries from the pico sdk to the executable file that cmake compiles
# pico_stdlib -> core pico functions
# hardware_gpio -> gpio pin control
# hardware_irq -> interupt handlers: this allows functions to execute imedielty when hardware event occurs without using a while(true) loop
# tinyusb_device -> USB device stack for HID -> handles enumeration, descriptor reporting

pico_add_extra_outputs(mac-gaming)
# generates additional outputs from CMake, importantly a .uf2
# .uf2 is the drag-and-drop bootloader for pi picos
# normally a .elf is produced, which is the compiler's raw ouptut containing arm code, debug symbols, and memory section mappings
# running this allows the .elf to be transformed into something the pico's bootlaoder can understand
