file(GLOB_RECURSE TEST_SOURCES "src/*.c")
file(GLOB_RECURSE TEST_HEADERS "include/*.h")

add_executable(test.elf ${TEST_SOURCES} ${TEST_HEADERS})
target_link_libraries(test.elf poppyNetwork)
include_directories(test.elf PRIVATE include)
