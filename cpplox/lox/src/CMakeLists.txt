add_library(lox break.cpp environment.cpp files.cpp interpreter.cpp lox.cpp parser.cpp 
    runtime_error.cpp scanner.cpp token.cpp)
add_executable(Lox main.cpp)
target_link_libraries(Lox PRIVATE lox)