// beep boop some instructions no basal - add flag import once created
#include "Instruction.h"
#include <string>
#include <iostream>
using std::string;

// Instruction Constructor
Instruction::Instruction(int c, int t,  int p, int r, uint8_t m, int ti,int32_t op1, int32_t op2,int32_t op3,int32_t op4,int32_t op5,int32_t op6, int32_t op7){
    target = t;
    command = c;
    priority = p;
    recurring = r;
    mode = m;
    status = false;
    time = ti;
    p1 = op1;
    p2 = op2;
    p3 = op3;
    p4 = op4;
    p5 = op5;
    p6 = op6;
    p7 = op7;
    cmd_str = "CMD not yet performed";

    if (command == 4){
        orbital_param.x = p1;
        orbital_param.y = p2;
        orbital_param.z = p3;
        orbital_param.vx = p4;
        orbital_param.vy = p5;
        orbital_param.vz = p6;
        // point_param = NULL;
    }else if (command == 5){
        point_param.e1 = p1;
        point_param.e2 = p2;
        point_param.e3 = p3;
        point_param.eta = p4;
        point_param.ax = p5;
        point_param.ay = p6;
        point_param.az = p7;

        // orbital_param = NULL;
    }
}
void Instruction::setStatus(bool b){
    status = b;
}

void Instruction::dRecur(){
    recurring--;
}

void Instruction::setPriority(int p){
    priority = p;
}

string Instruction::printCmdStatus()
{
    if (status){
        cmd_str = "CMD:" + std::to_string(target) + std::to_string(command) + std::to_string(priority) + std::to_string(recurring) + 
            std::to_string(status);
    }
    else{
        cmd_str = "CMD not yet performed:" + std::to_string(target) + std::to_string(command) + std::to_string(priority) + std::to_string(recurring) + 
            std::to_string(status);
    }
    return cmd_str;
                
}

bool Instruction::operator > (Instruction& i1)
{
    return priority > i1.getPriority();
}

bool Instruction::operator < (Instruction& i1)
{
    return priority < i1.getPriority();
}