cmake_minimum_required(VERSION 3.0)

project(geminivm VERSION 0.10)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

add_subdirectory(Gemini)
add_subdirectory(Test)
