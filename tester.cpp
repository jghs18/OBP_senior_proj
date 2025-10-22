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
#include <poll.h>
#include <ctime>
#include <math.h>
#include "pollLib.h"
using namespace std;
// typedef enum State STATE;



typedef enum State {
    SAFE, STANDBY
} STATE;


class Compare {
    public:
       bool operator()(Instruction* below, Instruction* above){
           if(below->getPriority() < above->getPriority()){
               return true;
           }
           return false;
      }
};

class CompareTime{
    public:
        bool operator()(Instruction* below, Instruction* above){
            if(below->getTime() < above->getTime()){
                return true;
            }
            return false;
        }
};


priority_queue <Instruction*, vector<Instruction*>, Compare > command_list;
priority_queue <Instruction*, vector<Instruction*>, CompareTime > timed_list;
queue <Instruction*> executed_commands;
// // initlialize parser and instruction
Instruction *nextInstr;
Instruction *currInstr;
Parser pp = Parser();
packetFormatter p = packetFormatter();
TeleData *td;
uint8_t dataBuff[MAX_LEN];
uint8_t buff[MAX_LEN];
STATE s;
int q_len = command_list.size();

int printQueue(priority_queue <Instruction*, vector<Instruction*>, Compare > q,uint8_t *buff)
{
	//printing content of queue 
    int amount = 0;
    amount += sprintf((char *)buff+amount,"MAIN QUEUE: ");
    if (!q.empty()){
        while (!q.empty()){
            Instruction* topQ = q.top();
            string str1 = topQ->printCmdStatus();
            const char* strnew = str1.c_str();
            amount += sprintf((char *)buff+amount,"%s\n\n",strnew);
            q.pop();
        }
    }else{
        amount += sprintf((char *)buff+amount,"Main Empty\n\n");
    }
	return amount;
}

int printQueueTime(priority_queue <Instruction*, vector<Instruction*>, CompareTime > q,uint8_t *buff)
{
	//printing content of queue 
    int amount = 0;
    int count = 0;
    amount += sprintf((char *)buff+amount,"TIMED QUEUE: ");
    if(!q.empty()){
        while (!q.empty()){
            //cout<<"DID I MAKE IT?"<<endl;
            Instruction* topQ = q.top();
            string str1 = topQ->printCmdStatus();
            const char* strnew = str1.c_str();
            amount += sprintf((char *)buff+amount,"%s\n\n",strnew);
            q.pop();
        }
    }else{
        amount += sprintf((char *)buff+amount,"Timed Empty\n\n");
    }
	return amount;
}
int printQueue2(queue <Instruction*> q2, uint8_t *buff)
{
    int amount = 0;
    amount += sprintf((char *)buff+amount,"EXECUTED QUEUE: ");
	//printing content of queue 
    if(!q2.empty()){
        //cout<<"looking at executed"<<endl;
        while (!q2.empty()){
            Instruction* topQ2 = q2.front();
            string str2 = topQ2->printCmdStatus();
            const char* strnew = str2.c_str();
            amount += sprintf((char *)buff+amount,"%s\n\n", strnew);
            q2.pop();
	}
    }else{
        amount += sprintf((char *)buff+amount,"Executed Empty\n\n");
    }
    return amount;
}

void update_queue(Instruction *nextInstr){
    // WE MIGHT NOT WANT THIS LATER --
    int com = nextInstr->getCommand();
    if (com==SETMODE){
        nextInstr->setPriority(pow(2,14));
    }
    bool timed = false;
    if (nextInstr->getTime()>0){
        timed = true;
    }
    if (timed==true){
        timed_list.push(nextInstr);
        cout<<"pushed to timed list"<<endl;
    }else{
        cout<<"pushed to cmd list"<<endl;
        command_list.push(nextInstr);
    }
}

Instruction* getnextInstr(){
     Instruction *topInstr;
     time_t now = time(0);
    // first check the timed list
    // make sure it is not empty
    //cout<<"time now is:"<<now<<endl;
    if (!(timed_list.empty()) && timed_list.top()->getTime() <= now){
        topInstr = timed_list.top();
        timed_list.pop();
        cout<<"Timed command executed"<<endl;
        return topInstr;
    }else if(!command_list.empty())
    {
        topInstr = command_list.top();
        command_list.pop();
        return topInstr;
    }
    return NULL;
    
}

