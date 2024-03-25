#include "../HeaderFiles/preAssembler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/Globals.h"
#include "../HeaderFiles/tables.h"

int deleteLine(char *file_name, int delete_line)
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
        if(feof(fp))
            keep_reading = 0;
        else if(current_line != delete_line)
            fputs(buffer, temp);

        current_line++;

    } while(keep_reading);

    fclose(fp);
    fclose(temp);

    remove(file_name);
    rename(temp_filename, file_name);

    return 0;
}
macro_list* isInMacroTable(macro_list *table, char *name)
{
    macro_list *node = table;
    while(node != NULL)
    {
        if(strcmp(table->name, name) == 0)
            return node;
    }
    return NULL;
}

int insertToMacroTable(macro_list *table, char *mcro_name, char *content) //inserts content under the macro name
{
    macro_list *node = isInMacroTable(table,mcro_name);
    if(node != NULL) /* Only adding a line to content */
    {
        realloc(content,sizeof(content) + sizeof(node->content));
        strcat(node->content, content);
        node->line_amount++;
    }
    else /* Creating a new node */
    {
        macro_list *new_node = malloc(sizeof(macro_list));
        new_node->name = mcro_name;
        new_node->content = content;
        new_node->line = 0;
        new_node->line_amount = 0;
        new_node->next = NULL;

        node = table;
        while(node->next != NULL)
        {
            node = node->next;
        }
        node->next = new_node;

    }
    return 0;
}
insertLine(char *file_name, int replace_line, char *content)
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
        if(feof(fp))
            keep_reading = 0;
        else if(current_line == replace_line) {
            fputs(content, temp);
            fputs(buffer, temp);
        }
        else
            fputs(buffer, temp);

        current_line++;

    } while(keep_reading);

    fclose(fp);
    fclose(temp);

    remove(file_name);
    rename(temp_filename, file_name);

    return 0;
}


int pre_assemble(char *file_name) {
    FILE *fp, *fp2;
    char *token;
    int line_counter = 0, is_mcr = 0, error_flag = 0;
    char *mcro_name, *line = malloc(MAX_LINE_LENGTH);
    macro_list *table = NULL;
    macro_list *node = NULL;

    fp = fopen(file_name, "r");
    fp = fopen("___temp", "w");
    int algo_counter = 1;
    while(algo_counter != 0)
    {
        switch (algo_counter) {
            case 1:
                fgets(line, MAX_LINE_LENGTH, fp);
                line_counter++;
                if (feof(fp)) {
                    algo_counter = 9;
                    break;
                }
            case 2:
                token = strtok(line," \t");
                node = isInMacroTable(table,token);
                if(node != NULL) {
                    deleteLine(file_name, line_counter--);
                    insertLine(file_name, line_counter+1, node->content);
                    line_counter += node->line_amount;
                    algo_counter = 1;
                    break;
                }
            case 3:
                token = strtok(line," \t");
                if(strcmp(token, "mcr") != 0)
                {
                    algo_counter = 6;
                    break;
                }
            case 4:
                is_mcr = 1;
            case 5:
                token = strtok(line, " \t");
                mcro_name = strtok(NULL, " \t");

                if (isInMacroTable(table, mcro_name) != NULL) {
                    error_flag = 1;
                    printf("macro already exists\n");
                }
                else {
                    insertToMacroTable(table, mcro_name, "");
                }
            case 6:
                fgets(line,MAX_LINE_LENGTH,fp);
                line_counter++;
                if(feof(fp)) {
                    algo_counter = 9;
                    break;
                }
                if(is_mcr == 1 && strstr(line, "endmcr") != 0)
                {
                    insertToMacroTable(table, mcro_name, line);
                    deleteLine(file_name, line_counter--);
                }
                else
                {
                    algo_counter = 1;
                    break;
                }
            case 7:
                if(strstr(line, "endmcr") == 0)
                    deleteLine(file_name, line_counter--);
                else
                {
                    algo_counter = 6;
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
    return 0;
}

int main() {
    pre_assemble("something.txt");
    return 0;
}
