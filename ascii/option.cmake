add_executable(char ${target_path}/char.c)
target_compile_options(char PRIVATE ${target_option})
add_executable(color ${target_path}/color.c)
target_compile_options(color PRIVATE ${target_option})
