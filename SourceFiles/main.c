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

int main(int argc, char *argv[]) {
    char *file_name = malloc(FILENAME_MAX);
    while (--argc > 0) {
        /* Generate a new file with the ".as" extension by adding it to the input filename.*/
        printf("Start pre-proc\n");
        strcpy(file_name, argv[argc]);
        strcat(file_name, ".as");

        /*Execute the macro preprocessor on the ".as" file.*/
        if (exe_pre_assembler(file_name) != 0) {
            /*If it failed, move to the next file.*/
            continue;
        }

        printf("Start first pass\n");
        /* Generate a new file with the ".am" extension by adding it to the input filename.*/
        strcpy(file_name, argv[argc]);
        strcat(file_name, ".am");
        /*Execute the first pass, and then the second on the ".am" file.*/
        if (exe_first_pass(file_name) != 0) {
            /*If it failed, move to the next file.*/
            continue;
        }
    }

    /*Free allocated memory*/
    free(file_name);

    printf("end\n");
    return 0;
}

