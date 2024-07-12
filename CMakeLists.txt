cmake_minimum_required(VERSION 2.8.3)


project(warzone)

include_directories(./)

message(${CMAKE_CURRENT_SOURCE_DIR})

# set(CLONE "${CMAKE_CURRENT_SOURCE_DIR}/SS.sh")

# execute_process(COMMAND ${CLONE}
#                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

add_executable(${PROJECT_NAME} ./src/main.cpp)