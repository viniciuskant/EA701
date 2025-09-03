# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(USERHOME $ENV{HOME})

set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)

include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

set(PICO_BOARD pico CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(main C CXX ASM)

pico_sdk_init()


add_executable(main main.c )

pico_set_program_name(main "main")
pico_set_program_version(main "0.1")

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

target_link_libraries(main
        pico_stdlib)

target_include_directories(main PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(main)

