#include <stdio.h>
#include <string.h>

#define LIMIT 10000
#define LINE_SIZE 128

//For part 1
void assemble(char[]);
int findOrigin(FILE*);
int firstPass(FILE*, int[], int);
void printLabels(int[]);

//For part 2
int getAdd(char[]);
int getAnd(char[]);
int getTrap(char[]);
int getNot(char[]);
int getLd(char[], int[], int);
int getLdr(char[]);
int getSt(char[], int[], int);
int getStr(char[]);
int getBr(char[], int[], int);
int getIt(int, char[]);
int secondPass(FILE*, int[], int);

//Helper functions
void processLine(char[], char[]);
void toLower(char[], char[]);
void removeSpace(char[], char[]);
int checkOperand(char[]);
int checkOperand(char[], char[]);

//Part 1
void assemble(char filename[]){
	//Open the file for reading
    FILE *infile = fopen( filename , "r" );
    
	if (infile != NULL) {    
		//Create labels array and set all elements to -1.  
		int labels[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
		
		int lc = findOrigin(infile);
		if (lc > -1){
			//Read in label values
			if (!firstPass(infile, labels, lc)){
				//Show the labels.
				printLabels(labels);
				
				//The following is for part 2
				secondPass(infile, labels, lc);
			}
		}
		
		//Close the file
		fclose(infile);
		
	} 
	else {	
		printf("Can't open input file.\n");		
	}    

}

void printLabels(int labels[]){
	//labels = {-1, -1, 12289, -1, 12292, -1, -1, -1, -1, -1}
	
	printf("labels = {");
	
	for(int i = 0; i < 9; i++){
		printf("%d, ",labels[i]);
	}
	
	printf("%d}\n",labels[9]);
}

int findOrigin(FILE *infile)
{
	//Each trip through the while loop will read the next line of infile
	//into the line char array as a null terminated string.
	char line[LINE_SIZE]; 
	
	//For holding error message
	int error = 0;
	
	//The variable lineCount keeps track of how many lines have been read.
	//It is used to guard against infinite loops.  Don't remove anything
	//that has to do with linecount.
	int lineCount = 0; //
	
	//For getting out of the while loop.
	int done = 0;
	
	//For checking if found
	int found = 0;
	
	//For storing IC
	int lc = 0;
	
	//For getting rid of the trailing newline
	char c;
	
	//Read lines until EOF reached or LIMIT lines read.  Limit prevent infinite loop.
	while (fscanf(infile, "%[^\n]s", line) != EOF && lineCount < LIMIT && !done){
		lineCount++;
		
		fscanf(infile, "%c", &c);  //Get rid of extra newline.
		
		//At this point, line contains the next line of the ASM file.
		//Put your code here for seeing if line is an origin.
		//Options:
		//	1. line is an origin (save value, set done = 1).  
		//  2. line is a blank line (skip).
		//  3. line is a comment (skip).
		//  4. line is anything else (print error, set done = 1).
		
		char newLine[LINE_SIZE];
		processLine(line, newLine);
		
		char first5[LINE_SIZE];	
		
		if(strlen(newLine) != 0 && newLine[0] != ';'){
			for(int i = 0; i < 5; i++){
				if(!line[i]){
					first5[i] = 0;
					i = 5;
				}
				else{
					first5[i] = newLine[i];
				}
			}
		
			if(!strcmp(first5, ".orig")){	
				if(newLine[5] == 'x'){
					sscanf(&newLine[6], "%x", &lc);
					found = done = 1;
				}
				else if (newLine[5] >= 48 && newLine[5] <= 57) {
					sscanf(&newLine[5], "%d", &lc);
					found = done = 1;
				}
			}
			else if(lineCount!=1){
				done = 1;
				found = 0;
				error = 1;
			}
		}
		
		//Set the line to empty string for the next pass.
		line[0] = 0;
	}
	
	
	//At this point you must decide if an origin was found or not.
	//How you do this is up to you.
	//If a good origin was found, check the size.  
	//		if it is too big, print an error and return -1.
	//      if it is not too big, return the value.
	//If a good origin was NOT found, print the error message and return -1.
	if(found){
		if(lc> 0xffff){
			lc = -1;
			error = 2;
		}
	}
	else
		lc = -1;
	if(error){
		if(error == 1)
			printf("ERROR 1: Missing origin directive. Origin must be first line in program.\n");	
		if(error == 2)
			printf("ERROR 2: Bad origin address. Address too big for 16 bits.\n");		
	}
	
	
	return lc;
}

int firstPass(FILE *infile, int labels[], int lc)
{
	//Create a while loop similar to findOrigin.
	//You can rewind if you want to but you shouldn't have to.
	
	//Read a line.
		//If the line is a comment, a blank line or the .orig directive, don’t do anything.
		//If the line is a label on a line by itself, save the lc to the labels array at the appropriate position.
		//	For example, if L3, store the lc value in labels[3].
		//If the line is a label followed by .fill, save the lc in labels AND increment lc.
		//If the line is .end, return 0 for done with no error.
		//If the end of file is reached before .end is found print the error and return -1.
		//If the line is one of the allowed instructions (ADD, AND, NOT, LD, LDR, ST, STR, BR, and TRAP) increment the lc.
		//If the line is anything else print the unknown instruction error and return -1.

	//Each trip through the while loop will read the next line of infile
	//into the line char array as a null terminated string.
	char line[LINE_SIZE]; 
	
	//The variable lineCount keeps track of how many lines have been read.
	//It is used to guard against infinite loops.  Don't remove anything
	//that has to do with linecount.
	int lineCount = 0; //
	
	//For getting out of the while loop.
	int done = 0;
	
	//For checking if found
	int found = 0;
	
	//For getting rid of the trailing newline
	char c;
	
	int endFound = 0;
	
	int error = 0;
	
	//Read lines until EOF reached or LIMIT lines read.  Limit prevent infinite loop.
	while (fscanf(infile, "%[^\n]s", line) != EOF && lineCount < LIMIT && !done)
	{
		lineCount++;
		
		fscanf(infile, "%c", &c);  //Get rid of extra newline.
		
		char newLine[LINE_SIZE];
		processLine(line, newLine);
		char first4[LINE_SIZE];
				
		if(strlen(newLine) != 0 && newLine[0] != ';'){
			for(int i = 0; i < 4; i++){
				if(!newLine[i]){
					first4[i] = 0;
					i = 5;
				}
				else{
					first4[i] = newLine[i];
				}
			}
			first4[4] = 0;
			
			if(!strcmp(first4, ".end")){	
				done = endFound = 1;
			}
			else if(strcmp(first4, ".ori")){
				int op = checkOperand(newLine);
				
				if(op > 0)
					lc++;
					
				if(!op && newLine[0] == 'l'){
					int index = 0;
					if(newLine[1] >= 48 && newLine[1] <= 57){
						index = newLine[1] - 48;
						labels[index] = lc;
						if(newLine[2] == '.')
							lc++;
					}
				}
				else if(op == -1){
					done = endFound  = error = 2;
					printf("%s\n",line);
				}
			}
		}
		line[0] = 0;
	}
	
	
	if(!endFound){
		error = 1;
	}
	
	if(error){
		if(error == 1)
			printf("ERROR 4: Missing end directive.\n");	
		if(error == 2)
			printf("ERROR 3: Unknown instruction.\n");	
		found = -1;
	}
	
	return found;
}

//Part 2
int secondPass(FILE* infile, int labels[], int lc)
{
	//If the line is a blank line (possibly contains spaces and tabs), ignore it.
	//If the line begins with a semicolon (possibly contains spaces and tabs), ignore it.
	//If the line starts with .orig ignore it.
	//If the line is a label on a line by itself, ignore it.
	//If the line is a label followed by .fill, increment the lc and print 0000. (All fills will be .fill 0)
	//If the line is the .end directive you should return from secondPass.
	//If the line is an instruction, increment lc and call the appropriate get method as described in the next step. 
		//The get method will return the integer value of the machine language interpretation of the instruction.

	//Each trip through the while loop will read the next line of infile
	//into the line char array as a null terminated string.
	char line[LINE_SIZE]; 
	
	//The variable lineCount keeps track of how many lines have been read.
	//It is used to guard against infinite loops.  Don't remove anything
	//that has to do with linecount.
	int lineCount = 0; //
	
	//For getting out of the while loop.
	int done = 0;
	
	//For checking if found
	int found = 0;
	
	//For getting rid of the trailing newline
	char c;
		
	int error = 0;
	
	rewind(infile);
	printf("%04X\n",lc);
	//Read lines until EOF reached or LIMIT lines read.  Limit prevent infinite loop.
	while (fscanf(infile, "%[^\n]s", line) != EOF && lineCount < LIMIT && !done){
		lineCount++;
		
		fscanf(infile, "%c", &c);  //Get rid of extra newline.
		
		char newLine[LINE_SIZE];
		processLine(line, newLine);
		char first4[LINE_SIZE];

		if(strlen(newLine) != 0 && newLine[0] != ';'){
			for(int i = 0; i < 4; i++){
				if(!line[i]){
					first4[i] = 0;
					i = 5;
				}
				else{
					first4[i] = newLine[i];
				}
			}
			first4[4] = 0;

			if(!strcmp(first4, ".end")){	
				done = 1;
			}
			else if(strcmp(first4, ".ori")){
				if(!checkOperand(newLine) && newLine[0] == 'l' && newLine[2] == '.'){
					lc++;
					printf("0000\n");
				}
				
				int op = checkOperand(newLine);
				if(op > 0){
					lc++;
					int result = 0;
					//ADD = 8, AND = 6, NOT = 7, LD = 5, LDR = 4, ST = 3, STR = 2, BR = 9, TRAP = 1
					if(op == 1)
						result=getTrap(newLine);
					if(op == 2)
						result=getStr(newLine);
					if(op == 3)
						result=getSt(newLine, labels, lc);
					if(op == 4)
						result=getLdr(newLine);
					if(op == 5)
						result=getLd(newLine, labels, lc);
					if(op == 6)
						result=getAnd(newLine);
					if(op == 7)
						result=getNot(newLine);
					if(op == 8)
						result=getAdd(newLine);
					if(op == 9)
						result=getBr(newLine, labels, lc);
								
					if(result == -1)
					{
						done = error = 1;
					}
					else if(result)
						printf("%04X\n",result); // can print leading 0s
					
				}
			}
		}
		
		line[0] = 0;
	}
	
	if(error){
		if(error == 1)
			printf("ERROR 3: Unknown instruction.\n");	
		error = -1;
	}
	
	return error;
}


int getAdd(char line[])
{
	//The add instruction has both a register and an immediate form. Your getAdd function should recognize this appropriately.
	//0001 000 000 1 00000
	//0001 000 000 000 000
	//ADDR0,R1,R2
	//    4  7   10
	int instruct = 0x1000;
	
	int dr = line[4] - 48;
	int sr1 = line[7] - 48;
	
	dr = dr <<9;
	instruct = instruct | dr;

	sr1 = sr1 <<6;
	instruct = instruct | sr1;
	
		
	if(line[9] == '#'){
		int imm = 0;
		sscanf(&line[10], "%d", &imm);
		
		instruct = instruct | 0x20;
		//mask 001f
		imm = imm & 0x1f;
		instruct = instruct | imm;
		
	}
	else{
		int sr2 = line[10] - 48;
		instruct = instruct | sr2;
	}
	
	return instruct;
}

int getAnd(char line[])
{
	//Very similar to getAdd. You could probably copy getAdd and make minor revisions.
	//0101 000 000 1 00000
	//0101 000 000 000 000
	//AnDR6,R7,R0
	//   4   7  10
	int instruct = 0x5000;
	
	int dr = line[4] - 48;
	int sr1 = line[7] - 48;
	
	dr = dr <<9;
	instruct = instruct | dr;

	sr1 = sr1 <<6;
	instruct = instruct | sr1;
	
		
	if(line[9] == '#'){
		int imm = 0;
		sscanf(&line[10], "%d", &imm);
		
		instruct = instruct | 0x20;
		//mask 001f
		imm = imm & 0x1f;
		instruct = instruct | imm;
		
	}
	else{
		int sr2 = line[10] - 48;
		instruct = instruct | sr2;
	}
	
	return instruct;
}

int getTrap(char line[])
{
	//The trap instruction will only have hex values x20 through x25 as operands.
	//1111 0000 00000000
	int instruct = 0xF000;
		
	int imm = 0;

	sscanf(&line[5], "%x", &imm);
	
	instruct = instruct | imm;
	
	return instruct;
}

int getNot(char line[])
{
	//Note that bits 0 through 4 are always 1 in the NOT instruction.
	//1001 000 000 111111
	
	int instruct = 0x9000;
	
	int dr = line[4] - 48;
	int sr1 = line[7] - 48;
	
	dr = dr <<9;
	instruct = instruct | dr;

	sr1 = sr1 <<6;
	instruct = instruct | sr1;
	
	instruct = instruct | 0x3f;
	
	
	return instruct;
}

int getLd(char line[], int labels[], int lc)
{
	//Uses PCOFFSET. Note that the LC should be incremented before calling. So when you calculate the 
	//PCOFFSET you DO NOT NEED TO ADD 1 because it has already been added to the LC.
	//0010 000 000000000
	//ldR0,L9
	//   3   6
	int instruct = 0x2000;
	
	int dr = line[3] - 48;
	int imm = 0;
	
	dr = dr <<9;
	instruct = instruct | dr;

	if(line[5] == 'l'){
		int index = 0;
		if(line[6] >= 48 && line[6] <= 57){
			index = line[6] - 48;
			imm = labels[index];
		}
	}
	else {
		sscanf(&line[6], "%d", &imm);
	}
	imm = imm - lc;
		
	instruct = instruct | imm;
	return instruct;
}

int getLdr(char line[])
{
	//Note that the offset IS NOT a PCOFFSET. This instruction DOES NOT need labels or the lc.
	//0110 000 000 000000
	//ldrR0,R1,#-1
	
	int instruct = 0x6000;
	
	int dr = line[4] - 48;
	int sr1 = line[7] - 48;
	int imm = 0;
	
	dr = dr <<9;
	instruct = instruct | dr;
	
	sr1 = sr1 <<6;
	instruct = instruct | sr1;
	
	sscanf(&line[10], "%d", &imm);
	imm = imm & 0x3f;
	instruct = instruct | imm;

	return instruct;
}

int getSt(char line[], int labels[], int lc)
{
	//Very similar to getLd. You could probably copy getLd and make minor revisions.
	//0011 000 00000000
	int instruct = 0x3000;
	
	int dr = line[3] - 48;
	int imm = 0;
	
	dr = dr <<9;
	instruct = instruct | dr;

	if(line[5] == 'l'){
		int index = 0;
		if(line[6] >= 48 && line[6] <= 57){
			index = line[6] - 48;
			imm = labels[index];
		}
	}
	else {
		sscanf(&line[6], "%d", &imm);
	}
	imm = imm - lc;
		
	instruct = instruct | imm;
	return instruct;
}

int getStr(char line[])
{
	//Very similar to getLdr. You could probably copy getLdr and make minor revisions.
	//0111 000 000 00000
	
	int instruct = 0x7000;
	
	int dr = line[4] - 48;
	int sr1 = line[7] - 48;
	int imm = 0;
	
	dr = dr <<9;
	instruct = instruct | dr;
	
	sr1 = sr1 <<6;
	instruct = instruct | sr1;
	
	sscanf(&line[10], "%d", &imm);
	imm = imm & 0x3f;
	instruct = instruct | imm;

	return instruct;
}

int getBr(char line[], int labels[], int lc)
{
	//Uses the PCOFFSET. Pay particular attention to the way NZP are stored in the instruction.
	//0000 0 0 0 000000000
	int instruct = 0x0000;
	
	char f = line[2];
	char s = line[3];
	char l = line[4];
	
	int nzp = 0;
	
	if(f=='n'||s=='n'||l=='n')
		nzp = nzp | 4;
	if(f=='z'||s=='z'||l=='z')
		nzp = nzp | 2;
	if(f=='p'||s=='p'||l=='p')
		nzp = nzp | 1;
	
	if(!nzp)
		nzp = 7;
	
	nzp = nzp <<9;
	instruct = instruct | nzp;
	
	
	int imm = 0;

	for (int i = 0; i < LINE_SIZE; i++)
	{
		if(line[i] == 'l')
		{
			int index = 0;
			index = line[i+1] - 48;
			imm = labels[index];
			i=LINE_SIZE;			
		}
	}
	imm = imm - lc;
	imm = imm & 0x1ff;
	instruct = instruct | imm;
	
	return instruct;
}

int getIt(int, char[]){
	return 0;
}


//Helper functions
void processLine(char line[], char newLine[]){
	char temp[LINE_SIZE];
	removeSpace(line, temp);
	toLower(temp, newLine);
}

void toLower(char line[], char newLine[]){
	for(int i = 0; i < LINE_SIZE; i++){
		if(!line[i]){
			newLine[i] = 0;
			i = LINE_SIZE;
		}
		else if(line[i] >= 65 && line[i] <= 90){
			newLine[i] = line[i] + 32;
		}
		else{
			newLine[i] = line[i];
		}
	}
}

void removeSpace(char line[], char newLine[])
{
	int newIndex = 0;
	for(int i = 0; i < LINE_SIZE; i++){
		if(!line[i]){
			newLine[newIndex] = 0;
			i = LINE_SIZE;
		}
		else if(line[i]!=' ' && line[i]!='\t' && line[i]!='\r' && line[i]!='\n'){
			newLine[newIndex] = line[i];
			newIndex++;
		}
	}
}

int checkOperand(char line[])
{
	//ADD = 8, AND = 6, NOT = 7, LD = 5, LDR = 4, ST = 3, STR = 2, BR = 9, TRAP = 1
	int found = 0;
	char op4[5];
	char op3[4];
	char op2[3];
	
	for(int i = 0; i < 5; i++){
		if(line[i]){
			op4[i] = line[i];
			if(i < 3){
				op3[i] = line[i];
			}	
			if(i < 2){
				op2[i] = line[i];
			}
		}
		else
		{
			op4[i] = 0;
			if(i <= 3){
				op3[i] = 0;
			}
			if(i <= 2){
				op2[i] = 0;
			}
			i = 4;
		}
	}
	op4[4] = op3[3] = op2[2] = 0;
	

	if(!strcmp(op4, "trap"))
		found = 1;
	else if(!strcmp(op3, "str")){
		if(op4[3] == 'r'){
			found = 2;
		}
		else{
			found = 3;
		}
	}
	else if(!strcmp(op3, "ldr")){
		if(op4[3] == 'r'){
			found = 4;
		}
		else{
			found = 5;
		}
	}
	else if(!strcmp(op3, "and"))
		found = 6;
	else if(!strcmp(op3, "not"))
		found = 7;
	else if(!strcmp(op3, "add"))
		found = 8;
	else if(!strcmp(op2, "br"))
		found = 9;
	else if(!strcmp(op2, "st"))
		found = 3;
	else if(!strcmp(op2, "ld"))
		found = 5;
	else if(op4[0] != 'l' && (op4[1] < 48 || op4[1] > 57))
		found = -1;
	
	return found;
}

