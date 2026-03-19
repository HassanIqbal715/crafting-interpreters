add_library(lox files.cpp lox.cpp token.cpp)
add_executable(Lox main.cpp)
target_link_libraries(Lox PRIVATE lox)