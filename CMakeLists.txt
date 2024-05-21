cmake_minimum_required(VERSION 3.27)
project(SimpleRayCaster)

set(CMAKE_CXX_STANDARD 17)

add_executable(SimpleRayCaster main.cpp)
include_directories(/usr/local/include)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(SimpleRayCaster sfml-system sfml-window sfml-graphics sfml-audio sfml-network)