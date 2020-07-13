#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <regex.h>

#define FILENAME "assemble.cc"
#define FILESIZE 16384
#define TEXTSIZE 1024
#define STDOUT_FILENO 1
#define TRUE 1
#define FALSE 0
#define NUM_MATCH 20
#pragma GCC diagnostic ignored "-Wwrite-strings"

regex_t regex;

regmatch_t matches[NUM_MATCH];
int reti;

int testNoMain();
int testAssemble();
int testGetAdd();
int testGetAnd();
int testGetBr();
int testGetLd();
int testGetLdr();
int testGetNot();
int testGetSt();
int testGetStr();
int testGetTrap();
int testPrintLabels();
int testFirstPass();
int testSecondPass();

int getMain(char *method);
int getMethod(char **sig, int num, char *method);
int readStudentCFile(char *filename);
int loopExists();
int getStudentFile();
int loopExists(char[]); //Checks to see if a loop exists in a specified function.
int containsForLoop(char[]); //Check specified method for presence of "for" loop.
int containsWhileLoop(char[]);//Check specified method for presence of "while" loop.
int containsDoLoop(char[]);//Check specified method for presence of "do" loop.
int containsTypedef();//Check  for presence of "for" loop.
int containsStruct();//Check for presence of "truct" loop.
int containsAssembleWords(); //Look for specific declarations and function calls in assemble.
int containsWord(char[]); //Loof for a specific word in a function.
int checkBadStuff(); //Stop use of continue, break, goto, and bool.
int continueFound(); //Check for continue usage.
int boolFound(); //Check for the bool type.
int gotoFound(); //Check for goto usage.
int breakWithNoSwitchFound(); //Check for break with no switch.
int switchFound(); //Check for switch.

int countFunctions(); //Counts the number of student functions.
void copyStr(char*, char*, int, int); //Copys one string to another start and end positions are given
int checkBadHeaders();  //Check for disallowed libraries
int checkBadLib(char*);  //Helper for checkBadHeaders
void getParamRE(char*, char*); //Creates a regular expression for parameters given an array of types.


char input[TEXTSIZE];
char msg[TEXTSIZE];
char studentFile[FILESIZE];
char method[FILESIZE];
int stdout_save;


//Allowed library headers
int allowedLibNum = 2;
char *allowedLibs[] = {"stdio.h", "string.h"};


int main() {
	
	int e = 0;

	e = readStudentCFile(FILENAME);
	if (e == 1)
	{
		printf("Cannot open file %s.\n", FILENAME);
		exit(1);
	}
	else if (e == 2) 
	{
		printf("File %s is too big to read.\n", FILENAME);
		exit(2);
	}
	
	if (testNoMain()) {
		printf("ERROR: File %s has a main method.  \n", FILENAME);
		e++;
	}
	if (testAssemble()) {
		if (e == 1) printf("In %s:\n", FILENAME);
		printf("ERROR: Return type or parameter wrong in assemble.\n");
		e++;
	}
	//containsAssembleyWords MUST IMMEDIATLY FOLLOW testAssemble so 
	//that the assemble funciton is loaded. 
	if (!containsAssembleWords())
	{
		if (e == 1) printf("In %s:\n", FILENAME);
		e++;
	}
	if (testPrintLabels()) {
		if (e == 1) printf("In %s:\n", FILENAME);
		printf("ERROR: Return type or parameter wrong in printLabels.\n");
		e++;
	}
	if (testFirstPass()) {
		if (e == 1) printf("In %s:\n", FILENAME);
		printf("ERROR: Return type or parameter wrong in firstPass.\n");
		e++;
	}
	if (testSecondPass()) {
		if (e == 1) printf("In %s:\n", FILENAME);
		printf("ERROR: Return type or parameter wrong in secondPass.\n");
		e++;
	}
	if (testGetLd()) {
		if (e == 1) printf("In %s:\n", FILENAME);
		printf("ERROR: Return type or parameter wrong in getLd.\n");
		e++;
	}
	if (testGetLdr()) {
		if (e == 1) printf("In %s:\n", FILENAME);
		printf("ERROR: Return type or parameter wrong in getLdr.\n");
		e++;
	}
	if (testGetNot()) {
		if (e == 1) printf("In %s:\n", FILENAME);
		printf("ERROR: Return type or parameter wrong in getNot.\n");	
		e++;
	}
	if (testGetSt()) {
		if (e == 1) printf("In %s:\n", FILENAME);
		printf("ERROR: Return type or parameter wrong in getSt.\n");
		e++;
	}
	if (testGetStr()) {
		if (e == 1) printf("In %s:\n", FILENAME);
		printf("ERROR: Return type or parameter wrong in getStr.\n");
		e++;
	}
	if (testGetTrap()) {
		if (e == 1) printf("In %s:\n", FILENAME);
		printf("ERROR: Return type or parameter wrong in getTrap.\n");
		e++;
	}
	
	if (checkBadStuff()) {
		if (e == 1) printf("In %s:\n", FILENAME);
		//Check bad stuff makes sure continue, goto, and break without
		//a switch statement are not used.  
		//Also prevents using bool as a data type.
		//Check bad stuff prints its own errors.
		e++;
	}	
	
	if (!e)
	{
		printf("No errors found in function definitions in %s.\n", FILENAME);
	}	
	return 0;
}

