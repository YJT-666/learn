
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

add_library(message-objs
	OBJECT
		Message.hpp
		Message.cpp
	)

add_library(message-shared
	SHARED
		$<TARGET_OBJECTS:message-objs>
	)
set_target_properties(message-shared
	PROPERTIES
		OUTPUT_NAME "message"
	)
add_library(message-static
	STATIC
		$<TARGET_OBJECTS:message-objs>
	)
set_target_properties(message-static
	PROPERTIES
		OUTPUT_NAME "message"
	)

add_executable(hello-world hello-world.cpp) 
target_link_libraries(hello-world message-shared)
