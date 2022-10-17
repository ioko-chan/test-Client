cmake_minimum_required(VERSION 3.23)
project(testClient)

set(CMAKE_CXX_STANDARD 14)

add_executable(testClient main.cpp)

if(MINGW)
    target_link_libraries(testClient ws2_32)
endif()