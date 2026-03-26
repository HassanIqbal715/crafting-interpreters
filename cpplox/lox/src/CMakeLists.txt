add_library(
    lox break.cpp environment.cpp files.cpp interpreter.cpp lox_function.cpp 
    lox.cpp native_function.cpp object.cpp parser.cpp resolver.cpp return.cpp 
    runtime_error.cpp scanner.cpp token.cpp
)
add_executable(Lox main.cpp)
target_link_libraries(Lox PRIVATE lox)