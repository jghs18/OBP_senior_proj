#ifndef _UNPA
#define _UNPA

#include "Instruction.h"
#define VMASK 7
#define TMASK 1
#define PIDMASK 2032
#define SEQCNT 16383
#define PCATMASK 15
#define GRPMASK 3

class Unpacker {
    public:
        Unpacker();
        Instruction *unpackMe(uint8_t *buff);
        Instruction *recvCOM(uint8_t *buff, int socket);
        void unpackMeTEL(uint8_t *buff);
};

#endif