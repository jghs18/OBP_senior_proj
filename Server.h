#ifndef _SERV
#define _SERV

class Server {
    public:
        Server();
        int tcpAccept(int mainServerSocket);
        int tcpServerSetup(int serverPort);
};

#endif