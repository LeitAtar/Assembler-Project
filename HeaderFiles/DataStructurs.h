#ifndef ASSEMBLERPERSONAL_DATASTRUCTURS_H
#define ASSEMBLERPERSONAL_DATASTRUCTURS_H

#endif //ASSEMBLERPERSONAL_DATASTRUCTURS_H


typedef struct node {
    char *name;         /* Name associated with the node */
    char *content;      /* Content stored in the node */
    int line;           /* Line number associated with the content */
    struct node *next;  /* Pointer to the next node in the linked list */
} node;
