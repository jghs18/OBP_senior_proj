#ifndef NETW_
#define NETW_

#define MAXBUF 1700

int waitForInput(struct pollfd *fds, int sec);
int sendPDU(int clientSocket, uint8_t *dataBuffer, int lengthOfData);
int recvPDU(int clientSocket, uint8_t *dataBuffer, int bufferSize);
char * getIPAddressString6(unsigned char * ipAddress);
char * getIPAddressString46(unsigned char * ipAddress, int addressFamily);
unsigned char * getIPAddress46(const char * hostName, struct sockaddr_storage * aSockaddr, int addressFamily) ;
unsigned char * gethostbyname6(const char * hostName, struct sockaddr_in6 * aSockaddr6);
unsigned short in_cksum(unsigned short *addr,int len);

#endif