#ifndef ASSEMBLERPERSONAL_DATASTRUCTURES_H
#define ASSEMBLERPERSONAL_DATASTRUCTURES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct macro_list { //for macros
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


int insertToSymbolTable(symbol_list **head, char *symbol, int value, char *identifier, int is_entry);

#endif //ASSEMBLERPERSONAL_DATASTRUCTURES_H