cmake_minimum_required(VERSION 3.5)

project(PasswordManager)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)

FetchContent_Declare(Sodium GIT_REPOSITORY https://github.com/robinlinden/libsodium-cmake.git)

set(SODIUM_DISABLE_TESTS ON)
FetchContent_MakeAvailable(Sodium)

set(SOURCE_FILES
	src/cli.cpp
	src/encryption.cpp
	src/serialization.cpp
	src/login.cpp
	src/map.cpp
	src/main.cpp
)

add_executable(PasswordManager ${SOURCE_FILES})
target_include_directories(PasswordManager PRIVATE include)

target_link_libraries(PasswordManager PRIVATE 
	sodium
)

### TESTS
enable_testing()

add_executable(encryption_test tests/encryption.cpp)
target_link_libraries(encryption_test PRIVATE sodium)

add_test(NAME encryption_test 
	COMMAND $<TARGET_FILE:encryption_test>
)
