//Name: Priya Jain
//Date: Thursday, October 6, 2022
//COEN 146: Lab 1 - Step 3. This program copies files (binary and text) using functions.

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

int func_copy(char* src_filename, char* dst_filename) {
	FILE *src;
	FILE *dst;
	char *buf;	

	src = fopen(src_filename, "r");	// opens a file for reading
	if (src == NULL) { // fopen() error checking
		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
		exit(0);
	}
	dst = fopen(dst_filename, "w");	// opens a file for writing; erases old file/creates a new file
	if (dst == NULL) { // fopen() error checking
		fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
		exit(0);
	}

	buf = malloc((size_t)BUF_SIZE);  // allocate a buffer to store read data
	// reads content of file is loop iterations until end of file
	while (fread(buf, 1, sizeof(buf), src)>0) {
	//writes bytes_read to dst_filename
		fwrite(buf, 1, sizeof(buf), dst);
	}

	// closes src file pointer
	fclose(src);

	// closes dst file pointer
	fclose(dst);

	// frees memory used for buf
	free(buf);

	return 0;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {  // check correct usage
		fprintf(stderr, "usage: %s <src_filename> <dst_filename>\n", argv[0]);
		exit(1);
	}
	//get the source and destination files from the command line arguments
	char *src = argv[1];
	char *dest = argv[2];	

	func_copy(src, dest);
	
	return 0;
}