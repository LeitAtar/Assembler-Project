👾 Labratory-C-Final-Project 👾 
       Final grade - 92 😎
-----------------------------------------------------------------------------------------

This is the Final project (maman 14) in Laboratory C course at the Open University. 
It's a comprehensive assembler program for a specific assembly language, written in C. 
The purpose is to simulate the operation of common system programs, and thus experience writing a large-scale software project.

-----------------------------------------------------------------------------------------

🤖 **Project Structure** 🤖

The project consists of multiple C and header files, each serving a specific purpose in the program. It adheres to principles of clarity, readability, structured writing, and data abstraction.

The assembler works in three main stages:

1️⃣  Pre-Processing

2️⃣  First Pass

3️⃣  Second Pass

These stages form the core algorithm of the assembler.

Pre-Processing:

The pre-assembler reads the source file line by line and identifies macro definitions. If a macro is identified, the lines of the macro are added to a macro table and replaced in the source file.

First Pass:

The first pass identifies symbols (labels) and assigns them numerical values representing their corresponding memory addresses.

Second Pass:

In the second pass, the assembler generates the final machine code, replacing operation names with their binary equivalents and symbol names with their assigned memory locations.

✨ for a more detailed explanation you're more than welcome to take a look at the Documentation ✨
-----------------------------------------------------------------------------------------



-----------------------------------------------------------------------------------------
🧪 **Testing** 🧪

Several assembly language input files are provided to demonstrate the use of various operations and data types, as well as the assembler's handling of errors.

📚 **Documentation** 📚

Each function in the code has a clear documentation in the form of header comments, explaining its purpose and usage. Aswell as important variables are also explained. 
Detailed comments are present throughout the code for better understanding.

🤝 **Creators** 🤝

👏🏻 **Amitai Land and Dean Gabai ©** 👏🏻