int testNoMain()
{
	return !getMain(method);
}

int testAssemble()
{
	char *signature[3];
	signature[0] = "void";
	signature[1] = "assemble";
	signature[2] = "*char";
	return getMethod(signature, 3, method);	
}

int testPrintLabels()
{
	char *signature[3];
	signature[0] = "void";
	signature[1] = "printLabels";
	signature[2] = "*int";
	return getMethod(signature, 3, method);	
}

int testFirstPass()
{
	char *signature[5];
	signature[0] = "int";
	signature[1] = "firstPass";
	signature[2] = "*FILE";
	signature[3] = "*int";
	signature[4] = "int";
	return getMethod(signature, 5, method);	
}

int testSecondPass()
{
	char *signature[5];
	signature[0] = "int";
	signature[1] = "secondPass";
	signature[2] = "*FILE";
	signature[3] = "*int";
	signature[4] = "int";
	return getMethod(signature, 5, method);	
}

int testGetAdd()
{
	char *signature[3];
	signature[0] = "int";
	signature[1] = "getAdd";
	signature[2] = "*char";
	return getMethod(signature, 3, method);	
}

int testGetAnd()
{
	char *signature[3];
	signature[0] = "int";
	signature[1] = "getAnd";
	signature[2] = "*char";
	return getMethod(signature, 3, method);	

}

int testGetBr()
{
	char *signature[5];
	signature[0] = "int";
	signature[1] = "getBr";
	signature[2] = "*char";
	signature[3] = "*int";
	signature[4] = "int";
	return getMethod(signature, 5, method);	
}

int testGetLd()
{
	char *signature[5];
	signature[0] = "int";
	signature[1] = "getLd";
	signature[2] = "*char";
	signature[3] = "*int";
	signature[4] = "int";
	return getMethod(signature, 5, method);	
}


int testGetLdr()
{
	char *signature[3];
	signature[0] = "int";
	signature[1] = "getLdr";
	signature[2] = "*char";
	return getMethod(signature, 3, method);	
}


int testGetNot()
{
	char *signature[3];
	signature[0] = "int";
	signature[1] = "getNot";
	signature[2] = "*char";
	return getMethod(signature, 3, method);	
}


int testGetSt()
{
	char *signature[5];
	signature[0] = "int";
	signature[1] = "getSt";
	signature[2] = "*char";
	signature[3] = "*int";
	signature[4] = "int";
	return getMethod(signature, 5, method);
}

int testGetStr()
{
	char *signature[3];
	signature[0] = "int";
	signature[1] = "getStr";
	signature[2] = "*char";
	return getMethod(signature, 3, method);	
}

int testGetTrap()
{
	char *signature[3];
	signature[0] = "int";
	signature[1] = "getTrap";
	signature[2] = "*char";
	return getMethod(signature, 3, method);	
}

//Make sure user opens file, creates labels array, calls firstPass, 
//calls printLables and closes the file.
int containsAssembleWords()
{
    if (!containsWord("=\\s*fopen\\s*([^,]*,[^)]*)")) 
    {
        printf("Function assemble failed.  Assemble should open file with fopen.\n");
        return 0;
    }
    else if (!containsWord("int\\s\\+labels\\s*\\[[^]]*]")) 
    {
        printf("Function assemble failed.  Assemble should create an int array named labels.\n");
        return 0;
    }
    else if (!containsWord("lc\\s*=\\s*findOrigin\\s*(\\s*[^\\)]\\+\\s*)")) 
    {
        printf("Function assemble failed.  Assemble should create a variable named lc and set it with a call to findOrigin.\n");
        return 0;
    }
    else if (!containsWord("firstPass\\s*([^,]*,[^)]*)")) 
    {
        printf("Function assemble failed.  Assemble should call firstPass.\n");
        return 0;
    }
    else if (containsWord("printLabels\\s*([^)]*)")) 
    {
        printf("Function assemble failed.  Assemble should NOT call printLabels.\n");
        return 0;
    }
    else if (!containsWord("fclose\\s*([^)]*)")) 
    {
        printf("Function assemble failed.  Assemble should close open file with fclose.\n");
        return 0;
    }		
    else if (!containsWord("secondPass\\s*([^,]*,[^)]*)")) 
    {
        printf("Function assemble failed.  Part 2 function assemble should call secondPass.\n");
        return 0;
    }		
    return 1;
}

