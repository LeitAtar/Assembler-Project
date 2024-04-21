#ifndef ASSEMBLERPERSONAL_UTILITIES_H
#define ASSEMBLERPERSONAL_UTILITIES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* decimal_to_binary(int num, int length);

int find_L(char *line);

int search_command(char *token);

int check_operand(char *strtok);

char *to_binary (char *line);

int ext_file_creator(char *file_name);

char* data_to_binary (char* line);

char* string_to_binary (char* line);

int label_check(char* label);

int num_check(char* num);

#endif


