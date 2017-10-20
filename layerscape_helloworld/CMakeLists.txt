## Minimal cmake version
cmake_minimum_required(VERSION 2.8)

## Project name
project(layerscape_helloworld CXX ASM)

## Options
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FALGS} -std=c++11 -nostdlib -nostartfiles -ffreestanding")
set(CMAKE_C_FLAGS "${CMAKE_C_FALGS} -nostdlib -nostartfiles -ffreestanding")
set(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_DEBUG}")

#If a source file is not detected, try to 'touch' this file

## Assembler source code
file(GLOB src_syscalls "${CMAKE_CURRENT_SOURCE_DIR}/src/*.S")

## cxx source code
file(GLOB src_main "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

## executable
add_executable(${PROJECT_NAME} ${src_syscalls} ${src_main})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-T ${PROJECT_SOURCE_DIR}/ldscript")

## build image
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ${PROJECT_NAME}.elf -O binary ./${PROJECT_NAME}.img
    COMMAND echo -n 'Entrypoint: '
    COMMAND ${CMAKE_OBJDUMP} -S ${PROJECT_NAME}.elf | grep 'main.:' | cut -d' ' -f1
    COMMAND echo -n 'Size: '
    COMMAND du -b ${PROJECT_NAME}.img | cut -f1
    COMMAND cp ${PROJECT_NAME}.img ../tftp/
)
