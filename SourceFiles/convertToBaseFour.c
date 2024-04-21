#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/convertToBaseFour.h"


int encrypt(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    FILE *final;
    final = fopen("temp____", "w");
    char currentNum[WORD_LENGTH];
    char *baseFourNum;

    if (fp == NULL) {
        printf("Error opening file.\n");
        fclose(final);
        return 1;
    }

    while(fgets(currentNum, WORD_LENGTH, fp) != NULL) {
        baseFourNum = convertToBaseFour(currentNum);
        fprintf(final, "%s\n",baseFourNum); /*Corrected format specifier and added newline*/
        free(baseFourNum);
        baseFourNum = NULL;
    }

    fclose(fp);
    fclose(final);
    remove(file_name);
    rename("temp____", file_name);
    free(baseFourNum);

    return 0;

}

char* convertToBaseFour(const char *str) {
    int i = 0;
    int j = 0;
    char *newStr = malloc(BASE_FOUR_WORD); /*Allocate memory for the string*/

    char num[3]; /*Increase size to accommodate null terminator*/
    for (i = 0; i < BASE_FOUR_WORD - 1; ++i, j += 2) {
        num[0] = str[j];
        num[1] = str[j + 1];
        num[2] = '\0'; /*Null-terminate the string*/
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
    newStr[BASE_FOUR_WORD - 1] = '\0'; /*Null-terminate the string*/

    return newStr;
}
