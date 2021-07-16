#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>          

bool isLoop(char* string);

char* variableList[10000][2];    // keeps the defined variables and their values
int variableListIndex = 0;

bool* inComment = false;      // if the next char is {, inComment is true
bool* inBlock = false;        // if the next char is [, inComment is true
bool* needBlock = false;      // if there is no code block in loop statement, needBlock is true
bool* inString = false;       

char* commands[500];          // keeps the lines in loop
int cmdIndex = 0;            
int loop1Int = 0;             // keeps how many times run is the first loop
int loop2Int;                 // keeps how many times run is the second loop
int loop2Index = 0;
int loopCounter = 0;          // keeps how many loops are in the code
int loop3Int = 0;             // keeps how many times run is the third loop
int loop3Index = 0;
int openBInt = 1;             // keeps how many openblocks are there
int loop1Index = 0;

int variableAddr = 0;         

char src[20];
char value[10];

char* stringEx[100];          // keeps strings between double quotes in the line
int strExIndex = 0;

int x=0;
int y=0;

int rowIndex = 0;


bool isNumeric(char a){               
	if(a == '0' || a == '1' || a == '2' || a == '3' || a == '4' || a == '5' || a == '6' || a == '7' || a == '8' || a == '9')
		return(true);
	else
		return(false);
}

bool isAlphabetic(char a){
	if(sizeof(a) != 1)
		return(false);
	if(isalpha(a) == 0)
		return(false);
	else
		return(true);
}

bool isAnyChar(char a){
	if(isAlphabetic(a) || isNumeric( a ))
		return(true);
	else
		return(false);
}

bool isType(char* str){             
	if(strcmp(str, "int") == 0)
		return(true);
	else
		return(false);
}

bool isVariable(char* str){          // checks the parameter str is the possible variable name
	if (strlen(str) > 20)
		return(false);
	
	if (!isAlphabetic(str[0]))
		return(false);
		
	int i;
	for(i=1; i<strlen(str); i++){
		if (ispunct(str[i]) != 0 && str[i] != '_' )
			return(false);
					
		if (str[i] == '_')
			if (str[i+1] == '_')
				return(false);
	}
	
	return(true);
}

bool isInVariableList(char* str){         // checks the parameter str is declared before
	if(!isVariable(str))
		return(false);
		
	int size = sizeof(variableList)/sizeof(variableList[0]);
	int i;	
	variableAddr = 0;
	for (i = 0; i<variableListIndex; i++){
		if(strcmp(variableList[i][0], str) == 0 ){
			variableAddr = i;
			return(true);
		}
			
	}
	return(false);
}

bool isIntValue(char* str){           // checks the parameter str is possible int value or defined as a variable before
	int size = strlen(str);
	if (size == 0){
		return(false);
	}
	else if (size > 100)
		return(false);
	
	else{
		if ( isAlphabetic(str[0]) )
			if (isInVariableList(str))
				return(true);
			else return(false);
		int i;
		for(i = 0; i<size; i++){
			if(i>-1){
				if( ispunct(str[i]) != 0  || isAlphabetic(str[i]) || !isNumeric(str[i]) )
					return(false);
			}
		}
	}
	return(true);
}

bool isEndOfLine(char str){
	if (str == '.')
		return(true);
	else return(false);
}

bool isDeclaration(char* string){       // checks the parameter line is the correct declaration statement
	char str[100];
	strcpy( str, string );
	
	char* words[10];
	int i = 0;
	char *token = strtok(str, " ");
	
    while (token != NULL){
    	words[i++] = token;	
        token = strtok(NULL, " ");
    }
    
    char* type = words[0];
    char* variable = words[1];
    
    if ( isEndOfLine(variable[strlen(variable)-1] )){
    	variable[strlen(variable)-1] = '\0';
    	if(isType(type) && isVariable(variable)){
    		strncpy(src, "", strlen(src));
    		
    		char* varName = (char* ) malloc(sizeof(char)*(strlen(variable)));
    		strcpy(varName, src);
    		
    		strcat(varName, variable);
    		variableList[variableListIndex][0] = varName;           // variable added to variableList
    		variableList[variableListIndex++][1] = "0";             // variable initialized "0" as default
    		
    		return(true);
		}	
		else return(false);
	}
	else {
		if(i<3)
			return(false);
		
		if(isType(type) && isVariable(variable) && strcmp(words[2], ".") == 0 ){
			strncpy(src, "", strlen(src));
    		char* varName = (char* ) malloc(sizeof(char)*(strlen(variable)));
    		strcpy(varName, src);
    		
    		strcat(varName, variable);
			variableList[variableListIndex][0] = varName;           // variable added to variableList
    		variableList[variableListIndex++][1] = "0";             // variable initialized "0" as default
    		return(true);
		}
		else return(false);
	}	
}

