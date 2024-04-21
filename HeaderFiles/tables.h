#ifndef ASSEMBLERPERSONAL_TABLES_H
#define ASSEMBLERPERSONAL_TABLES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/**
 * @struct macro_list
 * @brief A structure to represent a macro in C.
 *
 * This structure represents a macro in C. It contains the name of the macro, its content, the line number associated with the content, the amount of lines in the content, and a pointer to the next macro in the list.
 *
 * @var macro_list::name
 * Member 'name' is a pointer to a character string that represents the name associated with the node.
 *
 * @var macro_list::content
 * Member 'content' is a pointer to a character string that represents the content stored in the node.
 *
 * @var macro_list::line
 * Member 'line' is an integer that represents the line number associated with the content.
 *
 * @var macro_list::line_amount
 * Member 'line_amount' is an integer that represents the amount of lines in the content.
 *
 * @var macro_list::next
 * Member 'next' is a pointer to the next node in the linked list.
 */
typedef struct macro_list { /*for macros*/
    char *name;         /* Name associated with the node */
    char *content;      /* Content stored in the node */
    int line;           /* Line number associated with the content */
    int line_amount;     /* Amount of lines in content*/
    struct macro_list *next;  /* Pointer to the next node in the linked list */
} macro_list;

/**
 * @struct symbol_list
 * @brief A structure to represent a symbol in C.
 *
 * This structure represents a symbol in C. It contains the name of the symbol, its value, its identifier, a flag indicating whether the symbol is an entry, and a pointer to the next symbol in the list.
 *
 * @var symbol_list::symbol
 * Member 'symbol' is a pointer to a character string that represents the name of the symbol.
 *
 * @var symbol_list::value
 * Member 'value' is an integer that represents the value of the symbol.
 *
 * @var symbol_list::identifier
 * Member 'identifier' is a pointer to a character string that represents the identifier of the symbol.
 *
 * @var symbol_list::is_entry
 * Member 'is_entry' is an integer that represents whether the symbol is an entry. It is 1 if the symbol is an entry, 0 otherwise.
 *
 * @var symbol_list::next
 * Member 'next' is a pointer to the next symbol in the list.
 */
typedef struct symbol_list {
    char *symbol;
    int value;
    char *identifier;
    int is_entry;
    struct symbol_list *next;
} symbol_list;

/**
 * @brief Inserts a new symbol into the symbol table in C.
 *
 * This function takes a pointer to the head of the symbol table, a symbol, a value, an identifier, and a flag indicating whether the symbol is an entry as input.
 * It first allocates memory for the symbol, the identifier, and the new symbol node.
 * It then copies the symbol and the identifier into the allocated memory.
 * It sets the fields of the new symbol node to the specified values.
 * If the symbol is in the macro table, it frees the allocated memory and returns 1.
 * If the symbol is a reserved word (a command, a directive, or a register), it frees the allocated memory and returns 1.
 * If the symbol table is empty, it sets the head of the symbol table to the new symbol node and returns 0.
 * If the symbol table is not empty, it traverses the symbol table to find the end of the list.
 * If it finds a symbol in the symbol table that matches the input symbol, it checks if the symbol is an entry.
 * If the symbol is an entry, it updates the value and the identifier of the symbol in the symbol table and returns 0.
 * If the symbol is not an entry, it frees the allocated memory and returns 1.
 * If it does not find a symbol in the symbol table that matches the input symbol, it adds the new symbol node to the end of the symbol table and returns 0.
 *
 * @param head The pointer to the head of the symbol table.
 * @param symbol The symbol to insert.
 * @param value The value of the symbol.
 * @param identifier The identifier of the symbol.
 * @param is_entry The flag indicating whether the symbol is an entry.
 * @return Returns 0 if the insertion is successful, 1 otherwise.
 */
int insert_to_symbol_table(symbol_list **head, char *symbol, int value, char *identifier, int is_entry);

/**
 * @brief Checks if a macro is in the macro table in C.
 *
 * This function takes a pointer to the macro table and a macro name as input.
 * It first checks if the macro table is NULL, and if so, it returns NULL.
 * If the macro table is not NULL, it traverses the macro table.
 * For each node in the macro table, it checks if the name of the macro matches the input name.
 * If it finds a match, it returns a pointer to the node.
 * If it does not find a match, it continues to the next node.
 * If it has checked all nodes and has not found a match, it returns NULL.
 *
 * @param table The pointer to the macro table.
 * @param name The name of the macro to check.
 * @return Returns a pointer to the node if the macro is in the table, NULL otherwise.
 */
macro_list* is_in_macro_table(macro_list *table, char *name);

/**
 * @brief Inserts a new macro into the macro table in C.
 *
 * This function takes a pointer to the macro table, a macro name, and its content as input.
 * It first checks if the macro is already in the macro table.
 * If the macro is in the macro table, it reallocates memory for the combined content of the existing macro and the new content.
 * It then concatenates the new content to the existing content and increments the line amount of the macro.
 * If the memory allocation fails, it returns -1.
 * If the macro is not in the macro table, it allocates memory for a new macro node, the macro name, and the content.
 * It then copies the macro name and the content into the allocated memory.
 * It sets the fields of the new macro node to the specified values.
 * If the memory allocation fails, it frees the new macro node and returns -1.
 * If the macro table is empty, it sets the head of the macro table to the new macro node.
 * If the macro table is not empty, it traverses the macro table to find the end of the list and adds the new macro node to the end of the macro table.
 *
 * @param table The pointer to the macro table.
 * @param mcro_name The name of the macro to insert.
 * @param content The content of the macro.
 * @return Returns 0 if the insertion is successful, -1 otherwise.
 */
int insert_to_macro_table(macro_list **table, char *mcro_name, char *content);


#endif
