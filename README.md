# Y86-Instruction-Disassembler

Y86 Instruction Disassembler
The Y86 Instruction Disassembler is a C program designed to disassemble Y86 machine bytecode, enabling the user to comprehend the instructions written in human-readable mnemonics. The disassembler works by processing a Y86 binary program file and subsequently printing the opcode mnemonics, along with associated registers and immediate values, to the console.

**Features:**
Array of Register Mnemonics: The program contains an array of Y86 register names for easy translation.

Opcode Translation: The byte0 function is leveraged to convert opcodes to their mnemonic representations.

Memory Space Representation: 4KB memory space representation for loading and analyzing binary code.

Little Endian Decoding: The program implements decoding instructions in little endian format for specific Y86 operations, including immediate value loading and jump destinations.

Conditional Instruction Execution: Depending on the opcode found, the disassembler appropriately decodes the subsequent bytes and prints the full Y86 instruction to the console.

**Usage:**
Compile the C program.
Run the executable followed by the path to the Y86 binary program file.
bash
Copy code
./disassembler path/to/y86/binaryfile.bin
**External Functions:**
loadBinFromFile: This utility function is provided to read a Y86 binary program from a file into the designated memory array. The function determines the program size, ensuring it doesn't surpass the available memory size, and returns the total bytes read from the file.
Note:
The main focus of this project is on disassembling the Y86 instructions. The provided loadBinFromFile function aids in reading the binary from a file, and users are not required to modify it.


