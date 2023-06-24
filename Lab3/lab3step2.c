// Name: Priya Jain
//Date: October 20, 2022
//Title: Lab 3 - Step 2
// Description: C  program for a TCP client that connects to the server.

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
	//Get from the command line, server IP, src and dst files
	if(argc != 5) {
		printf("Usage: %s <ip of server> <port #> <src_file> <dest_file>\n",argv[0]);
		exit(0);
	}
	//Declare socket file descriptor and buffer
	int sockfd;
	char buf[1024];

	//Declare server address to connect to
	struct sockaddr_in servAddr;
	struct hostent *host;

	//get hostname
	host = (struct hostent *) gethostbyname(argv[1]);

	//Open a socket, if successful, returns
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Failure to set up an important socket");
		exit(0);
	}

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[2]));
	servAddr.sin_addr = *((struct in_addr *)host->h_addr);

	//Connect to the server
	if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) {
		perror("Failure to connect to the server");
		exit(1);
	}

	//while(1) {
		//printf("Client: Type a message to send to Server\n");
		//scanf("%s", sbuf);
		//write(sockfd, sbuf, sizeof(sbuf));
		//read(sockfd, rbuf, sizeof(rbuf));
		//printf("Server: sent message %s\n", rbuf);
	//}
	//Close socket descriptor
	//close(sockfd);
	//return 0;


	write(sockfd, argv[3], strlen(argv[3]) + 1);
	
	FILE *fp;
	int x;
	fp = fopen(argv[4], "w");
	while((x = read(sockfd, buf, 10)) != 0) {
		fwrite(&buf, x, 1, fp);
	}
	
	fclose(fp);
	close(sockfd);
	return 0;
	
}
