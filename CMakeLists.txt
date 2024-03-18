cmake_minimum_required(VERSION 3.27)
project(AssemblerPersonal C)

set(CMAKE_C_STANDARD 90)

add_executable(AssemblerPersonal SourceFiles/main.c
        SourceFiles/convertToBaseFour.c
        SourceFiles/firstPass.c)
