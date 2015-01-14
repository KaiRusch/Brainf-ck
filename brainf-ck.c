#include <stdio.h>

//Returns pointer to character buffer containing brainfuck code
char *open_file(char *fileName, int *codeLength)
{
  FILE *file = fopen(fileName,"rb");
  if(file == NULL)
    {
      return NULL;
    }
  fseek(file,0L,SEEK_END);
  int fileSize = ftell(file);
  *codeLength = fileSize;
  fseek(file,0L,SEEK_SET);
  char *fileBuffer = (char *)malloc(sizeof(char)*fileSize);
  fread(fileBuffer,sizeof(char),fileSize,file);
  fclose(file);
  return fileBuffer;
}

void execute_instruction(char *codeBuffer, char *array, int *pointer,int *instructionPointer,char instruction)
{
  *instructionPointer += 1;
  
  switch(instruction)
    {
    case '<': *pointer -= 1; break;
    case '>': *pointer += 1; break;
    case '+': *(array + *pointer) += 1; break;
    case '-': *(array + *pointer) -= 1; break;
    case '.': putchar(*(array+*pointer)); break;
    case ',': scanf("%c",(array + *pointer)); break;
    case '[':
      {
	if(*(array + *pointer) == 0)
	  {
	    int current = *instructionPointer;
	    int stackHeight = 0;
	    while(1)
	      {
		if(*(codeBuffer + current) == '[')
		  {
		    ++stackHeight;
		  }
		if(*(codeBuffer + current) == ']')
		  {
		    if(stackHeight == 0)
		      {
			*instructionPointer = current + 1;
			break;
		      }
		    else
		      {
			--stackHeight;
		      }
		  }

		++current;
	      }
	  }
      }break;
    case ']': 
      {
	if(*(array + *pointer) != 0)
	  {
	    int current = *instructionPointer - 2;
	    int stackHeight = 0;
	    while(1)
	      {
		if(*(codeBuffer + current) == ']')
		  {
		    ++stackHeight;
		  }
		if(*(codeBuffer + current) == '[')
		  {
		    if(stackHeight == 0)
		      {
			*instructionPointer = current + 1;
			break;
		      }
		    else
		      {
			--stackHeight;
		      }
		  }

		--current;
	      }	    
	  }
      }break;
    }
}

int main(int argc, char **argv)
{

  if(argc != 2)
    {
      return -1;
    }

  char *fileName = argv[1];

  int codeLength;
  char *codeBuffer;
  
  codeBuffer = open_file(fileName,&codeLength);

  if(codeBuffer == NULL)
    {
      return -1;
    }

  //The byte array
  char *array = (char *)malloc(sizeof(char)*30000);
  //Clear array
  for(int i = 0; i < 30000; ++i)
    {
      *(array + i) = 0;
    }

  //The location of the byte pointer
  int ptr = 0;

  int instructionIndex = 0;
  while(instructionIndex < codeLength)
    {
      char instruction = *(codeBuffer + instructionIndex);
      execute_instruction(codeBuffer, array,&ptr,&instructionIndex,instruction);
    }

  return 0;
  
}
