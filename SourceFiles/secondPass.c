#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/Globals.h"
#include "../HeaderFiles/tables.h"
#include "../HeaderFiles/secondPass.h"
#include "../HeaderFiles/utilities.h"


int second_pass(char *filename)
{
    FILE *fp;
    fp = fopen(filename, "r");

    if(fp == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    label_address *table = NULL, *node = NULL;
    char *token = malloc(MAX_LINE_LENGTH);
    int algo_counter = 1, L = 0;
    int IC, is_error = 0;
    char *line = malloc(MAX_LINE_LENGTH);
    char *temp_line = malloc(MAX_LINE_LENGTH);

    while(algo_counter != 0)
    {
        switch(algo_counter)
        {
            case 1:
                IC = 0;
            case 2:
                fgets(line, MAX_LINE_LENGTH, fp);
                 L = findL(line);
                if (feof(fp)) {
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
                insertToSymbolTable(&table, token, 0, ".entry");
                algo_counter = 2;
                break;
            case 7:

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
}