cmake_minimum_required(VERSION 3.10)
project(loglib_test_project)

set(CMAKE_CXX_STANDARD 17)

# Опция для выбора типа библиотеки: ON — динамическая, OFF — статическая
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# ---- loglib library ----
add_library(loglib
    src/logger.cpp
)

target_include_directories(loglib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ---- mainL ----
add_executable(mainL 
    src/main.cpp
)

target_link_libraries(mainL PRIVATE loglib)

# ---- main_testL ----
add_executable(main_testL 
    tests/test_logger.cpp
)

target_link_libraries(main_testL PRIVATE loglib)

# ---- log_app ----
add_executable(log_app
    include/app/src/main.cpp
    include/app/src/app.cpp
)


target_include_directories(log_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/app/include
)

target_link_libraries(log_app PRIVATE loglib)


