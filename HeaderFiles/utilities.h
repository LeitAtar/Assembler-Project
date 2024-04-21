#ifndef ASSEMBLERPERSONAL_UTILITIES_H
#define ASSEMBLERPERSONAL_UTILITIES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Converts a decimal number to a binary string in C.
 *
 * This function takes a decimal number and its length as input and converts it to a binary string.
 * It first checks if the number is negative, and if so, it performs a two's complement operation.
 * It then converts the absolute value of the number to binary by repeatedly dividing the number by 2 and storing the remainder.
 * If the number was negative, it adds 1 to the binary number to complete the two's complement operation.
 * It then allocates memory for the binary string and the reverse binary string.
 * If the memory allocation fails, it frees any allocated memory and returns NULL.
 * It then reverses the binary string and stores it in the reverse binary string.
 * If the number was negative, it pads the binary string with 1s to the specified length.
 * If the number was not negative, it pads the binary string with 0s to the specified length.
 * It then concatenates the reverse binary string to the binary string and frees the reverse binary string.
 *
 * @param num The decimal number to convert.
 * @param length The length of the binary string.
 * @return Returns the binary string if the conversion is successful, NULL otherwise.
 */
char* decimal_to_binary(int num, int length);

/**
 * @brief Calculates the length of a line in assembly language in C.
 *
 * This function takes a string as input and calculates the length of the line in assembly language.
 * It first checks if the input string is NULL, and if so, it returns 0.
 * If the string is not NULL, it tokenizes the string by spaces, tabs, and commas.
 * For each token, it checks if it is a register, a string, a data directive, an entry directive, or an extern directive.
 * If it is a register, it increments the length by 1.
 * If it is a string, it increments the length by the number of characters in the string minus 2 (for the quotes).
 * If it is a data, entry, or extern directive, it does not change the length.
 * If it is an index (contains both '[' and ']'), it increments the length by 2.
 * If it is none of the above, it increments the length by 1.
 * The function continues this process until all tokens have been processed.
 *
 * @param line The string to calculate the length of.
 * @return Returns the length of the line in assembly language.
 */
int find_L(char *line);

/**
 * @brief Searches for a command in C.
 *
 * This function takes a string as input and checks if it matches any of the predefined assembly commands.
 * If the input string matches a command, it returns the corresponding command code.
 * The command codes are as follows:
 * - mov: 0
 * - cmp: 1
 * - add: 2
 * - sub: 3
 * - not: 4
 * - clr: 5
 * - lea: 6
 * - inc: 7
 * - dec: 8
 * - jmp: 9
 * - bne: 10
 * - red: 11
 * - prn: 12
 * - jsr: 13
 * - rts: 14
 * - hlt: 15
 * If the input string does not match any command, it returns -1.
 *
 * @param token The string to check.
 * @return Returns the command code if the string matches a command, -1 otherwise.
 */
int search_command(char *token);

/**
 * @brief Checks the type of the operand in C.
 *
 * This function takes a string as input and checks the type of the operand it represents.
 * It first checks if the input string is NULL, and if so, it returns -1 and prints an error message.
 * If the string is not NULL, it checks if the first character is '#', and if so, it returns 0, indicating that the operand is an immediate operand.
 * If the first character is not '#', it checks if it is a minus sign, a plus sign, or a digit (0-9). If it is, it returns -1 and prints an error message, indicating that the operand is an immediate operand but doesn't have a '#'.
 * If the first character is not a minus sign, a plus sign, or a digit, it checks if the string contains both '[' and ']'. If it does, it returns 2, indicating that the operand is an index.
 * If the string does not contain both '[' and ']', it checks if the first character is 'r'. If it is, it checks if the second character is a digit between 0 and 7. If it is, it returns 3, indicating that the operand is a register. If it is not, it returns -1 and prints an error message, indicating that the register doesn't exist.
 * If the first character is not 'r', it returns 1, indicating that the operand is a label.
 *
 * @param token The string to check.
 * @return Returns 0 if the string is an immediate operand, 1 if it is a label, 2 if it is an index, 3 if it is a register, and -1 if it is not a valid operand.
 */
int check_operand(char *strtok);

