#ifndef ASSEMBLERPERSONAL_TABLES_H
#define ASSEMBLERPERSONAL_TABLES_H

#endif //ASSEMBLERPERSONAL_TABLES_H

typedef struct label_address {
    int address;
    char *label_name;
    int assembly_line;
    int is_data_line;
} label_address;




typedef struct other_table {
    char *label_name;
    int assembly_line;
} other_table;

int insertToSymbolTable(symbol_list **head, char *symbol, int value, char *identifier, int is_entry);
