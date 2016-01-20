cmake_minimum_required(VERSION 2.6)

project(Aversive++ CXX C ASM)

set(TARGETS
  atmega328p
  #atmega64
  )

set(FLAGS "-Os")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS} -std=c++11")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${FLAGS}")

file(GLOB_RECURSE SOURCES "src/*")

include_directories(.)

foreach(t ${TARGETS})
  file(GLOB_RECURSE HAL_SOURCES_${t} "extras/hal/${t}/*")
  add_library(poppyNetwork_${t} ${SOURCES} ${HAL_SOURCES_${t}})
  target_compile_options(poppyNetwork_${t} PUBLIC "-mmcu=${t}")

  file(GLOB_RECURSE TEST_SOURCES "extras/test/*")
  add_executable(test_${t} ${TEST_SOURCES})
  target_link_libraries(test_${t}.elf poppyNetwork_${t})
endforeach()