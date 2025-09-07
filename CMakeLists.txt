cmake_minimum_required(VERSION 3.30.1)
project(platform)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp")

include_directories(include)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${SDL2_IMAGE})

add_executable(platform ${SOURCES})
add_definitions(-DPROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

target_link_libraries(platform PRIVATE SDL2 SDL2_ttf SDL2_image)