//Check to see that all headers in the student file are in the 
//list of allowed headers.
int checkBadHeaders()
{
	if (strlen(studentFile) == 0) return 0;
	
	/* Execute regular expression */
	char *regexp = "\\s*#include\\s\\+<\\([^>]\\+\\)>";
	char *src = studentFile;
	char libName[20];
	int start = 0;
	int end = 0;
	int count = 0;
	
	//Compile the regular expression
	reti = regcomp(&regex, regexp, 0);
	
	//Run the regular expression
	reti = regexec(&regex, src, NUM_MATCH, matches, 0);
	
	while (!reti && count < 30) {
		
		//If a match was found, process it.  
		//Matches[1] is library text within < >
		start = (int)matches[1].rm_so;
		end  = (int)matches[1].rm_eo;
		
		//Copy the library name to libName. 
		copyStr(libName, src, start, end);
		
		if (checkBadLib(libName))
		{
			msg[0] = '\0';
			sprintf(msg, 
				"Library %s not allowed.\n", 
				libName);
			printf("%s", msg);
			return 1; //libName is not in list
		}
		
		//Search for next match AFTER the end of last match
		src += (int)matches[0].rm_eo;
		reti = regexec(&regex, src, NUM_MATCH, matches, 0);
		count++;
	}
	return 0;
}

//HELPER FUNCTION for checkBadHeaders
//Check this name against all of the ones in the allowedLibs array.
int checkBadLib(char *libName)
{
	for (int i = 0; i < allowedLibNum; i++)
	{
		if (strcmp(allowedLibs[i], libName) == 0)
		{
			//If the library name matches on in the array
			//it is not bad.  Return false.
			return 0 ;

		}
	}
	//The library name was not found in the list.
	//It IS bad, return true.
	return 1;
}

//HELPER FUNCTION for checkBadHeaders	
//Copy from start to end in src to dest.  Make dest a proper
//string by null terminating it.
void copyStr(char *dest, char *src, int start, int end)
{
	int i = 0;
	int j = 0;
	for (i = start; i < end; i++)
	{
		dest[j++] = src[i];
	}
	dest[j++] = '\0'; //Null terminate the string
}

//Check specified method for ANY kind of looop
int loopExists(char method[])
{
	return 	containsForLoop(method) || 
		containsWhileLoop(method) || 
		containsDoLoop(method);
}

//Find a for loop in the given method.  
//The method is passed in as a c-style string.
int containsForLoop(char method[])
{
	if (strlen(method) == 0) return 0;
	
	/* Execute regular expression */
	char *regexp = "\\s*for\\s*([^;]*;[^;]*;[^)]*)";
	
	//Compile the regular expression
	reti = regcomp(&regex, regexp, 0);
	
	//Run the regular expression
	reti = regexec(&regex, method, NUM_MATCH, matches, 0);
	
	//if (!reti) printf("\n\n\n----ForLoopFound----\n\n\n");
	//else printf("\n\n\n----ForLoopNotFound----\n\n\n");
	return !reti;
}

//Find a while loop in the given method.  
//The method is passed in as a c-style string.
int containsWhileLoop(char method[])
{
	if (strlen(method) == 0) return 0;
	
	/* Execute regular expression */
	char *regexp = "\\s*while\\s*([^)]*)";
	
	//Compile the regular expression
	reti = regcomp(&regex, regexp, 0);
	
	//Run the regular expression
	reti = regexec(&regex, method, NUM_MATCH, matches, 0);
	
	//if (!reti) printf("\n\n\n----WhileLoopFound----\n\n\n");
	//else printf("\n\n\n----WhileLoopNotFound----\n\n\n");
	return !reti;
}

