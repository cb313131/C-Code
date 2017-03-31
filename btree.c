#include "/c/cs223/hw5/btree.h"
// #include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
// This file containts the functions necessary to implement a binary search tree

// function creates and initializes new node
// sets the key equal to the key fed in.  Sets count to 1 and left/right/next pointers to null 
struct node *makeNode(char * key){
	struct node *new_node = malloc(sizeof(struct node));
	assert(new_node);
	new_node->key = malloc(strlen(key) + 1);
	assert(new_node->key);
	strcpy(new_node->key, key);
	new_node->count = 1;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->next = NULL;
	return new_node;
}

// this function returns the size of the tree fed into the function
int treeSize(struct node *root){
	// if the root is null, return 0.  Else, add 1 and recursively call treeSize on the left and right nodes 
	if (root == NULL){
		return 0; 
	}
	else {
		return 1 + treeSize(root->left) + treeSize(root->right);
	}
}
// function to calculate height of tree
int treeHeight(struct node *root){
	int left_height = 0;
	int right_height = 0;
	// return -1 so that treeHeight starts at 0
	if (root == NULL){
		return -1;
	}
	else {
		// recursively call height function on left and right sides of tree.  Return whichever is taller 
		left_height = treeHeight(root->left);
		right_height = treeHeight(root->right);
		return 1 + (left_height > right_height ? left_height : right_height);
	}
}

struct node * treeSearch(struct node *root, char * target){
	// if the root is null, return null 
	if (root == NULL){
		return NULL;
	}
	// if the root key is equal to the target, return the root
	if (strcmp(root->key, target) == 0){
		return root;
	}
	// if the root key is alphabetically after the target, search the left side of the tree
	else if (strcmp(root->key, target) > 0){
		return treeSearch(root->left, target);
	}
	// if the root key is alphabetically before the target, search the right side of the tree
	else {
		return treeSearch(root->right, target);
	}
}

void treeInsert(struct node *root, char * key){
	// if root is null, we must make the tree
	if (root == NULL){
		root = makeNode(key);
	}
	else{
		// if the word is already in the tree, increase the count in that node to reflect the duplicate word
		if (strcmp(root->key, key) == 0){
			root->count++;
			return;
		}
		// if the root key comes later in the alphabet, place a new node on the left side 
		else if (strcmp(root->key, key) > 0){
			// if no node exists on the left side, make one
			if (root->left == NULL){
				root->left = makeNode(key);
				return;
			}
			// if a node exists on the left side, recursively call treeInsert on the left side 
			else {
				treeInsert(root->left, key);
			}
		}
		// if the wroot key comes earlier in the alphabet, place a new node on the right side
		else if (strcmp(root->key, key) < 0){
			if (root->right == NULL){
				root->right = makeNode(key);
				return;
			}
			else{
				treeInsert(root->right, key);
			}
		}
	}
}

void printTreePre(struct node * tree){
	if (tree == NULL){
		return;
	}
	// print the data in the root then recursively call this function on the left side then right side 
	printf("%s [%d / %d] ", tree->key, tree->count, treeHeight(tree));
	printTreePre(tree->left);
	printTreePre(tree->right);
}

void printTreeIn(struct node * tree){
	if (tree == NULL){
		return;
	}
	// print data in left side of tree, then root, then right side 
	printTreeIn(tree->left);
	printf("%s [%d / %d] ", tree->key, tree->count, treeHeight(tree));
	printTreeIn(tree->right);
}

void printTreePost(struct node * tree){
	if (tree == NULL){
		return;
	}
	// print left side, then right side, then root 
	printTreePost(tree->left);
	printTreePost(tree->right);
	printf("%s [%d / %d] ", tree->key, tree->count, treeHeight(tree));
}

void treeDestroy(struct node * tree){
	if (tree == NULL){
		return;
	}
	// free the key then recursively call treeDestroy on the left and right side, and finally free the tree 
	free(tree->key);
	treeDestroy(tree->left);
	treeDestroy(tree->right);
	free(tree);
}




