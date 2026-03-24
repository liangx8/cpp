cmake_minimum_required(VERSION 4.2)
# 文档不建议在这里指定编译器
# set(CMAKE_C_COMPILER gcc)
project(cpphelp)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set (targets expense uzip ascii)
set(target_option -Wall -Werror)
foreach(target_path IN LISTS targets)
    file(GLOB sources CONFIGURE_DEPENDS "${target_path}/*.c")
    include(${target_path}/option.cmake OPTIONAL)
endforeach(target_path IN LISTS targets)

