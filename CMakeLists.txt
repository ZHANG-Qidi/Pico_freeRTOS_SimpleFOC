# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(FREERTOS_KERNEL_PATH "D:/HOME/workspace - pico/FreeRTOS/FreeRTOS-KernelV11.3.0" CACHE PATH "Path to FreeRTOS Kernel")

set(FREERTOS_VARIANT "one_core" CACHE STRING "Choose FreeRTOS variant")
set_property(CACHE FREERTOS_VARIANT PROPERTY STRINGS one_core two_cores static)
set(FREERTOS_VARIANT "one_core")

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

project(Pico_freeRTOS_SimpleFOC C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

# add_executable(Pico_freeRTOS_SimpleFOC main.cpp)

# pico_set_program_name(Pico_freeRTOS_SimpleFOC "Pico_freeRTOS_SimpleFOC")
# pico_set_program_version(Pico_freeRTOS_SimpleFOC "0.1")

# # Modify the below lines to enable/disable output over UART/USB
# pico_enable_stdio_uart(Pico_freeRTOS_SimpleFOC 1)
# pico_enable_stdio_usb(Pico_freeRTOS_SimpleFOC 0)

# # Add the standard library to the build
# target_link_libraries(Pico_freeRTOS_SimpleFOC
#     pico_stdlib)

# # Add the standard include files to the build
# target_include_directories(Pico_freeRTOS_SimpleFOC PRIVATE
#     ${CMAKE_CURRENT_LIST_DIR}
# )

# # Add any user requested libraries
# target_link_libraries(Pico_freeRTOS_SimpleFOC
#     hardware_spi
#     hardware_i2c
#     hardware_timer
#     hardware_clocks
# )

# pico_add_extra_outputs(Pico_freeRTOS_SimpleFOC)

include(FreeRTOS_Kernel_import.cmake)

add_subdirectory(Modules/ArduinoCore-avr-1.8.7)
add_subdirectory(Modules/Arduino-FOC-2.2)

add_executable(Pico_freeRTOS_SimpleFOC
    main.cpp
)

target_include_directories(Pico_freeRTOS_SimpleFOC PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(Pico_freeRTOS_SimpleFOC PRIVATE
    pico_stdlib
    hardware_uart
    hardware_spi
    hardware_i2c
    hardware_irq
    hardware_pwm
    hardware_timer
    hardware_clocks
)
target_link_libraries(Pico_freeRTOS_SimpleFOC PRIVATE
    arduino_core
    arduino_foc
)

if(FREERTOS_VARIANT STREQUAL "one_core")

    # Example running FreeRTOS on 1 core
    # Linking to FreeRTOS-Kernel-Heap4 means we use a dynamic heap for allocations
    target_link_libraries(Pico_freeRTOS_SimpleFOC PRIVATE
        pico_async_context_freertos
        FreeRTOS-Kernel-Heap4
        pico_stdlib
    )
    # Set the nunber of cores to 1.
    # This defaults to 2 in FreeRTOSConfig_examples_common.h if not defined in here
    target_compile_definitions(Pico_freeRTOS_SimpleFOC PRIVATE
        configNUMBER_OF_CORES=1
    )
    if(PICO_CYW43_SUPPORTED)
        # For led support on pico_w
        target_link_libraries(Pico_freeRTOS_SimpleFOC PRIVATE
            pico_cyw43_arch_none
        )
    endif()
    pico_add_extra_outputs(Pico_freeRTOS_SimpleFOC)

elseif(FREERTOS_VARIANT STREQUAL "two_cores")

    # Example running FreeRTOS on 2 cores
    # Linking to FreeRTOS-Kernel-Heap4 to use a dynamic heap for allocations
    target_link_libraries(Pico_freeRTOS_SimpleFOC PRIVATE
        pico_async_context_freertos
        FreeRTOS-Kernel-Heap4
        pico_stdlib
    )
    if(PICO_CYW43_SUPPORTED)
        # For led support on pico_w
        target_link_libraries(Pico_freeRTOS_SimpleFOC PRIVATE
            pico_cyw43_arch_none
        )
    endif()
    pico_add_extra_outputs(Pico_freeRTOS_SimpleFOC)

elseif(FREERTOS_VARIANT STREQUAL "static")

    # Example running FreeRTOS on 2 cores with static RAM allocation
    # Linking to FreeRTOS-Kernel-Static to use static memory instead of a dynamic heap for allocations
    target_link_libraries(Pico_freeRTOS_SimpleFOC PRIVATE
        pico_async_context_freertos
        FreeRTOS-Kernel-Static
        pico_stdlib
    )
    # Change the configuration to just use static RAM allocation
    # If configSUPPORT_DYNAMIC_ALLOCATION is left undefined it will default to 1
    # If configSUPPORT_STATIC_ALLOCATION is left undefined it will default to 0
    target_compile_definitions(Pico_freeRTOS_SimpleFOC PRIVATE
        configSUPPORT_STATIC_ALLOCATION=1
        configSUPPORT_DYNAMIC_ALLOCATION=0
    )
    if(PICO_CYW43_SUPPORTED)
        # For led support on pico_w
        target_link_libraries(Pico_freeRTOS_SimpleFOC PRIVATE
            pico_cyw43_arch_none
        )
    endif()
    pico_add_extra_outputs(Pico_freeRTOS_SimpleFOC)

else()
    message(FATAL_ERROR "Unknown FREERTOS_VARIANT: ${FREERTOS_VARIANT}")
endif()

pico_set_program_name(Pico_freeRTOS_SimpleFOC "Pico_freeRTOS_SimpleFOC")
pico_set_program_version(Pico_freeRTOS_SimpleFOC "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Pico_freeRTOS_SimpleFOC 1)
pico_enable_stdio_usb(Pico_freeRTOS_SimpleFOC 1)

add_custom_command(TARGET Pico_freeRTOS_SimpleFOC POST_BUILD
    COMMAND arm-none-eabi-size Pico_freeRTOS_SimpleFOC.elf
)
