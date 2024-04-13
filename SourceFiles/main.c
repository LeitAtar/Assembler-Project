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
    setvbuf(stdout, NULL, _IONBF, 0);
    while (--argc > 0) {
        /* Generate a new file with the ".as" extension by adding it to the input filename.*/
        strcpy(file_name, argv[argc]);
        strcat(file_name, ".as");
        printf("Starting pre-proc on file: %s\n", file_name);
        /*Execute the macro preprocessor on the ".as" file.*/
        if (exe_pre_assembler(file_name) != 0) {
            /*If it failed, move to the next file.*/
            continue;
        }
        /* Generate a new file with the ".am" extension by adding it to the input filename.*/
        strcpy(file_name, argv[argc]);
        strcat(file_name, ".am");
        printf("Starting first pass on file: %s\n", file_name);
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

