cmake_minimum_required(VERSION 3.21)

include(pico_sdk_import.cmake)

project(pico-projects C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(FREERTOS_KERNEL_PATH "C:/Users/20235607/Downloads/BAS_System/FreeRTOS-Kernel")
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2350_ARM_NTZ/FreeRTOS_Kernel_import.cmake)
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
add_executable(freertos_blink
        main.c)
target_link_libraries(
    multicore
    pico_stdlib
    hardware_adc
    pico_multicore
)
target_include_directories(freertos_blink PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(freertos_blink
    pico_stdlib
    FreeRTOS-Kernel
    hardware_adc
    FreeRTOS-Kernel-Heap4
)
pico_enable_stdio_usb(blink 1)
pico_enable_stdio_uart(blink 0)
pico_enable_stdio_usb(multicore 1)
pico_enable_stdio_uart(multicore 0)
pico_enable_stdio_usb(freertos_blink 1)
pico_enable_stdio_uart(freertos_blink 0)

pico_add_extra_outputs(blink)
pico_add_extra_outputs(multicore)
pico_add_extra_outputs(freertos_blink)