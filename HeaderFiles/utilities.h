#ifndef ASSEMBLERPERSONAL_UTILITIES_H
#define ASSEMBLERPERSONAL_UTILITIES_H

#endif //ASSEMBLERPERSONAL_UTILITIES_H

char* decimalToBinary(int num);

int findL(char *line);

int deleteLine(char *file_name, int delete_line);

macro_list* isInMacroTable(macro_list *table, char *name);

int insertToMacroTable(macro_list **table, char *mcro_name, char *content);

int insertLine(char *file_name, int replace_line, char *content);

int search_command(char *token);

const char *check_operand(char *strtok);