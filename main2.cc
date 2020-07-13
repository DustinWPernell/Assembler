#include <stdio.h>
#include <string.h>

void assemble(char[]);

int main(int argc, char **argv)
{
	
	if (argc > 1)
	{
		assemble(argv[1]);
	}
	else
	{
		char filename[128] = "test1.asm";		
		assemble(filename);
	}
}
