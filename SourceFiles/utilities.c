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

char* decimalToBinary(int num, int length) {
    /*Array to store binary number*/
    char binaryLine[16];
    char *reverse_binaryLine;
    char *ones;
    int j = 0, k = 0;
    int is_negative = (num < 0) ? 1 : 0;
    num = abs(num);

    if (is_negative == 1 && length == 0) {
        printf("Error: negative number with no length\n");
        return NULL;
    }
    /*Counter for binary array*/
    int i = 0;
    while (num > 0) {
        // Store remainder in binary array
        binaryLine[i] = (char) (num % 2 + '0');
        num = num / 2;
        i++;
    }
    if (is_negative) {
        /* Two's complement*/
        for (j = 0; j < i; j++) {
            binaryLine[j] = (binaryLine[j] == '0') ? '1' : '0';
        }
        /*Add 1 to the binary number*/
        for (j = 0; j < 16; j++) {
            if (binaryLine[j] == '1') {
                binaryLine[j] = '0';
            } else {
                binaryLine[j] = '1';
                break;
            }
        }
    }
    if (length == 0) {
        ones = calloc(i + 1, sizeof(char));
    }
    else
    {
        ones = calloc(length + 1, sizeof(char));
    }
    reverse_binaryLine = calloc(i + 1, sizeof(char));

    if (reverse_binaryLine == NULL || ones == NULL) { // Check for allocation failure
        printf("Memory allocation failed\n");
        free(reverse_binaryLine);
        free(ones);
        return NULL;
    }
    k = 0;
    /* Print binary array in reverse order*/
    for (j = i - 1; j >= 0; j--) {
        //printf("%d", binaryNum[j]);
        reverse_binaryLine[k] = binaryLine[j];
        k++;
    }
    reverse_binaryLine[k] = '\0';
    if(is_negative == 1) {
        for (j = 0; j < length - strlen(reverse_binaryLine); j++) {
            ones[j] = '1';
        }
    }
    if (length != 0 && is_negative == 0) {
        for (j = 0; j < length - strlen(reverse_binaryLine); j++) {
            ones[j] = '0';
        }
    }
    strcat(ones, reverse_binaryLine);
    free(reverse_binaryLine);
    return ones;
}


int findL(char *line)
{
    char temp_line[1000];
    strcpy(temp_line, line);
    char *token = strtok(temp_line, ", \t");
    int L = 0, i = 0, is_quote = 0, second_r = 0;

    while(token != NULL)
    {
        if(strstr(token, "[") != NULL &&  strstr(token, "]") != NULL)
        {
            L += 2;
        }
        else if(strcmp(token, ".data") == 0 || strcmp(token, ".entry") == 0 || strcmp(token, ".extern") == 0)
        {
            L; // we dont change the L value
        }
        else if(strcmp(token, ".string") == 0)
        {
            is_quote = 1;
        }
        else if(is_quote)
        {
            for(i = 0; i <= strlen(token) - 6; i++)
            {
                L++;
            }
            is_quote = 0;
        }
        else if(token[0] == 'r' && second_r == 0 && token[1] >= '0' && token[1] <= '7')
        {
            second_r = 1;
            L++;
        }
        else if(second_r == 1 && token[0] == 'r' && token[1] >= '0' && token[1] <= '7')
        {
            //TODO: check if the number of register is legal
            second_r = 0;
        }
        else
        {
            L++;
        }

        token = strtok(NULL, ", \t\n");
    }

    return L;
}