bool isAssignment(char* string){          // checks the parameter line is the correct assignment statement
	char str[100];
	strcpy( str, string );
	
	char* words[6];
	int i = 0;
	char *token = strtok(str, " ");
	
    while (token != NULL){
        words[i++] = token;
        token = strtok(NULL, " ");
    }
    
    if(i<4)
    	return(false);
	
	char *move = words[0];
	char *intValue = words[1];
	char *to = words[2];
	char *variable = words[3];
	if ( isEndOfLine(variable[strlen(variable)-1] )){
    	variable[strlen(variable)-1] = '\0';
    	if(strcmp(move, "move") == 0 && isIntValue(intValue) && strcmp(to, "to") == 0 && isInVariableList(variable)){
			strncpy(value, "", strlen(value));
    		
    		char* moveInt = (char* ) malloc(sizeof(char)*(strlen(intValue)));
    		strcpy(moveInt, value);
    		
    		strcat(moveInt, intValue);

    		variableList[variableAddr][1] = moveInt;       // assign given integer value to its defined variable
    		return(true);
		}
		else return(false);
		
	}
	else {
		if(i<5) 
			return(false);
			
		if(strcmp(move, "move") == 0 && isIntValue(intValue) && strcmp(to, "to") == 0 && isInVariableList(variable) && strcmp(words[4], ".") == 0 ){
			strncpy(value, "", strlen(value));
    		
    		char* moveInt = (char* ) malloc(sizeof(char)*(strlen(intValue)));
    		strcpy(moveInt, value);
    		
    		strcat(moveInt, intValue);

    		variableList[variableAddr][1] = moveInt;    // assign given integer value to its defined variable
    		return(true);
		}
		else
			return(false);
	}
}   

bool isAddition(char* string){              // checks the parameter line is the correct addition statement
	char str[100];
	strcpy( str, string );
	
	char* words[6];
	int i = 0;
	char *token = strtok(str, " ");
	
    while (token != NULL){
        words[i++] = token;
        token = strtok(NULL, " ");
    }
    
    if(i<4)
    	return(false);
	
    char* add = words[0];
    char* intValue = words[1];
    char* to = words[2];
    char* variable = words[3];

	if ( isEndOfLine(variable[strlen(variable)-1] )){
    	variable[strlen(variable)-1] = '\0';
    	if(strcmp(add, "add") == 0 && isIntValue(intValue) && strcmp(to, "to") == 0 && isInVariableList(variable)){
			int addInt;
			if(isInVariableList(intValue))
    			addInt = atoi(variableList[variableAddr][1]);
			
			else
    			addInt = atoi(intValue);
			
			if(isInVariableList(variable)){	}
			if(variableList[variableAddr][1] == NULL)
				return(false);
			
			int addedVar;
			addedVar = atoi(variableList[variableAddr][1]);
			
			int result = addInt + addedVar;
			
			char* newVarInt = (char* ) malloc(sizeof(char)*result);
			itoa(result, newVarInt, 10);

    		variableList[variableAddr][1] = newVarInt;
    		return(true);
		}
			
		else return(false);
	}
	else {
		if(i<5)
			return(false);
			
		if(strcmp(add, "add") == 0 && isIntValue(intValue) && strcmp(to, "to") == 0 && isInVariableList(variable) && strcmp(words[4], ".") == 0 ){
			int addInt;
			if(isInVariableList(intValue))
    			addInt = atoi(variableList[variableAddr][1]);
			else
    			addInt = atoi(intValue);
			
			if(isInVariableList(variable)){	}
			if(variableList[variableAddr][1] == NULL)
				return(false);
			
			int addedVar;
			addedVar = atoi(variableList[variableAddr][1]);
			
			int result = addInt + addedVar;
			
			char* newVarInt = (char* ) malloc(sizeof(char)*result);
			itoa(result, newVarInt, 10);
			
    		variableList[variableAddr][1] = newVarInt;
    		return(true);
		}
		else return(false);
	}
}

