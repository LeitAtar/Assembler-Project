#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/Globals.h"
#include "../HeaderFiles/tables.h"
#include "../HeaderFiles/secondPass.h"
#include "../HeaderFiles/utilities.h"


/*
int second_pass(char *binary_name, char *assembly_name)
{
    FILE *binary, *assembly;
    binary = fopen(binary_name, "r");
    assembly = fopen(assembly_name, "r");

    if(binary == NULL || assembly == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    char *token = malloc(MAX_LINE_LENGTH);
    int algo_counter = 1, L = 0;
    int IC, is_error = 0, i = 0;
    char *line = malloc(MAX_LINE_LENGTH);
    char *temp_line = malloc(MAX_LINE_LENGTH);
    symbol_list *node = NULL;

    while(algo_counter != 0)
    {
        switch(algo_counter)
        {
            case 1:
                IC = 0;
            case 2:
                fgets(line, MAX_LINE_LENGTH, assembly);
                 L = findL(line);
                if (feof(assembly)) {
                    algo_counter = 9;
                    break;
                }
            case 3:
                strcpy(temp_line, line);
                token = strtok(temp_line, " \t");
                if(strstr(token, ":") != NULL)
                {
                    line = strtok(NULL, "\n");
                    strcat(line,"\n");
                }
            case 4:
                strcpy(temp_line, line);
                token = strtok(temp_line, " \t");
                if(strcmp(line, ".extern") == 0 || strcmp(line, ".data") == 0 || strcmp(line, ".string") == 0)
                {
                    if(strcasecmp(line, ".extern") != 0 || strcasecmp(line, ".data") != 0
                    || strcasecmp(line, ".string") != 0)
                    {
                         is_error = 1;
                        printf("error message"); // make an error message later
                    }
                    else
                    {
                        algo_counter = 2;
                        break;
                    }
                }
            case 5:
                if(strcmp(line, ".entry") != 0)
                {
                    if(strcasecmp(line, ".entry") != 0)
                    {
                        is_error = 1;
                        printf("error message"); // make an error message later
                    }
                }
                else
                {
                    algo_counter = 7;
                    break;
                }
            case 6:
                strcpy(temp_line, line);
                token = strtok(temp_line, " \t");
                token = strtok(NULL, ", \n\t");
                insertToSymbolTable(&symbol_table, token, 0, ".entry");
                algo_counter = 2;
                break;
            case 7:
                for(i = 0; i < IC; i++)
                {
                    fgets(temp_line, MAX_LINE_LENGTH, binary);
                }
                if(temp_line[0] != '1' && temp_line[0] != '0') */
/*is a label*//*

                {
                    node = symbol_table;
                    while(node != NULL)
                    {
                        if(strcmp(node->symbol, temp_line) == 0)
                        {
                            break;
                        }
                        node = node->next;
                    }
                    if(node == NULL)
                    {
                        is_error = 1;
                        printf("error message"); // make an error message later
                    }
                    fclose(binary);
                    deleteLine(binary_name, IC);
                    token = decimalToBinary(node->value);
                    insertLine(binary_name, IC, token);
                    fopen(binary_name, "r");
                }
            case 8:
                IC += L;
                algo_counter = 2;
                break;
            case 9:
                if(is_error == 1)
                {
                    algo_counter = 0;
                    break;
                }
            case 10:
                // need to finish this later
            default:
                is_error = 1;
                printf("something is wrong buddy\n");
        }
    }
    fclose(binary);
    fclose(assembly);
    return 1;
}
*/


