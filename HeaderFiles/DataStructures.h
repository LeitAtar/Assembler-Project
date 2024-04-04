#ifndef ASSEMBLERPERSONAL_DATASTRUCTURS_H
#define ASSEMBLERPERSONAL_DATASTRUCTURS_H

#endif //ASSEMBLERPERSONAL_DATASTRUCTURES_H


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
    const char *identifier;
    struct symbol_list *next;
} symbol_list;


