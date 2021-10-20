cmake_minimum_required(VERSION 3.1)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(cpothero CXX)
add_executable(main 
main.cpp
othero.cpp
eval.cpp
learn.cpp
solve.cpp
engine.cpp
engine/one.cpp
engine/static.cpp
engine/rand_ai.cpp
engine/nteyomi.cpp
test/aitest.cpp
)