/**
 * @brief Converts a given assembly line to binary in C.
 *
 * This function takes an assembly line as input and converts it to a binary string.
 * It first identifies the command in the line, then processes the operands based on the command type.
 * It handles different types of operands including immediate, label, index, and register.
 * It also handles special cases where both operands are registers.
 * The function uses helper functions to convert decimal to binary, check operands, and search commands.
 *
 * @param line The assembly line to convert.
 * @return Returns a pointer to the binary string. If an error occurs during processing, it returns NULL and prints an error message.
 */
char *to_binary (char *line);

/**
 * @brief Creates an external file (.ext) for the given file.
 *
 * This function takes a file name as input and creates an external file (.ext) for it.
 * It reads the input file line by line and checks for external symbols in the symbol table.
 * If an external symbol is found, it writes the symbol and its value to the .ext file.
 * The function uses a switch-case structure to control the flow of the algorithm.
 *
 * @param file_name The name of the file for which to create an .ext file.
 * @return Returns 0 if the .ext file is successfully created, 1 otherwise.
 */
int ext_file_creator(char *file_name);

/**
 * @brief Converts data to binary in C.
 *
 * This function takes a string of data as input and converts it to a binary string.
 * It first checks if the input data is NULL or empty, and if so, it returns NULL after freeing any allocated memory.
 * If the data is not NULL or empty, it tokenizes the data string by spaces, tabs, and commas.
 * For each token, it checks if it is a number. If it is not a number, it searches the symbol table for a macro definition with the same name as the token.
 * If a macro definition is found, it uses the value of the macro definition. If not, it prints an error message and returns NULL.
 * It then converts the value to binary and appends it to the final binary string.
 * The function continues this process until all tokens have been processed.
 * The function uses dynamic memory allocation for the final binary string, and reallocates memory as needed.
 * The function frees any allocated memory before returning.
 *
 * @param line The data string to convert to binary.
 * @return Returns a pointer to the binary string. If an error occurs during processing, it returns NULL and prints an error message.
 */
char* data_to_binary (char* line);

/**
 * @brief Converts data to binary in C.
 *
 * This function takes a string of data as input and converts it to a binary string.
 * It first checks if the input data is NULL or empty, and if so, it returns NULL after freeing any allocated memory.
 * If the data is not NULL or empty, it tokenizes the data string by spaces, tabs, and commas.
 * For each token, it checks if it is a number. If it is not a number, it searches the symbol table for a macro definition with the same name as the token.
 * If a macro definition is found, it uses the value of the macro definition. If not, it prints an error message and returns NULL.
 * It then converts the value to binary and appends it to the final binary string.
 * The function continues this process until all tokens have been processed.
 * The function uses dynamic memory allocation for the final binary string, and reallocates memory as needed.
 * The function frees any allocated memory before returning.
 *
 * @param line The data string to convert to binary.
 * @return Returns a pointer to the binary string. If an error occurs during processing, it returns NULL and prints an error message.
 */
char* string_to_binary (char* line);

/**
 * @brief Checks if a given string is a valid label in C.
 *
 * This function takes a string as input and checks if it is a valid label.
 * It first checks if the input string is NULL, and if so, it returns 1.
 * If the string is not NULL, it checks if the length of the string is greater than the maximum label length,
 * or if the string contains a space or tab, or if the first character is not a letter. If any of these conditions are met, it returns 1.
 * It then checks if the string is equal to any of the register names (r0 to r7), and if so, it returns 1.
 * Finally, it checks if the string is equal to any of the names in the macro table, and if so, it returns 1.
 * If none of these conditions are met, it returns 0, indicating that the string is a valid label.
 *
 * @param label The string to check.
 * @return Returns 0 if the string is a valid label, 1 otherwise.
 */
int label_check(char* label);

/**
 * @brief Checks if a given string is a valid number in C.
 *
 * This function takes a string as input and checks if it is a valid number.
 * It first checks if the input string is NULL, and if so, it returns 1.
 * If the string is not NULL, it checks if the first character is a minus or plus sign, and if so, it starts checking from the second character.
 * It then checks each character of the string to see if it is a digit (0-9).
 * If it encounters a character that is not a digit, it returns 1.
 * If all characters are digits, it returns 0.
 *
 * @param num The string to check.
 * @return Returns 0 if the string is a valid number, 1 otherwise.
 */
int num_check(char* num);

#endif


