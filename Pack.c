#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

/* 
	Written by Conor Brennan
	This function seeks to implement several different bin packing algorithms based on command line input
*/


int FirstAlgorithm(int bin[], int num_sizes[], int size, int trace);
int BestAlgorithm(int bin[], int num_sizes[], int size, int trace);

int main(int argc, char *argv[]){
	if (argc == 1){
		fprintf(stderr, "usage: Pack [sizes]* [-next | -first | -best | -ffd | -bfd | -optm]+ -trace*\n");
		exit(0);
	}
	// variable to keep track of where you are in argv[]
	int i = 1;
	// array to store the size values
	int num_sizes[argc];
	// error checking to make sure sizes are valid.  Loop also used to fill num_sizes[] with the sizes input 
	while(atoi(argv[i]) != 0){
		if (atoi(argv[i]) <= 0 || atoi(argv[i]) > 100){
			fprintf(stderr, "Fatal error: Invalid size: %s\n", argv[i]);
			exit(0);
		}
		for (int j = 0; j < strlen(argv[i]); j++){
			if (isdigit(argv[i][j]) == 0){
				fprintf(stderr, "Fatal error: Invalid size: %s\n", argv[i]);
				exit(0);
			}
		}
		i++;
		if (i == (argc)){
			break;
		}
		num_sizes[i-2] = atoi(argv[i-1]);
	}
	// the integer variable size will store the index of num_sizes where the flags begin 
	int size = i;
	
	// error checking for the order of inputs, if an algorithm is specified, and if the arguments are valid
	for (int j = i; j < argc; j++){
		if (atoi(argv[j]) != 0){
			fprintf(stderr, "Fatal error: Size option out of order: %s\n", argv[j]);
			exit(0);
		}
	}
	if (atoi(argv[(argc - 1)]) != 0){
		fprintf(stderr, "Fatal error: no algorithm specified\n");
		exit(0);
	}
	for (int k = i; k < argc; k++){
		if (strcmp(argv[k], "-next") != 0 && strcmp(argv[k], "-first") != 0 && strcmp(argv[k], "-best") != 0 &&
			strcmp(argv[k], "-ffd") != 0 && strcmp(argv[k], "-bfd") != 0 && strcmp(argv[k], "-optm") != 0 &&
			strcmp(argv[k], "-trace") != 0){
			fprintf(stderr, "Fatal error: bad argument: %s\n", argv[k]);
			exit(0);
		}
	}
	
	
	// these will be indicator variables for whether or not I have output the number of bins for that algorithm
	int next = 0, first = 0, best = 0, ffd = 0, bfd = 0, optm = 0;

	// variable to indicate whether trace flag is present 
	int trace = 0;
	// checks if trace flag is one of arguments
	for (int i = 0; i < argc; i++){
		if (strcmp(argv[i], "-trace") == 0){
			trace = 1;
			next = 2, first = 2, best = 2, ffd = 2, bfd = 2, optm = 2;
			break;
		}
	}

	// variables to keep track of the number of bins used for each algorithm
	int bin_counter_next = 0, bin_counter_first = 0, bin_counter_best = 0, bin_counter_ffd = 0, bin_counter_bfd = 0;
	int bin_counter_optm = 0;


	while (i < argc){

		// This for loop checks to see if -next is an argument and if trace is on.  If so, tracing for next will occur
		// first as specified by the problem set specifications
		for (int i = size; i < argc; i++){
			// this statement checks if trace is on and if next is the argument being read
			if (strcmp(argv[i], "-next") == 0 && next == 2){
				next = 0;
			}
		}
		// if next is the algorithm, it is applied in this logic block
		if ((strcmp(argv[i], "-next") == 0 && next == 0) || (trace == 1 && next == 0)){
			bin_counter_next = 0;
			if (trace == 1 && size != 1){
				printf("Trace -next\n");
				next = 1;
			}
			if (size == 1){
				printf("-next: %d\n", bin_counter_next);
				next = 1;
			}
			// Variable to keep track of how much space is left in bin
			int bin_sum = 100;
			// for loop puts the size in the next available bin
			for (int j = 0; j < (size-1); j++){
				bin_sum = 100;
				while (bin_sum - num_sizes[j] >= 0 && j < (size-1)){
					bin_sum = bin_sum - num_sizes[j];
					// if trace is on, more details are output
					if (trace == 1){
						printf("arg: %d val: %d bin: %d total: %d\n", j, num_sizes[j], bin_counter_next, (100-bin_sum));
					}
					j++;
				}
				// keeping track of bins
				bin_counter_next++;
				j--;
			}
			// if trace is off, output occurs here
			if (trace == 0 && size != 1){
				printf("-next: %d\n", bin_counter_next);
			}
			// want to move i up 1 to take in the next algorithm
			i++;
			continue;
		}

		// for loop serves same purpose as above
		for (int i = size; i < argc; i++){
			if (strcmp(argv[i], "-first") == 0 && first == 2){
				first = 0;
			}
		}
		if ((strcmp(argv[i], "-first") == 0 && first == 0) || (trace == 1 && first == 0)){
			bin_counter_first = 0;
			if (trace == 1 && size != 1){
				printf("Trace -first\n");
				first = 1;
			}
			if (size == 1){
				printf("-first: %d\n", bin_counter_first);
				first = 1;
			}
			// This array will keep track of the values of every previous bin so I can check where to first put
			// the next number 

			int bin[(size-1)];
			for (int i = 0; i < (size-1); i++){
				bin[i] = 100;
			}
			// bin_counter_first takes value returned by function FirstAlgorithm
			bin_counter_first = FirstAlgorithm(bin, num_sizes, size, trace);
			
			// if trace flag not encountered, output occurs here
			if (trace == 0 && size != 1){
				printf("-first: %d\n", bin_counter_first);
			}			
			i++;	

			continue;
		}


		for (int i = size; i < argc; i++){			
			if (strcmp(argv[i], "-best") == 0 && best == 2){
				best = 0;
			}
		}

		if ((strcmp(argv[i], "-best") == 0 && best == 0) || (trace == 1 && best == 0)){
			bin_counter_best = 0;
			if (trace == 1 && size != 1){
				printf("Trace -best\n");
				best = 1;
			}
			if (size == 1){
				printf("-best: %d\n", bin_counter_best);
				best = 1;
			}

			int bin[(size-1)];
			for (int i = 0; i < (size-1); i++){
				bin[i] = 100;
			}
			// bin_counter_best takes value returned by BestAlgorithm
			bin_counter_best = BestAlgorithm(bin, num_sizes, size, trace);
			
			// if no trace, output occurs here
			if (trace == 0 && size != 1){
				printf("-best: %d\n", bin_counter_best);
			}
			i++;
			continue;
		}

		for (int i = size; i < argc; i++){
			if (strcmp(argv[i], "-ffd") == 0 && ffd == 2){
				ffd = 0;
			}
		}

		if ((strcmp(argv[i], "-ffd") == 0 && ffd == 0) || (trace == 1 && ffd == 0)){
			bin_counter_ffd = 0;
			if (trace == 1 && size != 1){
				printf("Trace -ffd\n");
				ffd = 1;
			}
			if (size == 1){
				printf("-ffd: %d\n", bin_counter_ffd);
				ffd = 1;
			}
			// array to store sorted values
			int replica[(size-1)];
			// implementation of bubble sort algorithm
			for (int i = 0; i < (size-1); i++){
				replica[i] = num_sizes[i];
			}
			for (int j = 0; j < (size-1); j++){
				for (int i = 0; i < (size-1); i++){
					if ((i+1) == (size-1)){
						continue;
					}
					int current = replica[i];
					int next_num = replica[i+1];
					if (current < next_num){
						replica[i] = next_num;
						replica[i+1] = current; 
					}
				}
			}
			int bin[(size-1)];
			for (int i = 0; i < (size-1); i++){
				bin[i] = 100;
			}
			
			// bin_counter_ffd takes results of FirstAlgorithm after sizes are sorted decreasingly
			bin_counter_ffd = FirstAlgorithm(bin, replica, size, trace);
			
			if (trace == 0 && size != 1){
				printf("-ffd: %d\n", bin_counter_ffd);
			}
			i++;
			continue;
		}


		for (int i = size; i < argc; i++){
			if (strcmp(argv[i], "-bfd") == 0 && bfd == 2){
				bfd = 0;
			}
		}

		if ((strcmp(argv[i], "-bfd") == 0 && bfd == 0) || (trace == 1 && bfd == 0)){
			bin_counter_bfd = 0;
			if (trace == 1 && size != 1){
				printf("Trace -bfd\n");
				bfd = 1;
			}
			if (size == 1){
				printf("-bfd: %d\n", bin_counter_bfd);
				bfd = 1;
			}
			// to store sorted values
			int replica[(size-1)];
			// bubble sort implementation
			for (int i = 0; i < (size-1); i++){
				replica[i] = num_sizes[i];
			}
			for (int j = 0; j < (size-1); j++){
				for (int i = 0; i < (size-1); i++){
					if ((i+1) == (size-1)){
						continue;
					}
					int current = replica[i];
					int next_num = replica[i+1];
					if (current < next_num){
						replica[i] = next_num;
						replica[i+1] = current; 
					}
				}
			}
			int bin[(size-1)];
			for (int i = 0; i < (size-1); i++){
				bin[i] = 100;
			}
			
			bin_counter_bfd = BestAlgorithm(bin, replica, size, trace);
			
			if (trace == 0 && size != 1){
				printf("-bfd: %d\n", bin_counter_bfd);
			}
			i++;
			continue;
		}

		for (int i = size; i < argc; i++){
			if (strcmp(argv[i], "-optm") == 0 && optm == 2){
				optm = 0;
			}
		}

		if ((strcmp(argv[i], "-optm") == 0 && optm == 0) || (trace == 1 && optm == 0)){
			bin_counter_optm = 0;
			if (trace == 1 && size != 1){
				printf("Trace -optm\n");
				optm = 1;
			}
			if (size == 1){
				printf("-optm: %d\n", bin_counter_optm);
				optm = 1;
			}
			int replica[(size-1)];
			// int trace = 0;
			for (int i = 0; i < (size-1); i++){
				replica[i] = num_sizes[i];
			}
			for (int j = 0; j < (size-1); j++){
				for (int i = 0; i < (size-1); i++){
					if ((i+1) == (size-1)){
						continue;
					}
					int current = replica[i];
					int next_num = replica[i+1];
					if (current < next_num){
						replica[i] = next_num;
						replica[i+1] = current; 
					}
				}
			}
			int bin[(size-1)];
			for (int i = 0; i < (size-1); i++){
				bin[i] = 100;
			}


			bin_counter_optm = BestAlgorithm(bin, replica, size, trace);

			if (trace == 0 && size != 1){
				printf("-optm: %d\n", bin_counter_optm);
			}
			i++;
			// optm = 1;
			continue;
		}	

		// if none of the if or else if blocks are entered, then I iterate i in order to move along argv
		// if it enters this else statement it means that the algorithm 
		else{
			i++;
		}
	}
	if (trace == 1 && size != 1){
			for (int j = 0; j < argc; j++){
				if (strcmp(argv[j], "-next") == 0){
					printf("-next: %d\n", bin_counter_next);
					i++;
				}
				if (strcmp(argv[j], "-first") == 0){
					printf("-first: %d\n", bin_counter_first);
					i++;
				}
				if (strcmp(argv[j], "-best") == 0){
					printf("-best: %d\n", bin_counter_best);
					i++;
				}
				if (strcmp(argv[j], "-ffd") == 0){
					printf("-ffd: %d\n", bin_counter_ffd);
					i++;
				}
				if (strcmp(argv[j], "-bfd") == 0){
					printf("-bfd: %d\n", bin_counter_bfd);
					i++;
				}
				if (strcmp(argv[j], "-optm") == 0){
					printf("-optm: %d\n", bin_counter_optm);
					i++;
				}
			}
		}
}

