# Version, project name, specified programming language
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(demo1 LANGUAGES CXX C)

# set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find all source files in the current directory and save the name to DIR_ SRCS variables
aux_source_directory(. DIR_SRCS)
aux_source_directory(src DIR_SRCS)

# Add subdirectory sub_ Dir (if any)
# add_subdirectory(sub_dir)

# Add the executable file bb as the generated executable file name
add_executable(bb ${DIR_SRCS})

# Automatically locate and add library files Take the CURL library as an example
find_package(Pangolin REQUIRED)
find_package(OpenCV REQUIRED)

# Manually add target header file path
target_include_directories(bb
	PUBLIC
		include/
		/usr/local/include/
		/usr/include/eigen3/
		${Pangolin_INCLUDE_DIRS}
		${OpenCV_INCLUDE_DIRS}
)
# Manually add a target link library
target_link_libraries(bb
	PUBLIC
		${Pangolin_LIBRARIES}
		${OpenCV_LIBRARIES}
		fmt
)


# Specify the installation path
install (TARGETS bb DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/config.h"
         DESTINATION include)
         
         



















# print some infomation
message(STATUS "Here some information about you environment:")
message(STATUS "Configuring on/for ${CMAKE_SYSTEM_NAME}")
message(STATUS "CXX Compileer ID : ${CMAKE_CXX_COMPILER_ID}")
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(STATUS "Target is 64 bits")
else()
  message(STATUS "Target is 32 bits")
endif()
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i386")
    message(STATUS "i386 architecture detected")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i686")
    message(STATUS "i686 architecture detected")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
    message(STATUS "x86_64 architecture detected")
else()
    message(STATUS "host processor architecture is unknown")
endif()
