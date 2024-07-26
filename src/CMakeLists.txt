include(FetchContent)

FetchContent_Declare(Sodium GIT_REPOSITORY https://github.com/robinlinden/libsodium-cmake.git)

set(SODIUM_DISABLE_TESTS ON)
FetchContent_MakeAvailable(Sodium)

add_library(encryption encryption.cpp)
target_link_libraries(encryption PRIVATE sodium)
target_include_directories(encryption PRIVATE ${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILES
	cli.cpp
	serialization.cpp
	login.cpp
	map.cpp
	main.cpp
)

add_executable(PasswordManager ${SOURCE_FILES})

target_include_directories(PasswordManager PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(PasswordManager PRIVATE encryption)
