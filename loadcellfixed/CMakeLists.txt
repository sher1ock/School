# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/pi/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(loadcellfixed C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(loadcellfixed loadcellfixed.c hx711.c)

pico_set_program_name(loadcellfixed "loadcellfixed")
pico_set_program_version(loadcellfixed "0.1")

pico_enable_stdio_uart(loadcellfixed 1)
pico_enable_stdio_usb(loadcellfixed 0)

# Add the standard library to the build
target_link_libraries(loadcellfixed pico_stdlib)

# Add any user requested libraries
target_link_libraries(loadcellfixed
        hardware_i2c
        hardware_pio
        #hx711-pico-c
        pico_stdlib
        pico_stdio
        pico_time
        )
include_directories(hx711-pico-c)


pico_add_extra_outputs(loadcellfixed)

