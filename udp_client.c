/****************Client program********************************/

/* 
   NAME: Matthew Sleeth
   CS 4310 – 251 - Computer Networks
   
   udp_client.c is on zeus.cs.txstate.edu
   open a window on zeus.

   compile:	gcc -o udp_client udp_client.c

   $./udp_client eros.cs.txstate.edu
*/

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAX 1024
#define PORT 8080
#define SA struct sockaddr


int main(int argc, char** argv)
{
	int s, server_address_size;
	unsigned short port;
	struct sockaddr_in server;
	char buf[32];
	struct hostent* hostnm;
	int num;
	char msg[MAX];
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("socket creation error");
	}
	server.sin_family = AF_INET; // Internet domain
	server.sin_port = htons(PORT); // port
	hostnm = gethostbyname(argv[1]);  //get server's name
	server.sin_addr.s_addr = *((unsigned long*)hostnm->h_addr);

	// communication starts from here
	server_address_size = sizeof(server);

	for (;;) {
		// send an integer to the server
		printf("Enter : ");
		scanf("%s", &msg);
		sendto(s, &msg, sizeof(msg), 0, (struct sockaddr*)&server, server_address_size);
		
		// receive a message from the server
		recvfrom(s, msg, sizeof(msg), 0, (struct sockaddr*)&server, &server_address_size);
		printf("%s\n", msg);
		if ((strncmp(msg, "Exiting...\n ", 12)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}

	close(s);

	return 0;
}