add_library(
    lox chunk.c debug.c memory_manager.c value.c
)

add_executable(Lox main.c)
target_link_libraries(Lox PRIVATE lox)