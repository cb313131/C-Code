#include "/c/cs223/hw5/btree.h"
// #include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

// set default threshold to 5 and debugflag to false 
int threshold = 5; 
bool debugflag = false; 
int inorder = 0;
int preorder = 0;
int postorder = 0;
int html = 0;

bool checkToken(char *token);
bool checkList(struct node *root, char *token);

int main(int argc, char **argv){
	// iterate through arguments to do some basic error checking 
	for (int i = 0; i < argc; i++){
		// if any of these flags are seen, change the value of the corresponding integer from 0 to 1
		if (strcmp(argv[i], "-debug") == 0){
			debugflag = true;
			continue;
		}
		if (strcmp(argv[i], "-inorder") == 0){
			inorder = 1;
			continue;
		}
		if (strcmp(argv[i], "-preorder") == 0){
			preorder = 1;
			continue;
		}
		if (strcmp(argv[i], "-postorder") == 0){
			postorder = 1;
			continue;
		}
		if (strcmp(argv[i], "-html") == 0){
			html = 1; 
			continue;
		}
		// if we see a threshold tag, check the next argument to see what the threshold should be assigned to 
		if (strcmp(argv[i], "-threshold") == 0){
			// if 
			if (i == (argc - 1)){
				printf("Missing threshold argument on command line\n");
				continue;
			}
			if (atoi(argv[i+1]) < 2){
				printf("Invalid threshold value: %s\n", argv[i+1]);
				i++;
				continue;
			}
			if ((atoi(argv[i+1]) == 0) != 0){
				printf("Invalid threshold value: %s\n", argv[i+1]);
				i++;
				continue;
			}
			else{
				threshold = atoi(argv[i+1]);
				i++;
				continue;
			}
		}
		else if (strcmp(argv[i], "-avl") != 0 && strcmp(argv[i], "./Cloud") != 0){
			fprintf(stderr, "Fatal error: invalid command line argument: %s\n", argv[i]);
			exit(1);
		}
	}
	// make the root for the tree
	struct node *root = makeNode("**root**");
	// character array to take in the input from each line
	char input[1024];
	// this struct node pointer will act as the beginning of the linked list 
	struct node *over_thresh = NULL;	

	// this while loop will continue until all input is read 
	while (fgets(input, 1024, stdin) != NULL) {
		// get rid of new line character 
		if (input[strlen(input)-1] == '\n'){
			input[strlen(input)-1] = '\0';
			if (debugflag == true){
				printf("Input: %s\n", input);
			}
		}
		// tokenize the string
		char *token = strtok(input, " ");
		// while there are valid tokens, the following loop will continue to run 
		while (token != NULL){
			// want to lowercase every token before adding it to tree
			for (int i = 0; i < strlen(token); i++){
				token[i] = tolower(token[i]);
			}
			// if the token is valid, insert it into the tree
			if (checkToken(token) == true){
				treeInsert(root, token);
				// check temp to see if the given token is over the threshold 
				struct node *temp = treeSearch(root, token);
				// if the count is over the threshold and the token isn't already in the linked lis, then put it in the linked list 
				if (temp->count >= threshold && checkList(over_thresh, token) == false){
					// printf("the list exists\n");
					temp->next = over_thresh;
					over_thresh = temp;
				}
			}
			// new token
			token = strtok(NULL, " ");
		}
	}
	// if debugflag is present, print out the height and size of the tree before anything else 
	if (debugflag == 1){
		printf("Tree height: %d\n", treeHeight(root));
		printf("Tree size: %d\n", treeSize(root));
	}
	// these if statements handle the situation where only one print order flag is specified 
	// must print new line after each call to print in a particular order 
	if (inorder == 1 && preorder == 0 && postorder == 0){
		printf("INORDER\n");
		printTreeIn(root);
		printf("\n");
	}
	if (inorder == 0 && preorder == 1 && postorder == 0){
		printf("PREORDER\n");
		printTreePre(root);
		printf("\n");
	}
	if (inorder == 0 && preorder == 0 && postorder == 1){
		printf("POSTORDER\n");
		printTreePost(root);
		printf("\n");
	}
	// if there are multiple traversal arguments, then we will enter this if statement and print in the correct order depending on which are present 
	if (inorder + preorder + postorder > 1){
		if (preorder == 1){
			printf("PREORDER\n");
			printTreePre(root);
			printf("\n");
		}
		if (inorder == 1){
			printf("INORDER\n");
			printTreeIn(root);
			printf("\n");
		}
		if (postorder == 1){
			printf("POSTORDER\n");
			printTreePost(root);
			printf("\n");
		}
	}
	// if there is a linked list of words over the threshold and html is not present, enter this logic
	if (over_thresh != NULL && html == 0){
		// counter variable to keep track of number of word in cloud 
		int i = 0;
		printf("The Word Cloud:\n");
		struct node *temp = over_thresh;
		// this while loop will print the words that are in the word cloud 
		while (temp != NULL){
			printf("[%d] %s [%d]\n", i, temp->key, temp->count);
			temp = temp->next;
			i++;
		}
	}
	// if a linked list is present and html flag exists, enter here
	else if (over_thresh != NULL && html == 1){
		struct node *temp2 = over_thresh;
		// while we traverse the linked list, print out the html code for the cloud 
		while (temp2 != NULL){
			printf("<span style=\"font-size: %dpx\"> %s </span>\n", temp2->count, temp2->key);
			temp2 = temp2->next;
		}
	}
	// if there are no words over the threshold, over_thresh will be null, and the following will be printed 
	else if (over_thresh == NULL){
		printf("No words seen %d times.\n", threshold);
	}
	// destroy the tree, freeing all allocated memory 
	treeDestroy(root);
}

// function returns false if the token contains non-alphabetic characters.  Otherwise, it returns true
bool checkToken(char *token){
	int length = strlen(token);
	for (int i = 0; i < length; i++){
		// if any character isn't alphabetic, return false.  Else return true after you've gone thru the whole token 
		if (isalpha(token[i]) == 0){
			return false;
		}
		else{
			continue;
		}
	}
	return true;
}

// function returns true if the token is already included in the linked list of words over threshold value.  Otherwise, it returns false
bool checkList(struct node *root, char *token){
	// set temporary node equal to root
	struct node *temp = root;
	// while temp isn't null, check the key.  If the key does not match, set temp equal to the next node in the linked list and check again
	while (temp != NULL){
		if (strcmp(temp->key, token) == 0){
			return true;
		}
		else{
			temp = temp->next; 
		}
	}
	return false;
}




