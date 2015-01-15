#include <stdio.h>

int main(int argc, char **argv)
{
  if(argc != 2)
    {
      printf("Please enter 1 input file\n");
      return -1;
    }

  char *inputFileName = argv[1];
  FILE *inputFile = fopen(inputFileName, "r");

  if(!inputFile)
    {
      printf("Could not open file\n");
      return -1;
    }

  int extIndex = 0;
  for(extIndex = 0; inputFileName[extIndex] != '\0'; ++extIndex)
    {
      if(inputFileName[extIndex] == '.')
	{
	  break;
	}
    }

  char *outputFileName = (char *)malloc(sizeof(char) * (extIndex+3));

  for(int i = 0; i < extIndex; ++i)
    {
      outputFileName[i] = inputFileName[i];
    }

  outputFileName[extIndex] = '.';
  outputFileName[extIndex+1] = 'c';
  outputFileName[extIndex+2] = '\0';

  FILE *outputFile = fopen(outputFileName,"w");

  fprintf(outputFile,"#include<stdio.h>\nint main(){\nchar array[30000] = {0};\nchar *ptr = (char *)array;\n");

  char instruction;
  while(fscanf(inputFile,"%c",&instruction) != EOF)
    {
      switch(instruction)
	{
	case '<': fprintf(outputFile,"--ptr;\n"); break;
	case '>': fprintf(outputFile,"++ptr;\n"); break;
	case '+': fprintf(outputFile,"++*ptr;\n"); break;
	case '-': fprintf(outputFile,"--*ptr;\n"); break;
	case '.': fprintf(outputFile,"putchar(*ptr);\n"); break;
	case ',': fprintf(outputFile,"*ptr = getchar();\n"); break;
	case '[': fprintf(outputFile,"while(*ptr){\n"); break;
	case ']': fprintf(outputFile,"}\n"); break;
	}
    }

  fprintf(outputFile,"return 0;\n}");

  fclose(inputFile);
  fclose(outputFile);
  

  return 0;
}
