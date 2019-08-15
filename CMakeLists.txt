cmake_minimum_required (VERSION 2.8)
project (example)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# Run re2c to generate the lexer
add_custom_command (
  OUTPUT exprLexer.cpp
  COMMAND re2c -o exprLexer.cpp ${PROJECT_SOURCE_DIR}/exprLexer.re
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/exprLexer.re
)

add_executable(${PROJECT_NAME} main.cpp exprLexer.cpp)