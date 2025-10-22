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
#include "networks.h"

// Wait for input takes in socket
// and the amount of time to wait for 
//input. 0 will wait indefiently
int waitForInput(struct pollfd *pd, int sec){
	int pollcall = 0;
	if ((pollcall = poll(pd, 1, sec)) < 0){
		perror("pollcall");
		exit(-1);
	}
	return pollcall;

}
/* SendPDU sends PDU with 2 bytes data len 
*/
int sendPDU(int clientSocket, uint8_t *dataBuffer, int lengthOfData){
  unsigned char buff[MAXBUF];

  int newLen = lengthOfData+2;
  
  uint16_t networkOrderLen = htons(newLen);

  memcpy(buff, &networkOrderLen, 2);
  memcpy(buff+2, dataBuffer, lengthOfData);
 // printf("I am sending now\n\n");
  return (send(clientSocket, buff, lengthOfData+2, 0)-2);

}
//RecvPDU recvs PDU into given bufferand returns bytes read
int recvPDU(int clientSocket, uint8_t *dataBuffer, int bufferSize){

  uint16_t len;
  int sizeBytes, bytes;
	//printf("IM RECVING\n\n");
  if ((sizeBytes = recv(clientSocket, (uint8_t *)&len, 2, MSG_WAITALL))<0){
    perror("Recv call");
    exit(-1);
  }
  else if (sizeBytes ==0){
	printf("I revcvd 0 BYTES\n\n");
    return 0;
  }
  len = ntohs(len);
  //printf("LEN IS: %i\n\n", len);
  len = len-2;
  if (len > bufferSize){
    perror("Buffer size too small");
    exit(-1);
  }
  bytes =recv(clientSocket, dataBuffer, len, MSG_WAITALL);
  return bytes;
}
unsigned char * gethostbyname6(const char * hostName, struct sockaddr_in6 * aSockaddr6)
{
	// returns ipv6 address and fills in the aSockaddr6 with address (unless its NULL)
	struct sockaddr_in6 * aSockaddr6Ptr = aSockaddr6;
	struct sockaddr_in6 aSockaddr6Temp;
	
	// if user does not care about the struct make a temp one
	if (aSockaddr6 == NULL)
	{
		aSockaddr6Ptr = &aSockaddr6Temp;
	}
		
	return(getIPAddress46(hostName, (struct sockaddr_storage *) aSockaddr6Ptr, AF_INET6));
}
unsigned char * getIPAddress46(const char * hostName, struct sockaddr_storage * aSockaddr, int addressFamily) 
{
	// Puts host IPv6 (or mapped IPV4) into the aSockaddr6 struct and return pointer to 16 byte address (NULL on error)
	// Only pulls the first IP address from the list of possible addresses
	
	static unsigned char ipAddress[INET6_ADDRSTRLEN];
	
	uint8_t * returnValue = NULL;
	int addrError = 0;
	struct addrinfo hints;	
	struct addrinfo *hostInfo = NULL;

	memset(&hints,0,sizeof(hints));
	if (addressFamily == AF_INET)
	{
		hints.ai_family = AF_INET;
	}
	else
	{
		hints.ai_flags = AI_V4MAPPED | AI_ALL;
		hints.ai_family = AF_INET6;
	}
	
	if ((addrError = getaddrinfo(hostName, NULL, &hints, &hostInfo)) != 0)
	{
		fprintf(stderr, "Error getaddrinfo (host: %s): %s\n", hostName, gai_strerror(addrError));
		returnValue = NULL;
	}
	else 
	{
		if (addressFamily == AF_INET)
		{
			memcpy(&((struct sockaddr_in *)aSockaddr)->sin_addr.s_addr, &((struct sockaddr_in*)hostInfo->ai_addr)->sin_addr.s_addr, 4);
			memcpy(ipAddress, &((struct sockaddr_in *)aSockaddr)->sin_addr.s_addr, 4); 
		}
		else
		{
			memcpy(((struct sockaddr_in6 *)aSockaddr)->sin6_addr.s6_addr, &(((struct sockaddr_in6 *)hostInfo->ai_addr)->sin6_addr.s6_addr), 16);
			memcpy(ipAddress, &((struct sockaddr_in6 *)aSockaddr)->sin6_addr.s6_addr, 16); 
		}
		
		returnValue = ipAddress;
		freeaddrinfo(hostInfo);
	}

  return returnValue;    // Either Null or IP address
}
char * getIPAddressString6(unsigned char * ipAddress)
{
	return getIPAddressString46(ipAddress, AF_INET6);
}
char * getIPAddressString46(unsigned char * ipAddress, int addressFamily)
{
	// makes it easy to print the IP address (v4 or v6)
	static char ipString[INET6_ADDRSTRLEN];

	if (ipAddress != NULL)
	{
		inet_ntop(addressFamily, ipAddress, ipString, sizeof(ipString));			
	}
	else
	{
		strcpy(ipString, "(IP not found)");
	}
	
	return ipString;
}


/*
 * in_cksum --
 *      Checksum routine for Internet Protocol family headers (C Version)
 */
