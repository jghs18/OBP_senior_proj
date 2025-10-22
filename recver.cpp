#include "Instruction.h"
#include "Parser.h"
#include "packetFormatter.h"
#include "Server.h"
#include <iostream>
#include <queue>
#include <string>
#include <string.h>
#include <limits>
#include <arpa/inet.h>
#include "Unpacker.h"
#include "networks.h"
#include "TeleData.h"
#include "Unpacker.h"
#include <ctime>

using namespace std;
Instruction *nextInstr;
Server s = Server();
packetFormatter p = packetFormatter();
Unpacker unpack = Unpacker();

int main(int argc, char *argv[]){
    //struct telData t;
    int sock;
    sock = s.tcpServerSetup(atoi(argv[1]));
    uint8_t buff[MAX_LEN];
    while(1){
        memset(buff, 0, MAX_LEN);
        printf("I am waiting for Input...\n\n");
        //waitForInput(sock, -1);
        printf("I have recvd something\n\n");
        nextInstr = unpack.recvCOM(buff, sock);
        //p.makeTelPacket(buff, sock, 0, &t);
    }
    return 1;
}