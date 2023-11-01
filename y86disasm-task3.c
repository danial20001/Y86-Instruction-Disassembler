  #include <stdio.h>
#include <stdlib.h>

//This is an array of register mnemonics in y86
const char *register_names[] =
    {
        "%eax",
        "%ecx",
        "%edx",
        "%ebx",
        "%esp",
        "%ebp",
        "%esi",
        "%edi",
        "UNKNOWN_REGSITER"};


//Function for printing opcode() , 1st Byte
void byte0(int opcode)
{
    unsigned char byte0[] = {0x00,0x10, 0x90, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 
    0x60, 0x61, 0x62, 0x63, 0xA0, 0xB0, 0x30, 0x40, 0x50, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 
    0x76, 0x80}; 

    char* mnemonics[] = { "halt", "nop", "ret","rrmovl", "cmovle", "cmovl", "cmove", "cmovne", 
    "cmovge","cmovg","addl","subl","andl", "xorl", "pushl", "popl","irmovl", "rmmovl", 
    "mrmovl","jmp", "jle","jl", "je", "jne", "jge","jg","call"};

    for (int i = 0; i < sizeof(byte0); i++)
    {
       if(opcode == byte0[i]) 
        printf("%s", mnemonics[i]);
    }
}

//This is the help function that reads y86 binary code from a file
unsigned int loadBinFromFile(const char *filename, unsigned char memory[], unsigned int memsize);

int main(int argc, char **argv)
{
  unsigned int MAX_MEMSIZE = 4096;
  unsigned char memory[MAX_MEMSIZE]; //This array represents the 4KB memory space
  unsigned int PC = 0;               //This is the initial program counter address

  if (argc < 2)
  {
    printf("Usage: please specify a y86 binary program file in the argument.\n");
    return -1;
  }

  unsigned int program_size = loadBinFromFile(argv[1], memory, MAX_MEMSIZE);

  if (program_size == 0)
    return 0;

  
  while (PC < program_size)
  {
    
    //Calling Function to print the opcode
    byte0(memory[PC]);
    int operandA = memory[PC+1]>>4;//Rightshift 4 bits to find regA
    int operandB = memory[PC+1]%0x10;//Modular Instruction to find regB
    /*Instruction to Dissamble/Decode value via little endian. Left Shift << after each 
    instruction will allow you to define the order you want to print the result.*/
    int value = (memory[PC+2])+(memory[PC+3]<<8)+(memory[PC+4]<<16)+(memory[PC+5]<<24);
    /*Instruction to Dissamble/Decode dest jump via little endian.Composed of 5 bytes(1 byte is opcode,4 
    byte is jump destination)*/
    unsigned int dest = (memory[PC+4]<<24) + (memory[PC+3]<<16) + (memory[PC+2]<<8) + 
    memory[PC+1];
    
    
    /***********************************************************************************************************
    Once entered in an If-Else statment it will print the decoded result from the instruction(written a the top) 
    in the given printf order//[PC] is increased based on how many byte the instruction has , thus dissambiling the
    program based on the position and advancement of the program counter[PC] 
    ************************************************************************************************************/
    
    if(memory[PC]==0x00 || memory[PC]==0x10 || memory[PC]==0x90 )
    {
        printf("\n");  
        PC += 1;
    }
      
    else if(memory[PC]>=0x20 && memory[PC]<=0x26 || memory[PC]>=0x60 && memory[PC]<=0x63)
    { 
        printf(" %s, %s\n",register_names[operandA],register_names[operandB]);
        PC +=2;
    }
      
    else if(memory[PC]==0xA0|| memory[PC]==0xB0)
    {
        printf(" %s\n",register_names[operandA]);
        PC +=2;
    }
    
    else if (memory[PC]==0X30)
    {
       printf(" $%d, %s\n", value, register_names[operandB]);
       PC +=6;
    }
    
    else if(memory[PC]==0x40)
    {
       printf(" %s, %d(%s)\n",register_names[operandA] , value, register_names[operandB]);
       PC +=6;
    }
    
    else if(memory[PC]==0x50)
    {
       printf(" %d(%s), %s\n", value ,register_names[operandB], register_names[operandA]);
       PC +=6;
    }
      
    else if(memory[PC]>=0x70 && memory[PC]<=0x76 || memory[PC]==0x80)
    {
       printf(" %u\n", dest);
       PC +=5;
    }
    
  }

  return 0;
}

/****************************************************************************
N.B. You do not need to modify or work in this function.
Description:
This function reads in a y86 machine bytecode from file and
store them in an unsigned char array.
******************************************************************************/
unsigned int loadBinFromFile(const char *filename, unsigned char memory[], unsigned int memsize)
{
  unsigned int bytes_read = 0;

  unsigned int file_size = 0;

  FILE *pfile = fopen(filename, "rb");

  if (!pfile)
  {
    printf("Unable to load file %s, please check if the path and name are correct.\n", filename);
    return 0;
  }

  fseek(pfile, 0, SEEK_END);
  file_size = ftell(pfile);
  rewind(pfile);

  if (file_size > memsize)
  {
    printf("Program size exceeds memory size of %d.\n", memsize);
    return 0;
  }

  bytes_read = fread(memory, 1, file_size, pfile);

  if (bytes_read != file_size)
  {
    printf("Bytes read does not match the file size.\n");
    return 0;
  }

  fclose(pfile);

  return bytes_read;
}
