cmake_minimum_required(VERSION 3.19)

include(
    pico_sdk_import.cmake
    shaft_speed.h
    )

project(variable_rate_planter)

pico_sdk_init()

add_executable(vrp main.cpp)

pico_enable_stdio_usb(vrp 1)
pico_enable_stdio_uart(vrp 0)

target_link_libraries(vrp 
    pico_stdlib
    pico_multicore
    )

pico_add_extra_outputs(vrp)