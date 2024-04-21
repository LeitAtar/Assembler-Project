#ifndef ASSEMBLERPERSONAL_PRE_ASSEMBLER_H
#define ASSEMBLERPERSONAL_PRE_ASSEMBLER_H

/**
 * @brief Executes the pre-assembler on a given file.
 *
 * This function takes a file name as input and performs the pre-assembler operations on it.
 * It first opens the input file and a new file with the same name but with the extension ".am".
 * If it fails to open either file, it frees the allocated memory and returns 1.
 * It then enters a loop that continues until the algorithm counter is 0.
 * In each iteration of the loop, it performs different operations based on the value of the algorithm counter.
 * If the algorithm counter is 1, it reads a line from the input file and checks if it is the end of the file.
 * If the algorithm counter is 2, it checks if the token is in the macro table and if so, it writes the content of the macro to the ".am" file and increments the line counter.
 * If the algorithm counter is 3, it checks if the token is "mcr" and if not, it skips to step 6.
 * If the algorithm counter is 4, it sets the macro flag to 1.
 * If the algorithm counter is 5, it checks if the macro is in the macro table and if so, it sets the error flag to 1 and prints an error message.
 * If the macro is not in the macro table, it inserts the macro into the macro table.
 * If the algorithm counter is 6, it checks if the token is "mcr" and if so, it goes back to step 1.
 * If the token is not "mcr" and the macro flag is 1 and the line does not contain "endmcr", it inserts the line into the macro table and goes back to step 1.
 * If the token is "endmcr", it skips to step 7.
 * If none of the above conditions are met, it writes the line to the ".am" file and goes back to step 1.
 * If the algorithm counter is 7, it checks if the line contains "endmcr" and if not, it writes the line to the ".am" file and goes back to step 1.
 * If the algorithm counter is 8, it sets the macro flag to 0 and goes back to step 1.
 * If the algorithm counter is 9, it sets the algorithm counter to 0 to exit the loop.
 * After the loop, it closes the files and frees the allocated memory.
 * If the error flag is 1, it prints an error message and returns 1.
 * If the error flag is 0, it prints a success message and returns 0.
 *
 * @param file_name The name of the file to perform the pre-assembler operations on.
 * @return Returns 0 if the pre-assembler operations are successful, 1 otherwise.
 */
int exe_pre_assembler(char *file_name);

#endif