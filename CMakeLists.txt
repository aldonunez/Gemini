cmake_minimum_required(VERSION 3.13.4)

project(geminivm)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    add_compile_options(/W4 /wd4100 /wd4389 /wd4458 /wd4505)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Wno-parentheses -Wno-unused)
endif()

enable_testing()

add_subdirectory(Gemini)
add_subdirectory(Test)
