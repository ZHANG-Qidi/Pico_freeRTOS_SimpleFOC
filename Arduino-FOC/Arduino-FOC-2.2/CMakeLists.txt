file(GLOB_RECURSE ARDUINO_FOC_SRC
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_library(arduino_foc STATIC
    ${ARDUINO_FOC_SRC}
)

target_include_directories(arduino_foc PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
)

target_link_libraries(arduino_foc PUBLIC
    arduino_core
)
