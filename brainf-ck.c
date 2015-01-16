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

//Returns false if instruction leads to fatal error
void execute_instruction(char *array, char **pointer,char **instructionPointer)
{
  switch(**instructionPointer)
    {
    case '<': --*pointer; break;
    case '>': ++*pointer; break;
    case '+': ++**pointer; break;
    case '-': --**pointer; break;
    case '.': putchar(**pointer); break;
    case ',': **pointer = getchar(); break;
    case '[':
      {
	if(!**pointer)
	  {
	    char *current = *instructionPointer;
	    int stackHeight = 0;
	    while(1)
	      {
		if(*current == '[')
		  {
		    ++stackHeight;
		  }
		if(*current == ']')
		  {
		    if(stackHeight == 1)
		      {
			*instructionPointer = current;
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
	if(**pointer)
	  {
	    char *current = *instructionPointer;
	    int stackHeight = 0;
	    while(1)
	      {
		if(*current == ']')
		  {
		    ++stackHeight;
		  }
		if(*current == '[')
		  {
		    if(stackHeight == 1)
		      {
			*instructionPointer = current;
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

    *instructionPointer += 1;
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
  char *ptr = array;

  char *instructionPtr = codeBuffer;
  while(instructionPtr < codeBuffer + codeLength)
    {
      execute_instruction(array,&ptr,&instructionPtr);
    }

  return 0;
  
}