bool isSubtraction(char* string){             // checks the parameter line is the correct subtruction statement
	char str[100];
	strcpy( str, string );
	
	char* words[6];
	int i = 0;
	char *token = strtok(str, " ");
	
    while (token != NULL){
		words[i++] = token;
        token = strtok(NULL, " ");
    }
    
    if(i<4)
    	return(false);
	
    char* sub = words[0];
    char* intValue = words[1];
    char* from = words[2];
    char* variable = words[3];

	if ( isEndOfLine(variable[strlen(variable)-1] )){
    	variable[strlen(variable)-1] = '\0';
    	if(strcmp(sub, "sub") == 0 && isIntValue(intValue) && strcmp(from, "from") == 0 && isInVariableList(variable)){
    		int subInt;
			if(isInVariableList(intValue))
    			subInt = atoi(variableList[variableAddr][1]);
			else
    			subInt = atoi(intValue);
			
			if(isInVariableList(variable)){	}
			if(variableList[variableAddr][1] == NULL)
				return(false);
			
			int subVar;
			subVar = atoi(variableList[variableAddr][1]);
			
			int result = subVar - subInt;
			
			char* newVarInt = (char* ) malloc(sizeof(char)*result);
			itoa(result, newVarInt, 10);

    		variableList[variableAddr][1] = newVarInt;
    		return(true);
		}
			
		else return(false);
	}
	else {
		if(i<5)
			return(false);

		if(strcmp(sub, "sub") == 0 && isIntValue(intValue) && strcmp(from, "from") == 0 && isInVariableList(variable) && strcmp(words[4], ".") == 0 ){
			int subInt;
			if(isInVariableList(intValue))
    			subInt = atoi(variableList[variableAddr][1]);
			else
    			subInt = atoi(intValue);
			
			if(isInVariableList(variable)){	}
			if(variableList[variableAddr][1] == NULL)
				return(false);
				
			int subVar;
			subVar = atoi(variableList[variableAddr][1]);
			
			int result = subVar - subInt;
			
			char* newVarInt = (char* ) malloc(sizeof(char)*result);
			itoa(result, newVarInt, 10);

    		variableList[variableAddr][1] = newVarInt;
    		return(true);
		}
		else return(false);
	}
}

bool isListElement(char* str){  
	if(strcmp(str, "newline") == 0 ){
		printf("\n");
		return(true);
	}
	else if (isIntValue(str)){
		if (isInVariableList(str)){
			printf("%s ", variableList[variableAddr][1]);
			return(true);
		}
		else{
			printf("%s ", str);
			return(true);
		}
	}
	else if (strcmp(str, "&") == 0 && !inString){
		int i;
		for(i=0; i<strExIndex;i++){
			printf("%c", stringEx[i]);
		}
		strExIndex = 0;
		return(true);
	}

	else return(false);
}

bool isOutList(char* string){
	char str[100];
	strcpy( str, string );
	
	int a;   
	int b;
	for (a =0; a<strlen(str); a++){       // before code parsed, the string added to stringEx list and replaced with space at the line 
		if(str[a] == '\"'){
			str[a] = '&';                 // to find where string starts, we added '&' character
			 
			inString = true;
			
			for (b = a+1; b<strlen(str); b++){
				if(str[b] == '\"'){
					str[b] = ' ';
					inString = false;
					stringEx[strExIndex] = '\0';
					break;
				}
				else{
					stringEx[strExIndex++] = str[b];
					str[b] = ' ';
				}
			}	
		}
	}

	char* words[100];
	int i = 0;
	char *token = strtok(str, ",");
																
    while (token != NULL){
        words[i++] = token;
        if(strncmp(token, " ", strlen(token)) == 0)
        	return(false);
        
		while (token[strlen(token)-1] == ' '){
			token[strlen(token)-1] = '\0';
		}
		while (token[0] == ' '){
			int j;
			for(j = 0; j<strlen(token); j++){
				token[j] = token[j+1];
			}
		}
				
		if (!isListElement(token))
        	return(false);

       	token = strtok(NULL, ",");
    }
	return(true);    
}

