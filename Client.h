#ifndef _CLI
#define _CLI

using namespace std;
#include <string>

class Client {
    public:
        Client();
        int init(char *port, char *serverName);
};

#endif