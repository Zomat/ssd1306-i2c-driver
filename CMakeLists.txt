cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
project(oled)

project(blink C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

FILE(GLOB oled_includes includes/*.h)

add_executable(oled
    src/main.c
    ${oled_includes}
)

target_include_directories(oled PUBLIC
  include/
)

target_link_libraries(oled
        pico_stdlib              # for core functionality
        hardware_i2c
        )

pico_enable_stdio_usb(oled 1)
pico_enable_stdio_uart(oled 0)

pico_add_extra_outputs(oled)
