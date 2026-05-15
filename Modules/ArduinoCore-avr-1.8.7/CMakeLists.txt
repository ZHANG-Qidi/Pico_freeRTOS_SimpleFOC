file(GLOB_RECURSE ARDUINO_CORE_SRC
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_library(arduino_core STATIC
    ${ARDUINO_CORE_SRC}
)

target_include_directories(arduino_core PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cores/arduino
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(arduino_core PUBLIC
    pico_stdlib
    hardware_uart
    hardware_spi
    hardware_i2c
    hardware_irq
    hardware_pwm
    hardware_timer
    hardware_clocks
)

if(FREERTOS_VARIANT STREQUAL "one_core")

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
    if(PICO_CYW43_SUPPORTED)
        # For led support on pico_w
        target_link_libraries(arduino_core PRIVATE
            pico_cyw43_arch_none
        )
    endif()

elseif(FREERTOS_VARIANT STREQUAL "two_cores")

    # Linking to FreeRTOS-Kernel-Heap4 to use a dynamic heap for allocations
    target_link_libraries(arduino_core PRIVATE
        pico_async_context_freertos
        FreeRTOS-Kernel-Heap4
        pico_stdlib
    )
    if(PICO_CYW43_SUPPORTED)
        # For led support on pico_w
        target_link_libraries(arduino_core PRIVATE
            pico_cyw43_arch_none
        )
    endif()

elseif(FREERTOS_VARIANT STREQUAL "static")

    # Linking to FreeRTOS-Kernel-Static to use static memory instead of a dynamic heap for allocations
    target_link_libraries(arduino_core PRIVATE
        pico_async_context_freertos
        FreeRTOS-Kernel-Static
        pico_stdlib
    )
    # Change the configuration to just use static RAM allocation
    # If configSUPPORT_DYNAMIC_ALLOCATION is left undefined it will default to 1
    # If configSUPPORT_STATIC_ALLOCATION is left undefined it will default to 0
    target_compile_definitions(arduino_core PRIVATE
        configSUPPORT_STATIC_ALLOCATION=1
        configSUPPORT_DYNAMIC_ALLOCATION=0
    )
    if(PICO_CYW43_SUPPORTED)
        # For led support on pico_w
        target_link_libraries(arduino_core PRIVATE
            pico_cyw43_arch_none
        )
    endif()

else()
    message(FATAL_ERROR "Unknown FREERTOS_VARIANT: ${FREERTOS_VARIANT}")
endif()
