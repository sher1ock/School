# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/pi/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake /home/pi/pico/pico-sdk/src/rp2_common/hardware_pio/include)
# target_include_directories(displaytest PRIVATE ${/home/pi/pico/pico-sdk/src/rp2_common/hardware_pio/include/hardware})
include_directories("/home/pi/pico/pico-sdk/src/rp2_common/hardware_pio/include/")

# if (NOT PICO_NO_HARDWARE)
#     add_subdirectory(addition)
#     add_subdirectory(apa102)
#     add_subdirectory(clocked_input)
#     add_subdirectory(differential_manchester)
#     add_subdirectory(hello_pio)
#     add_subdirectory(hub75)
#     add_subdirectory(i2c)
#     add_subdirectory(ir_nec)
#     add_subdirectory(logic_analyser)
#     add_subdirectory(manchester_encoding)
#     add_subdirectory(pio_blink)
#     add_subdirectory(pwm)
#     add_subdirectory(quadrature_encoder)
#     add_subdirectory(spi)
#     add_subdirectory(squarewave)
#     add_subdirectory(st7789_lcd)
#     add_subdirectory(uart_rx)
#     add_subdirectory(uart_tx)
#     add_subdirectory(ws2812)
# endif ()


project(displaytest C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(displaytest
    displaytest.c 
    LCDops.c
    generalOps.c
    presetChars.c
    presetMessages.c
#    ws2812.pio.h

)

pico_set_program_name(displaytest "displaytest")
pico_set_program_version(displaytest "0.1")

pico_enable_stdio_uart(displaytest 1)
pico_enable_stdio_usb(displaytest 0)

# Add the standard library to the build
target_link_libraries(displaytest pico_stdlib /home/pi/pico/pico-sdk/src/rp2_common/hardware_pio/include/hardware)

pico_add_extra_outputs(displaytest)

