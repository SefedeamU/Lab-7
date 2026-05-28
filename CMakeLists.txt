cmake_minimum_required(VERSION 3.31)
project(Sem10_Code)

set(CMAKE_CXX_STANDARD 14)

include_directories(Laboratorio)

add_executable(Sem10_Code
        Laboratorio/ast.cpp
        Laboratorio/ast.h
        Laboratorio/environment.h
        Laboratorio/main.cpp
        Laboratorio/parser.cpp
        Laboratorio/parser.h
        Laboratorio/scanner.cpp
        Laboratorio/scanner.h
        Laboratorio/token.cpp
        Laboratorio/token.h
        Laboratorio/visitor.cpp
        Laboratorio/visitor.h)