int FirstAlgorithm(int bin[], int num_sizes[], int size, int trace){
	for (int i = 0; i < (size-1); i++){
		bin[i] = 100;
	}
	int bin_counter = 0;
	// implementation of the first algorithm.  If size can fit in bin[j], it is placed there, otherwise
	// we move on to the next bin and check it.
	for (int i = 0; i < (size-1); i++){
		for (int j = 0; j < (size-1); j++){
			if (bin[j] - num_sizes[i] >= 0){
				bin[j] = bin[j] - num_sizes[i];
				if (trace == 1){
					printf("arg: %d val: %d bin: %d total: %d\n", i, num_sizes[i], j, (100-bin[j]));
				}
				break;
			}
			else{
				continue;
			}
		}
		
	}
	// bin_counter checks how many bins are less than 100 and that is how many bins were used 
	for (int i = 0; i < (size-1); i++){
		if (bin[i] != 100){
			bin_counter++;
		}
		// printf("%d\n", num_sizes[i]);
	}
	return bin_counter;
}

int BestAlgorithm(int bin[], int num_sizes[], int size, int trace){
	int bin_counter = 0;
	// Variable to keep track of what bin will result in the tightest fit
	int min_bin = 0;
	int min = 0;
	
	// the minimum value is stored in variable min as long as min >= 0
	// the algorithm then looks ahead one value in bin and sees if that would be a better fit.  If so, min changes
	// to the difference between the next bin and the current size, and the variable min_bin changes to j+1
	// if nothing else is a tighter fit, then bin[min_bin] is changed to bin[min_bin] - num_sizes[i]
	for (int i = 0; i < (size-1); i++){
		min_bin = 0;
		min = 100;
		for (int j = 0; j < (size-1); j++){
			
			if (j+1 == (size-1)){
				break;
			}
			
			if ((bin[j] - num_sizes[i] < min) && (bin[j] - num_sizes[i] >= 0)){
				min = bin[j] - num_sizes[i];
				min_bin = j; 
			}
			
		}
		if (bin[min_bin] - num_sizes[i] < 0){
			for (int k = 0; k < (size-1); k++){
				if (bin[k] == 100){
					bin[k] = bin[k] - num_sizes[k];
					break;
				}
			}
		}
		else{
			bin[min_bin] = bin[min_bin] - num_sizes[i];
		}

		// if trace is on, print more detailed output 
		if (trace == 1){
			printf("arg: %d val: %d bin: %d total: %d\n", i, num_sizes[i], min_bin, (100-bin[min_bin]));
		}
	}
	for (int i = 0; i < (size-1); i++){
		if (bin[i] != 100){
			bin_counter++;
		}
	}
	return bin_counter;
}
