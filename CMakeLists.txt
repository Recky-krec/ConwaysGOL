cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

project(Conway_s_Game_of_Life)

# Includes
file(GLOB_RECURSE myIncludes "include/*")
include_directories(include)

# Sources
file(GLOB_RECURSE mySources "src/*.cpp")

# Executable
add_executable(${PROJECT_NAME} ${mySources} include/Application.h src/Application.cpp src/Button.cpp include/Button.h include/Config.h include/Grid.h src/Grid.cpp)

# Libraries
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)