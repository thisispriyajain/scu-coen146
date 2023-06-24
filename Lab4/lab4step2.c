//Name: Priya Jain
//Date: October 27, 2022
//Title: Lab 4 - Step 2
//Description: UDP client sends a file to a server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>

typedef struct {
	int len;
} Header;

typedef struct {
	Header header;
	char data[10];
} Packet;

void printPacket(Packet packet) {
	printf("Packet{ header: {len: %d}, data: \"", packet.header.len);
	fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
	printf("\" }\n");
}

void clientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, Packet packet) {
	while(1) {
		printf("Client sending packet...\n");
		sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
		Packet recvpacket;
		recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0, (struct sockaddr *)address, &addrlen);
		printPacket(recvpacket);
		break;
	}
}

int main(int argc, char *argv[]) {
	if(argc != 4) {
		printf("Usage: %s <ip> <port> <srcfile>\n", argv[0]);
		exit(0);
	}

	int sockfd;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Failure to setup an endpoint socket");
		exit(1);
	}

	struct sockaddr_in servAddr;
	struct hostent *host;

	host = (struct hostent *) gethostbyname(argv[1]);

	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_port = htons(atoi(argv[2]));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr = *((struct in_addr *)host->h_addr);

	int fp = open(argv[3], O_RDWR);
	if(fp < 0) {
		perror("Failed to open file\n");
		exit(1);
	}

	socklen_t addr_len = sizeof(servAddr);
	int x;
	Packet packet;
	while ((x= read(fp, packet.data, sizeof(packet.data))) > 0) {
		packet.header.len = x;
		clientSend(sockfd, (struct sockaddr *)&servAddr, addr_len, packet);
	}

	printf("%d", x);

	Packet final;
	final.header.len = 0;

	clientSend(sockfd, (struct sockaddr *)&servAddr, addr_len, final);

	close(fp);
	close(sockfd);

	return 0;
}
