#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
    char *name;         /* Name associated with the node */
    char *content;      /* Content stored in the node */
    int line;           /* Line number associated with the content */
    struct node *next;  /* Pointer to the next node in the linked list */
} node;

node *make_node(char *name, char *content, int line_num){
    node *temp;

    /* Check if memory allocation for the node succeeded */
    temp = malloc(sizeof(node)); //handle

    temp->name = name;        /* Set the name of the node */
    temp->content = content;  /* Set the content string of the node */
    temp->line = line_num;    /* Set the line number associated with the content */
    temp->next = NULL;        /* Initialize the next pointer to NULL */

    return temp;  /* Return a pointer to the newly created node */
}



int main(int argc, char *argv[]) {
    char *as_file, *am_file;
    while (--argc > 0) {
        /* Generate a new file with the ".as" extension by adding it to the input filename.*/
        printf("Start pre-proc\n");
        as_file = add_new_file(argv[argc], ".as");

        /*Execute the macro preprocessor on the ".as" file.*/
        if (!mcro_exec(as_file)) {
            /*If it failed, move to the next file.*/
            continue;
        }

        printf("Start first pass\n");
        /* Generate a new file with the ".am" extension by adding it to the input filename.*/
        am_file = add_new_file(argv[argc], ".am");
        /*Execute the first pass, and then the second on the ".am" file.*/
        if (exe_first_pass(am_file) || exe_second_pass(am_file)) {
            /*If it failed, move to the next file.*/
            continue;
        }

        /*Free allocated memory*/
        free(as_file);
        free(am_file);
    }
    printf("end\n");
    return 0;
}

