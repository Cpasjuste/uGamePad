cmake_minimum_required(VERSION 3.5)
#set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON")

option(PLATFORM_LINUX "Linux target platform" OFF)

if (NOT PLATFORM_LINUX)
    set(ENV{PICO_SDK_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/external/pico-sdk)
    set(PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/pico-sdk)
    if (NOT PICO_BOARD)
        #set(PICO_BOARD "adafruit_feather_rp2040" CACHE STRING "" FORCE) # v1.0 rev board
        #set(PICO_BOARD "waveshare_rp2040_plus_16mb" CACHE STRING "" FORCE) # v1.1 rev board
    endif ()
    # initialize the SDK based on PICO_SDK_PATH
    # note: this must happen before project()
    include(${CMAKE_CURRENT_SOURCE_DIR}/external/pico-sdk/external/pico_sdk_import.cmake)
endif ()

# pico options
option(PICO_DEBUG_UART "Pico uart debug" OFF)
option(PICO_DEBUG_USB "Pico usb debug" OFF)

project(uGamePad)

if (PLATFORM_LINUX)
    # i want all executable located at root of build directory
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} CACHE STRING "" FORCE)
else ()
    # initialize the Raspberry Pi Pico SDK
    pico_sdk_init()
endif ()

# set source files
file(GLOB SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utility/*.cpp
)
set(INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utility
)
set(LIBS)
set(FLAGS)

if (PLATFORM_LINUX)

else ()
    file(GLOB TEMP
            ${CMAKE_CURRENT_SOURCE_DIR}/src/pico/*.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/pico/hid/*.c
    )
    list(APPEND SOURCES ${TEMP})
    list(APPEND INCLUDES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/pico
            ${CMAKE_CURRENT_SOURCE_DIR}/src/pico/hid
    )
    list(APPEND LIBS pico_stdlib tinyusb_host tinyusb_board)
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

