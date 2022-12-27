# CMakeList.txt : CMake project for LSystemEditor, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("LSystemEditor" VERSION 1.0)

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

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/vcpkg")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/vcpkg/vcpkg.exe")
    message(STATUS "vcpkg.exe not found, running bootstrap.")
    execute_process(COMMAND ${PROJECT_SOURCE_DIR}/vcpkg/bootstrap-vcpkg.bat WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/vcpkg)
endif()

execute_process(COMMAND cd vcpkg)
execute_process(COMMAND  .\\vcpkg install)

find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

target_link_libraries(LSystemEditor PRIVATE imgui::imgui)
target_link_libraries(LSystemEditor PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(LSystemEditor PRIVATE sfml-main)
target_link_libraries(LSystemEditor PRIVATE ImGui-SFML::ImGui-SFML)

# TODO: Add tests and install targets if needed.
