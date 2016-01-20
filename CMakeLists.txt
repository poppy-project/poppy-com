cmake_minimum_required(VERSION 2.6)

project(Aversive++ CXX C ASM)

set(MCU
  atmega328p
  )

set(FLAGS "-Os -mmcu=${MCU}")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS} -std=c++11")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${FLAGS}")

file(GLOB_RECURSE SOURCES "src/*")

include_directories(.)

file(GLOB_RECURSE HAL_SOURCES "extras/hal/${MCU}/*")
add_library(poppyNetwork ${SOURCES} ${HAL_SOURCES})

file(GLOB_RECURSE TEST_SOURCES "extras/test/*")
add_executable(test.elf ${TEST_SOURCES})
target_link_libraries(test.elf poppyNetwork)