bool isOutput(char* string){                   // checks the parameter line is the correct output statement
	char str[100];
	strcpy( str, string );

	char* words[100];
	int i = 0;
	char *token = strtok(str, " ");
	
    while (token != NULL){
		words[i++] = token;
		token = strtok(NULL, "");
    }
    
    if(i<2)
    	return(false);
    
    char* out = words[0];
    char* outList = words[1];
    
    int c = 0;
	int a;
	for (a=0; a<strlen(outList)-1; a++){           // before code parsed, we checked is there any seperator one after another 
		if(outList[a] != '\"'){
			int b ;
			for(b=a; b< strlen(outList)-1;b++){
				if(c%2==0 && outList[b] == ',' && outList[b+1] == ',')
					return(false);
			}
		}
		else c ++;
	}
    
    removeSpacesR(outList);
	
	if (strcmp(out, "out") == 0 && isEndOfLine(outList[strlen(outList)-1])){
		outList[strlen(outList)-1] = '\0';
		
		while (outList[strlen(outList)-1] == ' '){
			outList[strlen(outList)-1] = '\0';
		}
		
		if(isOutList(outList) && !inString)
			return(true);
			
		else return(false);
	}
	else return(false);
}

void removeSpacesR(char* str){                 // removes spaces from right side
	int index, i;
    index = -1;
    i = 0;
    while(str[i] != '\0'){
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        	index= i;
        i++;
    }
    str[index + 1] = '\0';
}

bool isStatement(char* string){               // checks given string is supported by language
	char str[100];
	strcpy( str, string );
	
	while (str[0] == '[' ){        
			int j;
			for(j = 0; j<strlen(str); j++){
				str[j] = str[j+1];
			}
	}
	str[strlen(str)-1] = '\0';

	char* words[100];
	int i = 0;
	char *token = strtok(str, ".");
	
    while (token != NULL){
        words[i++] = token;
        
        while (token[strlen(token)-1] == ' '){
			token[strlen(token)-1] = '\0';
		}
		while (token[0] == ' '){
			int j;
			for(j = 0; j<strlen(token); j++){
				token[j] = token[j+1];
			}
		}
		strcat(token, ".");	
			
		if(isDeclaration(token) || isAssignment(token) || isAddition(token) || isSubtraction(token) || isOutput(token) || isLoop(token)  )
			token = strtok(NULL, " ");
        else
        	return(false);
	}
	return(true);	
}

bool isCodeBlock(char* string){
	if(string == NULL){
		needBlock = true;
		return(true);
	}
	char str[100];
	strcpy( str, string );
	if(str[0] == '[' ){
		inBlock = true;

		if(str[strlen(str)-1] == ']' ){
			commands[cmdIndex] = str;
			cmdIndex = cmdIndex + 1;
			int i; 
			for (i = 0; i<loop1Int; i++){         // first loop turns loop1Int times and runs codes from commands list
				if (isStatement(commands[i])){
				}
				else{
					printf("\nERROR: Line %d is not supported by The BigAdd Language.\n", rowIndex+1);
					return(0);
				} 
			}
			
			inBlock = false;
			return(true);
		}
		else if(loopCounter>0) {
			commands[cmdIndex] = str;
			cmdIndex = cmdIndex + 1;
			return(true);
		}
			
		else{
			commands[cmdIndex] = str;
			cmdIndex = cmdIndex + 1;
			loop1Index = cmdIndex;
			return(true);
		} 
	}
	
	else{
		if (loopCounter>0) {
			int i;
			if (loopCounter>1) {
				for (i = 0; i<loop3Int; i++){      // third loop turns loop3Int times and runs codes from commands list
					if (isStatement(str)){
					}
					else{
						printf("\nERROR: Line %d is not supported by The BigAdd Language.\n", rowIndex+1);
						return(0);	
					}
				}
			}
			else {
				for (i = 0; i<loop2Int; i++){         // second loop turns loop2Int times and runs codes from commands list
					if (isStatement(str)){
					}
					else{
						printf("\nERROR: Line %d is not supported by The BigAdd Language.\n", rowIndex+1);
						return(0);
					} 
				}	
			}
				
		}
		else {
			int i; 
			for (i = 0; i<loop1Int; i++){      // first loop turns loop1Int times and runs codes from commands list
				if (isStatement(str)){
				}
				else{
					printf("\nERROR: Line %d is not supported by The BigAdd Language.\n", rowIndex+1);
					return(0);
				} 
			}
		}
		
		return(true);
	}
}

