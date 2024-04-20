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

extern symbol_list *symbol_table;
extern macro_list *mcr_table;

int exe_first_pass(char *file_name) {
    int algoCounter = 1, IC, DC, label_flag = 0, value = 0, error_flag = 0, L = 0, line_counter = 0;
    char *token = malloc(MAX_LINE_LENGTH);
    char *str, *temp, *ptr;

    FILE *fp = fopen(file_name, "r");
    FILE *machine = fopen("temp____", "w");

    if (fp == NULL || machine == NULL) {
        printf("Error opening file.\n");
        free(token);
        token = NULL;
        return 1;
    }
    str = malloc(MAX_LINE_LENGTH), temp = malloc(MAX_LINE_LENGTH);

    free(token);
    token = NULL;
    symbol_table = NULL;
    symbol_list *node;

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
                line_counter++;
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
                        printf("Error: wrong define syntax | line:%d\n", line_counter);
                    }
                    algoCounter = 5;
                    break;
                }
            case 4:
                if (strstr(str, ":") != NULL) { //if there's a label
                    error_flag = 1;
                    printf("Error: label definition on a define command | line:%d\n", line_counter);
                    algoCounter = 2;
                    break;
                }
                token = strtok(NULL, " \t \n ="); //definition name
                ptr = strtok(NULL, " \t \n ="); //definition value
                if (num_check(ptr) == 1) { //if the value is not a number
                    error_flag = 1;
                    printf("Error: wrong define syntax | line:%d\n", line_counter);
                    algoCounter = 2;
                    break;
                }
                value = atoi(ptr);
                value = insertToSymbolTable(&symbol_table, token, value, ".mdefine", 0);
                if (value == 1) { /*failed to insert symbol*/
                    error_flag = 1;
                    printf(" | line:%d\n", line_counter);
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

                if (label_check(token) == 1) {
                    error_flag = 1;
                    printf("Illegal label declaration | line:%d\n", line_counter);
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
                        printf("wrong .data or .string | line:%d\n", line_counter);
                        algoCounter = 2;
                        break;
                    }
                    algoCounter = 10;
                    break;
                }
            case 8:
                strcpy(temp, str);
                token = strtok(temp, ":");
                if (label_flag == 1) { //label
                    if (strcmp(strtok(NULL, " \t"), ".entry") != 0) { /*not .entry*/
                        value = insertToSymbolTable(&symbol_table, token, DC, ".data", 0);
                        if (value == 1) { //failed to insert label
                            error_flag = 1;
                            printf(" | line:%d\n", line_counter);
                        }
                    } else {
                        printf("Warning: entry label | line:%d\n", line_counter);
                    }
                }
            case 9:
                strcpy(temp, str); //copy the string to temp
                token = strtok(temp, " \t");

                if (strstr(token, ":") != NULL) { //if it's a label
                    token = strtok(NULL, " \t");
                }

                if (strcmp(token, ".data") == 0) {
                    token = strtok(NULL, "\n");
                    token = data_to_binary(token);
                    fprintf(machine, "%s", token);
                    free(token);
                    token = NULL;
                    strcpy(temp, str);
                    token = strtok(temp, ".");
                    token = strtok(NULL, " \t");
                    token = strtok(NULL, " \t");

                    while (token != NULL) {
                        DC++;
                        token = strtok(NULL, ",\n");
                    }
                }
                else if (strcmp(token, ".string") == 0) {
                    token = strtok(NULL, " \t");
                    token = string_to_binary(token);
                    if (token == NULL) {
                        error_flag = 1;
                        printf(" | line:%d\n", line_counter);
                        algoCounter = 2;
                        break;
                    }
                    fprintf(machine, "%s", token);
                    strcpy(temp, str);
                    token = strtok(temp, ".");
                    token = strtok(NULL, " \t\n");
                    token = strtok(NULL, " \t \n \" ");
                    DC += (int) strlen(token) + 1;
                }
                else {
                    error_flag = 1;
                    printf("Error: can't recognise a command | line:%d\n", line_counter);
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
                            printf(" | line:%d\n", line_counter);
                        }
                        token = strtok(NULL, ", \n \t");
                    }
                }
                else if (strstr(str, ".entry") != NULL) { //if its entry command
                    while (token != NULL) {
                        if (insertToSymbolTable(&symbol_table, token, -1, ".entry", 1) == 1) { //failed to insert label
                            error_flag = 1;
                            printf(" | line:%d\n", line_counter);
                        }
                        token = strtok(NULL, ", \n \t");
                    }
                }
                else {
                    error_flag = 1;
                    printf("Error: can't recognise a command | line:%d\n", line_counter);
                }
                algoCounter = 2;
                break;
            case 12: //done

                if (label_flag == 1) {
                    strcpy(temp, str); //copy the string to temp
                    token = strtok(temp, ":"); //label name
                    if (insertToSymbolTable(&symbol_table, token, IC + 100, ".code", 0) == 1) { //failed to insert label
                        error_flag = 1;
                        printf(" | line:%d\n", line_counter);
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
                    printf("Error: can't recognise a command | line:%d\n", line_counter);
                }
            case 14:
                strcpy(temp, str); //copy the string to temp
                if (strstr(temp, ":") != NULL) { /*skip label*/
                    token = strtok(temp, ":");
                    token = strtok(NULL, ":\n");
                }
                else {
                    token = temp;
                }
                L = findL(token);
                if (L == -1) { //failed to find L
                    error_flag = 1;
                    printf("Error: can't recognise a command | line:%d\n", line_counter);
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
                    printf(" | line:%d\n", line_counter);
                }
                else
                    fprintf(machine, "%s", token);
                free(token);
                token = NULL;
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
    str = NULL;
    free(temp);
    temp = NULL;
    free(token);
    token = NULL;
    if (error_flag == 1) {
        remove("temp____");
        printf("Error: failed first pass on file: %s\n", file_name);
        return 1;
    }

    if (exe_second_pass(file_name, IC, DC) != 0) {
        printf("Error: failed second pass on file: %s\n", file_name);
        return 1;
    }
    printf("File: %s assembled successfully\n", file_name);
    symbol_table = NULL;
    mcr_table = NULL;
    node = NULL;
    return 0;
}
