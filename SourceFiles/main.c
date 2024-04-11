#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/tables.h"
#include "../HeaderFiles/firstPass.h"
#include "../HeaderFiles/utilities.h"
#include "../HeaderFiles/secondPass.h"
#include "../HeaderFiles/preAssembler.h"

/*node *make_node(char *name, char *content, int line_num){
    node *temp;

    *//* Check if memory allocation for the node succeeded *//*
    temp = malloc(sizeof(node)); //handle

    temp->name = name;        *//* Set the name of the node *//*
    temp->content = content;  *//* Set the content string of the node *//*
    temp->line = line_num;    *//* Set the line number associated with the content *//*
    temp->next = NULL;        *//* Initialize the next pointer to NULL *//*

    return temp;  *//* Return a pointer to the newly created node *//*
}*/



int main(int argc, char *argv[]) {
    char *as_file, *am_file;
    while (--argc > 0) {
        /* Generate a new file with the ".as" extension by adding it to the input filename.*/
        printf("Start pre-proc\n");
        as_file = add_new_file(argv[argc], ".as");

        /*Execute the macro preprocessor on the ".as" file.*/
        if (exe_pre_assembler(as_file) != 0) {
            /*If it failed, move to the next file.*/
            continue;
        }

        printf("Start first pass\n");
        /* Generate a new file with the ".am" extension by adding it to the input filename.*/
        am_file = add_new_file(argv[argc], ".am");
        /*Execute the first pass, and then the second on the ".am" file.*/
        if (exe_first_pass(am_file) != 0 || exe_second_pass(am_file) != 0) {
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

