file(GLOB sources CONFIGURE_DEPENDS "${target_path}/*.c")
add_executable(${target_path} ${sources})
target_link_libraries(${target_path} sqlite3)
target_compile_options(${target_path} PRIVATE ${target_option})