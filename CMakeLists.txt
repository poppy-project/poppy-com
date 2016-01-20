cmake_minimum_required(VERSION 2.6)

project(Aversive++ CXX C ASM)

set(MCU
  atmega328p
  )

set(FLAGS "-Os -mmcu=${MCU}")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS} -std=c++11")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${FLAGS}")

file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h")

include_directories(include)
include_directories(hal/include)

file(GLOB_RECURSE HAL_SOURCES "hal/src/hal_${MCU}.c")
file(GLOB_RECURSE HAL_HEADERS "hal/include/*.h")

add_library(poppyNetwork ${SOURCES} ${HEADERS} ${HAL_SOURCES} ${HAL_HEADERS})

add_subdirectory(test)
