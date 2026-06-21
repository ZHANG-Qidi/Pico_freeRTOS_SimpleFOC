file(GLOB_RECURSE ARDUINO_CORE_SRC
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_library(arduino_core STATIC
    ${ARDUINO_CORE_SRC}
)

target_include_directories(arduino_core PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cores/arduino
    ${CMAKE_CURRENT_LIST_DIR}/libraries/Wire/src
    ${CMAKE_CURRENT_LIST_DIR}/libraries/SPI/src
    ${CMAKE_SOURCE_DIR}
)

if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(arduino_core PRIVATE
        pico_cyw43_arch_none
    )
endif()

target_link_libraries(arduino_core PUBLIC
    pico_stdlib
    hardware_uart
    hardware_spi
    hardware_i2c
    hardware_irq
    hardware_pwm
    hardware_timer
    hardware_clocks
    hardware_pio
)

# Generate PIO header
pico_generate_pio_header(arduino_core ${CMAKE_CURRENT_LIST_DIR}/cores/arduino/svpwm.pio)
pico_generate_pio_header(arduino_core ${CMAKE_CURRENT_LIST_DIR}/cores/arduino/pwm.pio)

# Example running FreeRTOS on 1 core
# Linking to FreeRTOS-Kernel-Heap4 means we use a dynamic heap for allocations
target_link_libraries(arduino_core PRIVATE
    pico_async_context_freertos
    FreeRTOS-Kernel-Heap4
    pico_stdlib
)
# Set the nunber of cores to 1.
# This defaults to 2 in FreeRTOSConfig_examples_common.h if not defined in here
target_compile_definitions(arduino_core PRIVATE
    configNUMBER_OF_CORES=1
)
