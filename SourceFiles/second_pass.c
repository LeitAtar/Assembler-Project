#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/tables.h"
#include "../HeaderFiles/utilities.h"
#include "../HeaderFiles/second_pass.h"
#include "../HeaderFiles/convert_to_base_four.h"

extern symbol_list *symbol_table;

int exe_second_pass(char *file_name, int IC, int DC, int prev_error)
{
    symbol_list *node;
    FILE *fp, *machine;
    char *after_first_pass = calloc(MAX_LINE_LENGTH, sizeof (char));
    char *line = calloc(MAX_LINE_LENGTH, sizeof (char));
    char *temp_line;
    char *token = calloc(MAX_LINE_LENGTH, sizeof (char));
    int line_counter = 0, error_flag = 0, external_flag = 0, entry_flag = 0;

    printf("Starting second pass on file: %s\n", file_name);
    strcpy(after_first_pass, "temp____");
    fp = fopen(after_first_pass, "r");
    strcpy(token, file_name);
    token = strtok(token, ".");
    strcat(token, ".ob");
    machine = fopen(token, "w");

    if(fp == NULL || machine == NULL)
    {
        printf("Failed to open file (second pass)\n");
        free(token);
        token = NULL;
        free(line);
        line = NULL;
        return 1;
    }
    node = symbol_table;
    while(node != NULL)
    {
        if(strcmp(node->identifier, ".external") == 0)
        {
            external_flag = 1;
        }
        if(node->is_entry == 1)
        {
            entry_flag = 1;
        }
        node = node->next;
    }
    while(!feof(fp))
    {
        fgets(line, MAX_LINE_LENGTH, fp);
        if (feof(fp))
        {
            continue;
        }
        temp_line = calloc(MAX_LINE_LENGTH, 1);
        line_counter++;
        strcpy(temp_line, line);
        node = symbol_table;
        if(temp_line[0] == '1' || temp_line[0] == '0')
        {
            fprintf(machine, "%s", line);
            free(temp_line);
            temp_line = NULL;
            continue;
        }
        node = symbol_table;
        while(node != NULL)
        {
            strcpy(token, node->symbol);
            strcat(token, "\n");
            if(strcmp(token, temp_line) == 0)
            {
                break;
            }
            node = node->next;
        }
        if(node == NULL)
        {
            error_flag = 1;
            strcpy(token, temp_line);
            token = strtok(token, "\n");
            printf("Error: can't find symbol: %s | line:%d\n", token, line_counter);
        }
        else
        {
            if (strcmp(node->identifier, ".external") == 0)
            {
                strcpy(temp_line, "00000000000001\n");
            }
            else
            {
                free(temp_line);
                temp_line = NULL;
                temp_line = decimal_to_binary(node->value, 12);
                temp_line = realloc(temp_line, strlen (temp_line) + 4);
                if (temp_line == NULL)
                {
                    error_flag = 1;
                    printf("Failed reallocation (second pass) | line:%d\n", line_counter);
                }
                strcat(temp_line, "10\n");
            }
            if(temp_line == NULL)
            {
                error_flag = 1;
                printf("Failed to convert to binary | line:%d\n", line_counter);
            }
            else
            {
                fprintf(machine, "%s", temp_line);
            }
        }
        free(temp_line);
        temp_line = NULL;
    }


    fclose(fp);
    fclose(machine);
    remove("temp____");
    strcpy(token, file_name);
    token = strtok(token, ".");
    strcat(token, ".ob");
    encrypt(token);

    /*add numbers and ic dc*/
    machine = fopen("temp____", "w");
    fp = fopen(token, "r");
    if (fp == NULL || machine == NULL)
    {
        printf("Failed to open file (second pass)\n");
        free(token);
        token = NULL;
        free(line);
        line = NULL;
        free(temp_line);
        temp_line = NULL;
        return 1;
    }
    temp_line = calloc(MAX_LINE_LENGTH, sizeof (char));
    strcpy(token, "\t");
    sprintf(temp_line, "%d", IC);
    strcat(token, temp_line);
    strcat(token, "\t");
    sprintf(temp_line, "%d", DC);
    strcat(token, temp_line);
    strcat(token, "\n");
    fprintf(machine, "%s", token);

    line_counter = 0;
    while (!feof(fp)) {
        strcpy(token, "0");
        sprintf(temp_line, "%d", IC_INITIAL + line_counter);
        strcat(token, temp_line);
        strcat(token, "\t");
        fgets(line, MAX_LINE_LENGTH, fp);
        if (feof(fp)) {
            continue;
        }
        strcat(token, line);
        fprintf(machine, "%s", token);
        line_counter++;
    }

    fclose(fp);
    fclose(machine);
    strcpy(token, file_name);
    token = strtok(token, ".");
    strcat(token, ".ob");
    remove(token);
    rename("temp____", token);


    if (error_flag == 1)
    {
        remove(token);
        free(token);
        token = NULL;
        free(line);
        line = NULL;
        free(temp_line);
        temp_line = NULL;
        /*maybe delete file*/
        printf("Failed second pass on file: %s\n", file_name);
        return 1;
    }
    if(external_flag == 1 && prev_error == 0)
    {
        ext_file_creator(file_name);
    }
    if(entry_flag == 1 && prev_error == 0)
    {
        strcpy(token, file_name);
        token = strtok(token, ".");
        strcat(token, ".ent");
        fp = fopen(token, "w");

        node = symbol_table;
        while (node != NULL)
        {
            if (node->is_entry == 1)
            {
                fprintf(fp, "%s\t%d\n", node->symbol, node->value);
            }
            node = node->next;
        }
        fclose(fp);
    }

    free(token);
    token = NULL;
    free(line);
    line = NULL;
    free(temp_line);
    temp_line = NULL;
    free(after_first_pass);
    after_first_pass = NULL;

    printf("Finished second pass successfully on file: %s\n", file_name);
    return 0;
}