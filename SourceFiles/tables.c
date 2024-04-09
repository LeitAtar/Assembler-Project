#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/Globals.h"

int insertToSymbolTable(symbol_list **head, char *symbol, int value, char *identifier) {
    char *temp_symbol = malloc(100);
    strcpy(temp_symbol, symbol);
    char *temp_identifier = malloc(100);
    strcpy(temp_identifier, identifier);

    symbol_list *temp = *head;
    symbol_list *newSymbol = malloc(sizeof(symbol_list));
    newSymbol->symbol = temp_symbol;
    newSymbol->value = value;
    newSymbol->identifier = temp_identifier;
    newSymbol->next = NULL;


    if (temp == NULL) {
        *head = newSymbol;
        return 0;
    }


    /*find the end of the list*/
    while ((temp)->next != NULL) {
        if (strcmp(temp->symbol, symbol) == 0) {
            if (temp->value == -1 && strcmp(temp->identifier,".entry") == 0) {
                temp->value = value;
                return 0;
            }
            if (strcmp(identifier, ".entry") == 0)
            {
                temp->identifier = temp_identifier;
                return 0;
            }
            printf("symbol already exists error\n");
            return 1;
        }
        temp = (temp)->next;
    }
    (temp)->next = newSymbol;

    return 0;
}
