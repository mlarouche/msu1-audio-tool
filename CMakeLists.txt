cmake_minimum_required(VERSION 3.6)

project(msu1)

include_directories("${PROJECT_SOURCE_DIR}/3rdparty/")

add_subdirectory("${PROJECT_SOURCE_DIR}/3rdparty/sox")
add_subdirectory("${PROJECT_SOURCE_DIR}/src")
