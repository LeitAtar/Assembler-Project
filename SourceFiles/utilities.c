#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/Globals.h"
#include "../HeaderFiles/tables.h"
#include "../HeaderFiles/secondPass.h"

char* decimalToBinary(int num) {
    // Array to store binary number
    char *binaryLine = calloc(14,1);
    char *reverse_binaryLine;
    int j = 0;
    int is_negative = (num < 0) ? 1 : 0;
    num = abs(num);

    // Counter for binary array
    int i = 0;
    while (num > 0) {
        // Store remainder in binary array
        binaryLine[i] = num % 2 + '0';
        num = num / 2;
        i++;
    }
    if ((binaryLine = realloc(binaryLine, i + 1)) == NULL) {
        printf("Memory reallocation failed\n");
        return NULL;
    }

    if (is_negative) {
        // Two's complement
        for (j = 0; j < i; j++) {
            binaryLine[j] = (binaryLine[j] == '0') ? '1' : '0';
        }
        // Add 1 to the binary number
        for (j = 0; j < 16; j++) {
            if (binaryLine[j] == '1') {
                binaryLine[j] = '0';
            } else {
                binaryLine[j] = '1';
                break;
            }
        }
    }
    char *ones = calloc(i + 1,1);

    reverse_binaryLine = calloc(i + 1,1);
    int k = 0;
    // Print binary array in reverse order
    for (j = i - 1; j >= 0; j--) {
        //printf("%d", binaryNum[j]);
        reverse_binaryLine[k] = binaryLine[j];
        k++;
    }
    reverse_binaryLine[k] = '\0';

    if(is_negative == 1) {
        for (j = 0; j < 14 - strlen(reverse_binaryLine); j++) {
            ones[j] = '1';
        }

    }
    strcat(ones, reverse_binaryLine);
    free(binaryLine);
    free(reverse_binaryLine);
    return ones;
}

int findL(char *line)
{
    char temp_line[1000];
    strcpy(temp_line, line);
    char *token = strtok(temp_line, ", \t");
    int L = 0, i = 0, is_quote = 0, second_r = 0;

    while(token != NULL)
    {
        if(strstr(token, "[") != NULL &&  strstr(token, "]") != NULL)
        {
            L += 2;
        }
        else if(strcmp(token, ".data") == 0 || strcmp(token, ".entry") == 0 || strcmp(token, ".extern") == 0)
        {
            L; // we dont change the L value
        }
        else if(strcmp(token, ".string") == 0)
        {
            is_quote = 1;
        }
        else if(is_quote)
        {
            for(i = 0; i <= strlen(token) - 6; i++)
            {
                L++;
            }
            is_quote = 0;
        }
        else if(token[0] == 'r' && second_r == 0 && token[1] >= '0' && token[1] <= '7')
        {
            second_r = 1;
            L++;
        }
        else if(second_r == 1 && token[0] == 'r' && token[1] >= '0' && token[1] <= '7')
        {
            //TODO: check if the number of register is legal
            second_r = 0;
        }
        else
        {
            L++;
        }

        token = strtok(NULL, ", \t\n");
    }

    return L;
}
