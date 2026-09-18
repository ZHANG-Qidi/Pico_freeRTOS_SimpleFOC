file(GLOB_RECURSE ARDUINO_FOC_SRC
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_library(arduino_foc STATIC
    ${ARDUINO_FOC_SRC}
)

target_include_directories(arduino_foc PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(arduino_foc PRIVATE
    pico_stdlib
    hardware_uart
    hardware_spi
    hardware_i2c
    hardware_irq
    hardware_pwm
    hardware_timer
    hardware_clocks
)

target_link_libraries(arduino_foc PRIVATE
    arduino_core
)
