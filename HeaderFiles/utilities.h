#ifndef ASSEMBLERPERSONAL_UTILITIES_H
#define ASSEMBLERPERSONAL_UTILITIES_H

#endif //ASSEMBLERPERSONAL_UTILITIES_H

char* decimalToBinaryOLD(int num);
char* decimalToBinary(int num, int length);

int findL(char *line);

int deleteLine(char *file_name, int delete_line);

macro_list* isInMacroTable(macro_list *table, char *name);

int insertToMacroTable(macro_list **table, char *mcro_name, char *content);

int insertLine(char *file_name, int replace_line, char *content);

int search_command(char *token);

int check_operand(char *strtok);

char *to_binary (char *line);

int file_creator_with_identifier(char *file_name, const char *identifier);

