/*
The MIT License (MIT)

Copyright (c) 2014 blu3b0lt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/*
programName.out input.c output.txt

input.c is the program that needs to be tokenized.

output.txt stores the tokenized values.

Additional keywords can be included in keyword variable and make correspoding changes to the array size
for various loops.
*/

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>

char* trimwhitespace(char *line);
 
int tokensizer(char *);
int variableIdentifier(char *);
char operator[7][2] = {"+","-","*","/","%","="};
char splChar[9][2] = {";",",","(",")","[","]","{","}",":"};
char keyword[6][10] = {"const", "int", "float", "char", "define", "include"};
char identifier[20][20];
int number[20];

int operatorCheck[7] = {0,0,0,0,0,0,0};
int splCharCheck[9] = {0};
int keywordCheck[6] = {0};

int finalCheck = 0;
int identifierCount = 0;
int numberCount = 0;

int main(int argc, char const *argv[])
{
	FILE *in, *out;
	//puts(argv[1]);
	//puts(argv[2]);
	in = fopen(argv[1], "r");
	if(!in) {
		printf("Error Opening File for Read\n");
		return -1;
	}
	out = fopen(argv[2], "w");
	if(!out) {
		printf("Error Opening File for Write\n");
		return -1;
	}

	char line[200];
	char *test;
	test = fgets(line, 198, in);
	while(test) {
		if(line != "\n") {
			tokensizer(line);
		}
		test = fgets(line, 198, in);
	}


	fputs("\t\tTokenisation\n\n", out);
	fputs("Operators: \n\n", out);
	int i;
	for(i=0; i<7;i++) {
		if(operatorCheck[i]>0) {
			fputs(operator[i], out);
			fputc(32, out);
		}
	}
	fputs("\n\nPunctuators:\n\n", out);
	for(i=0; i<9;i++) {
		if(splCharCheck[i]>0) {
			fputs(splChar[i], out);
			fputc(32, out);
		}
	}

	fputs("\n\nKeywords:\n\n", out);
	for(i=0; i<9;i++) {
		if(keywordCheck[i]>0) {
			fputs(keyword[i], out);
			fputc(32, out);
		}
	}
	fputs("\n\nIdentifiers: \n\n", out);
	for(i=0; i<identifierCount; i++) {
		if(isdigit(atoi(identifier[i]))) {
			puts("inside");
			number[numberCount++] = identifier[i];
			continue;
		}
		fputs(identifier[i], out);
		fputc(32, out);
	}

	fclose(in);
	fclose(out);
	return 0;
}

int tokensizer(char line[]) {
	int i=0;
	char *test;
	for(i=0; i<9;i++) {
		if(!splCharCheck[i]) {
			test = strstr(line, splChar[i]);
			if(test) {
				splCharCheck[i]++;
				finalCheck++;
			}
		}
	}
	
	for(i=0; i<7;i++) {
		if(!operatorCheck[i]) {
			test = strstr(line, operator[i]);
			if(test) {
				operatorCheck[i]++;
				finalCheck++;
			}
		}
	}

	char *token;
	char splitter[] = {"# "};
	char lineCopy[200];
	strcpy(lineCopy, line);
	token = strtok(line, splitter);
	token = trimwhitespace(token);
	if(token) {
		for(i=0; i<6;i++) {
			if(strcmp(token, keyword[i]) == 0) {
				keywordCheck[i]++;
				if(i<5) {
					keywordChecker(lineCopy, i);
				}
				break;
			}
		}
	}
	return 0;
}

char* trimwhitespace(char *line) {
	while(isspace(*(line))) {
		line = line + 1;
	}
	return line;
}
int keywordChecker(char *line, int i) {
	char splitter[] = {"#,;= "};
	char *token;
	token = strtok(line, splitter);
	token = strtok(NULL, splitter);
	if(i == 0) {
		token = strtok(NULL, splitter);
	}
	while(token) {
		puts(token);
		if(isdigit(atoi(token))) {
			number[numberCount++] = atoi(token);
			token = strtok(NULL, splitter);
			continue;
		}
		if(!(isSplChar(token))) {
			strcpy(identifier[identifierCount++], token);
		}
		token = strtok(NULL, splitter);
	}
	return 0;
}

int isSplChar(char *txt) {
	int i;
	for(i=0; i<9;i++) {
		if(strstr(txt, splChar[i])) {
			return 1;
		}
	}
	return 0;
}
