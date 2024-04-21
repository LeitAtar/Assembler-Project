#ifndef ASSEMBLERPERSONAL_CONVERT_TO_BASE_FOUR_H
#define ASSEMBLERPERSONAL_CONVERT_TO_BASE_FOUR_H

/**
 * @brief Encrypts a file by converting its content from binary to base four.
 *
 * This function takes a file name as input and encrypts its content.
 * It first opens the input file and a new temporary file.
 * If it fails to open the input file, it prints an error message, closes the temporary file, and returns 1.
 * It then enters a loop that continues until it has read all lines from the input file.
 * In each iteration of the loop, it reads a line from the input file, converts it from binary to base four, and writes it to the temporary file.
 * After writing the line to the temporary file, it frees the allocated memory for the base four string.
 * After the loop, it closes the files, removes the input file, and renames the temporary file to the input file name.
 * It then frees the allocated memory for the base four string and returns 0.
 *
 * @param file_name The name of the file to encrypt.
 * @return Returns 0 if the encryption is successful, 1 otherwise.
 */
int encrypt(char *file_name);

/**
 * @brief Converts a binary string to a base four string in C.
 *
 * This function takes a binary string as input and converts it to a base four string.
 * It first allocates memory for the new string.
 * It then enters a loop that continues until it has processed all pairs of characters in the input string.
 * In each iteration of the loop, it extracts a pair of characters from the input string and compares it to four possible binary values.
 * If the pair of characters matches "00", it appends '*' to the new string.
 * If the pair of characters matches "01", it appends '#' to the new string.
 * If the pair of characters matches "10", it appends '%' to the new string.
 * If the pair of characters matches "11", it appends '!' to the new string.
 * After the loop, it null-terminates the new string and returns it.
 *
 * @param str The binary string to convert.
 * @return Returns the base four string.
 */
char* convertToBaseFour(const char *str);

#endif