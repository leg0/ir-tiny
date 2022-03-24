cmake_minimum_required(VERSION 3.16)

#set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/avr-gcc.toolchain.cmake")
set(AVR_MCU "attiny9")

project(ir-tiny)


add_subdirectory(firmware)
