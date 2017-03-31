#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "hash.h"
/*
This function reads in either numbers or a word from the command line.  If it reads a word, it outputs a potential number to match the
word based on its letters.  If it reads a number, it searches the hashtable for words potentially the number and prints them
*/


int MatchDisplay(Hash hashtable, int number[], int current, char output[], int num_length, int matches){
	// This array stores the letters corresponding to the number input by the user
	char words[10][5] = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

	// If we've reached the end of the numbers, we check if the word is in the dictionary.  If it is, we print it.  Else, we return.
    if (current == num_length){
    	if (HashSearch(hashtable, output) == true){
    		printf(" %s", output);
    		matches = matches + 1;
    		return matches;
    	}
    	else{
    		return matches;
    	}
    }
    
    // This for loop iterates through all the letters that correspond to each number input and recursively calls this function
    // in order to find every combination of letters and check if they are in the dictionary. 
    for (int i = 0; i < strlen(words[number[current]]); i++){
    	output[current] = words[number[current]][i];
    	matches = MatchDisplay(hashtable, number, current+1, output, num_length, matches);

    	if (number[i] == 0 || number[i] == 1){
    		return matches;
    	}
    }
    return matches;
}

int main(int argc, char **argv){
	// If there are 3 arguments, the 3rd argument must be -debug, otherwise there's an error
	if (argc == 3){
		if (strcmp(argv[2], "-debug") != 0){
			fprintf(stderr, "Invalid argument: %s\n", argv[2]);
			exit(1);
		}
	}
	// if there are 4 arguments, there must be an alternative dictionary that is specified.  If not, there is an error.
	if (argc == 4){
		if (strcmp(argv[2], "-file") != 0 || fopen(argv[3], "r") == NULL){
			if (strcmp(argv[2], "-file") != 0){
				fprintf(stderr, "Invalid argument: %s\n", argv[2]);
			}
			else if (fopen(argv[3], "r") == NULL){
				fprintf(stderr, "dictionary not found\n");
			}
			exit(1);
		}
	}
	// if there are 5 arguments, there must be a debug flag and an alternative dictionary
	if (argc == 5){
		if (strcmp(argv[2], "-debug") != 0 || strcmp(argv[3], "-file") != 0 || fopen(argv[4], "r") == NULL){
			fprintf(stderr, "Invalid argument\n");
			exit(1);
		}
	}
	// if there's only 1 argument, there is an error.
	if (argc == 1){
		fprintf(stderr, "usage: Callme (digits | letters) [-debug]? [-file filename]?\n");
		exit(1);
	}
	// For loop searches for debug flag
	for (int i = 0; i < argc; i++){
		if (strcmp(argv[i], "-debug") == 0){
			debugflag = true;
		}
	}
	char number_output[strlen(argv[1])];
	strcpy(number_output, argv[1]);

	// This logic turns word into number
	if (tolower(argv[1][0]) >= 'a' && tolower(argv[1][0]) <= 'z'){
		for (int i = 0; i < strlen(argv[1]); i++){
			if (tolower(argv[1][i]) >= 'a' && tolower(argv[1][i]) <= 'c'){
				number_output[i] = '2';
				continue;
			}
			else if (tolower(argv[1][i]) >= 'd' && tolower(argv[1][i]) <= 'f'){
				number_output[i] = '3';
				continue;
			}
			else if (tolower(argv[1][i]) >= 'g' && tolower(argv[1][i]) <= 'i'){
				number_output[i] = '4';
				continue;
			}
			else if (tolower(argv[1][i]) >= 'j' && tolower(argv[1][i]) <= 'l'){
				number_output[i] = '5';
				continue;
			}
			else if (tolower(argv[1][i]) >= 'm' && tolower(argv[1][i]) <= 'o'){
				number_output[i] = '6';
				continue;
			}
			else if (tolower(argv[1][i]) >= 'p' && tolower(argv[1][i]) <= 's'){
				number_output[i] = '7';
				continue;
			}
			else if (tolower(argv[1][i]) >= 't' && tolower(argv[1][i]) <= 'v'){
				number_output[i] = '8';
				continue;
			}
			else if (tolower(argv[1][i]) >= 'w' && tolower(argv[1][i]) <= 'z'){
				number_output[i] = '9';
				continue;
			}
			// if a non-integer is encountered, it is an error 
			else{
				fprintf(stderr, "Invalid argument\n");
				exit(1);
			}
		}
		printf("alphabetic: %s => %s\n", argv[1], number_output);
		// free(number_output);
		exit(1);
		// printf("alphabetic\n");
	}

	// This logic deals with situation when user inputs a number.  num_length stores length of number
	int num_length = 1;
	if (argv[1][0] >= '0' && argv[1][0] <= '9'){
		for (int i = 1; i < strlen(argv[1]); i++){
			if (argv[1][i] >= '0' && argv[1][i] <= '9'){
				num_length++;
				continue;
			}
			else{
				fprintf(stderr, "Invalid argument\n");
				exit(1);
			}
		}
		// fp is a file pointer that will point to the dictionary file
		FILE *fp; 
		fp = fopen("/usr/share/dict/words", "r");
		if (fp == NULL){
			printf("couldn't open dictionary\n");
		}
		// if the user inputs an alternative dictionary, it will be recognized here
		for (int i = 0; i < argc; i++){
			if (strcmp(argv[i], "-file") == 0){
				fp = fopen(argv[i+1], "r");
				if (debugflag == true){
					printf("Loading dictionary: %s\n", argv[i+1]);
				}
			}
		}
		if (fopen(argv[argc-1], "r") == NULL && debugflag == true){
			printf("Loading dictionary: /usr/share/dict/words\n");
		}

		Hash hashtable = HashCreate();
		// character array that will store words being read from dictionary
		char word[256];
		int num_words = 0;
		int alphabetic = 0;
		// This loop scans words into the dictionary that are the correct length
		while(fscanf(fp, "%s", word) != EOF){
			alphabetic = 0;
			if (strlen(word) != strlen(argv[1])){
				continue;
			}
			else{
				for (int i = 0; i < strlen(word); i++){
					word[i] = tolower(word[i]);
					// if a non-alphabetic character is encountered, the word is not added to dictionary 
					if (isalpha(word[i]) == 0){
						alphabetic = 1;
					}
				}
				if (alphabetic == 1){
					continue;
				}
				if (HashSearch(hashtable, word) == false){
					HashInsert(hashtable, word);
					num_words++;
				}
				else{
					continue;
				}
			}
		}
		// displays word count if debug flag is present 
		if (debugflag == true){
			printf("Word Count: %d\n", num_words);
		}
		// character array to be fed into MatchDisplay to turn number into word
		char output[strlen(argv[1]) + 1];
		output[strlen(argv[1])] = '\0';

		int number[strlen(argv[1])];

		for (int i = 0; i < strlen(argv[1]); i++){
			number[i] = argv[1][i] - '0';
		}
		int matches = 0;
		printf("numeric: %s =>", argv[1]);
		// if matches is 0, then we print that there were no matches.  Otherwise, we print the matches 
		matches = MatchDisplay(hashtable, number, 0, output, num_length, matches);
		if (matches == 0){
			printf(" ** no matches **");
		}
		printf("\n");
		
		fclose(fp);
		// destroy the hashtable, freeing all allocated memory 
		HashDestroy(hashtable);
	}

	else{
		fprintf(stderr, "Invalid argument\n");
		exit(1);
	}
}
