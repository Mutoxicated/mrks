cmake_minimum_required(VERSION 3.10.0)
project(c.lex VERSION 0.1.0 LANGUAGES C)


file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "./src/*.c")
add_executable("mkrs" "${MY_SOURCES}")
target_include_directories(mkrs PUBLIC "./include/")