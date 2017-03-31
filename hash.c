#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "hash.h"



struct elt {
  struct elt *next;
  char *key;
};


struct hash {
  int size;           // size of the pointer table 
  int n;              // number of elements stored 
  int buckets;		  // number of buckets that have elements stored in them 
  struct elt **table;
};


#define MULTIPLIER (37)
int hash_size = INITIAL_SIZE;
double occupants = 0;
double buckets = 0;

unsigned long hash(const char *s)
{
    unsigned long h;
    unsigned const char *us;

    /* cast s to unsigned const char * */
    /* this ensures that elements of s will be treated as having values >= 0 */
    us = (unsigned const char *) s;

    h = 0;
    while(*us != '\0') {
        h = h * MULTIPLIER + *us;
        us++;
    } 
    return h % hash_size;
}

Hash HashCreate(void){
	Hash hashtable = malloc(sizeof(struct hash));
	if (hashtable == NULL){
		exit(1);
	}
	hashtable->size = hash_size;
	hashtable->n = occupants;
	hashtable->table = malloc(sizeof(struct elt*) * 1024);
	if (hashtable->table == NULL){
		exit(1);
	}
	for (int i = 0; i < INITIAL_SIZE; i++){
		hashtable->table[i] = NULL;
	}
	return hashtable;
}

void HashDestroy(Hash hashtable){
	for (int i = 0; i < hash_size; i++){
		struct elt *cursor = hashtable->table[i];
		while (cursor != NULL){
			struct elt *temp = cursor;
			cursor = cursor->next;
			free(temp->key);
			free(temp);
		}
	}
	free(hashtable->table);
	free(hashtable);
}

void HashInsert(Hash hashtable, const char *key){
	struct elt *new_node = malloc(sizeof(struct elt));
	if (new_node == NULL){
		exit(1);
	}
	new_node->key = malloc(sizeof(key));
	if (new_node->key == NULL){
		exit(1);
	}
	strcpy(new_node->key, key);
	// new_node->key = strdup(key);
	int index = hash(key);
	
	if (occupants == hash_size){
		buckets = 0;
		occupants = 0;
		hash_size = hash_size * GROWTH_FACTOR;


		struct elt *temp;
		hashtable->table = realloc(hashtable->table, sizeof(struct elt*) * hash_size);

		if (hashtable->table == NULL){
			exit(1);
		}
		double old_size = hash_size / GROWTH_FACTOR;

		for (int i = hash_size/GROWTH_FACTOR; i < hash_size; i++){
			hashtable->table[i] = NULL;
		}
		for (int i = 0; i < hash_size/GROWTH_FACTOR; i++){
			if (hashtable->table[i] == NULL){
				continue;
			}
			else{
				temp = hashtable->table[i];
				hashtable->table[i] = NULL;
				while (temp != NULL){
					HashInsert(hashtable, temp->key);
					struct elt *delete = temp;
					temp = temp->next;
					free(delete->key);
					free(delete);
				}
			}
		}
		hashtable->size = hash_size;
		hashtable->n = occupants;
		if (debugflag == true){
			printf("Growing to size: %d. n: %.f. Used buckets: %.f. Occupancy rate: %.2f\n", hash_size, old_size, buckets, (buckets / old_size));
		}
	}
	if (hashtable->table[index] == NULL){
		buckets++;
	}
	new_node->next = hashtable->table[index];
	hashtable->table[index] = new_node;
	occupants++;
	// free(new_node);
}

bool HashSearch(Hash hashtable, const char *key){
	int index = hash(key);
	struct elt *next_node = hashtable->table[index];
	while (next_node != NULL){
		if (strcmp(next_node->key, key) == 0){
			// free(next_node);
			return true;
		}
		else{
			next_node = next_node->next;
		}
	}
	// free(next_node);
	return false;
}

void HashDisplay(Hash hashtable){
	struct elt *next_node;
	for (int i = 0; i < hash_size; i++){
		if (hashtable->table[i] == NULL){
			continue;
		}
		else{
			next_node = hashtable->table[i];
			printf("[  %d  ] ", i);
			while (next_node != NULL){
				printf("%s ", next_node->key);
				next_node = next_node->next;
			}
			printf("\n");
		}
	}
}

