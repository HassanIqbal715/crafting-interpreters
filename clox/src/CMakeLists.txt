add_library(
    lox chunk.c compiler.c debug.c line.c memory_manager.c object.c scanner.c 
        table.c value.c vm.c
)

add_executable(Lox main.c)
add_executable(Lox_debug main.c)

target_link_libraries(Lox PRIVATE lox)
target_link_libraries(Lox_debug PRIVATE lox)
target_compile_options(Lox_debug PRIVATE -fsanitize=address)
target_link_options(Lox_debug PRIVATE -fsanitize=address)