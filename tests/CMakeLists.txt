enable_testing()

add_executable(encryption_test encryption.cpp)
target_link_libraries(encryption_test PRIVATE encryption)
target_include_directories(encryption_test PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_test(NAME encryption_test 
	COMMAND $<TARGET_FILE:encryption_test>
)

add_executable(serialization_test serialization.cpp ../src/login.cpp ../src/map.cpp)
target_link_libraries(serialization_test PRIVATE serialization)
target_include_directories(serialization_test PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_test(NAME serialization_test 
	COMMAND $<TARGET_FILE:serialization_test>
)
