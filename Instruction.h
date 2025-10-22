#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <string>
using std::string;
// using namespace std;
// define class

struct ob {
           int32_t x,y,z,vx,vy,vz;
        };
struct o_pointer{
            int32_t e1,e2,e3,eta,ax,ay,az;
        };

class Instruction {
    private:
        int command;
        int target;
        bool status;
        int priority;
        int recurring;
        uint8_t mode;
        int time;
        int32_t p1,p2,p3,p4,p5,p6,p7;
        struct ob orbital_param;
        struct o_pointer point_param;
        std::string cmd_str;
    public:
        Instruction(int c, int t,  int p, int r, uint8_t m, int ti,int32_t op1, int32_t op2,int32_t op3,int32_t op4,int32_t op5,int32_t op6, int32_t op7);
        Instruction();
        struct ob getOrbitalParam() {return orbital_param;}
        struct o_pointer getPointParam() {return point_param;}
        bool getStatus() {return status; }
        void setStatus(bool b);
        void setPriority(int p);
        int getTime(){return time;}
        void dRecur();
        int getTarget() {return target;}
        int getCommand() {return command;}
        int getPriority() {return priority; }
        int getRecur() {return recurring; }
        uint8_t getModeID(){return mode;}
        int32_t getOp1(){return p1;}
        int32_t getOp2(){return p2;}
        int32_t getOp3(){return p3;}
        int32_t getOp4(){return p4;}
        int32_t getOp5(){return p5;}
        int32_t getOp6(){return p6;}
        int32_t getOp7(){return p7;}

        string printCmdStatus();
        bool operator > (Instruction& i1);
        bool operator < (Instruction& i1);
        // friend bool operator< ( Instruction i1,  Instruction i2);
        // bool operator< (const Instruction& i1, const Instruction &i2);
        // bool operator<
};
#endif