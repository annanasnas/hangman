cmake_minimum_required(VERSION 3.22)
project(hangman CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(hangman main.cpp)

target_link_libraries(hangman sfml-graphics sfml-audio)
