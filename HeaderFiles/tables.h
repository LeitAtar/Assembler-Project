#ifndef ASSEMBLERPERSONAL_TABLES_H
#define ASSEMBLERPERSONAL_TABLES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct macro_list { /*for macros*/
    char *name;         /* Name associated with the node */
    char *content;      /* Content stored in the node */
    int line;           /* Line number associated with the content */
    int line_amount;     /* Amount of lines in content*/
    struct macro_list *next;  /* Pointer to the next node in the linked list */
} macro_list;

typedef struct symbol_list {
    char *symbol;
    int value;
    char *identifier;
    int is_entry;
    struct symbol_list *next;
} symbol_list;


int insert_to_symbol_table(symbol_list **head, char *symbol, int value, char *identifier, int is_entry);

macro_list* is_in_macro_table(macro_list *table, char *name);

int insert_to_macro_table(macro_list **table, char *mcro_name, char *content);


#endif
