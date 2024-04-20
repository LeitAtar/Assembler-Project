#ifndef ASSEMBLERPERSONAL_UTILITIES_H
#define ASSEMBLERPERSONAL_UTILITIES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* decimalToBinary(int num, int length);

int findL(char *line);

int deleteLine(char *file_name, int delete_line);

macro_list* isInMacroTable(macro_list *table, char *name);

int insertToMacroTable(macro_list **table, char *mcro_name, char *content);

int insertLine(char *file_name, int replace_line, char *content);

int search_command(char *token);

int check_operand(char *strtok);

char *to_binary (char *line);

int ext_file_creator(char *file_name);

char* data_to_binary (char* line);

char* string_to_binary (char* line);

int label_check(char* label);

int num_check(char* num);

#endif


