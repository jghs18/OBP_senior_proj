#include <arpa/inet.h>
#include <stdio.h>
#include <string>
#include <strings.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>
#include "Server.h"
#include "networks.h"



Server::Server(){}

int Server::tcpAccept(int mainServerSocket)
{
	struct sockaddr_in6 clientAddress;   
	int clientAddressSize = sizeof(clientAddress);
	int client_socket = 0;

	if ((client_socket = accept(mainServerSocket, (struct sockaddr*) &clientAddress, (socklen_t *) &clientAddressSize)) < 0)
	{
		perror("accept call");
		exit(-1);
	}
		printf("Client accepted.  Client IP: %s Client Port Number: %d\n",  
		getIPAddressString6(clientAddress.sin6_addr.s6_addr), ntohs(clientAddress.sin6_port));
	

	return(client_socket);
}

int Server::tcpServerSetup(int serverPort)
{
	// Opens a server socket, binds that socket, prints out port, call listens
	// returns the mainServerSocket
	
	int mainServerSocket = 0;
	struct pollfd fds[1];
	struct sockaddr_in6 serverAddress;     
	socklen_t serverAddressLen = sizeof(serverAddress);  

	mainServerSocket= socket(AF_INET6, SOCK_STREAM, 0);
	if(mainServerSocket < 0)
	{
		perror("socket call");
		exit(1);
	}

	memset(&serverAddress, 0, sizeof(struct sockaddr_in6));
	serverAddress.sin6_family= AF_INET6;         		
	serverAddress.sin6_addr = in6addr_any;
	serverAddress.sin6_port= htons(serverPort);         

	// bind the name (address) to a port 
	if (bind(mainServerSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
	{
		perror("bind call");
		exit(-1);
	}
	
	// get the port name and print it out
	if (getsockname(mainServerSocket, (struct sockaddr*)&serverAddress, &serverAddressLen) < 0)
	{
		perror("getsockname call");
		exit(-1);
	}

	if (listen(mainServerSocket, 6) < 0)
	{
		perror("listen call");
		exit(-1);
	}
	
	printf("Server Port Number %d \n", ntohs(serverAddress.sin6_port));

	fds[0].events = POLLIN;
    fds[0].fd = mainServerSocket;
    // THIS CALL WILL BLOCK UNTIL CLIENT CONNECTS
    if (poll(fds, 1, -1) < 0){
		printf("POLL CALL TIMED OUT\n\n");
		exit(-1);
	}
	printf("I HAVE A CONNECTION IN THE HOLE\n\n");
	mainServerSocket = tcpAccept(mainServerSocket);

	return mainServerSocket;
}
