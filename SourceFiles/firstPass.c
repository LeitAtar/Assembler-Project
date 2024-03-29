#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/Globals.h"
#include "../HeaderFiles/tables.h"

int exe_first_pass(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    int algoCounter = 1, IC, DC, label_flag = 0, value = 0, error_flag = 0, L = 0;
    char *str = malloc(MAX_LINE_LENGTH);
    char *token = malloc(MAX_LINE_LENGTH);
    symbol_list *symbol_table = NULL;

    while(algoCounter != 0) {
        switch (algoCounter) {
            case 1: //done
                IC = 0;
                DC = 0;
            case 2: //done
                fgets(str, MAX_LINE_LENGTH, fp);
                label_flag = 0; //reset
                if (feof(fp)) {
                    algoCounter = 16;
                    break;
                }
            case 3:
                token = strtok(str, " \t");
                if (strcmp(token, ".define") != 0) { //if not define statement move to 5
                    if (strcasecmp(token, ".define") == 0) { //wrong define definition
                        error_flag = 1;
                    }
                    algoCounter = 5;
                    break;
                }
            case 4:
                token = strtok(NULL, " \t"); //name of the define
                value = atoi(strtok(NULL, "= \t"));
                if (value == 1) { //failed to read the value
                    error_flag = 1;
                }
                value = insertToSymbolTable(symbol_table, token,value, "mdefine");
                if (value == 1) { //failed to insert symbol
                    error_flag = 1;
                }

                algoCounter = 2;
                break;

            case 5:
                token = strtok(str, " \t"); //first field in the word

                if (strchr(token, ':') == NULL) {//not a label
                    algoCounter = 7;
                    break;
                }

                if (strlen(token) > MAX_LABEL_LENGTH
                || strchr(token, ' ') != NULL) { //wrong label definition
                    error_flag = 1;
                }

            case 6: //done
                label_flag = 1;
            case 7:
            if (label_flag == 1) {
                token = strtok(NULL, " \t");
            }
            else {
                token = strtok(str, " \t");
            }
                if (strcmp(token, ".data") != 0
                || strcmp(token, ".string") != 0) { //not string or data
                    if (strcasecmp(token, ".data") == 0
                    || strcasecmp(token, ".string") == 0) { //wrong data or string
                        error_flag = 1;
                    }
                    algoCounter = 10;
                    break;
                }
            case 8:
                token = strtok(str, ":"); //reset token
                if (label_flag == 1) { //label
                    if (strcmp(strtok(NULL, " \t"), ".entry") != 0) { //not .entry CHECK IF NECESSARY
                        value = insertToSymbolTable(symbol_table, token, DC, ".data");
                        if (value == 1) { //failed to insert label
                            error_flag = 1;
                        }
                    }
                    else {
                        printf("entry label warning\n");
                    }
                }
            case 9:

                algoCounter = 2;
                break;
            case 10:
                if (strstr(str, ".extern") != 0
                || strstr(str, ".entry") != 0) { //not .extern or .entry
                    algoCounter = 12;
                    break;
                }
            case 11:
                if (strstr(str, "extern") == 0) { //if its extern command
                    while (token != NULL) {
                        if (insertToSymbolTable(symbol_table, token, 69, "external") == 1) { //failed to insert label
                            error_flag = 1;
                        }
                        token = strtok(NULL, ",");
                    }
                }
                algoCounter = 2;
                break;
            case 12:
                if (label_flag == 1) {
                    token = strtok(str, ":"); //label name
                    if (insertToSymbolTable(symbol_table, token, IC + 100, "code") == 1) { //failed to insert label
                        error_flag = 1;
                    }
                }

            case 13:

            case 14:

            case 15: //done
                IC += L;
                algoCounter = 2;
                break;
            case 16: //done
                if (error_flag == 1) {
                    algoCounter = 0; //finished first pass with errors
                    break;
                }
            case 17:

            case 18: //done
                algoCounter = 0; //finished first pass successfully
                break;
            default:
                return -1;
        }
    }
}