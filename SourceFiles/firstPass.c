#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_LINE_LENGTH 100
int exe_first_pass(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    int algoCounter = 1, IC, DC, label_flag = 0;
    char *str = malloc(MAX_LINE_LENGTH);
    char *token = malloc(MAX_LINE_LENGTH);

    while(algoCounter != 0) {
        switch (algoCounter) {
            case 1: //done
                IC = 0;
                DC = 0;
                algoCounter++;
            case 2: //done
                fgets(str, MAX_LINE_LENGTH, fp);
                if(feof(fp)) {
                    algoCounter = 16;
                    break;
                }
            case 3:
                if(is_define(str) == 0) { //if not define statement move to 5
                    algoCounter = 5;
                    break;
                }


            case 4:
                insert_define(str, mdefine);

                algoCounter = 2;
                break;

            case 5:
                token = strtok(str, " "); //first field in the word
                if(is_label(token) == 0) { //not a label
                    algoCounter = 7;
                    break;
                }

            case 6: //done
                label_flag = 1;
            case 7:
                if(is_data_or_string(str) == 0) { //not .data or .string
                    algoCounter = 10;
                     break;
                }

            case 8:

            case 9:

                algoCounter = 2;
                break;
            case 10:
                if(is_extern_or_entry(str) == 0) { //not .extern or .entry
                    algoCounter = 12;
                    break;
                }

            case 11:


                algoCounter = 2;
                break;
            case 12:

            case 13:

            case 14:

            case 15: //done
                IC += L;
                algoCounter = 2;
                break;
            case 16: //done
                if (error_flag == 1) {
                    algoCounter = 0; //finished first pass with errors
                    break;
                }
            case 17:

            case 18: //done
                algoCounter = 0; //finished first pass successfully
                break;
            default:
                return -1;
        }
    }
}