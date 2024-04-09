#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/Globals.h"
#include "../HeaderFiles/tables.h"
#include "../HeaderFiles/secondPass.h"
#include "../HeaderFiles/utilities.h"

int exe_second_pass(char *file_name)
{
    symbol_list *node = symbol_table;
    FILE *fp;
    char *after_first_pass = malloc(MAX_LINE_LENGTH);
    strcpy(after_first_pass, "temp____");
    char *line = malloc(MAX_LINE_LENGTH);
    char *temp_line = malloc(MAX_LINE_LENGTH);
    char *token = malloc(MAX_LINE_LENGTH);
    int line_counter = 0 ,i = 0, error_flag = 0, external_flag = 0, entry_flag = 0;
    fp = fopen(after_first_pass, "r");

    if(fp == NULL)
    {
        printf("error message\n"); // make an error message later
        return 1;
    }
    while(node != NULL)
    {
        if(strcmp(node->identifier, ".extern") == 0)
        {
            external_flag = 1;
        }
        if(strcmp(node->identifier, ".entry") == 0)
        {
            entry_flag = 1;
        }
        node = node->next;
    }
    while(!feof(fp))
    {
        fgets(line, MAX_LINE_LENGTH, fp);
        line_counter++;
        strcpy(temp_line, line);
        node = symbol_table;
        if(temp_line[0] == '1' || temp_line[0] == '0')
        {
            continue;
        }
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
            printf("error message\n"); // make an error message later
        }
        else
        {
            fclose(fp);
            deleteLine(after_first_pass, line_counter);
            temp_line = calloc(14, 1);
            for (i = 0; i < 14 - strlen(decimalToBinaryOLD(node->value)); i++) {
                temp_line[i] = '0';
            }
            strcat(temp_line, decimalToBinaryOLD(node->value));
            strcat(temp_line, "\n");
            if(temp_line == NULL)
            {
                error_flag = 1;
                printf("error message\n"); // make an error message later
            }
            else
            {
                insertLine(after_first_pass, line_counter, temp_line);
                fopen(after_first_pass, "r");
                for (i = 0; i < line_counter; ++i) {
                    fgets(line, MAX_LINE_LENGTH, fp);
                }
            }
        }
    }

    fclose(fp);
    if (error_flag == 1)
    {
        free(token);
        free(line);
        //free(temp_line);
        free(node);
        free(file_name);
        //maybe delete file
        return 1;
    }
    if(external_flag == 1)
    {
        file_creator_with_identifier(file_name, ".external");
    }
    if(entry_flag == 1)
    {
        file_creator_with_identifier(file_name, ".entry");
    }


    strcpy(token, file_name);
    token = strtok(token, ".");
    strcat(token, ".ob");
    rename(after_first_pass, token);
    free(token);
    free(line);
    //free(temp_line);
    free(node);
    symbol_table = NULL;

    return 0;
}