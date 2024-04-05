#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/Globals.h"
#include "../HeaderFiles/tables.h"
#include "../HeaderFiles/firstPass.h"
#include "../HeaderFiles/utilities.h"

symbol_list *symbol_table = NULL;

int exe_first_pass(char *file_name) {
    FILE *fp = fopen(file_name, "r");

    char *token = malloc(MAX_LINE_LENGTH);
    strcpy(token, file_name);
    token = strtok(token, ".txt");
    strcat(token, ".am");
    FILE *pre_assembled = fopen(token, "r");

    if (fp == NULL || pre_assembled == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    while (!feof(fp)) {
        fgets(token, MAX_LINE_LENGTH, fp);
        fprintf(pre_assembled, "%s", token);
    }

    fclose(fp);
    fclose(pre_assembled);

    fp = fopen(file_name, "r");

    int algoCounter = 1, IC, DC, label_flag = 0, value = 0, error_flag = 0, L = 0, i = 0;
    char *str = malloc(MAX_LINE_LENGTH), *temp = malloc(MAX_LINE_LENGTH);
    char *binary_line;
    char *binary_line2;
    char *binary_line3;
    char *binary_line4;
    char *binary_line5;

    symbol_table = NULL;
    symbol_list *node = NULL;

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
                strcpy(temp, str); //copy the string to temp
                token = strtok(temp, " \t");
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
                if (value == 0) { //failed to read the value
                    error_flag = 1;
                }
                value = insertToSymbolTable(&symbol_table, token,value, "mdefine");
                if (value == 1) { //failed to insert symbol
                    error_flag = 1;
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
                    }
                    algoCounter = 10;
                    break;
            }
            case 8:
                strcpy(temp, str); //copy the string to temp
                token = strtok(temp, ":"); //reset token
                if (label_flag == 1) { //label
                    if (strcmp(strtok(NULL, " \t"), ".entry") != 0) { //not .entry
                        value = insertToSymbolTable(&symbol_table, token, DC, ".data");
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
                        if (insertToSymbolTable(&symbol_table, token, 69, "external") == 1) { //failed to insert label
                            error_flag = 1;
                        }
                        token = strtok(NULL, ",");
                    }
                }
                algoCounter = 2;
                break;
            case 12: //done
                if (label_flag == 1) {
                    strcpy(temp, str); //copy the string to temp
                    token = strtok(temp, ":"); //label name
                    if (insertToSymbolTable(&symbol_table, token, IC + 100, "code") == 1) { //failed to insert label
                        error_flag = 1;
                    }
                }

            case 13:
                strcpy(temp, str); //copy the string to temp
                token = strtok(temp, " \t");
                value = search_command(token);
                if (value == -1) { //not a command
                    error_flag = 1;
                }
            case 14:
                binary_line = malloc(16);
                binary_line2 = malloc(16);
                binary_line3 = malloc(16);
                binary_line4 = malloc(16);
                binary_line5 = malloc(16);

                L = findL(str);
                strcpy(binary_line, "0000");
                //opcode
                token = decimalToBinary(value);
                for (i = 0; i < 4 - strlen(token); ++i) {
                    strcat(binary_line, "0");
                }
                strcat(binary_line, token);
                //source operand
                token = strtok(NULL, ", \t");
                value = check_operand(token);
                token = decimalToBinary(value);
                strcat(binary_line, token);
                //destination operand
                token = strtok(NULL, ", \t");
                value = check_operand(token);
                token = decimalToBinary(value);
                strcat(binary_line, token);
                //A,R,E
                strcat(binary_line, "00");

                free(binary_line);
                free(binary_line2);
                free(binary_line3);
                free(binary_line4);
                free(binary_line5);


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
                    if (strcmp(node -> identifier, "data") == 0) {
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

    if (error_flag == 1) {
        return 1;
    }

    return 0;
}