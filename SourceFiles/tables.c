#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/tables.h"
#include "../HeaderFiles/utilities.h"

symbol_list *symbol_table;
macro_list *mcr_table;

int insert_to_symbol_table(symbol_list **head, char *symbol, int value, char *identifier, int is_entry) {
    char *temp_symbol = malloc(strlen(symbol) + 1);
    strcpy(temp_symbol, symbol);
    char *temp_identifier = malloc(strlen(identifier) + 1);
    strcpy(temp_identifier, identifier);

    symbol_list *temp = *head;
    symbol_list *newSymbol = malloc(sizeof(symbol_list));
    newSymbol->symbol = temp_symbol;
    newSymbol->value = value;
    newSymbol->identifier = temp_identifier;
    newSymbol->is_entry = is_entry;
    newSymbol->next = NULL;

    if (is_in_macro_table(mcr_table, symbol) != NULL) {
        printf("Error: symbol is a macro");
        free(newSymbol);
        newSymbol = NULL;
        free(temp_symbol);
        temp_symbol = NULL;
        free(temp_identifier);
        temp_identifier = NULL;
        return 1;
    }
    if (strcmp(symbol, "r0") == 0 || strcmp(symbol, "r1") == 0 || strcmp(symbol, "r2") == 0
    || strcmp(symbol, "r3") == 0 || strcmp(symbol, "r4") == 0 || strcmp(symbol, "r5") == 0
    || strcmp(symbol, "r6") == 0 || strcmp(symbol, "r7") == 0 || strcmp(symbol, ".define") == 0
    || strcmp(symbol, ".entry") == 0 || strcmp(symbol, ".extern") == 0 || strcmp(symbol, ".data") == 0
    || strcmp(symbol, ".string") == 0 || search_command(symbol) != -1) {
        printf("Error: symbol is a saved word (command or directive or register)");
        free(newSymbol);
        newSymbol = NULL;
        free(temp_symbol);
        temp_symbol = NULL;
        free(temp_identifier);
        temp_identifier = NULL;
        return 1;
    }

    if (temp == NULL) {
        *head = newSymbol;
        return 0;
    }

    /*find the end of the list*/
    while ((temp)->next != NULL) {
        if (strcmp(temp->symbol, symbol) == 0) {
            if (temp->value == -1 && strcmp(temp->identifier,".entry") == 0) {
                temp->value = value;
                temp->is_entry = 1;
                temp->identifier = temp_identifier;
                return 0;
            }
            if (strcmp(identifier, ".entry") == 0 && temp->is_entry == 0) {
                temp->is_entry = 1;
                return 0;
            }
            printf("Error: symbol already exists");
            free(newSymbol);
            newSymbol = NULL;
            free(temp_symbol);
            temp_symbol = NULL;
            free(temp_identifier);
            temp_identifier = NULL;
            return 1;
        }
        temp = (temp)->next;
    }
    (temp)->next = newSymbol;

    return 0;
}

macro_list* is_in_macro_table(macro_list *table, char *name)
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

int insert_to_macro_table(macro_list **table, char *mcro_name, char *content) {
    macro_list *node = is_in_macro_table(*table, mcro_name);
    if (node != NULL) {
        /* Reallocate memory for combined content*/
        size_t new_size = strlen(node->content) + strlen(content) + 1; /* +1 for null terminator*/
        node->content = realloc(node->content, new_size);
        if (node->content == NULL) {
            /* Handle memory allocation failure*/
            return -1;
        }
        strcat(node->content, content);
        node->line_amount++;
    }
    else {
        macro_list *new_node = malloc(sizeof(macro_list)); /*Allocate memory based on size of macro_list*/
        if (new_node == NULL) {
            /* Handle memory allocation failure*/
            free(new_node);
            new_node = NULL;
            return -1;
        }
        new_node->name = malloc(strlen(mcro_name) + 1); /*Allocate memory based on length of mcro_name*/
        strcpy(new_node->name, mcro_name);

        new_node->content = malloc(strlen(content) + 1); /*Allocate memory based on length of content*/
        strcpy(new_node->content, content);

        new_node->line = 0;
        new_node->line_amount = 0;
        new_node->next = NULL;

        /* Update table*/
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
