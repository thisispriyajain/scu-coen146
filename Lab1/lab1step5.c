//Name: Priya Jain
//Date: Thursday, October 6, 2022
//COEN 146: Lab 1 - Step 5. This program calculates the time taken to copy files from both Steps 3 and 4. 

#include <stdio.h>   // fprintf(), fread(), fwrite(), fopen(), fclose()
#include <stdlib.h>  // malloc(), free()
#include <pthread.h> // pthread_create()
#include <unistd.h>  // read(), write()
#include <fcntl.h>   // open(), close()
#include <errno.h>   // errno
#include <time.h>    // clock()
#include <sys/mman.h> // mmap()
#include <sys/stat.h>
#include <sys/types.h>

#define BUF_SIZE 2048 //buffer size

void check_copy_times(char* src_filename, char* dst_filename) {
	clock_t func_start, func_end, syscall_start, syscall_end;
	double func_time, syscall_time;
	
	// capture runtime of func_copy() using start clock, call the copy, end the clock
	func_start = clock();
	func_copy(src_filename, dst_filename);
	func_end = clock();
	func_time = ((double) (func_end - func_start)) / CLOCKS_PER_SEC;

	// capture runtime of syscall_copy() using start clock, call the copy, end the clock
	syscall_start = clock();
	syscall_copy(src_filename, dst_filename);
	syscall_end = clock();
	syscall_time = ((double) (syscall_end - syscall_start)) / CLOCKS_PER_SEC;	
	
	printf("time to copy using functions: %.7f\n", func_time);
	printf("time to copy using syscalls: %.7f\n", syscall_time);
}

int main(int argc, char* argv[]) {
	if (argc != 3) {  // check correct usage
		fprintf(stderr, "usage: %s <src_filename> <dst_filename>\n", argv[0]);
		exit(1);
	}
	//get the source and destination files from the command line arguments
	char *src = argv[1];
	char *dest = argv[2];	

	//call the check copy times function
	check_copy_times(src, dest);
	
	return 0;
}