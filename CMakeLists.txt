cmake_minimum_required(VERSION 3.15)
project(MyProject)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(basic_header test/basic/basic_header.c)
target_compile_features(basic_header PRIVATE c_std_99)
target_include_directories(basic_header PRIVATE src)

add_executable(basic_reflect test/basic/basic_reflect.c)
target_compile_features(basic_reflect PRIVATE c_std_99)
target_include_directories(basic_reflect PRIVATE src)
