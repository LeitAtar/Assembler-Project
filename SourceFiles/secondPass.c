#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../HeaderFiles/DataStructures.h"
#include "../HeaderFiles/Globals.h"
#include "../HeaderFiles/tables.h"
#include "../HeaderFiles/secondPass.h"

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

char* decimalToBinary(int num) {
    // Array to store binary number
    char *binaryLine = calloc(16,1);
    char *final;
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

    if (is_negative) {
        // Two's complement
        for (j = 0; j < 16; j++) {
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
    char *ones = calloc(20,1);

    final = calloc(i,1);
    int k = 0;
    // Print binary array in reverse order
    for (j = i - 1; j >= 0; j--) {
        //printf("%d", binaryNum[j]);
        final[k] = binaryLine[j];
        k++;
        printf("%c", binaryLine[j]);
    }

    char *temp = malloc(k + 1);

    for(j = 0; j < k; j++)
    {
        temp[j] = final[j];
    }
    temp[j+1] = '\0'; // temp = [final

    int len = strlen(temp);


    if(is_negative == 1) {
        for (j = 0; j < 14 - (strlen(temp) - 1); j++) {
            ones[j] = '1';
        }
        strcat(ones, final);
    }

    printf("\n");

    return ones;
}




int second_pass(char *filename)
{
    FILE *fp;
    fp = fopen(filename, "r");

    if(fp == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    label_address *table = NULL, *node = NULL;
    char *token = malloc(MAX_LINE_LENGTH);
    int algo_counter = 1, L = 0;
    int IC, is_error = 0;
    char *line = malloc(MAX_LINE_LENGTH);
    char *temp_line = malloc(MAX_LINE_LENGTH);

    while(algo_counter != 0)
    {
        switch(algo_counter)
        {
            case 1:
                IC = 0;
            case 2:
                fgets(line, MAX_LINE_LENGTH, fp);
                 L = findL(line);
                if (feof(fp)) {
                    algo_counter = 9;
                    break;
                }
            case 3:
                strcpy(temp_line, line);
                token = strtok(temp_line, " \t");
                if(strstr(token, ":") != NULL)
                {
                    line = strtok(NULL, "\n");
                    strcat(line,"\n");
                }
            case 4:
                strcpy(temp_line, line);
                token = strtok(temp_line, " \t");
                if(strcmp(line, ".extern") == 0 || strcmp(line, ".data") == 0 || strcmp(line, ".string") == 0)
                {
                    if(strcasecmp(line, ".extern") != 0 || strcasecmp(line, ".data") != 0
                    || strcasecmp(line, ".string") != 0)
                    {
                         is_error = 1;
                        printf("error message"); // make an error message later
                    }
                    else
                    {
                        algo_counter = 2;
                        break;
                    }
                }
            case 5:
                if(strcmp(line, ".entry") != 0)
                {
                    if(strcasecmp(line, ".entry") != 0)
                    {
                        is_error = 1;
                        printf("error message"); // make an error message later
                    }
                }
                else
                {
                    algo_counter = 7;
                    break;
                }
            case 6:
                strcpy(temp_line, line);
                token = strtok(temp_line, " \t");
                token = strtok(NULL, ", \n\t");
                insertToSymbolTable(&table, token, 0, ".entry");
                algo_counter = 2;
                break;
            case 7:

            case 8:
                IC += L;
                algo_counter = 2;
                break;
            case 9:
                if(is_error == 1)
                {
                    algo_counter = 0;
                    break;
                }
            case 10:
                // need to finish this later
            default:
                is_error = 1;
                printf("something is wrong buddy\n");
        }
    }
}