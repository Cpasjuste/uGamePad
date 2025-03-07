cmake_minimum_required(VERSION 3.5)
#set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON")

# linux
option(PLATFORM_LINUX "Linux target platform" OFF)
# pico options
option(PICO_DEV_BOARD "Build for ugp dev board" OFF)
option(PICO_DEBUG_UART "Pico uart debug" OFF)
option(PICO_DEBUG_USB "Pico usb debug" OFF)

if (NOT PLATFORM_LINUX)
    set(ENV{PICO_SDK_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/external/pico-sdk)
    set(PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/pico-sdk)
    # initialize the SDK based on PICO_SDK_PATH
    # note: this must happen before project()
    include(${CMAKE_CURRENT_SOURCE_DIR}/external/pico-sdk/external/pico_sdk_import.cmake)
endif ()

project(uGamePad)

if (NOT PLATFORM_LINUX)
    # initialize the Raspberry Pi Pico SDK
    pico_sdk_init()
endif ()

# set source files
file(GLOB SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/src/fatfs/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/widgets/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utility/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/src/compat/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/external/Adafruit-GFX-Library/Adafruit_GFX.cpp
)
set(INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/fatfs
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/widgets
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utility
        ${CMAKE_CURRENT_SOURCE_DIR}/src/compat
        ${CMAKE_CURRENT_SOURCE_DIR}/external/Adafruit-GFX-Library
)
set(FLAGS -DARDUINO=123)
set(LIBS)

if (PLATFORM_LINUX)
    # i want all executable located at root of build directory
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} CACHE STRING "" FORCE)
    file(GLOB TEMP
            ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/*.c*
            ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/hid-api/*.c*
    )
    list(APPEND SOURCES ${TEMP})
    list(APPEND INCLUDES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/linux
            ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/hid-api
    )
    list(APPEND FLAGS -DLINUX)
    # SDL2
    include(FindPkgConfig)
    pkg_search_module(SDL2 REQUIRED sdl2)
    list(APPEND INCLUDES ${SDL2_INCLUDE_DIRS})
    list(APPEND LIBS ${SDL2_LIBRARIES})
    # hidapi
    pkg_search_module(LIBUSB1 REQUIRED libusb-1.0)
    list(APPEND INCLUDES ${LIBUSB1_INCLUDE_DIRS})
    list(APPEND LIBS ${LIBUSB1_LIBRARIES})
else ()
    file(GLOB TEMP
            ${CMAKE_CURRENT_SOURCE_DIR}/src/pico/*.c*
            ${CMAKE_CURRENT_SOURCE_DIR}/src/pico/hid/*.c*
    )
    list(APPEND SOURCES ${TEMP})
    list(APPEND INCLUDES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/pico
            ${CMAKE_CURRENT_SOURCE_DIR}/src/pico/hid
    )
    list(APPEND LIBS
            hardware_i2c
            pico_stdlib
            tinyusb_device
            tinyusb_host
            tinyusb_board
    )
    if (PICO_DEV_BOARD)
        list(APPEND FLAGS -DUGP_DEV_BOARD)
    endif ()
endif ()

####################
# InfoNES binary
####################
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${FLAGS})
target_link_libraries(${PROJECT_NAME} ${LIBS})

if (NOT PLATFORM_LINUX)
    #set_linker_flash_size_16(${PROJECT_NAME})
    pico_add_uf2_output(${PROJECT_NAME})
    # debug output
    if (PICO_DEBUG_UART)
        pico_enable_stdio_usb(${PROJECT_NAME} 0)
        pico_enable_stdio_uart(${PROJECT_NAME} 1)
    elseif (PICO_DEBUG_USB)
        pico_enable_stdio_usb(${PROJECT_NAME} 1)
        pico_enable_stdio_uart(${PROJECT_NAME} 0)
    else ()
        pico_enable_stdio_usb(${PROJECT_NAME} 0)
        pico_enable_stdio_uart(${PROJECT_NAME} 0)
    endif ()
endif ()

