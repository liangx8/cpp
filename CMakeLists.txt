cmake_minimum_required(VERSION 4.2)
project(cpphelp)
set (targets expense uzip ascii)
set(target_option -Wall)
foreach(target_path IN LISTS targets)
    file(GLOB sources CONFIGURE_DEPENDS "${target_path}/*.c")
    include(${target_path}/option.cmake OPTIONAL)
endforeach(target_path IN LISTS targets)

