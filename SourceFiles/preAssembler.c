#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/Globals.h"
#include "../HeaderFiles/tables.h"

int deleteLine(char *file_name, int delete_line)
{
    FILE *fp, *temp;
    char *temp_filename = malloc(1000);
    char buffer[1000];

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
        fgets(buffer, 1000, fp);

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

/*int insertToMacroTable(macro_list *table, char *mcro_name, char *content) //inserts content under the macro name
{
    macro_list *node = isInMacroTable(table,mcro_name);
    if(node != NULL) *//* Only adding a line to content *//*
    {
        realloc(content,sizeof(content) + sizeof(node->content));
        strcat(node->content, content);
        node->line_amount++;
    }
    else *//* Creating a new node *//*
    {
        macro_list *new_node = malloc(sizeof(macro_list));
        new_node->name = mcro_name;
        new_node->content = content;
        new_node->line = 0;
        new_node->line_amount = 0;
        new_node->next = NULL;

        node = table;

        if(node == NULL)
        {
            table = new_node;
            return 0;
        }

        while(node->next != NULL)
        {
            node = node->next;
        }
        node->next = new_node;

    }
    return 0;
}*/


int insertToMacroTable(macro_list **table, char *mcro_name, char *content) {
    macro_list *node = isInMacroTable(*table, mcro_name);
    if (node != NULL) {
        // Reallocate memory for combined content
        size_t new_size = strlen(node->content) + strlen(content) + 1; // +1 for null terminator
        node->content = realloc(node->content, new_size);
        strcat(node->content, content);
        node->line_amount++;
    } else {
        macro_list *new_node = malloc(1000);
        if (new_node == NULL) {
            // Handle memory allocation failure
            return -1;
        }
        new_node->name = malloc(1000); // +1 for null terminator
        strcpy(new_node->name, mcro_name);

        new_node->content = malloc(1000); // +1 for null terminator
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

insertLine(char *file_name, int replace_line, char *content)
{
    FILE *fp, *temp;
    char temp_filename[FILENAME_MAX];
    char buffer[1000];

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
        fgets(buffer, 1000, fp);
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


int pre_assemble(char *file_name) {
    FILE *fp, *fp2;
    char *token;
    int line_counter = 0, is_mcr = 0, error_flag = 0, i;
    char *mcro_name = malloc(1000), *line = malloc(1000), *temp_line = malloc(1000);
    macro_list *table = NULL;
    macro_list *node = NULL;

    fp = fopen(file_name, "r");
    fp2 = fopen("___temp", "w");
    int algo_counter = 1;
    while(algo_counter != 0)
    {
        switch (algo_counter) {
            case 1:
                fgets(line, 1000, fp);
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
                        fgets(line, 1000, fp);
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
                        fgets(line, 1000, fp);
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
                        fgets(line, 1000, fp);
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
                        fgets(line, 1000, fp);
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
    return 0;
}

int main() {
    pre_assemble("harta2.txt");

    return 0;
}

