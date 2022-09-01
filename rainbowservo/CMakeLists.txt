# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/pi/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include_directories("/home/pi/pico/pico-sdk/src/rp2_common/hardware_pio/include/")

project(rainbowservo C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(rainbowservo rainbowservo.c )

pico_set_program_name(rainbowservo "rainbowservo")
pico_set_program_version(rainbowservo "0.1")

pico_enable_stdio_uart(rainbowservo 1)
pico_enable_stdio_usb(rainbowservo 1)

# Add the standard library to the build
target_link_libraries(rainbowservo pico_stdlib hardware_pio hardware_pwm hardware_adc)

pico_add_extra_outputs(rainbowservo)

