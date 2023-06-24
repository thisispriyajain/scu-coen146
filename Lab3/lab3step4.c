//Name: Priya Jain
//Date: October 20, 2022
//Title: Lab 3 - Step 4
//Description: C program for a concurrent TCP server that accepts and responds to multiple client connection requestsi

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <pthread.h>

#define N 100

int threadCount = 0;

pthread_t clients[N];

int sockfd, connfd, sin_size;

char buf[1024];

struct sockaddr_in servAddr, clienAddr;

void *connectionHandler(void *sock) {
	char buff[20];
	int sockNum = *(int *) sock;
	int n;
	FILE *fp;

	printf("Connection established with client IP: %s and Port: %d\n", inet_ntoa(clienAddr.sin_addr), ntohs(clienAddr.sin_port));

	if((n = read(connfd, buff, sizeof(buff))) > 0) {
		printf("Opening file %s\n", buff);
		fp = fopen(buff, "r");
		if(fp != NULL) {
			printf("File opened\n");
		}
	}

	int x;
	while(x == fread(&buf, sizeof(char), 10, fp)) {
		write(sockNum, buf, x);
	}
	printf("File transfer complete\n");

	fclose(fp);

	close(connfd);

	pthread_exit(0);
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Usage: %s <port #> \n", argv[0]);
		exit(0);	
	}
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Failure to set up an endpoint socket");
		exit(1);	
	}
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[1]));
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0) {
		perror("Failure to bind server address to the endpoint socket");
		exit(1);
	} 
	
	printf("Server listening/waiting for client at port 5000\n");
	listen(sockfd, 5);
	sin_size = sizeof(struct sockaddr_in);

	while(1) {
		connfd = accept(sockfd, (struct sockaddr *)&clienAddr, (socklen_t *)&sin_size);
		
		if(pthread_create(&clients[threadCount], NULL, connectionHandler, (void*) &connfd) < 0) {
			perror("Unable to create a thread");
			exit(0);
		}
		else {
			printf("Thread %d has been created to service client request\n", ++threadCount);
		}

	}

	int i;

	for(i = 0; i < threadCount; i++) {
		pthread_join(clients[i], NULL);
	}

	return 0;

}
