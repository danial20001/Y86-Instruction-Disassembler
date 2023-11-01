#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int convertStrToByteCode(const char *str, unsigned char inst[], int size);


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
       if(opcode== byte0[i]) 
        printf("%s", mnemonics[i]);
    }
}

int main(int argc, char **argv)
{
  FILE *pFile = NULL;

  char buffer[15];

  if (argc < 2)
  {
    pFile = fopen("./test2.txt", "r");
  }
  else
  {
    pFile = fopen(argv[1], "r");
  }

  if (pFile == NULL)
  {
    printf("Error open test file, please make sure they exist.\n");

    return 0;
  }

  while (fgets(buffer, 15, pFile) && strlen(buffer) > 1)
  {
      //This unsigned char array stores an instruction read from the file
      //As the largest y86 instruction is 6 bytes, there are 6 unsigned char in the array where
      //each represents a byte.
      unsigned char instruction[6] = {0, 0, 0, 0, 0, 0};
      convertStrToByteCode(buffer, instruction, 6);

    
      //Calling Function to print the opcode
      byte0(instruction[0]);
      int operandA = instruction[1]>>4;//Rightshift 4 bits to find regA
      int operandB = instruction[1]%0x10;//Modular Instruction to find regB
      /*Instruction to Dissamble/Decode value via little endian. Left Shift << after each 
      instruction will allow you to define the order you want to print the result.*/
      int value = (instruction[2])+(instruction[3]<<8)+(instruction[4]<<16)+(instruction[5]<<24);
      /*Instruction to Dissamble/Decode dest jump via little endian.Composed of 5 bytes(1 byte is opcode,4 
      byte is jump destination)*/
      unsigned int dest = (instruction[4]<<24) + (instruction[3]<<16) + (instruction[2]<<8) + 
      instruction[1];
      
     //////////////////////////////////////////////////////////////////////
    
      /*Once entered in an If-Else statment it will print the decoded result from the 
      instruction(written a the top) in the given print order.*/
    
      if(instruction[0]==0x00 || instruction[0]==0x10 || instruction[0]==0x90 )
      {
        printf("\n");
      }
      
      else if(instruction[0]>=0x20 && instruction[0]<=0x26 || instruction[0]>=0x60 && instruction[0]<=0x63 )
      { 
        printf(" %s, %s\n",register_names[operandA],register_names[operandB]);
      }
    
      else if(instruction[0]==0xA0|| instruction[0]==0xB0)
      {
        printf(" %s\n",register_names[operandA]);
      }
      
      else if(instruction[0]==0x30)
      { 
        printf(" $%d, %s\n", value, register_names[operandB]);
      }
      
      else if(instruction[0]==0x40)
      {
        printf(" %s, %d(%s)\n",register_names[operandA] , value, register_names[operandB]);
      }
    
      else if(instruction[0]==0x50)
      {
        printf(" %d(%s), %s\n", value ,register_names[operandB], register_names[operandA]);
      }
      
      else if(instruction[0]>=0x70 && instruction[0]<=0x76 || instruction[0]==0x80)
      {
        printf(" %u\n", dest);
      }
  }

  fclose(pFile);

  return 0;
}

/****************************************************************************
N.B. You do not need to modify or work in this function.
Description:
This function converts a line of machine code read from the text file
into machine byte code.
The machine code is stored in an unsigned char array.
******************************************************************************/
int convertStrToByteCode(const char *str, unsigned char inst[], int size)
{
  int numHexDigits = 0;
  char *endstr;
  //Each instruction should consist of at most 12 hex digits
  numHexDigits = strlen(str) - 1;
  //Convert the string to integer, N.B. this integer is in decimal
  long long value = strtol(str, &endstr, 16);

  int numBytes = numHexDigits >> 1;
  int byteCount = numHexDigits >> 1;

  while (byteCount > 0)
  {
    unsigned long long mask = 0xFF;
    unsigned long shift = (numBytes - byteCount) << 3;

    inst[byteCount - 1] = (value & (mask << shift)) >> shift;
    byteCount--;
  }

  //Return the size of the instruction in bytes
  return numHexDigits >> 1;
}


