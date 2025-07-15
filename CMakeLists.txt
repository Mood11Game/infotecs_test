cmake_minimum_required(VERSION 3.10)
project(loglib_test_project)

set(CMAKE_CXX_STANDARD 17)

add_library(loglib STATIC
    src/logger.cpp
)

target_include_directories(loglib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(main_test
    src/main.cpp
)

target_link_libraries(main_test PRIVATE loglib)
