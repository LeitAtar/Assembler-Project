#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/tables.h"
#include "../HeaderFiles/secondPass.h"
#include "../HeaderFiles/utilities.h"

int exe_pre_assembler(char *file_name) {
    FILE *fp, *fp2;
    char *token = malloc(MAX_LINE_LENGTH);
    int line_counter = 0, is_mcr = 0, error_flag = 0, i;
    char *mcro_name = malloc(MAX_LINE_LENGTH), *line = malloc(MAX_LINE_LENGTH), *temp_line = malloc(MAX_LINE_LENGTH);
    macro_list *table = NULL;
    macro_list *node = NULL;

    fp = fopen(file_name, "r");
    fp2 = fopen("___temp", "w");
    int algo_counter = 1;
    while(algo_counter != 0)
    {
        switch (algo_counter) {
            case 1:
                fgets(line, MAX_LINE_LENGTH, fp);
                strcpy(temp_line, line);
                line_counter++;
                if (feof(fp)) {
                    algo_counter = 9;
                    break;
                }
            case 2:
                token = strtok(temp_line," \t");
                node = isInMacroTable(table,token);
                if(node != NULL) {
                    fclose(fp);
                    deleteLine(file_name, line_counter--);
                    insertLine(file_name, line_counter+1, node->content);
                    line_counter += node->line_amount;
                    fopen(file_name, "r");
                    for(i = 0; i < line_counter; i++)
                    {
                        fgets(line, MAX_LINE_LENGTH, fp);
                    }
                    algo_counter = 1;
                    break;
                }
            case 3:
                //token = strtok(temp_line," \t");
                if(strcmp(token, "mcr") != 0)
                {
                    algo_counter = 6;
                    break;
                }
            case 4:
                is_mcr = 1;
            case 5:
                //token = strtok(temp_line, " \t");
                strcpy(mcro_name, strtok(NULL, " \t"));

                if (isInMacroTable(table, mcro_name) != NULL) {
                    error_flag = 1;
                    printf("macro already exists\n");
                }
                else {
                    insertToMacroTable(&table, mcro_name, "");
                }
            case 6:
                strcpy(temp_line, line);
                token = strtok(temp_line, " \t");
                if(strcmp(token, "mcr") == 0)
                {
                    fclose(fp);
                    deleteLine(file_name, line_counter--);
                    fopen(file_name, "r");
                    for(i = 0; i < line_counter; i++)
                    {
                        fgets(line, MAX_LINE_LENGTH, fp);
                    }
                    algo_counter = 1;
                    break;
                }
                else if(is_mcr == 1 && strstr(line, "endmcr") == NULL)
                {
                    fclose(fp);
                    insertToMacroTable(&table, mcro_name, line);
                    deleteLine(file_name, line_counter--);
                    fopen(file_name, "r");
                    for(i = 0; i < line_counter; i++)
                    {
                        fgets(line, MAX_LINE_LENGTH, fp);
                    }
                    algo_counter = 1;
                    break;
                }
                else if (strcmp(token,"endmcr\n") == 0) {
                    algo_counter = 7;
                    break;
                }
                else
                {
                    algo_counter = 1;
                    break;
                }
            case 7:
                if(strstr(line, "endmcr") != NULL) {
                    fclose(fp);
                    deleteLine(file_name, line_counter--);
                    fopen(file_name, "r");
                    for(i = 0; i < line_counter; i++)
                    {
                        fgets(line, MAX_LINE_LENGTH, fp);
                    }
                }
                else
                {
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
    free(token);
    free(mcro_name);
    free(line);
    free(temp_line);
    free(node);

    if (error_flag == 1)
    {
        return 1;
    }
    return 0;
}

int main() {
    pre_assemble("harta2.txt");

    return 0;
}

