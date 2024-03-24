#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/Globals.h"

int insertToSymbolTable(symbol_list *head, char *symbol, int value, const char *identifier) {
    symbol_list *temp = head;
    symbol_list *newSymbol = malloc(sizeof(symbol_list));
    newSymbol->symbol = symbol;
    newSymbol->value = value;
    newSymbol->identifier = identifier;
    newSymbol->next = NULL;

    if (temp == NULL) {
        head = newSymbol;
        return 0;
    }


    /*find the end of the list*/
    while ((temp)->next != NULL) {
        if (strcmp(temp->symbol, symbol) == 0) {
            printf("symbol already exists error\n");
            return 1;
        }
        temp = (temp)->next;
    }
    (temp)->next = newSymbol;

    return 0;
}
