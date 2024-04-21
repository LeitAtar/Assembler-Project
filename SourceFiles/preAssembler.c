#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/tables.h"

extern macro_list *mcr_table;

int exe_pre_assembler(char *file_name) {
    FILE *fp, *fp2;
    char *token;
    int line_counter = 0, is_mcr = 0, error_flag = 0;
    char *mcro_name = calloc(MAX_LINE_LENGTH, sizeof(char)),
            *line = calloc(MAX_LINE_LENGTH, sizeof(char)),
            *temp_line = calloc(MAX_LINE_LENGTH, sizeof(char));
    mcr_table = NULL;
    macro_list *node = NULL;
    fp = fopen(file_name, "r");
    strcpy(temp_line, file_name);
    token = strtok(temp_line, ".");
    strcat(token, ".am");
    fp2 = fopen(token, "w");

    if (fp == NULL || fp2 == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    int algo_counter = 1;
    while(algo_counter != 0)
    {
        switch (algo_counter) {
            case 1:
                if (feof(fp)) {
                    algo_counter = 9;
                    break;
                }
                fgets(line, MAX_LINE_LENGTH, fp);
                strcpy(temp_line, line);
                line_counter++;
            case 2:
                token = strtok(temp_line," \t");
                node = is_in_macro_table(mcr_table,token);
                if(node != NULL) {

                    fprintf(fp2, "%s", node->content);
                    line_counter += node->line_amount;

                    algo_counter = 1;
                    break;
                }
            case 3:
                if(strcmp(token, "mcr") != 0)
                {
                    algo_counter = 6;
                    break;
                }
            case 4:
                is_mcr = 1;
            case 5:
                strcpy(mcro_name, strtok(NULL, " \t"));
                if (is_in_macro_table(mcr_table, mcro_name) != NULL) {
                    error_flag = 1;
                    printf("macro already exists\n");
                }
                else {
                    insert_to_macro_table(&mcr_table, mcro_name, "");
                }
            case 6:
                strcpy(temp_line, line);
                token = strtok(temp_line, " \t");
                if(strcmp(token, "mcr") == 0)
                {
                    algo_counter = 1;
                    break;
                }
                else if(is_mcr == 1 && strstr(line, "endmcr") == NULL)
                {
                    insert_to_macro_table(&mcr_table, mcro_name, line);
                    algo_counter = 1;
                    break;
                }
                else if (strcmp(token,"endmcr\n") == 0) {
                    algo_counter = 7;
                    break;
                }
                else
                {
                    fprintf(fp2, "%s", line);
                    algo_counter = 1;
                    break;
                }
            case 7:
                if(strstr(line, "endmcr") == NULL) {
                    fprintf(fp2, "%s", line);
                    algo_counter = 1;
                    break;
                }
            case 8:
                is_mcr = 0;
                algo_counter = 1;
                break;
            case 9:
                algo_counter = 0;
                break;
            default:
                printf("error message"); /*insert error message here*/
        }
    }
    fclose(fp);
    fclose(fp2);
    free(mcro_name);
    mcro_name = NULL;
    free(line);
    line = NULL;
    free(temp_line);
    temp_line = NULL;
    free(node);
    node = NULL;

    if (error_flag == 1)
    {
        return 1;
    }
    return 0;
}

