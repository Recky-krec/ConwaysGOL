cmake_minimum_required(VERSION 3.8)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

project(Conway-s-Game-of-Life)

# Includes
file(GLOB_RECURSE myIncludes "include/*")
include_directories(include)

# Sources
file(GLOB_RECURSE mySources "src/*.cpp")

# Libraries
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIRS})
    link_libraries(${SFML_LIBRARIES})
endif()

# Executable
add_executable(${PROJECT_NAME} ${mySources})

