cmake_minimum_required(VERSION 3.21)
project(W_Engine)

set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
add_subdirectory(lib/assimp)

add_subdirectory(lib/glad)

set(BUILD_SHARED_LIBS OFF)
add_subdirectory(lib/glfw)

add_subdirectory(lib/glm)

add_subdirectory(lib/imgui)

add_subdirectory(lib/simple_logger)

add_subdirectory(lib/stb_image)

file(GLOB_RECURSE W_ENGINE_SOURCES "src/*.cpp")
file(GLOB_RECURSE W_ENGINE_HEADERS "include/*.h")

add_library(W_Engine STATIC ${W_ENGINE_SOURCES} ${W_ENGINE_HEADERS})

include_directories(include)

target_link_libraries(imgui PRIVATE glfw)

target_link_libraries(W_Engine PRIVATE assimp)
target_link_libraries(W_Engine PRIVATE glad)
target_link_libraries(W_Engine PRIVATE glfw)
target_link_libraries(W_Engine PUBLIC glm)
target_link_libraries(W_Engine PUBLIC imgui)
target_link_libraries(W_Engine PUBLIC simple_logger)
target_link_libraries(W_Engine PRIVATE stb_image)

target_include_directories(W_Engine PRIVATE lib/assimp/include)
target_include_directories(W_Engine PRIVATE lib/glad/include)
target_include_directories(W_Engine PUBLIC lib/glfw/include)
target_include_directories(W_Engine PUBLIC lib/glm)
target_include_directories(W_Engine PUBLIC lib/imgui)
target_include_directories(W_Engine PUBLIC lib/Simple_Logger/include)
target_include_directories(W_Engine PRIVATE lib/stb_image/include)




