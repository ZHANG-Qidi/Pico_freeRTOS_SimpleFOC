# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(FREERTOS_KERNEL_PATH "D:/HOME/workspace - pico/FreeRTOS/FreeRTOS-KernelV11.3.0" CACHE PATH "Path to FreeRTOS Kernel")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if(EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_freertos_simplefoc C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

include(FreeRTOS_Kernel_import.cmake)

add_subdirectory(ArduinoCore-avr/ArduinoCore-avr-1.8.7)
add_subdirectory(Arduino-FOC/Arduino-FOC-2.2)

add_executable(pico_freertos_simplefoc
    main.cpp
    arduino_main.cpp
)

target_include_directories(pico_freertos_simplefoc PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(pico_freertos_simplefoc PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Add any user requested libraries
target_link_libraries(pico_freertos_simplefoc PRIVATE
    pico_stdlib
    hardware_uart
    hardware_spi
    hardware_i2c
    hardware_irq
    hardware_pwm
    hardware_timer
    hardware_clocks
)

target_link_libraries(pico_freertos_simplefoc PRIVATE
    arduino_core
    arduino_foc
)

# Example running FreeRTOS on 1 core
# Linking to FreeRTOS-Kernel-Heap4 means we use a dynamic heap for allocations
target_link_libraries(pico_freertos_simplefoc PRIVATE
    pico_async_context_freertos
    FreeRTOS-Kernel-Heap4
    pico_stdlib
)
# Set the nunber of cores to 1.
# This defaults to 2 in FreeRTOSConfig_examples_common.h if not defined in here
target_compile_definitions(pico_freertos_simplefoc PRIVATE
    configNUMBER_OF_CORES=1
)

pico_add_extra_outputs(pico_freertos_simplefoc)

pico_set_program_name(pico_freertos_simplefoc "pico_freertos_simplefoc")
pico_set_program_version(pico_freertos_simplefoc "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(pico_freertos_simplefoc 1)
pico_enable_stdio_usb(pico_freertos_simplefoc 0)

add_custom_command(TARGET pico_freertos_simplefoc POST_BUILD
    COMMAND arm-none-eabi-size pico_freertos_simplefoc.elf
)
