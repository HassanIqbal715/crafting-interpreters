add_library(
    lox chunk.c compiler.c debug.c line.c memory_manager.c object.c scanner.c 
        table.c value.c vm.c
)

add_executable(Lox main.c)
target_link_libraries(Lox PRIVATE lox)