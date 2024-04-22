#ifndef ASSEMBLERPERSONAL_SECOND_PASS_H
#define ASSEMBLERPERSONAL_SECOND_PASS_H

/**
 * @brief Executes the second pass on a given file.
 *
 * This function takes a file name, IC (Instruction Counter), and DC (Data Counter) as input and performs the second pass operations on it.
 * It first opens the input file and a new file with the same name but with the extension ".ob".
 * If it fails to open either file, it frees the allocated memory and returns 1.
 * It then enters a loop that continues until the end of the file.
 * In each iteration of the loop, it performs different operations based on the content of the line.
 * If the line starts with '1' or '0', it writes the line to the ".ob" file.
 * If the line does not start with '1' or '0', it checks if the symbol in the line is in the symbol table.
 * If the symbol is in the symbol table, it converts the value of the symbol to binary and writes it to the ".ob" file.
 * If the symbol is not in the symbol table, it sets the error flag to 1 and prints an error message.
 * After the loop, it closes the files and removes the temporary file.
 * It then opens the ".ob" file and a new temporary file.
 * It writes the IC and DC to the temporary file and then copies the content of the ".ob" file to the temporary file.
 * It then closes the files, removes the ".ob" file, and renames the temporary file to the ".ob" file.
 * If the error flag is 1, it prints an error message and returns 1.
 * If the error flag is 0, it checks if there are any external or entry symbols in the symbol table.
 * If there are any external symbols, it creates an external file.
 * If there are any entry symbols, it creates an entry file.
 * It then frees the allocated memory and prints a success message.
 *
 * @param file_name The name of the file to perform the second pass operations on.
 * @param IC The Instruction Counter.
 * @param DC The Data Counter.
 * @return Returns 0 if the second pass operations are successful, 1 otherwise.
 */
int exe_second_pass(char *file_name, int IC, int DC, int prev_error);

#endif