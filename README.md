👾 **Galactic Assembler Project** 👾  
Final grade - 92 😎

---

*In a galaxy not so far away, I embarked on an epic coding adventure...*

## 🌌 Table of Contents

1. [🚀 Introduction](#-introduction)
2. [🛠️ Project Structure](#%EF%B8%8F-project-structure)
3. [🧩 The Assembler Algorithm](#-the-assembler-algorithm)
   - [1️⃣ Pre-Processing](#1%EF%B8%8F%E2%83%A3-pre-processing)
   - [2️⃣ First Pass](#2%EF%B8%8F%E2%83%A3-first-pass)
   - [3️⃣ Second Pass](#3%EF%B8%8F%E2%83%A3-second-pass)
4. [📚 Code Highlights](#-code-highlights)
   - [🖥️ Main Function](#%EF%B8%8F-main-function)
   - [🔒 Encrypt Function](#-encrypt-function)
   - [🔄 Convert to Base Four Function](#-convert-to-base-four-function)
   - [📝 First Pass Execution](#-first-pass-execution)
   - [🛠️ Second Pass Execution](#%EF%B8%8F-second-pass-execution)
   - [⚙️ Pre-Assembler Execution](#%EF%B8%8F-pre-assembler-execution)
   - [📦 Tables Handling](#-tables-handling)
   - [🧰 Utilities](#-utilities)
5. [💥 Challenges Faced](#-challenges-faced)
6. [🧪 Testing](#-testing)
7. [📖 Documentation](#-documentation)
8. [🤝 Conclusion](#-conclusion)

---

## 🚀 Introduction

Hey there! I'm Dean Gabbai, and I want to share with you the journey I took while creating my Galactic Assembler Project for the Laboratory C course at the Open University. This assembler, written in the ancient and mysterious language of C, translates a specific assembly language into machine code. Think of it as my own personal Millennium Falcon, navigating through the asteroids of code to deliver something truly out of this world!

---

## 🛠️ Project Structure

Just like the intricate blueprints of the Death Star (minus the exhaust port flaw), this project is carefully structured into several C and header files. Each file serves a unique purpose, and together they form the backbone of the assembler. I focused on clarity, readability, and structured programming to ensure that everything runs smoother than the Kessel Run.

---

## 🧩 The Assembler Algorithm

The assembler operates in three main stages, akin to the trials of a Jedi Knight:

### 1️⃣ Pre-Processing

In this initial phase, the assembler scans the source file line by line, searching for macro definitions. Macros are like the Force—binding the code together and allowing for more efficient programming. When a macro is detected, its lines are stored in a macro table and replaced in the source file. This ensures that our code is DRY (Don't Repeat Yourself) and more maintainable.

### 2️⃣ First Pass

This is where the real adventure begins! The assembler makes its first pass through the code, identifying symbols (labels) and assigning them numerical values that represent their corresponding memory addresses. It's like mapping out the galaxy before embarking on a space voyage.

**Detailed Explanation:**

- **🌟 Symbol Table Construction:** The first pass reads each line, searching for label declarations and directives.
- **📈 Instruction and Data Counters (IC and DC):** These keep track of memory addresses for instructions and data.
- **🚨 Error Detection:** Syntax errors, undefined labels, and invalid instructions are caught here.
- **🔄 Macro Handling:** Processes macros and inserts them where called.
- **📜 Directive Processing:** Handles `.data`, `.string`, `.extern`, and `.entry` directives appropriately.

**Key Functions in First Pass:**

- `exe_first_pass`
- `label_check`
- `insert_to_symbol_table`
- `data_to_binary`
- `string_to_binary`
- `to_binary`

### 3️⃣ Second Pass

In the final stage, the assembler completes its mission by generating the machine code. It replaces operation names with their binary equivalents and symbol names with their assigned memory locations. This is where the code transforms from a humble script into a powerful machine-readable format.

**Detailed Explanation:**

- **🧩 Symbol Resolution:** All symbol references are resolved using the symbol table.
- **💾 Machine Code Generation:** Converts assembly instructions into binary code.
- **🔐 Encryption:** I added a custom encryption to the output, transforming the binary code into a base four representation using special symbols. It's like encoding messages in Aurebesh!
- **🌐 External and Entry Files:** Generates `.ext` and `.ent` files for external and entry symbols.

**Key Functions in Second Pass:**

- `exe_second_pass`
- `ext_file_creator`
- `decimal_to_binary`
- `encrypt`

---

## 📚 Code Highlights

Let me walk you through some of the key parts of the code.

### 🖥️ Main Function

The heart of the assembler, coordinating all its operations.

```c
int main(int argc, char *argv[]) {
    char *file_name = calloc(FILENAME_MAX, sizeof(char));
    setvbuf(stdout, NULL, _IONBF, 0);
    while (--argc > 0) {
        strcpy(file_name, argv[argc]);
        strcat(file_name, ".as");
        printf("Starting pre-proc on file: %s\n", file_name);
        if (exe_pre_assembler(file_name) != 0) {
            printf("\n");
            continue;
        }
        strcpy(file_name, argv[argc]);
        strcat(file_name, ".am");
        printf("Starting first pass on file: %s\n", file_name);
        if (exe_first_pass(file_name) != 0) {
            printf("\n");
            continue;
        }
        printf("\n");
    }

    free(file_name);
    file_name = NULL;
    printf("end\n");
    return 0;
}
```

### 🔒 Encrypt Function

Converts binary code into a special base four representation using symbols. Think of it as my own encryption algorithm!

```c
int encrypt(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    FILE *final;
    char currentNum[WORD_LENGTH];
    char *baseFourNum;

    final = fopen("temp____", "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        fclose(final);
        return 1;
    }

    while(fgets(currentNum, WORD_LENGTH, fp) != NULL) {
        baseFourNum = convertToBaseFour(currentNum);
        fprintf(final, "%s\n", baseFourNum);
        free(baseFourNum);
        baseFourNum = NULL;
    }

    fclose(fp);
    fclose(final);
    remove(file_name);
    rename("temp____", file_name);

    return 0;
}
```

### 🔄 Convert to Base Four Function

Translates binary strings into base four using custom symbols.

```c
char* convertToBaseFour(const char *str) {
    int i;
    int j = 0;
    char *newStr = calloc(BASE_FOUR_WORD, sizeof(char));

    char num[3];
    for (i = 0; i < BASE_FOUR_WORD - 1; ++i, j += 2) {
        num[0] = str[j];
        num[1] = str[j + 1];
        num[2] = '\0';
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
    newStr[BASE_FOUR_WORD - 1] = '\0';

    return newStr;
}
```

### 📝 First Pass Execution

This is where labels and symbols are collected, and the groundwork is laid for the second pass.

**Key Functions:**

- **`label_check`:** Ensures labels are valid.
- **`insert_to_symbol_table`:** Adds labels to the symbol table.
- **`data_to_binary`:** Converts `.data` directives to binary.
- **`string_to_binary`:** Converts `.string` directives to binary.
- **`to_binary`:** Converts instructions to binary.

### 🛠️ Second Pass Execution

Resolves symbols and generates the final machine code.

**Key Functions:**

- **`exe_second_pass`:** Orchestrates the second pass.
- **`ext_file_creator`:** Creates the `.ext` file.
- **`decimal_to_binary`:** Converts numbers to binary.
- **`encrypt`:** Applies the custom encryption to the output.

### ⚙️ Pre-Assembler Execution

Handles macro definitions before the first pass.

**Key Functions:**

- **`exe_pre_assembler`:** Processes macros.
- **`insert_to_macro_table`:** Stores macros.

### 📦 Tables Handling

Manages symbol and macro tables.

**Structures:**

- **`symbol_list`:** Stores symbols.
- **`macro_list`:** Stores macros.

**Functions:**

- **`insert_to_symbol_table`:** Inserts symbols.
- **`is_in_macro_table`:** Checks for macros.
- **`insert_to_macro_table`:** Inserts macros.

### 🧰 Utilities

These were the challenging bits that required extra attention!

#### `decimal_to_binary` Function

Converts decimal numbers to binary strings, handling negative numbers with two's complement.

```c
char* decimal_to_binary(int num, int length) {
    // Function implementation
}
```

#### `label_check` Function

Validates labels according to assembly rules.

```c
int label_check(char* label) {
    // Function implementation
}
```

---

## 💥 Challenges Faced

Creating this assembler was quite the adventure! Here are some of the challenges I encountered:

- **🌌 Complex Parsing:** Handling the nuances of assembly language was like navigating through an asteroid field. Every line had to be carefully analyzed.
- **🧠 Memory Management:** C doesn't have garbage collection, so I had to be vigilant about allocating and freeing memory to avoid leaks.
- **🔍 Error Handling:** Providing clear and helpful error messages was crucial. It's like having R2-D2 beep out diagnostics!
- **🔐 Encryption Implementation:** Designing and implementing the custom encryption added an extra layer of complexity.
- **🐛 Debugging:** Bugs are the dark side of coding. Squashing them required patience and a keen eye.
- **🖥️ Cross-Platform Line Endings:** Handling the difference in line endings between Windows (CR LF) and Unix-based systems (LF) caused unexpected issues. The carriage return characters added by Windows machines led to bugs that were difficult to trace and fix. Dealing with these hidden characters was particularly hellish and required careful handling of file input/output.

---

## 🧪 Testing

### ✅ Valid Input Example

**Input (`valid_input_1.as`):**

```assembly
;file valid_input_1.as
.define sz = 2
MAIN:       mov r3, LIST[sz]
LOOP:       jmp L1
            mcr m_mcr
                cmp r3, #sz
                bne END
            endmcr
            prn #-5
            mov STR[5], STR[2]
            sub r1, r4
            m_mcr
L1:         inc K
            bne LOOP
END:        hlt
.define len = 4
STR:        .string "abcdef"
LIST:       .data 6, -9, len
K:          .data 22
```

**Output (`valid_input_1.ob`):**

```
    25  11
0100    ****!%*
0101    ***#%**
0102    **%*#*%
0103    *****%*
0104    **%#*#*
0105    **#!%*%
0106    **!****
0107    !!!!%!*
0108    ****%%*
0109    **#!!#%
0110    ****##*
0111    **#!!#%
0112    *****%*
0113    ***!!!*
0114    ****!**
0115    ***#!**
0116    ***#%**
0117    *****%*
0118    **%%*#*
0119    **#!!*%
0120    **#!*#*
0121    **%*#!%
0122    **%%*#*
0123    **#%%*%
0124    **!!***
0125    ***#%*#
0126    ***#%*%
0127    ***#%*!
0128    ***#%#*
0129    ***#%##
0130    ***#%#%
0131    *******
0132    *****#%
0133    !!!!!#!
0134    *****#*
0135    ****##%
```

### ❌ Invalid Input Example

**Input (`invalid_input_1.as`):**

```assembly
; file invalid_input_1.as
.entry LIST

.extern W

.define sz = 2
MAIN:   mov r3, LIST[sz]
        mov r1 , UNREAL[2]
LOOP:   jmp W
        prn #-5
        mov STR[5], STR[2]
;error
        sub r1, r9
;error
        cmp K, #
        bne
L1:     inc L3
.entry LOOP
        bne LOOP
END:    hlt
.define len = 4
;error
STR:    .string "abcdef
;error
LIST:   .dATa 6, -9, len
;error
K:      .data
.extern L3
;ubuntu
```

**Output:**

- **🚨 Error Messages:**

```
Error: can't find symbol: UNREAL | line:5
Error: register doesn't exist | line:8
Error: missing operand after '#' | line:10
Error: missing operand for 'bne' | line:11
Error: can't find symbol: L3 | line:13
Error: string is not in quotes | line:16
Error: unrecognized directive '.dATa' | line:18
Error: missing data for '.data' directive | line:20
```

---

## 📖 Documentation

Every function is thoroughly documented, with explanations of their purpose, parameters, and usage. This was crucial for maintaining the code and for anyone else who might read it in the future.

---

## 🤝 Conclusion

This project was a fantastic learning experience. It pushed me to delve deep into the workings of assemblers and low-level programming. I hope you enjoyed this walkthrough as much as I enjoyed creating the assembler.

May the code be with you! 🌠

---

Feel free to explore the code, test it with different assembly files, and embark on your own coding adventure. Remember, the Force will be with you, always.

---

**Note:** The code snippets and outputs are based on the actual implementation. Your results may vary depending on specific inputs and any modifications made.

---