bool isLoop(char* string){                   // checks the parameter line is the correct loop statement
	char str[100];
	strcpy( str, string );
	
	char* words[5];
	int i = 0;
	char *token = strtok(str, " ");
	
    while (token != NULL){
    	words[i++] = token;
		
    	if(i<3)
        	token = strtok(NULL, " ");
		else break;
    }
    if(i<3)
    	return(false);
    
    char* loop = words[0];
    char* intValue = words[1];
    char* times = words[2];

	char* codeBlock;
	if(times[strlen(times)-1] == '['){
		times[strlen(times)-1] = '\0';
		codeBlock = "[";
	}
	else  codeBlock = strtok(NULL, "");
	
	if (loopCounter>0) {                  // setting how many times the loops will loop
		if (loopCounter>1) {
			if(isInVariableList(intValue)){
    			loop3Int = atoi(variableList[variableAddr][1]);
    		}
			else loop3Int = atoi(intValue);
		}
		else {
			if(isInVariableList(intValue)){
    			loop2Int = atoi(variableList[variableAddr][1]);
    		}
			else loop2Int = atoi(intValue);
		}
	}
	else {
		if(isInVariableList(intValue)){
    		loop1Int = atoi(variableList[variableAddr][1]);
    	}
		else loop1Int = atoi(intValue);
	}
    
    if (strcmp(loop, "loop") == 0 && isIntValue(intValue) && strcmp(times, "times") == 0 && isCodeBlock(codeBlock)  ){
    	loopCounter = loopCounter + 1;
    	return(true);
	}
    else return(false);
}

