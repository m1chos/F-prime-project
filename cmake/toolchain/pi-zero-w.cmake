####
# Raspberry Pi Zero W
# ARMv6 Linux hard-float cross-compilation toolchain
####

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

if(NOT DEFINED ENV{PI_ZERO_TC})
    message(FATAL_ERROR
        "PI_ZERO_TC is not set. "
        "Example: export PI_ZERO_TC=$HOME/opt/x-tools/armv6-rpi-linux-gnueabihf"
    )
endif()

set(PI_ZERO_TC "$ENV{PI_ZERO_TC}")

set(CMAKE_C_COMPILER
    "${PI_ZERO_TC}/bin/armv6-rpi-linux-gnueabihf-gcc")

set(CMAKE_CXX_COMPILER
    "${PI_ZERO_TC}/bin/armv6-rpi-linux-gnueabihf-g++")

set(CMAKE_ASM_COMPILER
    "${PI_ZERO_TC}/bin/armv6-rpi-linux-gnueabihf-gcc")

set(CMAKE_AR
    "${PI_ZERO_TC}/bin/armv6-rpi-linux-gnueabihf-ar")

set(CMAKE_OBJCOPY
    "${PI_ZERO_TC}/bin/armv6-rpi-linux-gnueabihf-objcopy")

set(CMAKE_OBJDUMP
    "${PI_ZERO_TC}/bin/armv6-rpi-linux-gnueabihf-objdump")

set(CMAKE_SYSROOT
    "${PI_ZERO_TC}/armv6-rpi-linux-gnueabihf/sysroot")

set(CMAKE_FIND_ROOT_PATH "${CMAKE_SYSROOT}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Classic Raspberry Pi Zero W CPU
set(PI_ZERO_FLAGS
    "-mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard")

set(CMAKE_C_FLAGS_INIT "${PI_ZERO_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${PI_ZERO_FLAGS}")
