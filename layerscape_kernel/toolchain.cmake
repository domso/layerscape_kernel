#OS
set(CMAKE_SYSTEM_NAME Generic)

## Cross-Compiler
set(CROSS_COMPILER_PREFIX aarch64-elf-)


## Compiler
SET(CMAKE_C_COMPILER ${CROSS_COMPILER_PREFIX}gcc)
SET(CMAKE_CXX_COMPILER ${CROSS_COMPILER_PREFIX}g++)
SET(CMAKE_ASM_COMPILER ${CROSS_COMPILER_PREFIX}as)
SET(CMAKE_LINKER ${CROSS_COMPILER_PREFIX}ld)
set(CMAKE_OBJCOPY ${CROSS_COMPILER_PREFIX}objcopy)

## Link-Command
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_LINKER} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

         
## force Compiler
include (CMakeForceCompiler)
cmake_force_c_compiler(${CROSS_COMPILER_PREFIX}gcc GNU)
cmake_force_cxx_compiler(${CROSS_COMPILER_PREFIX}g++ GNU)