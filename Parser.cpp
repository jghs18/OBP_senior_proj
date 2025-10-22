#include <iostream>
#include <string>
#include <cstring>
#include "Instruction.h"
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include "Parser.h"
#include <sstream>
#include <cmath>
#include <iomanip>
using namespace std;

int num_fields = -1;
bool hasTarget;
bool correctInput;
bool orbitals;
bool hasPrior;
bool checking_mode;
int param_num;
int32_t op1,op2,op3,op4,op5,op6,op7;
string err_msg;

int handleCommand(char *command);
int handleTarget(char *target);
bool checkInt(char *tok);
bool checkIntOP(char *tok);
void handleFlags(char *flags, int *mode, int *recurring, int *time);
    Parser::Parser() {}
    
    Instruction *Parser::loadInstruction(string inp){
        orbitals = false;
        correctInput = true;
        hasTarget = false;
        checking_mode = false;
        hasPrior = true;
        
        // Read in the Input as a string
        // make a new char buffer
        int command, prio, target, recurring, time, mode;
        char newBuff[MAX_LEN];
        char *tok;
        strcpy(newBuff, inp.c_str());
        if ((tok = strtok(newBuff, ","))==NULL)
        {
            return NULL;
        }
        target = -1;
        recurring = -1;
        mode = -1;
        time = -1;
        prio = 0;


        // read in command first to determine number of following fields
        command = handleCommand(tok);
        tok = strtok(NULL, ",");
        
        for (int i = 0; i < num_fields; i++){          
            switch (i){
                case 0:
                    if (tok==NULL){
                        correctInput = false;
                        return NULL;
                    }else{
                        if (hasPrior){
                            if (checkInt(tok)){
                                prio = atoi(tok);
                                tok = strtok(NULL, ",");
                            }
                        }
                    }
                    break;
                case 1:
                    if (hasTarget==true){
                        if (tok==NULL){
                            correctInput = false;
                            return NULL;
                        }
                    target = handleTarget(tok);
                    tok = strtok(NULL, ",");
                    }
                    break;
                case 2:
                    if (tok==NULL){
                        correctInput = false;
                        return NULL;
                    }
                    
                    handleFlags(tok, &mode, &recurring, &time);
                    tok = strtok(NULL, ",");
                    break;
                default:
                break;
            }    
        }
        // cout<<"what is tok:"<<tok<<endl;
        // tok = strtok(NULL,",");
        // cout<<"what is tok:"<<tok<<endl;
        // if (tok!=NULL){
        //     err_msg = "Too many parameters";
        //     return NULL;
        // };
        char orbital_params[15];
        if(orbitals){
            strcpy(orbital_params,tok);
            tok = strtok(orbital_params, " ");
            for (int i = 0; i < param_num; i++){
                switch (i){
                    case 0:{
                        if(checkIntOP(tok)){
                            (op1) = atoi(tok);
                        }
                        break;
                    }
                    case 1:{
                        if(checkIntOP(tok)){
                            (op2) = atoi(tok);
                        //returns 0 on error
                        }
                        break;
                    }
                    case 2:{
                        if(checkIntOP(tok)){
                            (op3) = atoi(tok);
                        //returns 0 on error
                        }
                        break;
                    }
                    case 3:{
                        if(checkIntOP(tok)){
                            (op4) = atoi(tok);
                        //returns 0 on error
                        }
                        break;
                    }
                    case 4:{
                        if(checkIntOP(tok)){
                            (op5) = atoi(tok);
                        //returns 0 on error
                        }
                        break;
                    }
                    case 5:{
                        if(checkIntOP(tok)){
                            (op6) = atoi(tok);
                        //returns 0 on error
                        }
                        break;
                    }
                    case 6:{
                        if(checkIntOP(tok)){
                            (op7) = atoi(tok);
                        }
                        break;
                    }
                    default:{
                        break;
                    }
                }
                if((tok = strtok(NULL, " "))==NULL && i<param_num-1){
                    printf("Not enough orbitals provided. TRY AGAIN\n");
                    correctInput = false;
                    break; // change to return int
                }
            }
        }
        if (tok!=NULL){
            err_msg = "Too many fields\n";
            correctInput = false;
            return NULL;
        }
        if (correctInput==true){
            if (time > 0) {
                prio = pow(2,14);
                // cout<<"time is: "<<time<<endl;
                recurring = -1;
            }
            return new Instruction(command,target,prio,recurring,mode,time,op1,op2,op3,op4,op5,op6,op7);
        }else{
            cout<<err_msg<<endl;
            return NULL;
        }

    }

    int handleCommand(char *command){
        if (!strcmp(command, "POINT")){
            num_fields = 1;
            orbitals = true;
            hasTarget = false;
            param_num = 7;
            return POINT;
        }
        else if (!strcmp(command, "MAN")){
            num_fields = 1;
            orbitals = true;
            hasTarget = false;
            param_num = 6;
            return MAN;
        }
        else if (!strcmp(command, "GETSTATUS")){
            num_fields = 3;
            hasTarget = true;
            return GETSTATUS;
        }
        else if (!strcmp(command, "GETQUEUE")){
            num_fields = 1;
            hasTarget = false;
            return GETQUEUE;
        }
        else if (!strcmp(command, "GETMODE")){
            num_fields = 1;
            hasTarget = false;
            return GETMODE;
        }
        else if (!strcmp(command, "SETMODE")){
            num_fields = 3;
            hasPrior = false;
            hasTarget = false;
            checking_mode = true;
            return SETMODE;
        }else{
            err_msg = "Bad Flag Command\n";
            correctInput = false;
            return -1;
        }
    }
    int handleTarget(char *target){
        if (!strcmp(target, "CDH")){
            return CDH;
        }
        if (!strcmp(target, "PROPS")){
            return PROPS;     
        }
        else if (!strcmp(target, "ADCS")){
            return ADCS;
        }
        else if (!strcmp(target, "COMMS")){
            return COMMS;
        }
        else if (!strcmp(target, "PAYLOAD")){
            return PAYLOAD;
        }
        else if (!strcmp(target, "POWER")){
            return POWER;
        }
        else if (!strcmp(target, "STRUCT")){
            return STRUCT;
        }
        else if (!strcmp(target, "THERMAL")){
            return THERMAL;
        }
        else if(!strcmp(target, "ORBITS")){
            return ORBITS;
        }
        else{
            err_msg = "Bad Flag Target\n";
            correctInput = false;
            return -1;
        }
    }
    //change to return an int
    void handleFlags(char *flags, int *mode, int *recurring, int *time){
        char *tok;
            tok = strtok(flags, " ");
            for (int i = 0; i < FLAG_LEN; i++){
            switch (i){
                case 0:{
                    if(checkInt(tok)){
                        (*mode) = atoi(tok);
                        if ((checking_mode) && !(*mode>=0 && *mode<=2)){
                            correctInput = false;
                            err_msg = "Mode must be between 0-1";
                        }
                    //returns 0 on error
                    }
                    break;
                }
                case 1:{
                    if(checkInt(tok)){
                        (*recurring) = atoi(tok);
                        // if recurring then cannot be timed
                    }
                    break;
                }
                case 2:{
                    if (tok == 0){
                        (*time) = atoi(tok);
                    }else{
                    // fix this

                        std::tm t = {};
                        std::istringstream ss(tok);
                        t.tm_isdst = 1;

                        if (ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
                        {
                            // std::cout << std::put_time(&t, "%c") << "\n"
                            //         << std::mktime(&t) << "\n";
                        }
                        else
                        {
                            // std::cout << "Incorrect time input\n";
                            err_msg = "Incorrect time input\n";
                            correctInput = false;
                        }
                        (*time) = std::mktime(&t);
                        // (*time) = mktime(&t) - timezone;
                        // (*time) = timegm(&t);
                        // cout<<"Converted time = "<<*time<<endl;
                    }
                    break;
                }
                    //  (*time) = atoi(tok);
                default:{
                    break;
                }
            }
            if((tok = strtok(NULL, " "))==NULL && i<FLAG_LEN-1){
                printf("Not enough flags provided. TRY AGAIN\n");
                correctInput = false;
                break; // change to return int
            }
        }
        if (tok!=NULL){
            printf("Too many flags\n");
            correctInput = false;
        }
        return;
    }

    bool checkInt(char *tok){
        for (int i = 0; i<strlen(tok); i++){
            if(tok[i]>=48 && tok[i]<=57){
                // do nothing
            }else{
                printf("Incorrect input, field only be integers\n");
                correctInput = false;
                return false;
            }
        }
        return true;
    }
    bool checkIntOP(char *tok){
        for (int i = 0; i<strlen(tok); i++){
            if (i==0){
                // allowed to be negative
                if((tok[i]==45) || (tok[i]>=48 && tok[i]<=57)){
                    // do nothing
                }else{
                    printf("Incorrect input, field only be integers\n");
                    correctInput = false;
                    return false;
                }
            }
            else if(tok[i]>=48 && tok[i]<=57){
                // do nothing
            }else{
                printf("Incorrect input, field only be integers\n");
                correctInput = false;
                return false;
            }
        }
        return true;
    }