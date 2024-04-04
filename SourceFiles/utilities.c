#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/Globals.h"
#include "../HeaderFiles/tables.h"
#include "../HeaderFiles/secondPass.h"

char* decimalToBinary(int num) {
    // Array to store binary number
    char *binaryLine = calloc(14,1);
    char *reverse_binaryLine;
    int j = 0;
    int is_negative = (num < 0) ? 1 : 0;
    num = abs(num);

    // Counter for binary array
    int i = 0;
    while (num > 0) {
        // Store remainder in binary array
        binaryLine[i] = num % 2 + '0';
        num = num / 2;
        i++;
    }
    if ((binaryLine = realloc(binaryLine, i + 1)) == NULL) {
        printf("Memory reallocation failed\n");
        return NULL;
    }

    if (is_negative) {
        // Two's complement
        for (j = 0; j < i; j++) {
            binaryLine[j] = (binaryLine[j] == '0') ? '1' : '0';
        }
        // Add 1 to the binary number
        for (j = 0; j < 16; j++) {
            if (binaryLine[j] == '1') {
                binaryLine[j] = '0';
            } else {
                binaryLine[j] = '1';
                break;
            }
        }
    }
    char *ones = calloc(i + 1,1);

    reverse_binaryLine = calloc(i + 1,1);
    int k = 0;
    // Print binary array in reverse order
    for (j = i - 1; j >= 0; j--) {
        //printf("%d", binaryNum[j]);
        reverse_binaryLine[k] = binaryLine[j];
        k++;
    }
    reverse_binaryLine[k] = '\0';

    if(is_negative == 1) {
        for (j = 0; j < 14 - strlen(reverse_binaryLine); j++) {
            ones[j] = '1';
        }

    }
    strcat(ones, reverse_binaryLine);
    free(binaryLine);
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
    //remove(file_name);
    //rename(temp_filename, file_name);

    if (rename(temp_filename, file_name) != 0) {
        printf("Error renaming file.\n");
        return 1;
    }
    //rename(temp_filename, "harta.txt");

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

const char *check_operand(char *strtok) {
    return NULL;
}
