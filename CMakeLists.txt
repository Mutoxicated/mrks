cmake_minimum_required(VERSION 3.10.0)
project(c.lex VERSION 0.1.0 LANGUAGES C)


file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_executable("c.lex" "${MY_SOURCES}")
target_include_directories(c.lex PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