//Find a do loop in the given method.  
//The method is passed in as a c-style string.
int containsDoLoop(char method[])
{
	if (strlen(method) == 0) return 0;
	
	/* Execute regular expression */
	char *regexp = "do\\s*{.*}\\s*while\\s*\\s*([^)]*)\\s*;";
	
	//Compile the regular expression
	reti = regcomp(&regex, regexp, 0);
	
	//Run the regular expression
	reti = regexec(&regex, method, NUM_MATCH, matches, 0);
	
	//if (!reti) printf("\n\n\n----DoWhileLoopFound----\n\n\n");
	//else printf("\n\n\n----DoWhileLoopNotFound----\n\n\n");
	return !reti;
}

//Check to see if this file contains ANY typedef or struct keywords.
int checkTypedefStruct()
{
	return containsTypedef() || containsStruct(); 	
}


//Find a typedef in the given method.  
//The method is passed in as a c-style string.
int containsTypedef()
{		
	if (strlen(studentFile) == 0) return 0;
	
	/* Execute regular expression */
	char *regexp = "\\W\\+typedef\\W\\+";
	
	
	//Compile the regular expression
	reti = regcomp(&regex, regexp, 0);
	
	//Run the regular expression
	reti = regexec(&regex, studentFile, NUM_MATCH, matches, 0);
	
	return !reti;
}

//Find a struct in the given method.  
//The method is passed in as a c-style string.
int containsStruct()
{
	if (strlen(studentFile) == 0) return 0;
	
	/* Execute regular expression */
	char *regexp = "\\W\\+struct\\W\\+";
	
	//Compile the regular expression
	reti = regcomp(&regex, regexp, 0);
	
	//Run the regular expression
	reti = regexec(&regex, studentFile, NUM_MATCH, matches, 0);
	
	return !reti;
}

//Find a specific word or instruction as specified by the parameter.
//wordRE - A regular expression defining the entity to look for.
int containsWord(char wordRE[])
{
    if (strlen(method) == 0) return 0;
    
    /* Execute regular expression */
    //char *regexp = "\\s*for\\s*([^;]*;[^;]*;[^)]*)";
    
    //Compile the regular expression
    reti = regcomp(&regex, wordRE, 0);
    
    //Run the regular expression
    reti = regexec(&regex, method, NUM_MATCH, matches, 0);
    
    //if (!reti) printf("\n\n\n----ForLoopFound----\n\n\n");
    //else printf("\n\n\n----ForLoopNotFound----\n\n\n");
    return !reti;
}


//Stop use of continue, break, goto, and bool
int checkBadStuff()
{
    if (continueFound()) return 1; 
    if (gotoFound()) return 1;
    if (breakWithNoSwitchFound()) return 1;
    if (boolFound()) return 1;
    return 0;
}

//Make sure no use of continue
int continueFound()
{
    if (strlen(studentFile) == 0) return 1;
    
    /* Execute regular expression */
    //char *regexp = "\\s*#include\\s\\+<\\([^>]\\+\\)>";
    char *regexp = "continue\\W*;";
    char *src = studentFile;
    
    //Compile the regular expression
    reti = regcomp(&regex, regexp, 0);
    
    //Run the regular expression
    reti = regexec(&regex, src, NUM_MATCH, matches, 0);
    if (!reti)
    {
        printf("You are not allowed to use continue.\n");
        return 1;
    }	    
    return 0;
}

//Make sure no use of bool
int boolFound()
{
    if (strlen(studentFile) == 0) return 1;
    
    /* Execute regular expression */
    //char *regexp = "\\s*#include\\s\\+<\\([^>]\\+\\)>";
    char *regexp = "bool\\W\\+[_a-zA-Z]";
    char *src = studentFile;
    
    //Compile the regular expression
    reti = regcomp(&regex, regexp, 0);
    
    //Run the regular expression
    reti = regexec(&regex, src, NUM_MATCH, matches, 0);
    if (!reti)
    {
        printf("You are not allowed to use bool as a type.\n");
        return 1;
    }	    
    return 0;
}

//Make sure no use of goto
int gotoFound()
{
    if (strlen(studentFile) == 0) return 1;
    
    /* Execute regular expression */
    //char *regexp = "\\s*#include\\s\\+<\\([^>]\\+\\)>";
    char *regexp = "goto\\W\\+[a-zA-Z]";
    char *src = studentFile;
    
    //Compile the regular expression
    reti = regcomp(&regex, regexp, 0);
    
    //Run the regular expression
    reti = regexec(&regex, src, NUM_MATCH, matches, 0);
    if (!reti)
    {
        printf("You are not allowed to use goto.\n");
        return 1;
    }	    
    return 0;
}	

