## Minimal cmake version
cmake_minimum_required(VERSION 2.8)

## Project name
project(layerscape_kernel C CXX ASM)

## Options
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FALGS} -std=c++11 -nostdlib -nostartfiles -ffreestanding")
set(CMAKE_C_FLAGS "${CMAKE_C_FALGS} -nostdlib -nostartfiles -ffreestanding")
set(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_DEBUG}")

#If a source file is not detected, try to 'touch' this file

## Assembler source code
file(GLOB src_kernel_asm "${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/*.S")

## C++ source code
file(GLOB src_util_cpp "${CMAKE_CURRENT_SOURCE_DIR}/src/util/*.cpp")
file(GLOB src_kernel_cpp "${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/*.cpp")
file(GLOB src_modules_cpp "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/*.cpp")
file(GLOB src_modules_syscall_cpp "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/syscall/*.cpp")
file(GLOB src_modules_mmu_cpp "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/mmu/*.cpp")
file(GLOB src_modules_scheduler_cpp "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/scheduler/*.cpp")
file(GLOB src_module_list_cpp "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


## Kernel-executable
add_executable(${PROJECT_NAME} ${src_kernel_asm} ${src_util_cpp} ${src_kernel_cpp} ${src_modules_cpp} ${src_modules_syscall_cpp} ${src_modules_mmu_cpp} ${src_modules_scheduler_cpp} ${src_module_list_cpp})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-T ${PROJECT_SOURCE_DIR}/ldscript")

## build image
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ${PROJECT_NAME}.elf -O binary ./${PROJECT_NAME}.img
    COMMAND mkimage -n ${PROJECT_NAME} -C none -A arm64  -T kernel -a 0x80010000 -e 0x80010000 -O rtems -d ${PROJECT_NAME}.img  ${PROJECT_NAME}.uimg    
    COMMAND cp ${PROJECT_NAME}.uimg ../tftp/
)