int deleteLine(char *file_name, int delete_line)
{
    FILE *fp, *temp;
    char *temp_filename = malloc(FILENAME_MAX);
    char buffer[MAX_LINE_LENGTH];

    strcpy(temp_filename, "temp____");
    strcat(temp_filename, file_name);
    fp = fopen(file_name, "r");
    temp = fopen(temp_filename, "w");

    if(fp == NULL || temp == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    int keep_reading = 1;
    int current_line = 1;
    do {
        fgets(buffer, MAX_LINE_LENGTH, fp);

        if(current_line != delete_line)
            fputs(buffer, temp);

        if(feof(fp)) {
            keep_reading = 0;
        }

        current_line++;

    } while(keep_reading);

    fclose(fp);
    fclose(temp);


    if(remove(file_name) != 0) {
        printf("Error deleting file.\n");
        return 1;
    }

    if (rename(temp_filename, file_name) != 0) {
        printf("Error renaming file.\n");
        return 1;
    }

    free(temp_filename);


    return 0;
}

macro_list* isInMacroTable(macro_list *table, char *name)
{
    macro_list *node = table;
    while(node != NULL)
    {
        if(strcmp(node->name, name) == 0)
            return node;
        node = node->next;
    }
    return NULL;
}

int insertToMacroTable(macro_list **table, char *mcro_name, char *content) {
    macro_list *node = isInMacroTable(*table, mcro_name);
    if (node != NULL) {
        // Reallocate memory for combined content
        size_t new_size = strlen(node->content) + strlen(content) + 1; // +1 for null terminator
        node->content = realloc(node->content, new_size);
        strcat(node->content, content);
        node->line_amount++;
    } else {
        macro_list *new_node = malloc(1000); // to change later from 1000
        if (new_node == NULL) {
            // Handle memory allocation failure
            return -1;
        }
        new_node->name = malloc(MAX_LINE_LENGTH); // +1 for null terminator
        strcpy(new_node->name, mcro_name);

        new_node->content = malloc(MAX_LINE_LENGTH); // +1 for null terminator
        strcpy(new_node->content, content);

        new_node->line = 0;
        new_node->line_amount = 0;
        new_node->next = NULL;

        // Update table
        if (*table == NULL) {
            *table = new_node;
        } else {
            macro_list *temp = *table;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = new_node;
        }
    }
    return 0;
}

int insertLine(char *file_name, int replace_line, char *content)
{
    FILE *fp, *temp;
    char temp_filename[FILENAME_MAX];
    char buffer[MAX_LINE_LENGTH];

    strcpy(temp_filename, "temp____");
    strcat(temp_filename, file_name);

    fp = fopen(file_name, "r");
    temp = fopen(temp_filename, "w");

    if(fp == NULL || temp == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    int keep_reading = 1;
    int current_line = 1;

    do
    {
        fgets(buffer, MAX_LINE_LENGTH, fp);
        if(current_line == replace_line) {
            fputs(content, temp);
            fputs(buffer, temp);
        }
        else
            fputs(buffer, temp);

        if(feof(fp))
            keep_reading = 0;


        current_line++;

    } while(keep_reading);

    fclose(fp);
    fclose(temp);

    remove(file_name);
    rename(temp_filename, file_name);

    return 0;
}

int search_command(char *token) {
    if (strcmp(token, "mov") == 0)
        return 0;
    if (strcmp(token, "cmp") == 0)
        return 1;
    if (strcmp(token, "add") == 0)
        return 2;
    if (strcmp(token, "sub") == 0)
        return 3;
    if (strcmp(token, "not") == 0)
        return 4;
    if (strcmp(token, "clr") == 0)
        return 5;
    if (strcmp(token, "lea") == 0)
        return 6;
    if (strcmp(token, "inc") == 0)
        return 7;
    if (strcmp(token, "dec") == 0)
        return 8;
    if (strcmp(token, "jmp") == 0)
        return 9;
    if (strcmp(token, "bne") == 0)
        return 10;
    if (strcmp(token, "red") == 0)
        return 11;
    if (strcmp(token, "prn") == 0)
        return 12;
    if (strcmp(token, "jsr") == 0)
        return 13;
    if (strcmp(token, "rts") == 0)
        return 14;
    if (strcmp(token, "hlt") == 0)
        return 15;

    return -1;
}

int check_operand(char *token) {

    if (token == NULL) {
        return -1;
    }

    if (token[0] == '#') {
        return 0;
    }

    if (strchr(token, '[') != NULL && strchr(token, ']') != NULL) {
        return 2;
    }

    if (token[0] == 'r' && token[1] >= '0' && token[1] <= '7') {
        return 3;
    }

    return 1; // label
}



char *to_binary (char *line) {
    char *binary_line = malloc(16);
    char *final = malloc(16 * 5);
    char *token;
    char *op1 = malloc(MAX_LINE_LENGTH);
    char *op2 = malloc(MAX_LINE_LENGTH);
    char *op = malloc(MAX_LINE_LENGTH);
    char *temp = malloc(MAX_LINE_LENGTH);
    int value;
    int command;
    int i, j, operands = 1;
    symbol_list *node = symbol_table;

    strcpy(temp, line);
    token = strtok(temp, " \t");
    command = search_command(token);

    strcpy(binary_line, "0000");
    //opcode
    token = decimalToBinary(command,4);
    strcat(binary_line, token);
    free(token);
    //source operand
    token = strtok(NULL, ", \t");
    if (token == NULL) {
        operands = 0;
        strcat(binary_line, "0000");
    }
    else
    {
        strcpy(op1, token);
        token = strtok(NULL, ", \t");
        if (token == NULL) {
            operands = 1;
            strcat(binary_line, "00");
            value = check_operand(op1);
            token = decimalToBinary(value, 2);
            strcat(binary_line, token);
            free(token);
        }
        else {
            operands = 2;
            strcpy(op2, token);
            value = check_operand(op1);
            token = decimalToBinary(value,2);
            strcat(binary_line, token);
            free(token);
            token = NULL;
            value = check_operand(op2);
            token = decimalToBinary(value,2);
            strcat(binary_line, token);
            free(token);
            token = NULL;
        }
    }
    //A,R,E
    strcat(binary_line, "00\n");
    strcpy(final, binary_line);

    //first word done, now second word
    if (operands == 0) {
        free(op1);
        op1 = NULL;
        free(op2);
        op2 = NULL;
        free(op);
        op = NULL;
        return final;
    }
    if (command == 14 || command == 15) { /*operands variable is bigger than 0*/
        printf("Error: hlt or rts with operands\n");
        free(op1);
        op1 = NULL;
        free(op2);
        op2 = NULL;
        free(op);
        op = NULL;
        free(final);
        return NULL;
    }

    if ((command == 4 || command == 5 || command >= 7) && operands > 1) {
        printf("Error: Too many operands for this command\n");
        free(op1);
        op1 = NULL;
        free(op2);
        op2 = NULL;
        free(op);
        op = NULL;
        free(final);
        return NULL;
    }

    /*dealing with 2 operand command errors*/
    if ((command <= 3 || command == 6) && operands <= 1) {
        printf("Error: Too few operands for this command\n");
        free(op1);
        op1 = NULL;
        free(op2);
        op2 = NULL;
        free(op);
        op = NULL;
        free(final);
        return NULL;
    }

    if (command <= 3 || command == 6) {
        if (check_operand(op1) > 3 || check_operand(op1) < 0
            || (command == 6 && check_operand(op1) != 1 && check_operand(op1) != 2)) {
            printf("Error: Invalid source operand for this command\n");
            free(op1);
            op1 = NULL;
            free(op2);
            op2 = NULL;
            free(op);
            op = NULL;
            free(final);
            return NULL;
        }
        if (check_operand(op2) > 3 || check_operand(op2) < 1) {
            if (command != 1 || check_operand(op2) < 0) {
                printf("Error: Invalid destination operand for this command\n");
                free(op1);
                op1 = NULL;
                free(op2);
                op2 = NULL;
                free(op);
                op = NULL;
                free(final);
                return NULL;
            }
        }
    }

    if (check_operand(op1) == 3 && operands == 2 && check_operand(op2) == 3) { //special
        strcpy(binary_line, "000000");
        value = op1[1] - '0';
        token = decimalToBinary(value,3);
        strcat(binary_line, token);
        free(token);
        value = op2[1] - '0';
        token = decimalToBinary(value,3);
        strcat(binary_line, token);
        free(token);
        strcat(binary_line, "00\n");
        strcat(final, binary_line);
    }
    else { //not special case
        value = check_operand(op1);
        strcpy(op, op1);
        if (operands == 2) {
            j = 0;
        }
        if (operands == 1) {
            j = 1;
        }
        for (j; j < 2; ++j) {
            strcpy(binary_line, "");
            switch (value) {
                case 0: //immediate
                    strcpy(temp, op);
                    token = strtok(temp, "#\n");
                    value = atoi(token);
                    if (value == 0) {
                        node = symbol_table;
                        while (node != NULL) {
                            if (strcmp(node->symbol, token) == 0 && strcmp(node->identifier, ".mdefine") == 0) {
                                break;
                            }
                            node = node->next;
                        }
                        if (node == NULL) {
                            printf("Error: definition not found\n");
                            return NULL;
                        }
                        value = node->value;
                    }
                    token = decimalToBinary(value,12);
                    if (token == NULL) {
                        printf("Error: decimalToBinary failed\n");
                        return NULL;
                    }
                    strcpy(binary_line, token);
                    free(token);
                    token = NULL;
                    strcat(binary_line, "00\n"); /*for ARE*/
                    strcat(final, binary_line);
                    break;
                case 1: /*label*/
                    node = symbol_table;
                    while (node != NULL) {
                        if (strcmp(node->symbol, op) == 0) {
                            break;
                        }
                        node = node->next;
                    }

                    if (node == NULL || node->value < 100) { /*label not found*/
                        strcpy(binary_line, op);
                    }
                    else {
                        value = node->value;
                        token = decimalToBinary(value,12);
                        for (i = 0; i < 12 - strlen(token); ++i) {
                            strcat(binary_line, "0");
                        }
                        strcat(binary_line, token);
                        free(token);

                        if (strcmp(node->identifier, ".external") == 0) {
                            strcat(binary_line, "01");
                        } else {
                            strcat(binary_line, "10");
                        }
                    }
                    strcat(binary_line, "\n");
                    strcat(final, binary_line);
                    break;
                case 2: /*index*/
                    strcpy(temp, op);
                    token = strtok(temp, "[");
                    node = symbol_table;
                    while (node != NULL) {
                        if (strcmp(node->symbol, token) == 0) {
                            break;
                        }
                        node = node->next;
                    }

                    if (node == NULL || node->value == -1) { //not found label or no finished value
                        strcpy(binary_line, token);
                        strcat(binary_line, "\n");
                        strcat(final, binary_line);
                    }
                    else {
                        if (strcmp(node->identifier, ".external") == 0) {
                            strcat(binary_line, "00000000000001\n");
                        }
                        else {
                            value = node->value;
                            token = decimalToBinary(value,12);
                            strcat(binary_line, token);
                            free(token);
                            strcat(binary_line, "10\n");
                        }
                        strcat(final, binary_line);
                    }
                    strcpy(binary_line, "");

                    //now index
                    strcpy(temp, op);
                    token = strtok(temp, "[");
                    token = strtok(NULL, "]");
                    value = atoi(token);
                    if (value == 0) {
                        node = symbol_table;
                        while (node != NULL) {
                            if (strcmp(node->symbol, token) == 0 && strcmp(node->identifier, ".mdefine") == 0) {
                                break;
                            }
                            node = node->next;
                        }
                        if (node == NULL) {
                            printf("Error: label not found\n");
                            return NULL;
                        }
                        value = node->value;
                    }
                    token = decimalToBinary(value,12);
                    strcat(binary_line, token);
                    free(token);
                    strcat(binary_line, "00\n");
                    strcat(final, binary_line);
                    break;
                case 3: /*register*/
                    strcpy(temp, op);
                    token = strtok(temp, "r");
                    value = atoi(token);
                    strcpy(token, "");
                    temp = decimalToBinary(value,3);
                    strcat(token, temp);
                    if (j == 0) { /*source register*/
                        strcat(binary_line, "000000");
                        strcat(binary_line, token);
                        strcat(binary_line, "00000\n");
                    } else { /*j==1 destination register*/
                        strcat(binary_line, "000000000");
                        strcat(binary_line, token);
                        strcat(binary_line, "00\n");
                    }
                    strcat(final, binary_line);
                    break;
                default:
                    break;
            }
            value = check_operand(op2);
            strcpy(op, op2);
        }

    }
    free(op1);
    op1 = NULL;
    free(op2);
    op2 = NULL;
    free(op);
    op = NULL;
    return final;
}

int ext_file_creator(char *file_name) {
    FILE *fp;
    FILE *temp;
    char *token = malloc(MAX_LINE_LENGTH);
    strcpy(token, file_name);
    strcat(token, ".am");
    fp = fopen(token, "r");
    if(fp == NULL)
    {
        printf("error message"); // make an error message later
        return 1;
    }
    int algo_counter = 1, IC = 100, first_register = 0;
    char *line = malloc(MAX_LINE_LENGTH);
    char *temp_line = malloc(MAX_LINE_LENGTH);
    symbol_list *node = symbol_table;

    strcpy(token, "");

    strcpy(token, file_name);
    token = strtok(token, ".");
    strcat(token, ".ext");
    temp = fopen(token, "w");

    if(temp == NULL)
    {
        printf("error message"); // make an error message later
        return 1;
    }
    strcpy(token, "");
    while(algo_counter != 0)
    {
        switch (algo_counter)
        {
            case 1:

                if(feof(fp))
                {
                    algo_counter = 0;
                    break;
                }

                fgets(line, MAX_LINE_LENGTH, fp);
                first_register = 0;

            case 2:
                strcpy(temp_line, line);
                token = strtok(temp_line, " \t\n");

                if(strstr(line, ".data") != NULL || strstr(line, ".string") != NULL || strstr(line, ".entry") != NULL
                   || strstr(line, ".extern") != NULL || strstr(line, ".define") != NULL || line[0] == ';' || token == NULL)
                {
                    algo_counter = 1;
                    break;
                }
                strcpy(temp_line, line);
            case 3:
                if(strstr(temp_line, ":") != NULL)
                {
                    token = strtok(temp_line, ":");
                    token = strtok(NULL, " \t\n");
                }
                else
                {
                    token = strtok(temp_line, " \t ,[] # \n");
                }
                while(token != NULL)
                {
                    node = symbol_table;
                    while(node != NULL)
                    {
                        if(strcmp(node->symbol, token) == 0 && strcmp(node->identifier, ".external") == 0)
                        {
                            fprintf(temp, "%s\t 0%d\n", node->symbol, IC);
                            break;
                        }
                        node = node->next;
                    }
                    token = strtok(NULL, " \t ,[] # \n");
                    if (token != NULL && token[0] == 'r' && token[1] >= '0' && token[1] <= '7')
                    {
                        if (first_register == 1)
                        {
                            IC--;
                        }
                        first_register = 1;
                    }

                    IC++;
                }
                algo_counter = 1;
                break;
            default:
                printf("error message"); // make an error message later
                return 1;
        }
    }
    fclose(temp);
    fclose(fp);
    free(token);
    free(line);
    return 0;
}

char* data_to_binary (char* line) {
    char binary_line[16];
    char* final = calloc(17, sizeof(char));
    char* token;
    char* temp = calloc(17, sizeof(char));
    int value;
    symbol_list* node;

    strcpy(final, "");
    strcpy(temp, line);
    token = strtok(temp, " \t , \n");

    if (token == NULL) {
        printf("Error: data is empty\n");
        free(final);
        final = NULL;
        free(temp);
        temp = NULL;
        return NULL;
    }
    while (token != NULL) {
        value = atoi(token);
        if (value == 0) {
            node = symbol_table;
            while (node != NULL) {
                if (strcmp(node->symbol, token) == 0 && strcmp(node->identifier, ".mdefine") == 0) {
                    value = node->value;
                    break;
                }
                node = node->next;
            }
            if (node == NULL) {
                printf("Error: data not found\n");
                free(final);
                free(token);
                free(temp);
                return NULL;
            }
        }
        token = decimalToBinary(value, 14);
        strcpy(binary_line, token);
        free(token);
        token = NULL;
        strcat(binary_line, "\n");
        final = realloc(final, strlen(final) + 16 * sizeof (char));
        if (final == NULL) {
            printf("Memory re-allocation failed\n");
            free(temp);
            return NULL;
        }
        strcat(final, binary_line);
        token = strtok(NULL, " \t , \n");
    }
    free(temp);
    return final;
}

/*
char* string_to_binary (char* line) {
    char binary_line[16];
    char* final = malloc(1000);
    char* token;
    char* temp = malloc(200);
    int i = 0;

    strcpy(final, "");

    strcpy(temp, line);
    token = strtok(temp, " \t , \n");
    if (token == NULL) {
        printf("Error: string is empty\n");
        free(final);
        free(temp);
        return NULL;
    }

    for (i = 3; i < strlen(token) - 3; i++) {
        strcpy(binary_line, "");
        temp = decimalToBinary(token[i], 14);
        strcat(binary_line, temp);
        free(temp);
        strcat(binary_line, "\n");
        strcat(final, binary_line);
    }
    strcat(final, "00000000000000\n");
    return final;
}*/

char* string_to_binary (char* line) {
    char binary_line[16];
    char* final = calloc(17, sizeof(char));
    char token[MAX_LINE_LENGTH];
    char* temp = malloc(MAX_LINE_LENGTH);
    int i = 0;

    strcpy(final, "");
    strcpy(temp, line);
    if (strtok(temp, " \t , \n") == NULL) {
        printf("Error: string is empty\n");
        free(final);
        final = NULL;
        free(temp);
        temp = NULL;
        return NULL;
    }
    strcpy(temp, line);
    strcpy(token, strtok(temp, " \t , \n"));

    for (i = 1; i < strlen(token) - 1; i++) {
        strcpy(binary_line, "");
        temp = decimalToBinary(token[i], 14);
        strcat(binary_line, temp);
        free(temp);
        temp = NULL;
        strcat(binary_line, "\n");
        strcat(final, binary_line);
        final = realloc(final, strlen(final) + 16 * sizeof (char));
        if (final == NULL) {
            printf("Memory re-allocation failed\n");
            return NULL;
        }
    }
    strcat(final, "00000000000000\n");
    return final;
}