//Make sure if break is used that there is also a switch.
int breakWithNoSwitchFound()
{
    if (strlen(studentFile) == 0) return 1;
    
    /* Execute regular expression */
    //char *regexp = "\\s*#include\\s\\+<\\([^>]\\+\\)>";
    char *regexp = "break\\W*;";
    char *src = studentFile;
    
    //Compile the regular expression
    reti = regcomp(&regex, regexp, 0);
    
    //Run the regular expression
    reti = regexec(&regex, src, NUM_MATCH, matches, 0);
    if (!reti)
    {
        if (!switchFound())
        {
            printf("You are not allowed to use break outside of a switch.\n");
            return 1;
        }
    }	    
    return 0;
}

//Make sure if break is used that there is also a switch.
int switchFound()
{
    if (strlen(studentFile) == 0) return 1;
    
    /* Execute regular expression */
    //char *regexp = "\\s*#include\\s\\+<\\([^>]\\+\\)>";
    char *regexp = "switch\\W*\\([^\\)]*\\)";
    char *src = studentFile;
    
    //Compile the regular expression
    reti = regcomp(&regex, regexp, 0);
    
    //Run the regular expression
    reti = regexec(&regex, src, NUM_MATCH, matches, 0);
    if (!reti)
    {
        printf("You are not allowed to use goto.\n");
        return 1;
    }	    
    return 0;
}


//Count the number of function headers that are in the students code.
int countFunctions()
{
	if (strlen(studentFile) == 0) return 0;
	
	/* Execute regular expression */
	char *r1 = "\\s*\\(\\(char\\)\\|\\(int\\)\\|\\(double\\)\\|\\(void\\)\\)";
	char *r2 = "\\(\\s\\|\\*\\)\\+\\w\\+\\s*([^)]*)\\s*{";
	
	char regexp[256];
	strcpy(regexp, r1);
	strcat(regexp, r2);
	
	char *src = studentFile;
	int count = 0;
	
	//Compile the regular expression
	reti = regcomp(&regex, regexp, 0);
	
	//Run the regular expression
	reti = regexec(&regex, src, NUM_MATCH, matches, 0);
	
	while (!reti && count < 30) {

		//Search for next match AFTER the end of last match
		src += (int)matches[0].rm_eo;
		reti = regexec(&regex, src, NUM_MATCH, matches, 0);
		count++;
	}
	
	//printf("\n\n\n-------------\ncount=%d\n-------------\n\n\n", count);
	return count;
}

/**
 * Find a method given the method header.  The method will be stored
 * in the global char array named 'method' as a string.
 * @param sig Signature string array with return type, method name
 * and parameter types, in that order.
 * @param num The total number of strings in sig.
 * @param method The char array to copy this method into if it is found. 
 */
int getMethod(char **sig, int num, char *method)
{
	if (strlen(studentFile) == 0) return 1;
	
	char regexp[TEXTSIZE];
	regexp[0] = 0;
	strcat(regexp, sig[0]);
	strcat(regexp, "\\s\\+");	
	strcat(regexp, sig[1]);
	strcat(regexp, "\\s*(");
	if (num > 2)
	{
		getParamRE(regexp, sig[2]);
		for (int i = 3; i < num; i++)
		{
			strcat(regexp,"\\s*,\\s*");
			getParamRE(regexp, sig[i]);
		}
	}
	else
	{
		strcat(regexp, "\\s*\\(void\\)?");
	}
	
	strcat(regexp, "\\s*)\\s*{");
	
	reti = regcomp(&regex, regexp, 0);
	if (reti) {
		return 3;
	}
	
	/* Execute regular expression */
	reti = regexec(&regex, studentFile, NUM_MATCH, matches, 0);
	if (!reti) {
		
		int openCount = 1;
		int start = (int)matches[0].rm_so;
		int end = (int)matches[0].rm_eo;		
		
		while (openCount > 0 && end < FILESIZE)
		{
			if (studentFile[end] == '{')
				openCount++;
			else if (studentFile[end] == '}')
				openCount--;
			end++;
		}
		
		strncpy(method, &studentFile[start], end - start);
		method[end - start] = '\0'; 
		return 0;
		
	}
	else
	{
		return 2;
	}	
}

