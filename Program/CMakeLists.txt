cmake_minimum_required(VERSION 3.21)
project(Program)

file(GLOB_RECURSE PROGRAM_SOURCES "src/*.cpp" "src/*.h")

add_executable(Program ${PROGRAM_SOURCES})

target_link_libraries(Program PRIVATE W_Engine)

target_include_directories(Program PRIVATE ../W_Engine/include)

add_custom_command(TARGET Program POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"$<TARGET_FILE:assimp>"
			"$<TARGET_FILE_DIR:Program>"
)

set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")
set(TARGET_DIR $<TARGET_FILE_DIR:Program>)

add_custom_command(TARGET Program POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${RESOURCES_DIR} ${TARGET_DIR}/resources
)