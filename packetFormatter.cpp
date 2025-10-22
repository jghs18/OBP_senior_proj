#include <iostream>
#include <string>
#include <cstring>
#include "networks.h"
#include "Instruction.h"
#include "packetFormatter.h"
#include "TeleData.h"
#include "Unpacker.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "checksum.h"

using namespace std;


void formatSRCHeader(uint8_t *buff, int8_t version, uint8_t pktType, uint8_t dataHeader,
                    uint8_t processID, int8_t category, uint8_t grpNum, uint16_t seqCount);
void formatDATAHeader(uint8_t *buff, uint8_t serv, int8_t subserv, int64_t time);


int sendTelPacket(uint8_t *buff, int socket, uint8_t version, uint8_t pktType, uint8_t dataHeader,
                    uint8_t processID, int8_t category, uint8_t grpNum, uint16_t seqCount,
                    uint8_t serv, uint8_t subserv, int64_t time, uint8_t *telData, uint16_t dataLen);
packetFormatter::packetFormatter() {};

//Send tel packet takes in empty buffer, socket number, loads of 
// packet data and sends it.

int packetFormatter::sendTelPacket(uint8_t *buff, int socket, uint8_t version, uint8_t pktType, uint8_t dataHeader,
                    uint8_t processID, int8_t category, uint8_t grpNum, uint16_t seqCount,
                    uint8_t serv, uint8_t subserv, int64_t time, uint8_t *telData, uint16_t dataLen){
    uint16_t totLen = dataLen;
    uint16_t cksm, totLenNetwork;
    formatSRCHeader(buff, version, pktType, dataHeader, processID, category, grpNum, seqCount);
    formatDATAHeader(buff, serv, subserv, time);
    totLen += 18;
    memcpy(buff+16, telData, dataLen);
    totLenNetwork = htons(totLen);
    memcpy(buff+4, &totLenNetwork, 2);
    //printf("I am now trying to calc checksum\n\n");
    cksm = in_cksum((unsigned short *)buff, totLen);
    cksm = htons(cksm);
    memcpy(buff+totLen-2, &cksm, 2);
   // printf("I am now trying to send the packet\n\n");
   // printf("TOTLEN: %i\n\n", totLen);
    return sendPDU(socket, buff, totLen);
}

void formatSRCHeader(uint8_t *buff, int8_t version, uint8_t pktType, uint8_t dataHeader,
                    uint8_t processID, int8_t category, uint8_t grpNum, uint16_t seqCount){
    int8_t packID = 0;
    int8_t packID2 = 0;
    uint16_t packSEQCNT = 0;

    //printf("I am now formatting the src header\n\n");
    memcpy(&packID, &version, 1);
   // printf("VERSION: %i\n\n", version);
    memcpy(&packID2, &category, 1);
   // printf("TARGET: %i\n\n", category);
    memcpy(buff, &packID, 1);
    memcpy(buff+1, &packID2, 1);

    packSEQCNT = seqCount;
    packSEQCNT = htons(packSEQCNT);
    memcpy(buff+2, &packSEQCNT, 2);
   // printf("I finished making the src header\n\n");
}

void formatDATAHeader(uint8_t *buff, uint8_t serv, int8_t subserv, int64_t time){
    int32_t frontEnd, backEnd;

    //printf("I am now making the data header\n\n");
    memcpy(buff+6, &serv, 1);
   // printf("This is the command: %i\n\n", serv);
    memcpy(buff+7, &subserv, 1);
   //printf("This is recurring: %i\n\n", subserv);
    memcpy(&frontEnd, &time, 4);
    memcpy(&backEnd, &time+4, 4);
    frontEnd = htonl(frontEnd);
    backEnd = htonl(backEnd);
    memcpy(buff+8, &backEnd, 4);
    memcpy(buff+12, &frontEnd, 4);
    //printf("I finished making the data headern\n");
}


void packetFormatter::makeComPacket(uint8_t *buff, int socket, Instruction *i){
   // printf("Okay here we go to make the apcet\n\n");
    uint8_t opBuff[232];
    struct ob op;
    struct o_pointer op11;
    int32_t op1,op2,op3,op4,op5,op6,op7;
    opBuff[0] = i->getModeID();
    if (i->getCommand() == 4){
        op = i->getOrbitalParam();
        op1 = htonl(op.x);
        op2 = htonl(op.y);
        op3 = htonl(op.z);
        op4 = htonl(op.vx);
        op5 = htonl(op.vy);
        op6 = htonl(op.vz);
    }
    else if (i->getCommand() == 5){
        op11 = i->getPointParam();
        op1 = htonl(op11.e1);
        op2 = htonl(op11.e2);
        op3 = htonl(op11.e3);
        op4 = htonl(op11.eta);
        op5 = htonl(op11.ax);
        op6 = htonl(op11.ay);
        op7 = htonl(op11.az);
    }
    else{
    sendTelPacket(buff, socket,  1, 1, 0, 0, i->getTarget(),
                 0, i->getPriority(), i->getCommand(), i->getRecur(), i->getTime(), opBuff, 1);
                 return;
    }

    memcpy(opBuff, &op1, 4);
    memcpy(opBuff+4, &op2, 4);
    memcpy(opBuff+8, &op3, 4);
    memcpy(opBuff+12, &op4, 4);
    memcpy(opBuff+16, &op5, 4);
    memcpy(opBuff+20, &op6, 4);
    memcpy(opBuff+24, &op7, 4);


    sendTelPacket(buff, socket,  1, 1, 0, 0, i->getTarget(),
                 0, i->getPriority(), i->getCommand(), i->getRecur(), i->getTime(), opBuff, 224);
    }

