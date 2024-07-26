cmake_minimum_required(VERSION 3.5)

project(PasswordManager)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)
add_subdirectory(tests)
