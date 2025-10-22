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
#include "Client.h"
#include "networks.h"

using namespace std;

Client::Client(){}

//Init client function that takes ina
// string of port # and server IP address to connect to.
// Returns newly obtained socket number

int Client::init(char *port, char *serverName){
    int client_fd;
    uint8_t *ipAddress = NULL;
    struct sockaddr_in6 serv_addr;

    if ((client_fd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr_in6));
    serv_addr.sin6_family = AF_INET6;
    
    serv_addr.sin6_port = htons(atoi(port));

    if ((ipAddress = gethostbyname6(serverName, &serv_addr))==NULL){
        exit(-1);
    }
    printf("%s THIS IS THE IP\n\n", getIPAddressString6(ipAddress));
	if(connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("connect call");
		exit(-1);
    }
	printf("Connected to %s IP: %s Port Number: %s\n", serverName, getIPAddressString6(ipAddress), port);
	
	return client_fd;
}
