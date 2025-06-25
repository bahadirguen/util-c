cmake_minimum_required(VERSION 3.10)

project(
  utils-c
  VERSION 1.0
  LANGUAGES C
)

if(MSVC)
  # Dummy
else()
  SET (CMAKE_C_COMPILER "clang")
  SET (CMAKE_CXX_COMPILER "clang++")

  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(Main main.c)

# Dummy target for clangd to recognize other source files
add_library(
  collection
  OBJECT
  arena.c
  array.c
)
set_target_properties(collection PROPERTIES EXCLUDE_FROM_ALL TRUE)
