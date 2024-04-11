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
#include "../HeaderFiles/convertToBaseFour.h"

symbol_list *symbol_table = NULL;

int exe_first_pass(char *file_name) {
    char *token = malloc(MAX_LINE_LENGTH);
    strcpy(token, file_name);
    strcat(token, ".am");
    FILE *fp = fopen(token, "r");
    FILE *machine = fopen("temp____", "w");

    if (fp == NULL || machine == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int algoCounter = 1, IC, DC, label_flag = 0, value = 0, error_flag = 0, L = 0, i = 0;
    char *str = malloc(MAX_LINE_LENGTH), *temp = malloc(MAX_LINE_LENGTH);

    symbol_table = NULL;
    symbol_list *node = symbol_table;

    while(algoCounter != 0) {
        switch (algoCounter) {
            case 1: //done
                IC = 0;
                DC = 0;
            case 2: //done
                if (feof(fp)) {
                    algoCounter = 16;
                    break;
                }

                fgets(str, MAX_LINE_LENGTH, fp);
                label_flag = 0; //reset
                strcpy(temp, str); //copy the string to temp
                token = strtok(temp, " \t\n");
                if (str[0] == ';' || token == NULL) { //if comment or empty line
                    algoCounter = 2;
                    break;
                }

            case 3:
                strcpy(temp, str); //copy the string to temp
                token = strtok(temp, " \t");
                if (strcmp(token, ".define") != 0) { //if not define statement move to 5
                    if (strcasecmp(token, ".define") == 0) { //wrong define definition
                        error_flag = 1;
                        printf("wrong define definition\n");
                    }
                    algoCounter = 5;
                    break;
                }
            case 4:
                token = strtok(NULL, " \t"); //name of the define
                value = atoi(strtok(NULL, "= \t"));
                if (value == 0) { //failed to read the value
                    error_flag = 1;
                    printf("failed to read the value\n");
                }
                value = insertToSymbolTable(&symbol_table, token, value, ".mdefine", 0);
                if (value == 1) { //failed to insert symbol
                    error_flag = 1;
                    printf("failed to insert symbol\n");
                }

                algoCounter = 2;
                break;

            case 5:
                strcpy(temp, str); //copy the string to temp
                token = strtok(temp, " \t"); //first field in the word

                if (strchr(token, ':') == NULL) {//not a label
                    algoCounter = 7;
                    break;
                }

                if (strlen(token) > MAX_LABEL_LENGTH
                    || strchr(token, ' ') != NULL) { //wrong label definition
                    error_flag = 1;
                    printf("wrong label definition\n");
                }

            case 6: //done
                label_flag = 1;
            case 7:
                strcpy(temp, str); //copy the string to temp
                token = strtok(temp, " \t");
                if (label_flag == 1) { //advance to next field because it's a label
                    token = strtok(NULL, " \t");
                }

                if (strcmp(token, ".data") != 0
                    && strcmp(token, ".string") != 0) { //not string or data
                    if (strcasecmp(token, ".data") == 0
                        || strcasecmp(token, ".string") == 0) { //wrong data or string
                        error_flag = 1;
                        printf("wrong data or string\n");
                    }
                    algoCounter = 10;
                    break;
                }
            case 8:
                strcpy(temp, str); //copy the string to temp
                token = strtok(temp, ":"); //reset token
                if (label_flag == 1) { //label
                    if (strcmp(strtok(NULL, " \t"), ".entry") != 0) { //not .entry
                        value = insertToSymbolTable(&symbol_table, token, DC, ".data", 0);
                        if (value == 1) { //failed to insert label
                            error_flag = 1;
                            printf("failed to insert label\n");
                        }
                    } else {
                        printf("entry label warning\n");
                    }
                }
            case 9:
                strcpy(temp, str); //copy the string to temp
                token = strtok(temp, " \t");

                if (strstr(token, ":") != NULL) { //if its a label
                    token = strtok(NULL, " \t");
                }

                if (strcasecmp(token, ".data") == 0) {
                    token = strtok(NULL, "\n");
                    token = data_to_binary(token);
                    fprintf(machine, "%s", token);
                    strcpy(temp, str);
                    token = strtok(temp, ".");
                    token = strtok(NULL, " \t");
                    token = strtok(NULL, " \t");

                    while (token != NULL) {
                        DC++;
                        token = strtok(NULL, ",\n");
                    }
                }
                else if (strcasecmp(token, ".string") == 0) {
                    token = strtok(NULL, " \t");
                    token = string_to_binary(token);
                    fprintf(machine, "%s", token);
                    strcpy(temp, str);
                    token = strtok(temp, ".");
                    token = strtok(NULL, " \t\n");
                    token = strtok(NULL, " \t\n");
                    DC += (int) strlen(token) - 5;
                }
                else {
                    error_flag = 1;
                    printf("error message\n");
                }

                algoCounter = 2;
                break;
            case 10:
                if (strstr(str, ".extern") == NULL
                    && strstr(str, ".entry") == NULL) { //not .extern or .entry
                    algoCounter = 12;
                    break;
                }
            case 11:
                token = strtok(NULL, ", \n \t");
                if (strstr(str, ".extern") != NULL) { //if its extern command
                    while (token != NULL) {
                        if (insertToSymbolTable(&symbol_table, token, 0, ".external", 0) == 1) { //failed to insert label
                            error_flag = 1;
                            printf("failed to insert label\n");
                        }
                        token = strtok(NULL, ", \n \t");
                    }
                }
                else if (strstr(str, ".entry") != NULL) { //if its entry command
                    while (token != NULL) {
                        if (insertToSymbolTable(&symbol_table, token, -1, ".entry", 1) == 1) { //failed to insert label
                            error_flag = 1;
                            printf("failed to insert label\n");
                        }
                        token = strtok(NULL, ", \n \t");
                    }
                }
                else {
                    error_flag = 1;
                    printf("error message case 11\n");
                }
                algoCounter = 2;
                break;
            case 12: //done
                if (label_flag == 1) {
                    strcpy(temp, str); //copy the string to temp
                    token = strtok(temp, ":"); //label name
                    if (insertToSymbolTable(&symbol_table, token, IC + 100, ".code", 0) == 1) { //failed to insert label
                        error_flag = 1;
                        printf("failed to insert label\n");
                    }
                }

            case 13:
                strcpy(temp, str); //copy the string to temp
                token = strtok(temp, " \t\n");
                if (strstr(token, ":") != NULL) { //if there's a label
                    token = strtok(NULL, " \t\n");
                }
                value = search_command(token);
                if (value == -1) { //not a command
                    error_flag = 1;
                    printf("not a command\n");
                }
            case 14:
                strcpy(temp, str); //copy the string to temp
                if (strstr(temp, ":") != NULL) { /*skip label*/
                    token = strtok(temp, ":");
                    token = strtok(NULL, ":\n");
                }
                else {
                    //strcpy(token, temp);
                    token = temp;
                }
                L = findL(token);
                if (L == -1) { //failed to find L
                    error_flag = 1;
                    printf("failed to find L\n");
                }

                strcpy(temp, str); //copy the string to temp
                if (strstr(temp, ":") !=NULL) { /*skip label*/
                    token = strtok(temp, ":");
                    token = strtok(NULL, ":");
                }
                else {
                    strcpy(token, temp);
                }
                /*converts an assembly line to binary*/
                token = strtok(token, "\n");
                token = to_binary(token);
                if (token == NULL) {
                    error_flag = 1;
                    printf("failed to machinise command: %s\n", str);
                }
                fprintf(machine, "%s", token);

            case 15: //done
                IC += L;
                algoCounter = 2;
                break;
            case 16: //done
                if (error_flag == 1) {
                    algoCounter = 0; //finished first pass with errors
                    break;
                }
            case 17: //done
                node = symbol_table;
                while (node != NULL) {
                    if (strcmp(node -> identifier, ".data") == 0) {
                        node -> value += IC + 100;
                    }
                    node = node -> next;
                }
            case 18: //done
                algoCounter = 0; //finished first pass successfully
                break;
            default:
                return 1;
        }
    }
    fclose(fp);
    fclose(machine);
    free(str);
    free(temp);


    exe_second_pass(file_name, IC, DC);

    if (error_flag == 1) {
        return 1;
    }

    return 0;
}
