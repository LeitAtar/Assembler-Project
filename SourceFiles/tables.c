#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/tables.h"
#include "../HeaderFiles/firstPass.h"
#include "../HeaderFiles/utilities.h"
#include "../HeaderFiles/secondPass.h"
#include "../HeaderFiles/preAssembler.h"
#include "../HeaderFiles/convertToBaseFour.h"

extern symbol_list *symbol_table;
extern macro_list *mcr_table;

int insertToSymbolTable(symbol_list **head, char *symbol, int value, char *identifier, int is_entry) {
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

    if (isInMacroTable(mcr_table, symbol) != NULL) {
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
