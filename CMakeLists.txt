cmake_minimum_required(VERSION 3.10)

project(fpmas-classic-pp)

find_package(fpmas 1.1 REQUIRED)
add_subdirectory(yaml-cpp)

set(CMAKE_CXX_FLAGS_DEBUG
	"${CMAKE_CXX_FLAGS_DEBUG} -g -O0\
	-Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

set(PP_LOG "false" CACHE STRING "Enables debug log (true/false)")

add_executable(fpmas-classic-pp
	src/main.cpp
	src/grass.cpp
	src/agent.cpp
	src/model.cpp
	src/config.cpp
	src/output.cpp
	)

message(STATUS "Debug log:  ${PP_LOG}")
target_compile_definitions(fpmas-classic-pp PUBLIC PP_LOG=${PP_LOG})
target_include_directories(fpmas-classic-pp PUBLIC include)

target_link_libraries(fpmas-classic-pp fpmas::fpmas yaml-cpp::yaml-cpp)
