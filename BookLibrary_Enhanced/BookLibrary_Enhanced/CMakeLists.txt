cmake_minimum_required(VERSION 3.15)
project(BookLibrary)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(Library
    src/Library.cpp
    src/BookStorage.cpp
)
target_include_directories(Library PUBLIC include)

add_executable(BookLibrary main.cpp)
target_link_libraries(BookLibrary PRIVATE Library)

enable_testing()
add_executable(LibraryTests tests/tests.cpp)
target_link_libraries(LibraryTests PRIVATE Library)
add_test(NAME LibraryTest COMMAND LibraryTests)
