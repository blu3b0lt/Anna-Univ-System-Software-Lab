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
Usage this program works for processing SIC instructions, the increment in location Counter can be included in
isDirective function.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hashTable {
	char label[20];
	char value[6];
};

typedef struct hashTable hashTable;

int toDec(char *hexNum) {
	int hexToDec = 0;
	int place = 1;
	int content = 0;
	int i=strlen(hexNum);
	i -= 2;
	for(; i>=0; --i)
	{
		if(hexNum[i] < 58 && hexNum[i] > 47) {
			content = (int)hexNum[i] - 48;
		}
		else {
			content = (int)hexNum[i] - 55;
		}
  		hexToDec += place * content;
  		place *= 16;
	}
	return hexToDec;
}


int main(int argc, char const *argv[])
{
	FILE *in, *symbol, *out;
	hashTable symbolTable[30];
	int hashCheck[30] = {0};
	in = fopen(argv[1], "r");
	if(!in) {
		printf("Error opening input file\n");
		return -1;
	}
	/*symbol = fopen(argv[2], "w");
	if(!symbol) {
		printf("Error opening file for write\n");
		return -1;
	}*/
	out = fopen(argv[2], "w");
	if(!out) {
		printf("Error opening file for write\n");
		return -1;
	}
	char line[200];
	char *token[5];
	int locationCounter;
	int increment;
	int len;
	int sum;
	char *test;
	test = fgets(line, 198, in);
	char lineCopy[200];
	//puts("Read line is: ");
	//puts(line);
	char hexNum[6]={"      "};
	while(test) {
		int i;
		for(i=0;i<6;i++) {
			hexNum[i] = '\0';
		}
		strcpy(lineCopy, line);
		i = 0;
		sum = 0;
		token[i] = strtok(line, " ");
		while(token[i]) {
			//puts(token[i]);
			token[++i] = strtok(NULL, " ");
		}
		if(strstr(token[0], "start")) {
			//puts("start");
			fputs(token[2], out);
			fputc(32, out);
			locationCounter = toDec(token[2]);
			//puts("Line Copy is:");
			//puts(lineCopy);
			fputs(lineCopy, out);
			toHex(hexNum, locationCounter);
			hashInsertion(sum, token[1], symbolTable, hashCheck, hexNum);
			test = fgets(line, 198, in);
			continue;
		}
		if(strstr(token[0], "end")) {
			locationCounter = locationCounter - 3;
		}
		if( i == 3) {
			increment = isDirective(token);
			len = strlen(token[0]);
			int j;
			for(j=0; j<len; j++) {
				sum += token[0][j];
			}
			toHex(hexNum, locationCounter);
			locationCounter = locationCounter + increment;
			hashInsertion(sum, token[0], symbolTable, hashCheck, hexNum);
		}
		else {
			toHex(hexNum, locationCounter);
			locationCounter = locationCounter + 3;
		}
		fputs("\n", out);
		fputs(hexNum, out);
		fputc(32, out);
		fputs(lineCopy, out);
		test = fgets(line, 198, in);
	}
	fclose(out);
	fclose(in);
	symbolTableDisplay(symbolTable, hashCheck);
	int choice;
	do {
		printf("MENU: \n1.Delete a label in Symbol Table\n2.Insert a Label in Symbol Table\nEnter your choice: ");
		scanf("%d", &choice);
		switch(choice) {
			case 2 : {
				char labelName[30];
				char lableValue[10];
				printf("Enter Label Name: ");
				scanf("%s", labelName);
				printf("Enter Value or Address: ");
				scanf("%s", lableValue);
				int j;
				sum = 0;
				for(j=0; j<len; j++) {
					sum += labelName[j];
				}
				hashInsertion(sum, labelName, symbolTable, hashCheck, lableValue);
				break;
			}
			case 1 : {
				char labelName[30];
				printf("Enter Label Name to Delete: ");
				scanf("%s", labelName);
				int j;
				sum = 0;
				for(j=0; j< 30; j++ ){
					if(strstr(symbolTable[j].label, labelName)) {
						hashCheck[j] = 0;
						break;
					}
				}
			}
		}
		symbolTableDisplay(symbolTable, hashCheck);
		puts("\nEnter 1 to continue or 0  to exit.");
		scanf("%d" ,&choice);
	}while(choice);
	return 0;
}

int symbolTableDisplay(hashTable symbolTable[], int hashCheck[]) {
	int i;
	puts("SYMBOL TABLE DISPLAY");
	printf("%10s %s\n","Label", "Address");
	for(i=0;i < 30; i++) {
		if(hashCheck[i] == 0 ) {
			continue;
		}
		printf("%10s %.6s\n",symbolTable[i].label, symbolTable[i].value);
	}
}

char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

int toHex(char hexNum[], int locationCounter) {
	int temp;
	//puts("break");
	int i = 0;
	for(i=0;i<6;i++) {
			hexNum[i] = '\0';
	}
	//puts(hexNum);
	//puts("Done");
	char ctr;
	i = 0;
    while(locationCounter!=0) {
        temp = locationCounter % 16;

      //To convert integer into character
        if(temp > 9)
    		ctr = (char)(temp + 55);
    	else 
    		ctr = (char)(temp + 48);
    	hexNum[i++]= ctr;
    	locationCounter = locationCounter / 16;
  	}	
  	hexNum = strrev(hexNum);
	return 0;
}

int toNum(char token[]) {
	int num = 0;
	int sum = 0;
	int len = strlen(token);
	int i;
	/*for(i=0; i<len; i++) {
		puts("Wonned");
		num = atoi(token[i]);
		sum = sum * 10 + num;
	}*/
	sum = atoi(token);
	return sum;
}

int hashInsertion(int sum, char *token, hashTable symbolTable[], int hashCheck[], char *hexNum) {
	int key = 0;
	key = sum%30;
	while(hashCheck[key++] != 0) {
		if(key>29) {
			key = 0;
		}
	}
	key--;
	int i = 0;
	for(i=0; i<30;i++) {
		if(!strcmp(symbolTable[i].label, token)) {
			printf("%s is already present.\n", token);
			return 0;
		}
	}
	strcpy(symbolTable[key].label, token);
	strcpy(symbolTable[key].value, hexNum);
	hashCheck[key]++;
	return 0;
}

int isDirective(char **token) {
	int num;
	num = toNum(token[2]);
	if(strstr(token[0], "end")) {
		return -3;
	}
	if(strstr(token[1], "resw")) {
		int len;
		len = atoi(token[2]);
		return len*3;
	}
	if(strstr(token[1], "word")) {
		return 3;
	}
	if(strstr(token[1], "byte")) {
		int len = strlen(token[2]);
		if(token[2][0] == 'x') {
			//printf("inside x %d\n", len);
			return (len-4)/2;
		}
		if(token[2][0] == 'c') {
			//printf("inside c %d\n", len);
			return len - 4;
		}
	}
	if(strstr(token[1], "resb")) {
		int len = strlen(token[2]);
		return len;
	}
	return 3;
}
