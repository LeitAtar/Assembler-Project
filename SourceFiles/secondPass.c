#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/Globals.h"
#include "../HeaderFiles/tables.h"
#include "../HeaderFiles/firstPass.h"
#include "../HeaderFiles/utilities.h"
#include "../HeaderFiles/secondPass.h"
#include "../HeaderFiles/preAssembler.h"
#include "../HeaderFiles/convertToBaseFour.h"

int exe_second_pass(char *file_name, int IC, int DC) //symbol_list **symbol_table
{
    symbol_list *node = symbol_table;
    FILE *fp, *machine;
    char *after_first_pass = malloc(MAX_LINE_LENGTH);
    strcpy(after_first_pass, "temp____");
    char *line = malloc(MAX_LINE_LENGTH);
    char *temp_line = malloc(MAX_LINE_LENGTH);
    char *token = malloc(MAX_LINE_LENGTH);
    int line_counter = 0 ,i = 0, error_flag = 0, external_flag = 0, entry_flag = 0;
    fp = fopen(after_first_pass, "r");
    strcpy(token, file_name);
    strcat(token, ".ob");
    machine = fopen(token, "w");

    if(fp == NULL || machine == NULL)
    {
        printf("failed to open file\n"); // make an error message later
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
        line_counter++;
        strcpy(temp_line, line);
        node = symbol_table;
        if(temp_line[0] == '1' || temp_line[0] == '0')
        {
            fprintf(machine, "%s", line);
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
            printf("error message\n"); // make an error message later
        }
        else
        {

            if (strcmp(node->identifier, ".external") == 0)
            {
                strcpy(temp_line, "00000000000001\n");
            }
            else
            {
                temp_line = decimalToBinary(node->value, 12);
                strcat(temp_line, "10\n");
            }
            if(temp_line == NULL)
            {
                error_flag = 1;
                printf("error message\n"); // make an error message later
            }
            else
            {
                fprintf(machine, "%s", temp_line);
            }
        }
    }

    fclose(fp);
    fclose(machine);
    remove("temp____");
    strcpy(token, file_name);
    strcat(token, ".ob");
    encrypt(token);

    /*add numbers and ic dc*/
    machine = fopen("temp____", "w");
    fp = fopen(token, "r");
    if (fp == NULL || machine == NULL)
    {
        printf("failed to open file\n"); // make an error message later
        return 1;
    }
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
        sprintf(temp_line, "%d", 100 + line_counter);
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
    strcat(token, ".ob");
    remove(token);
    rename("temp____", token);


    if (error_flag == 1)
    {
        free(token);
        free(line);
        //free(temp_line);
        free(node);
        free(file_name);
        //maybe delete file
        printf("failed second assembly pass\n");
        return 1;
    }
    if(external_flag == 1)
    {
        ext_file_creator(file_name);
    }
    if(entry_flag == 1)
    {
        strcpy(token, file_name);
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
    free(line);
    free(temp_line);
    free(node);
    symbol_table = NULL;


    return 0;
}