//Given a list of parameter types, this function will create a string
//which is the proper regular expression for matching those types in 
//a function header parameter list
void getParamRE(char *regexp, char *param)
{
	
	if (param[0] == '*')
	{
		strcat(regexp, "\\s*");
		strcat(regexp, "\\(\\(");
		strcat(regexp, &param[1]);
		strcat(regexp, "\\s\\+\\*\\w\\+");
		strcat(regexp, "\\)\\|\\(");
		strcat(regexp, &param[1]);
		strcat(regexp, "\\s\\+\\w\\+\\s*\\[\\s*\\]");
		strcat(regexp, "\\)\\)");
	}
	else
	{
		strcat(regexp, "\\s*");
		strcat(regexp, param);
		strcat(regexp, "\\s\\+\\w\\+");
	}
}

//Returns the pointer to the next c-style string after the one currently
//pointed to in a char array which may contain many strings.
char *findNextString(char *strings, char *next)
{
	while(*next != 0)
	{
		next++;
	}
	next++;
	return next; 
}

/**
 * Find a main method.  Function main has several different signature
 * possibilities which makes it a bit different from a normal function.
 * @param method The char array to copy main into if it is found. 
 */
int getMain(char *method)
{
	if (strlen(studentFile) == 0) return 1;
	
	char *regexp = "\\(\\(int\\)\\|\\(void\\)\\)\\s*main\\s*([^)]*)\\s*{";
	
	reti = regcomp(&regex, regexp, 0);
	if (reti) {
		//fprintf(stderr, "Could not compile regex\n");
		return 3;
	}
	
	// Execute regular expression 
	reti = regexec(&regex, studentFile, NUM_MATCH, matches, 0);
	if (!reti) {
		
		int start = (int)matches[0].rm_so;
		int end = (int)matches[0].rm_eo;		
		int openCount = 1;
		
		while (openCount > 0 && end < FILESIZE)
		{
			if (studentFile[end] == '{')
				openCount++;
			else if (studentFile[end] == '}')
				openCount--;
			end++;
		}
		
		strncpy(method, &studentFile[start], end - start);
		method[end - start] = '\0'; 
		return 0;
		
	}
	else
	{
		return 2;
	}	
}

/*
 *	Reads a file into the studentFile char array.  
 * 	This file is assumed to be a C program and will
 * 	have all comments removed.
 *	@param *filename The name of the student file to open.
 *	@return Returns true if the file was opened and read properly.
 */
int readStudentCFile(char *filename)
{
	FILE *file;
	char c = ' ';
	char lc = ' ';
	int charCount = 0;
	int inSingle = 0;
	int inMulti = 0;
	
	file = fopen(filename, "r");
	if (file == NULL) return 1;
	
	c = fgetc(file);
	lc = c;
	
	studentFile[charCount++] = c;
	
	c = fgetc(file);
	
	
	while (charCount < FILESIZE && c != EOF) {
		if (inMulti) {
			if (c == '\n') {
				studentFile[charCount++] = '\n';
			}else if(lc == '*' && c == '/') {
				inMulti = FALSE;
				lc = ' ';
				c = ' ';
				studentFile[charCount-1] = ' ';
				studentFile[charCount++] = ' ';
			}
			else
			{
				studentFile[charCount++] = ' ';
				lc = c;
			}
		} else if (inSingle) {
			if (c == '\n') {
				inSingle = FALSE;
				lc = ' ';
				studentFile[charCount++] = '\n';
			} else {
				studentFile[charCount++] = ' ';
				lc = c;
			}			
		} else { 
			if (lc == '/') { 
				if (c == '/') {
					inSingle = TRUE;
					lc = ' ';
					c = ' ';
					studentFile[charCount-1] = ' ';
					studentFile[charCount++] = ' ';
				}
				else if (c == '*')
				{
					inMulti = TRUE;
					lc = ' ';
					c = ' ';
					studentFile[charCount-1] = ' ';
					studentFile[charCount++] = ' ';
				}
				else
				{
					studentFile[charCount++] = c;
					lc = c;
				}
			
			} else {
				studentFile[charCount++] = c;
				lc = c;
			}
		}
	
		c = fgetc(file);
	}
	
	//Make it a proper string
	studentFile[charCount] = '\0';	
	
	
	//Returns 0 if EOF was reached which means the entire file
	//was read. 	
	if (c == EOF) 
	{
		return 0;
	}
	else 
	{
		return 2;
	}
}


//Return 1 if loop found.  Return 0 if no loop found.
int loopExists()
{
	if (!readStudentCFile(FILENAME)) 
	{
		printf("Can't read student file: %s.  Failing test.", FILENAME);
		return TRUE;
	}
	
	return FALSE;
}


