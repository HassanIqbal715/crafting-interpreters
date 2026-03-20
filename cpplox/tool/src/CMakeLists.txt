add_library(tool generate_ast.cpp utils.cpp)
add_executable(GenerateAst main.cpp)
target_link_libraries(GenerateAst PRIVATE tool)