void evaluate_queue(Instruction *currInst, int socket)
{  
    char line[] = "=====================\n\n";

    char uni[] = "GETMODE REPORT\n\n";
    char uni2[] = "GETQUEUE REPORT\n\n";
    if (currInst!=NULL){
    // check to see if a command was grabbed, if not its empty skip all this
    int CMD = currInst->getCommand();
    int amount;
    
    // do the command - fill these out
    switch(CMD){
        case GETSTATUS:{
            amount = td->getStatus(currInst->getTarget(),dataBuff);
            p.sendTelPacket(buff,socket,0, 1, 0, 0, 0,
                 0, 0, 0, 0, 0, dataBuff, amount);
            break;
        }case GETQUEUE:{
            amount = sprintf((char*)dataBuff,"%s%s\n\n",uni2, line);
            amount += printQueue(command_list,dataBuff+amount);
            amount += printQueueTime(timed_list,dataBuff+amount);
            amount += printQueue2(executed_commands,dataBuff+amount)+1;
            p.sendTelPacket(buff,socket,0, 1, 0, 0, 0,
                 0, 0, 0, 0, 0, dataBuff, amount);
            break;
        }case GETMODE:{
            // cout<<STATE;
            // int to_return = STATE;
            amount = sprintf((char*)dataBuff, "%s%sCurrent Mode: %i\n\n%s",uni,line,s,line)+1;
            p.sendTelPacket(buff,socket,0, 1, 0, 0, 0,
                 0, 0, 0, 0, 0, dataBuff, amount);
            break;
        }case SETMODE:{
            s = (STATE)currInst->getModeID();
            amount = sprintf((char*)dataBuff, "%s%sMode Set: %i\n\n%s",uni,line,s,line)+1;
            p.sendTelPacket(buff,socket,0, 1, 0, 0, 0,
                 0, 0, 0, 0, 0, dataBuff, amount);
            // add support for 
            break;
        }case MAN:{
            cout<<"I changerd studd"<<endl;
            // td->userPos = true;
            td->setJeanStructOrbit(currInstr->getOp1(),currInstr->getOp2(),currInstr->getOp3(),currInstr->getOp4(),currInstr->getOp5(),currInstr->getOp6());
            break;
        }case POINT:
            // td->userPos = true;
            td->setJeanStructPoint(currInstr->getOp1(),currInstr->getOp2(),currInstr->getOp3(),currInstr->getOp4(),currInstr->getOp5(),currInstr->getOp6(),currInstr->getOp7());
            break;
    }
    
    currInst->setStatus(true);
    executed_commands.push(currInst);
    
    if (currInst->getRecur()>0){
        cout<<"DID I ENTER"<<endl;
        // this means that command should recur no kizzy
        currInst->dRecur();
        command_list.push(currInst);
    }
    // acknowledge completion of command
    

    // prepare command report to be sent to ground- add timestamp of execution

    time_t now = time(0);
    char* dt = ctime(&now);
    tm *gmtm = gmtime(&now);
    dt = asctime(gmtm);
    //cout << "Time of execution: "<< dt << endl;

    }
    else{
        // both queues empty dont eval
        cout<<"Both queues are empty no evals done."<<endl;
    }
}



// test instruction obj
int main(int argc, char *argv[]){
    s = STANDBY;
    int telemetry_counter = 0;
    int com, telSent;

    Server serv = Server();
    time_t now, past;
    Unpacker unpack = Unpacker();
    
    //printf("MADE IT\n");

    Instruction *nextInstr;
    int mode, socket, amount;
    uint8_t buff[MAX_LEN];
    string str_read;
    (*td) = TeleData();
    // initialize server
    //printf("AMDE IT\n\n");
    socket = serv.tcpServerSetup(atoi(argv[1]));
    //printf("I AM HERREn\n");
    setupPollSet();
    addToPollSet(socket);


    //initlialize parser and instruction
    while (1) {
        
        nextInstr = NULL;
        //printf("WENT AROUND\n\n");
        // read input client here
        //printf("IN THE WHILE LOOP\n\n");
        if (pollCall(0) > 0){
            printf("I RECVD A COMMAND\n\n");
            if ((nextInstr = unpack.recvCOM(buff, socket)) == NULL){
                printf("Lost Connection\n\n");
                exit(-1);
            }
        }
        // input command into queue
        if (nextInstr){
            update_queue(nextInstr);
        }
        
        currInstr = getnextInstr();
        //printf("I HAVE MADE IT TO GETNEXT\n\n");
        // send command report/ Telemetry data
        if (currInstr){
        switch(s){
            case 0:
                com = currInstr->getCommand();
                if (com==SETMODE || com==GETMODE){
                    evaluate_queue(currInstr, socket);
                    // this function should update the mode
                }else{
                    amount = sprintf((char*)dataBuff, "In SAFE Mode. Commands not accepted\n")+1;
                    p.sendTelPacket(buff,socket,0, 1, 0, 0, 0,
                    0, 0, 0, 0, 0, dataBuff, amount);
                }
                break;
            case 1:
                evaluate_queue(currInstr, socket);
                //evaluate_queue();
                break;
            exit(-1);
        }
        // execute command from queue
        
        }
        //printf("MADE IT OUT OF SWITCH\n\n");
        td->update();
        //printf("TRYING TO UODATE\n\n");
        now = time(0);
        telSent = (past == now);
        if (now%60 == 0 && !telSent){
            telSent = 1;
            past = now;
            amount = td->sendTEL(dataBuff,telemetry_counter);
            telemetry_counter += 1;
            p.sendTelPacket(buff,socket,0, 1, 0, 0, 0,
                 0, 0, 0, 0, 0, dataBuff, amount);
        }
        if (td->batCheck()){
            if (s != SAFE){
            s = SAFE;
            amount = sprintf((char*)dataBuff, "Battery Voltage below 5 V.\nEntering Safe Mode.\n")+1;
            p.sendTelPacket(buff,socket,0, 1, 0, 0, 0,
                 0, 0, 0, 0, 0, dataBuff, amount);
            }
        }
        currInstr = NULL;
        }
        
    } 