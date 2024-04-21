cmake_minimum_required(VERSION 3.27)
project(AssemblerPersonal C)

set(CMAKE_C_STANDARD 90)

add_executable(AssemblerPersonal SourceFiles/assembler.c
        SourceFiles/convert_to_base_four.c
        SourceFiles/first_pass.c
        HeaderFiles/globals.h
        SourceFiles/tables.c
        HeaderFiles/tables.h
        SourceFiles/pre_assembler.c
        HeaderFiles/pre_assembler.h
        SourceFiles/second_pass.c
        HeaderFiles/second_pass.h
        SourceFiles/utilities.c
        SourceFiles/utilities.c
        HeaderFiles/utilities.h
        HeaderFiles/first_pass.h
        HeaderFiles/convert_to_base_four.h
        MakeFile
)
