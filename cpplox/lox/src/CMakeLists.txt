add_library(lox ast_printer.cpp files.cpp lox.cpp scanner.cpp token.cpp)
add_executable(Lox main.cpp)
target_link_libraries(Lox PRIVATE lox)