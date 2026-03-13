cmake_minimum_required(VERSION 4.2)
project(cpphelp)
set (targets expense uzip ascii)
set(target_option -Wall)
foreach(target_path IN LISTS targets)
    include(${target_path}/option.cmake OPTIONAL)
endforeach(target_path IN LISTS targets)

