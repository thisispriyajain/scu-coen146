//Name: Priya Jain
//Date: October 20, 2022
//Title: Lab 3 - Step 1
//Description: C program for TCP server that accepts a client connection for file transfer.

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int sockfd, connfd, rb, sin_size;

char rbuf[1024], sbuf[1024];

struct sockaddr_in servAddr, clienAddr;

void *connectionHandler (void *sock) {
	char buff[20];

	int sockNum = *(int *) sock;
	int n;

	FILE *fp;

	printf("Connection established with client IP: %s and Port: %d\n", inet_ntoa(clienAddr.sin_addr), ntohs(clienAddr.sin_port));

	if((n = read(sockNum, buff, sizeof(buff))) > 0) {
		printf("opening file %s\n", buff);
	}

	fp = fopen(buff, "r");

	if(fp != NULL) {
		printf("Opened file successfully\n");
	}

	int x;

	while((x = fread(&rbuf, sizeof(char), 10, fp)) > 0) {
		write(sockNum, rbuf, x);
	}

	printf("File transfer complete\n");

	fclose(fp);

	close(connfd);

	return 0;
}

int main(int argc, char *argv[]) {
	//Get from the command line port#
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
	servAddr.sin_addr.s_addr = INADDR_ANY;

	if((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0) {
		perror("Failure to bind server address to the endpoint socket");
		exit(1);
	}
	printf("Server waiting for client at port %d\n", atoi(argv[1]));

	//Server listening to the socket endpoint and can queue 5 client requests
	listen(sockfd, 5);
	sin_size = sizeof(struct sockaddr_in);
	
	//while (1) {
		//Server accepting to establish a connection with a connecting client, if successful, returns a connection descriptor
		if((connfd = accept(sockfd, (struct sockaddr*)&clienAddr, (socklen_t *)&sin_size)) < 0) {
			perror("Failure to accept connection to the client");
			exit(1);
		}
	
	//Connection established, server begins to read and write to the connecting client
	printf("Connection established with client: IP %s and Port %d\n", inet_ntoa(clienAddr.sin_addr), ntohs(clienAddr.sin_port));
	//while((rb = read(connfd, rbuf, sizeof(rbuf))) > 0) {
		//rbuf[rb] = '\0';
		//printf("Client sent: %s\n", rbuf);
		//write(connfd, "Acknowledge", 20);
	//}
	
	//connfd = accept(sockfd, (struct sockaddr *)&clienAddr, (socklen_t *)&sin_size);
	connectionHandler((void *)&connfd);

	close(connfd);
	
	close(sockfd);
	return 0;

	//connfd = accept(sockfd, (struct sockaddr *)&clienAddr, (socklen_t *)&sin_size);
	//connectionHandler((void *)&connfd);		
}
