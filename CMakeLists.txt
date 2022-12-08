cmake_minimum_required(VERSION 3.22.1)

project(create_raster)

set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++17")

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(PROJ REQUIRED proj)

target_link_libraries(${PROJECT_NAME} PUBLIC
  ${PROJ_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${PROJ_INCLUDEDIR}
)