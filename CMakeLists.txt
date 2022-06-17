cmake_minimum_required(VERSION 3.4)
project(hangman CXX)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(hangman main.cpp)

target_link_libraries(hangman sfml-graphics sfml-audio)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        tester
        tester.cpp
)
target_link_libraries(
        tester
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(tester)


#enable_testing()
#add_executable(tester tester.cpp)
#add_test(Tester tester)
#
#target_link_libraries(tester gtest_main)
#
#include(GoogleTest)
#gtest_discover_tests(tester)
