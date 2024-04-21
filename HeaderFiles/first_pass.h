#ifndef ASSEMBLERPERSONAL_FIRST_PASS_H
#define ASSEMBLERPERSONAL_FIRST_PASS_H

/**
 * @brief Executes the first pass of the assembler.
 *
 * This function takes a file name as input and performs the first pass of the assembler.
 * It initializes variables, opens the input file and a temporary file for writing, and checks for file opening errors.
 * It then allocates memory for strings and enters a loop that continues until the end of the first pass.
 * The loop contains a switch statement that executes different cases based on the value of the algoCounter variable.
 * The cases handle different parts of the assembly process, such as reading lines from the file, checking for comments or empty lines,
 * handling label declarations, processing .define, .data, .string, .extern, and .entry directives, and converting assembly lines to binary.
 * After the loop, the function closes the files, frees allocated memory, and checks for errors.
 * If there were no errors during the first pass, it calls the function to execute the second pass of the assembler.
 * If the second pass is successful, it prints a success message.
 * If there were errors during the first or second pass, it removes the temporary file, frees the symbol table, and returns 1.
 * If there were no errors, it frees the symbol table and returns 0.
 *
 * @param file_name The name of the file to assemble.
 * @return Returns 0 if the assembly is successful, 1 otherwise.
 */
int exe_first_pass(char *file_name);

#endif