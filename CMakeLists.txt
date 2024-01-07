cmake_minimum_required(VERSION 3.28)
project(wcag)

set(CMAKE_CXX_STANDARD 23)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
find_package(PkgConfig)
pkg_search_module(GTEST IMPORTED_TARGET gtest_main)

qt_standard_project_setup()

qt_add_executable(contrast_enhancer
        main.cpp
        wcag.hpp
        wcag.cpp
        test_window.hpp
)
target_link_libraries(contrast_enhancer PRIVATE Qt6::Core Qt6::Widgets)

add_executable(test
        tests.cpp
        wcag.cpp
        wcag.hpp
)
target_link_libraries(test PRIVATE PkgConfig::GTEST Qt6::Gui)

