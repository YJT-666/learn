
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)


option(USE_LIBRARY "Compile sources into a library?" OFF)
message(STATUS "Compile sources into a library? ${USE_LIBRARY}")
set(BUILD_SHARED_LIBS OFF)
list(APPEND _source Message.hpp Message.cpp)

if(USE_LIBRARY)
	add_library(message ${_source})
	add_executable(hello-world hello-world.cpp)
	target_link_libraries(hello-world message)
else()
	add_executable(hello-world hello-world.cpp ${_source})
endif()


