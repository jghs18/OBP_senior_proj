#ifndef PARS_
#define PARS_


#include <iostream>
#include <string>
#include "Instruction.h"
using namespace std;

#define MAX_LEN 1700
#define TOKEN_LEN 4
#define FLAG_LEN 3

// define class
enum COMMAND {
    GETSTATUS, GETQUEUE, GETMODE, SETMODE, MAN, POINT
};
enum SUBSYS{
    CDH, PROPS, ADCS, COMMS, PAYLOAD, POWER ,
    STRUCT, THERMAL, ORBITS
};
class Parser {
    public:
        Parser();
        Instruction *loadInstruction(string inp);
   
};
#endif