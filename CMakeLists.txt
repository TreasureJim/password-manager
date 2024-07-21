cmake_minimum_required(VERSION 3.2)

project(PasswordManager)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES
	src/serialization.cpp
	src/login.cpp
	src/map.cpp
	src/main.cpp
)

include_directories(include)
add_executable(PasswordManager ${SOURCE_FILES})
