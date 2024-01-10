cmake_minimum_required(VERSION 3.21)
project(Render_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(W_Engine)
add_subdirectory(Program)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY 	VS_STARTUP_PROJECT Program)
endif()