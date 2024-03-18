#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // Include <stdlib.h> for malloc

#define BASE_TWO_WORD 16 // Adjusted to accommodate null terminator and \n
#define BASE_FOUR_WORD 8  // Adjusted to accommodate null terminator

char* convertToBaseFour(const char str[BASE_TWO_WORD]);
void encrypt(FILE *fp);

int main() {

    FILE *fp = fopen("MachineCode.txt", "r");
    encrypt(fp);

    return 0;
}


void encrypt(FILE *fp) {
    FILE *final;
    final = fopen("encrypted.txt", "w");
    char currentNum[BASE_TWO_WORD];
    char *baseFourNum;

    if (fp == NULL) {
        printf("Error opening file.\n");
        fclose(final);
        return;
    }

    while(fgets(currentNum, BASE_TWO_WORD, fp) != NULL) {
        baseFourNum = convertToBaseFour(currentNum);
        fprintf(final, "%s\n",baseFourNum);  // Corrected format specifier and added newline
        free(baseFourNum);  // Free memory allocated for baseFourNum
    }

    fclose(fp);
    fclose(final);

}

char* convertToBaseFour(const char str[BASE_TWO_WORD]) {
    int i = 0;
    int j = 0;
    char *newStr = malloc(BASE_FOUR_WORD);  // Allocate memory for the string

    char num[3];  // Increase size to accommodate null terminator
    for (i = 0; i < BASE_FOUR_WORD - 1; ++i, j += 2) {
        num[0] = str[j];
        num[1] = str[j + 1];
        num[2] = '\0';  // Null-terminate the string
        if (strcmp(num, "00") == 0) {
            newStr[i] = '*';
        }
        else if (strcmp(num, "01") == 0) {
            newStr[i] = '#';
        }
        else if (strcmp(num, "10") == 0) {
            newStr[i] = '%';
        }
        else {
            newStr[i] = '!';
        }
    }
    newStr[BASE_FOUR_WORD - 1] = '\0';  // Null-terminate the string

    return newStr;
}
