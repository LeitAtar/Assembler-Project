cmake_minimum_required(VERSION 3.27)
project(AssemblerPersonal C)

set(CMAKE_C_STANDARD 90)

add_executable(AssemblerPersonal SourceFiles/main.c
        SourceFiles/convertToBaseFour.c
        SourceFiles/firstPass.c
        SourceFiles/DataStructures.c
        HeaderFiles/DataStructures.h
        HeaderFiles/Globals.h
        SourceFiles/tables.c
        HeaderFiles/tables.h)
