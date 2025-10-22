#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include "Unpacker.h"
#include "Instruction.h"
#include "networks.h"
#include "Parser.h"




Instruction *unpackMe(uint8_t *buff);

Unpacker::Unpacker(){}

Instruction *Unpacker::recvCOM(uint8_t *buff, int socket){
    if (recvPDU(socket, buff, MAXBUF) == 0){
        printf("bad connection\n");
        exit(-1);
    }
    //printf("I recvd something successfully\n\n");
    return unpackMe(buff);
}

Instruction *Unpacker::unpackMe(uint8_t *buff){
    int time;
    uint8_t mode;
    int32_t timefront, timeback;
    int8_t command, recurr, target, version;
    int16_t grpID, prio;
    int32_t op1,op2,op3,op4,op5,op6,op7;
    memcpy(&command, buff+6, 1);
    //printf("I read in the command as: %i\n\n", command);
    memcpy(&recurr,buff+7, 1);
    //printf("I have read in the recurr as: %i\n\n", recurr);
    memcpy(&target,buff+1, 1);
    memcpy(&version, buff, 1);
    //printf("I have read in target as: %i\n\n", target);
    //printf("VERSION: %i\n\n", version);
    memcpy(&prio, buff+2, 2);
    prio = ntohs(prio);
    //printf("I haev read in prio as: %i\n\n", prio);
    memcpy(&timeback, buff+8, 4);
    memcpy(&timefront, buff+12, 4);
    timeback = ntohl(timeback);
    timefront = ntohl(timefront);
    memcpy(&time, &timefront, 4);
    memcpy(&time+4, &timeback, 4);

    //printf("TIME: %i\n\n", time);

    memcpy(&op1, buff+16, 4);
    memcpy(&op2, buff+20, 4);
    memcpy(&op3, buff+24, 4);
    memcpy(&op4, buff+28, 4);
    memcpy(&op5, buff+32, 4);
    memcpy(&op6, buff+36, 4);
    memcpy(&op7, buff+40, 4);
    op1 = ntohl(op1);
    op2 = ntohl(op2);
    op3 = ntohl(op3);
    op4 = ntohl(op4);
    op5 = ntohl(op5);
    op6 = ntohl(op6);
    op7 = ntohl(op7);
/*
    printf("OP1: %i\n\n", op1);
    printf("OP2: %i\n\n", op2);
    printf("OP3: %i\n\n", op3);
    printf("OP4: %i\n\n", op4);
    printf("OP5: %i\n\n", op5);
    printf("OP6: %i\n\n", op6);
    printf("OP7: %i\n\n", op7);
*/


    if (command == SETMODE){
        //printf("I AM GETTING A MODE\n\n");
        memcpy(&mode, buff+16, 1);
        //printf("MODE IS: %i\n\n ", mode);
    }
    else{
        mode = -1;
    }
    if (version == 1){
        //printf("I AM MAKING A NEW OBJ\n\n");
        return new Instruction(command, target, prio, recurr, mode, time, op1,op2,op3,op4,op5,op6,op7);
    }
    else{
        unpackMeTEL(buff);
        return NULL;
    }
    
}
// TODO: Make a printer for tel data
void Unpacker::unpackMeTEL(uint8_t *buff){
    int32_t volt1, volt2, pressTank, tempTank, posX, posY, posZ, velX, velY, velZ;
    int8_t deploy;
    uint8_t bigData[MAX_LEN];

    strcpy((char *)bigData,(char *)buff+16);
    printf("%s",bigData);
    /*
    char line[] = "==================";

    memcpy(bigData, buff+16, 41);
    memcpy(&volt1, bigData, 4);
    memcpy(&volt2, bigData+4, 4);
    memcpy(&pressTank, bigData+8, 4);
    memcpy(&tempTank, bigData+12, 4);
    memcpy(&posX, bigData+16, 4);
    memcpy(&posY, bigData+20, 4);
    memcpy(&posZ, bigData+24, 4);
    memcpy(&velX, bigData+28, 4);    
    memcpy(&velY, bigData+32, 4);
    memcpy(&velZ, bigData+36, 4);
    memcpy(&deploy, bigData+37, 1);

    volt1 = ntohl(volt1);
    volt2 = ntohl(volt2);
    pressTank = ntohl(pressTank);
    tempTank = ntohl(tempTank);
    posX = ntohl(posX);
    posY = ntohl(posY);
    posZ = ntohl(posZ);
    velX = ntohl(velX);
    velY = ntohl(velY);
    velZ = ntohl(velZ);
    
    printf("%s\n", line);
    printf("Battery Report:\n\nBattery 1:\t%i (V)\n",volt1);
    printf("Battery 2:\t%i (V)\n",volt2);
    printf("%s\n", line);
    printf("Props Report:\n\nTank Pressure:\t%i (MPa)\n",pressTank);
    printf("Tank Temperature:\t%i (C)\n",tempTank);
    printf("%s\n", line);
    printf("Positional Data:\n\nPos X:\t%u (Km)\n", posX);
    printf("Pos Y:\t%i (Km)\n", posY);
    printf("Pos Z:\t%i (Km)\n",posZ);
    printf("Vel X:\t%i (Km/s)\n",velX);
    printf("Vel Y:\t%i (Km/s)\n",velY);
    printf("Vel Z:\t%i (Km/s)\n",velZ);
    printf("%s\n",line);
    */

}