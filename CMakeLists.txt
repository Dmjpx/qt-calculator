cmake_minimum_required(VERSION 3.21)
project(WuXiaoDongCalculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(Qt6 6.2 REQUIRED COMPONENTS Widgets)

qt_add_executable(calculator WIN32
    main.cpp
    calculatorengine.cpp
    calculatorengine.h
    calculatorwindow.cpp
    calculatorwindow.h
)
target_link_libraries(calculator PRIVATE Qt6::Widgets)
set_target_properties(calculator PROPERTIES OUTPUT_NAME "吴小东的计算器")
if(MSVC)
    target_compile_options(calculator PRIVATE /utf-8)
endif()
