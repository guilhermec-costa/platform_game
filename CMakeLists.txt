cmake_minimum_required(VERSION 3.30.1)
project(platform)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Código-fonte
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp")

# Includes do seu projeto
include_directories(include)
include_directories(vendor)          # inclui vendor/spdlog/include

# SDL
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${SDL2_IMAGE} ${SDL2_MIXER_INCLUDE_DIRS})

# spdlog
set(SPDLOG_ROOT "${CMAKE_SOURCE_DIR}/vendor/spdlog")
include_directories(${SPDLOG_ROOT}/include)

# Caminho da lib compilada
set(SPDLOG_LIB "${SPDLOG_ROOT}/build/libspdlog.a")

# Executável
add_executable(${PROJECT_NAME} ${SOURCES})
add_definitions(-DPROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}/")
add_definitions(-DSPDLOG_COMPILED_LIB)

# Link com bibliotecas
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    SDL2 SDL2_ttf SDL2_image SDL2_mixer
    ${SPDLOG_LIB}
)
