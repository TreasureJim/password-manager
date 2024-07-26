cmake_minimum_required(VERSION 3.5)

project(PasswordManager)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)

### TESTS
enable_testing()

add_executable(encryption_test tests/encryption.cpp)
target_link_libraries(encryption_test PRIVATE encryption)
target_include_directories(encryption_test PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_test(NAME encryption_test 
	COMMAND $<TARGET_FILE:encryption_test>
)
