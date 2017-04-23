# define chip used in this project, this set must define before project definition
# for this project dont use cmake commandline option -DSTM32_CHIP=<chip>
set(STM32_CHIP STM32F103C8)
set(TARGET_TRIPLET arm-none-eabi)
set(RUSTC_TARGET_TRIPLE "${TARGET_TRIPLET}")
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/stm32-cmake/cmake/gcc_stm32.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/stm32-cmake/cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

list(APPEND CMAKE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/rust")

set(CHIBIOS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/ChibiOS)
#set(STM32Cube_DIR /Users/proffan/STM32Cube/Repository/STM32Cube_FW_F1_V1.4.0)
cmake_minimum_required(VERSION 3.4)
project(chibiosf1)


ENABLE_LANGUAGE(ASM)

# test build all available ChibiOS COMPONENTS for F4 chip
FIND_PACKAGE(ChibiOS 16 COMPONENTS rt hal adc dac can dac ext gpt i2c i2s icu mmc_spi pal pwm rtc sdc serial serial_usb spi st uart usb chprintf memstreams nullstreams evtimer shell syscalls REQUIRED)

FIND_PACKAGE(rustc REQUIRED)

#FIND_PACKAGE(ChibiOS 16 COMPONENTS rt hal pal  REQUIRED)

INCLUDE_DIRECTORIES(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${ChibiOS_INCLUDE_DIRS}
        config
        board
        work
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/target/thumbv7em-none-eabi/debug/librust32.a
    COMMAND
        CARGO_TARGET_DIR=${CMAKE_CURRENT_BINARY_DIR}/target xargo build --target thumbv7em-none-eabi
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM
    )

add_custom_target(demo_target DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/target/thumbv7em-none-eabi/debug/librust32.a)

# create an library target out of the library compilation result
add_library(demo STATIC IMPORTED GLOBAL)
add_dependencies(demo demo_target)

# specify where the library is and where to find the headers
set_target_properties(demo
    PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/target/thumbv7em-none-eabi/debug/librust32.a)

ADD_DEFINITIONS(-DCORTEX_USE_FPU=FALSE)

SET(STM32_LINKER_SCRIPT ${ChibiOS_LINKER_SCRIPT})


set(SOURCE_FILES main.c board/board.c board/board.h config/mcuconf.h config/halconf.h config/chconf.h work/test.cpp work/test.hpp hack.cpp)

add_executable(${CMAKE_PROJECT_NAME}.elf ${SOURCE_FILES}  ${ChibiOS_SOURCES})

TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME}.elf demo)

STM32_SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME}.elf)
STM32_ADD_HEX_BIN_TARGETS(${CMAKE_PROJECT_NAME}.elf)
STM32_PRINT_SIZE_OF_TARGETS(${CMAKE_PROJECT_NAME}.elf)

add_custom_target(flash
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND openocd -f ./openocd_flash.cfg
        COMMENT "Flashing the target processor..."
        DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(TARGET flash POST_BUILD COMMENT "Flashing finished!")