int second_pass(char *file_name)
{
    symbol_list *node = symbol_table;
    FILE *fp;
    fp = fopen(file_name, "r");
    char *line = malloc(MAX_LINE_LENGTH);
    char *temp_line = malloc(MAX_LINE_LENGTH);
    char *token = malloc(MAX_LINE_LENGTH);
    int line_counter = 0 ,i = 0, error_flag = 0, external_flag = 0, entry_flag = 0;
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
            printf("error message"); // make an error message later
        }
        else
        {
            fclose(fp);
            deleteLine(file_name, line_counter);
            temp_line = calloc(14, 1);
            for (i = 0; i < 14 - strlen(decimalToBinary(node->value)); i++) {
                temp_line[i] = '0';
            }
            strcat(temp_line, decimalToBinary(node->value));
            strcat(temp_line, "\n");
            if(temp_line == NULL)
            {
                error_flag = 1;
                printf("error message"); // make an error message later
            }
            else
            {
                insertLine(file_name, line_counter, temp_line);
                fopen(file_name, "r");
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
        free(temp_line);
        free(node);
        free(file_name);
        //maybe delete file
        return 1;
    }

    if(external_flag == 1)
    {
        token = malloc(MAX_LINE_LENGTH);
        strcpy(token, file_name);
        token = strtok(token, ".");
        strcat(token, ".ext");
        FILE *ext = fopen(token, "w");
        node = symbol_table;
        int DC = 100;
        fp = fopen(file_name, "r");
        temp_line = malloc(MAX_LINE_LENGTH);
        while(!feof(fp))
        {
            fgets(line, MAX_LINE_LENGTH, fp);
            strcpy(temp_line, line);
            token = strtok(temp_line, ", \t");
            int is_quote = 0, second_r = 0;
            i = 0;

            while(token != NULL)
            {
                if(strstr(token, "[") != NULL &&  strstr(token, "]") != NULL)
                {
                    DC += 2;
                }
                else if(strcmp(token, ".data") == 0 || strcmp(token, ".entry") == 0 || strcmp(token, ".extern") == 0)
                {
                    DC; // we dont change the L value
                }
                else if(strcmp(token, ".string") == 0)
                {
                    is_quote = 1;
                }
                else if(is_quote)
                {
                    for(i = 0; i <= strlen(token) - 6; i++)
                    {
                        DC++;
                    }
                    is_quote = 0;
                }
                else if(token[0] == 'r' && second_r == 0 && token[1] >= '0' && token[1] <= '7')
                {
                    second_r = 1;
                    DC++;
                }
                else if(second_r == 1 && token[0] == 'r' && token[1] >= '0' && token[1] <= '7')
                {
                    //TODO: check if the number of register is legal
                    second_r = 0;
                }
                else
                {
                    DC++;
                    while (node != NULL)
                    {
                        if(strcmp(node->symbol, token) == 0 && strcmp(node->identifier, ".extern") == 0)
                        {
                            fprintf(ext, "%s\t 0%d\n", node->symbol, DC);
                            break;
                        }
                        node = node->next;
                    }
                }

                token = strtok(NULL, ", \t\n");
            }
        }
        fclose(ext);
    }

    if(entry_flag == 1)
    {
        token = malloc(MAX_LINE_LENGTH);
        strcpy(token, file_name);
        token = strtok(token, ".");
        strcat(token, ".ent");
        FILE *ent = fopen(token, "w");
        node = symbol_table;
        int DC = 100;
        fp = fopen(file_name, "r");
        temp_line = malloc(MAX_LINE_LENGTH);
        while(!feof(fp))
        {
            fgets(line, MAX_LINE_LENGTH, fp);
            strcpy(temp_line, line);
            token = strtok(temp_line, ", \t");
            int is_quote = 0, second_r = 0;
            i = 0;

            while(token != NULL)
            {
                if(strstr(token, "[") != NULL &&  strstr(token, "]") != NULL)
                {
                    DC += 2;
                }
                else if(strcmp(token, ".data") == 0 || strcmp(token, ".entry") == 0 || strcmp(token, ".extern") == 0)
                {
                    DC; // we dont change the L value
                }
                else if(strcmp(token, ".string") == 0)
                {
                    is_quote = 1;
                }
                else if(is_quote)
                {
                    for(i = 0; i <= strlen(token) - 6; i++)
                    {
                        DC++;
                    }
                    is_quote = 0;
                }
                else if(token[0] == 'r' && second_r == 0 && token[1] >= '0' && token[1] <= '7')
                {
                    second_r = 1;
                    DC++;
                }
                else if(second_r == 1 && token[0] == 'r' && token[1] >= '0' && token[1] <= '7')
                {
                    //TODO: check if the number of register is legal
                    second_r = 0;
                }
                else
                {
                    DC++;
                    while (node != NULL)
                    {
                        if(strcmp(node->symbol, token) == 0 && strcmp(node->identifier, ".entry") == 0)
                        {
                            fprintf(ent, "%s\t 0%d\n", node->symbol, DC);
                            break;
                        }
                        node = node->next;
                    }
                }

                token = strtok(NULL, ", \t\n");
            }
        }
        fclose(ent);
    }

    token = malloc(MAX_LINE_LENGTH);
    token = strtok(file_name, ".");
    strcat(token, ".ob");
    rename(file_name, token);
    free(token);
    free(line);
    free(temp_line);
    free(node);
    free(file_name);
    symbol_table = NULL;

    return 0;
}