cmake_minimum_required (VERSION 2.8)
project (example)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# Run treecc to generate the CFG
add_custom_command (
  PRE_BUILD
  OUTPUT cfg.cpp cfg.h
  COMMAND treecc -o cfg.cpp -h cfg.h ${PROJECT_SOURCE_DIR}/cfg.tc
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/cfg.tc
)

#Run treecc to generate the ast
add_custom_command(
  OUTPUT ast.cpp ast.h
  COMMAND treecc -o ast.cpp -h ast.h ${PROJECT_SOURCE_DIR}/ast.tc
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/ast.tc
)

# Run bison to generate the lexer
add_custom_command (
  OUTPUT exprParser.cpp
  COMMAND bison --defines=Tokens.h -o exprParser.cpp ${PROJECT_SOURCE_DIR}/exprParser.y
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/exprParser.y
)

# Run re2c to generate the lexer
add_custom_command (
  OUTPUT exprLexer.cpp
  COMMAND re2c -w -o exprLexer.cpp ${PROJECT_SOURCE_DIR}/exprLexer.re
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/exprLexer.re
)

add_executable(${PROJECT_NAME}
  #main.cpp
  exprParser.cpp
  x86.cpp
  ast.cpp
  cfg.cpp
  exprLexer.cpp
)