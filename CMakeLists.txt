# CMakeList.txt : CMake project for LSystemEditor, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("LSystemEditor")

include_directories(
		${PROJECT_SOURCE_DIR}/src
		${PROJECT_SOURCE_DIR}/include
		${PROJECT_SOURCE_DIR}/resoures
)

file(GLOB all_files
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
		"${PROJECT_SOURCE_DIR}/include*.h"
		"${PROJECT_SOURCE_DIR}/resources/*"
)

# Add source to this project's executable.
add_executable(LSystemEditor ${all_files})

find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

target_link_libraries(LSystemEditor PRIVATE imgui::imgui)
target_link_libraries(LSystemEditor PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(LSystemEditor PRIVATE sfml-main)
target_link_libraries(LSystemEditor PRIVATE ImGui-SFML::ImGui-SFML)

# TODO: Add tests and install targets if needed.
