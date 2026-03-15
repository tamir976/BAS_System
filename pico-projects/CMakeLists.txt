cmake_minimum_required(VERSION 3.21)

include(pico_sdk_import.cmake)

project(pico-projects C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()
add_executable(blink
        blink_led.c
        )
target_link_libraries(blink 
    pico_stdlib
    )
add_executable(multicore
        multicore.c
        )
target_link_libraries(
    multicore
    pico_stdlib
    hardware_adc
    pico_multicore
)
pico_enable_stdio_usb(blink 1)
pico_enable_stdio_uart(blink 0)
pico_enable_stdio_usb(multicore 1)
pico_enable_stdio_uart(multicore 0)

pico_add_extra_outputs(blink)
pico_add_extra_outputs(multicore)