bool isLine(char* str){         // checks given string is supported by language

	int a;     // before code parsed, deleted all comments
	int b;
	for (a =0; a<strlen(str); a++){
		if(inComment){
			for (b=a; b<strlen(str);b++){
				if(str[b] != '}')
					str[b] = ' ';
				else{
					str[b] = ' ';
					inComment=false;
					break;
				} 
			}
		}
		else if(str[a] == '{'){
			inComment = true;
			for (b=a; b<strlen(str);b++){
				if(str[b] != '}')
					str[b] = ' ';
				else{
					str[b] = ' ';
					inComment=false;
					break;
				} 
			}
		}
	}
	
	removeSpacesR(str);
	
	int c;                         // if there is only comment expression in a line
	for(c=0; c<strlen(str); c++){
		if(str[c] != ' ')
			break;
	}
	if(c==strlen(str))
		return(true);
	
	if(inBlock){
		if(x==0) {
			if(openBInt==2){
				loop2Index = cmdIndex;
				x = x + 1;
			}	
		}
		if(y==0) {
			if(openBInt == 3) {
				loop3Index = cmdIndex;
				y = y + 1;
			}
		}
		
		if(str[strlen(str) - 1] == ']' || str == "]"){
			needBlock = false;
			inBlock = false;
			int loop2End;
			int loop3End;
			commands[cmdIndex] = str;
			cmdIndex = cmdIndex + 1;
			loop3End = cmdIndex;
			
			int i;
			int k;
			int l;
			int m;
			int n;
			if (loopCounter>2) {
				for (i = 0; i<loop1Int; i++){
					int j;
					for (j=loop1Index; j<loop2Index-1; j++){
						if (isStatement(commands[j])){
						}
						else{
							printf("\nERROR: Line %d is not supported by The BigAdd Language.\n", rowIndex+1);
							return(0);
						} 
					}
					for (l=0; l<loop2Int; l++){
						for (k=loop2Index; k<loop3Index-1; k++) {
							if (isStatement(commands[k])){
							}
							else{
								printf("\nERROR: Line %d is not supported by The BigAdd Language.\n", rowIndex+1);
								return(0);
							} 
						}
						for (m = 0; m<loop3Int; m++) {
							for (n=loop3Index; n<loop3End; n++) {
								if (isStatement(commands[n])) {
								}
								else{
									printf("\nERROR: Line %d is not supported by The BigAdd Language.\n", rowIndex+1);
									return(0);
								} 
								
							}
						}	
					}
				}
			}
			else if (loopCounter>1) {
				loop2End = cmdIndex;
				for (i = 0; i<loop1Int; i++){
					int j;
					for (j=loop1Index; j<loop2Index-1; j++){
						if (isStatement(commands[j])){
						}
						else{
							printf("\nERROR: Line %d is not supported by The BigAdd Language.\n", rowIndex+1);
							return(0);
						} 
					}
					for (l=0; l<loop2Int; l++){
						for (k=loop2Index; k<loop2End; k++) {
							if (isStatement(commands[k])){
							}
							else{
								printf("\nERROR: Line %d is not supported by The BigAdd Language.\n", rowIndex+1);
								return(0);
							} 
						}
					}	
				}
			}
			else {
				for(i=0; i<loop1Int; i++) {
					int j;
					for(j=0; j<cmdIndex; j++) {
						if (isStatement(commands[j])){
						}
						else{
							printf("\nERROR: Line %d is not supported by The BigAdd Language.\n", rowIndex+1);
							return(0);
						} 
					}
				}
			}
		}
		else if (isLoop(str)) {
			openBInt++;
		}
		else{
			commands[cmdIndex] = str;
			cmdIndex = cmdIndex + 1;
		}
		return(true);
	}
	
	else if(needBlock){
		if(str[0] == '[' || str == "["){
			inBlock = true;
			if(str[strlen(str) - 1] == ']' || str == "]"){
				needBlock = false;
				inBlock = false;
				if (isStatement(str)){
				}
				else{
					printf("\nERROR: Line %d is not supported by The BigAdd Language.\n", rowIndex+1);
					return(0);
				} 
				return(true);
			}
			else{
				commands[cmdIndex] = str;
				cmdIndex = cmdIndex + 1;
				return(true);
			}
		}
	}
	
	else if (str[strlen(str) - 1] == ']' || str == "]")
		return(true);
	
	else if (isDeclaration(str) || isAssignment(str) || isAddition(str) || isSubtraction(str) || isOutput(str) || isLoop(str) )
		return(true);

	else{
		printf("\nERROR: Line %d is not supported by The BigAdd Language.\n", rowIndex+1);
		return(false);
	}
}

void analyze(char* str, long fileSize){       
	char* lines[200];
	int i=0;
	
	char *token = strtok(str, "\n");
	
    while (token != NULL){
    	lines[i++] = token;	
    	token = strtok(NULL, "\n");
	}
	
	int j;
	j=0;
	while (j<i){
		
		char *line = lines[j];
		if (line[strlen(line)-1] == '\r')
			line[strlen(line)-1] = '\0';
		
		if(isLine(line)){}
		else return(0);
		rowIndex = j;
		j++;               
	}
	
	if(inBlock){
		printf("\n%s", "ERROR: Missing close block.");
	}
	else if (inComment){
		printf("\n%s", "ERROR: Missing curly braces.");
	}
	else if (needBlock){
		printf("\n%s", "ERROR: Missing code block for loop statement.");
	}
	else if (inString){
		printf("\n%s", "ERROR: Missing double quotes.");
	}
		
}

int main(){
	FILE *file;
	long fileSize;
	char *code;
	
	char fileName[50];
    printf("Enter the file name: ");
    gets(fileName); 
	strcat(fileName, ".ba"); 

	file = fopen(fileName, "rb");
	if (file == NULL) {
		perror("myscript.ba");
		exit(1);
	}
	
	fseek(file, 0L, SEEK_END);
	fileSize = ftell(file);
	rewind(file);

	code = (char*)calloc(1, fileSize + 1);

	if (1 != fread(code, fileSize, 1, file)) {
		fclose(file);
		free(code);
		exit(1);
	}
	
	analyze(code, fileSize);
	
	system("pause");	
	return 0;
}
