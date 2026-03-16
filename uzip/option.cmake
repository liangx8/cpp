add_executable(${target_path} ${sources})
target_link_libraries(${target_path} minizip)
target_compile_options(${target_path} PRIVATE ${target_option})
