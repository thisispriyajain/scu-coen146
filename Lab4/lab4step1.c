//Name: Priya Jain
//Date: October 27, 2022
//Title: Lab 4 - Step 1
//Description: C program for a UDP server that receives a file from a client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

//Declare a Header structure that holds length of a packet
typedef struct { 
	int len;
} Header;

//Declare a packet structure that holds data and header
typedef struct {
	Header header;
	char data[10];
} Packet;

struct sockaddr_in servAddr, clienAddr;

void printPacket(Packet packet) {
	printf("Packet{ header: {len: %d }, data: \"", packet.header.len);
	fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
	printf("\" }\n");	
}

void serverSend(int sockfd, const struct sockaddr *address, socklen_t addrlen) {
	Packet packet;
	strcpy(packet.data, "Acknowledge");
	packet.header.len = sizeof(packet.data);
	
	sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
	printf("\t Server sending...%s \n", packet.data);
}

Packet serverReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen) {
	Packet packet;
	while (1) {
		recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);
		printPacket(packet);
		serverSend(sockfd, address, *addrlen);
		break;
	}
	return packet;
}

int main (int argc, char *argv[]) {
	if(argc != 3) {
		printf("Usage: %s <port> <dstfile>\n", argv[0]);
		exit(0);
	}

	int sockfd;
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Failure to set up an endpoint socket");
		exit(1);
	}

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[1]));
	servAddr.sin_addr.s_addr = INADDR_ANY;

	if((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0) {
		perror("Failure to bind server socket to endpoint socket \n");
		exit(1);
	}

	int fp = open(argv[2], O_CREAT | O_RDWR, 0666);
	if(fp < 0) {
		perror("File failed to open\n");
		exit(1);
	}
	
	Packet packet;
	socklen_t addr_len = sizeof(clienAddr);
	printf("Waiting for packets to come...\n");
	
	do {
		packet = serverReceive(sockfd, (struct sockaddr *)&clienAddr, (socklen_t *)&addr_len);
		write(fp, packet.data, packet.header.len);

	} while (packet.header.len != 0);

	close(fp);
	close(sockfd);

	return 0;
}
