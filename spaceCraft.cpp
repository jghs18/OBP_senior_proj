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

typedef enum State STATE;

enum STATE {
    SAFE, ERROR, DOWNLINK, STANDBY
};

STATE read_in(string str_read);

void update_queue();

void print_queue();
//initialize queue
priority_queue <Instruction*, vector<Instruction*>, greater<vector<Instruction*>::value_type> > command_list; // current onboard queue
queue<Instruction*> past_commands; // this queue stores a list of all the commands that have been executed in FIFO order
int q_len;
// SC will take in string and convert to instruction obj

int main(int argc, char *argv[]){
    Parser pp;
    Server s = Server();
    Unpacker unpack = Unpacker();
    packetFormatter p = packetFormatter();
    pp = Parser();
    Instruction *nextInstr;
    int mode, socket;
    uint8_t buff[MAX_LEN];
    string str_read;
    
    // initialize server
    socket = s.tcpServerSetup(atoi(argv[1]));


    //initlialize parser and instruction
    
    int STATE = STANDBY;
    while (1) {
        nextInstr = pp.loadInstruction();
        switch(STATE){
            case SAFE:
                int com = nextInstr->getCommand();
                if (com==SETMODE){
                    int a_max = std::numeric_limits<int>::max();
                    nextInstr->setPriority(a_max);
                    update_queue(nextInstr);
                    // switch to new mode using mode as inputted
                    evaluate_queue();
                    // this function should update the mode
                }
                break;
            case ERROR:
                break;
            case DOWNLINK:
                break;
            case STANDBY:
                update_queue(nextInstr);
                break;
            
        }
        // READ FROM SOCKET
    }
}

void update_queue(Instruction *nextInstr){
    command_list.push(nextInstr);
    q_len = command_list.size();
}

void printQueue(priority_queue <Instruction*, vector<Instruction*>, Compare > q)
{
	//printing content of queue 
	while (!q.empty()){
        Instruction* topQ = q.top();
        string str1 = topQ->printCmdStatus();
		cout<<" "<<str1;
		q.pop();
        cout<<" "<<topQ->getPriority();
	}
	cout<<endl;
}

void evaluate_queue()
{
    Instruction *currInstr = command_list.top();
    command_list.pop();
    past_commands.push(currInstr);

    int CMD = currInstr->getCommand();

    // do the command - fill these out
    switch(CMD){
        case GETSTATUS:
            break;
        case GETQUEUE:
            for (int j = 0; j < command_list.size(); j++){

            }
            break;
        case GETMODE:
            // int to_return = STATE;
            break;
        case SETMODE:
            // add support for 
            break;
        case HOLD:
            break;
        case CAP:
            break;
        case SEND:
            break;
    }

    // acknowledge completion of command
    currInstr->setStatus(true);

    // prepare command report to be sent to ground- add timestamp of execution
    currInstr->setCmdStatus();
    string cmd_report = currInstr->getCmdStatus();

    // send report to ground - incomplete
}

