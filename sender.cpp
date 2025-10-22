#include "Instruction.h"
#include "Parser.h"
#include "packetFormatter.h"
#include "Client.h"
#include "TeleData.h"
#include "networks.h"
#include "Unpacker.h"
#include <iostream>
#include <queue>
#include <string>
#include <string.h>
#include <limits>
#include "pollLib.h"
#include <ctime>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdint.h>

using namespace std;
Instruction *nextInstr;
Parser pp = Parser();
Client c = Client();
packetFormatter packer = packetFormatter();
int main(int argc, char *argv[]){
    int sock;
    setupPollSet();
    addToPollSet(STDIN_FILENO);


    sock = c.init(argv[1], argv[2]);
    addToPollSet(sock);
    int currPoll;
    Unpacker unpack = Unpacker();
    uint8_t buff[MAX_LEN];
    while(1){
        printf("MOJO$: ");
		fflush(stdout);
		if ((currPoll = pollCall(-1)) < 0){
			perror("The poll request timed out");
			exit(-1);
		}
        else{
            if (currPoll == STDIN_FILENO){
                memset(buff, 0, MAX_LEN);
                string mystr;
                getline(cin, mystr);
                if ((nextInstr = pp.loadInstruction(mystr))== NULL){
                    printf("Bad Instruction\n\n");
                    continue;
                }
                //printf("Okay I have an instruction, I am making a packet\n\n");
                //printf("Here is the Prio: %i\n\n", nextInstr->getPriority());
                //printf("Here is the recurr: %i\n\n", nextInstr->getRecur());
                //printf("Here is the target: %i\n\n", nextInstr->getTarget());
                //printf("Here is the command: %i\n\n", nextInstr->getCommand());
                //printf("HERE IS TIME: %i\n\n", nextInstr->getTime());
                packer.makeComPacket(buff, sock, nextInstr);
                free(nextInstr);
            }
            else{
                unpack.recvCOM(buff, currPoll);
            }    
        }
    }
}