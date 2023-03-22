# 版本、工程名称、指定编程语言
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(demo1 LANGUAGES CXX C)

# 设定C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 
# 查找当前目录下的所有源文件,并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
aux_source_directory(src DIR_SRCS)
# 添加子目录 sub_dir(有的话)
# add_subdirectory(sub_dir)

# 添加可执行文件 bb 为生成的可执行文件名
add_executable(bb ${DIR_SRCS})

# 自动查找并添加库文件 


# 手动添加目标头文件路径
target_include_directories(bb
	PUBLIC
		include/ 
		/usr/local/include/
)
# 手动添加目标链接库
target_link_libraries(bb
	PUBLIC
		glfw
		GLU 
		GL 
		X11 
		pthread 
		Xrandr 
		Xi 
		dl
)

# 安装
# 指定安装路径
install (TARGETS bb DESTINATION bin)










































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
