#ifndef _TELD
#define _TELD
#include <string>
#include <cstring>
#include <math.h>
#include <string.h>
#include <strings.h>
#include "Parser.h"
#include "Instruction.h"
#pragma pack(1)


struct telData{             // Structure declaration
  float volt1;
  float volt2;
  float pressTank;
  float tempTank;
  float spaceCraftPosx;
  float spaceCraftPosy;
  float spaceCraftPosz;
  float spaceCraftVelx;
  float spaceCraftVely;
  float spaceCraftVelz;
  float spaceCraftAVx;
  float spaceCraftAVy;
  float spaceCraftAVz;
  float e1;
  float e2;
  float e3;
  float eta;
  uint8_t deploymentStatus;
};
#pragma pack(1)
struct cdhData{
  float core1Temp, core2Temp, core3Temp, core4Temp;
};
#pragma pack(1)
struct propsData{
  float pressTank, tempTank;
};
#pragma pack(1)
struct adcsData{
  float e1, e2, e3, eta;
};
#pragma pack(1)
struct commsData{
  float ampTemp1, ampTemp2, gX1, gY1, gZ1, gX2, gY2, gZ2;
};
#pragma pack(1)
struct payloadData{
  float lensTemp;
};
#pragma pack(1)
struct powerData{
  float volt1, volt2, watt1, watt2;
};
#pragma pack(1)
struct structuralData{
  uint8_t deploy1, deploy2;
};
#pragma pack(1)
struct thermalData{
  float busTemp1, busTemp2, busTemp3;
};

class TeleData {            
    public:
        TeleData();
        int sendTEL(uint8_t *buff, int tc);
        void update(void);
        void setPosData(float posx, float posy, float posz);
        void setVelData(float velx, float vely, float velz);
        void setAVData(float avelx, float avely, float avelz);
        void setQuat(float e1, float e2, float e3, float eta);
        void setNew(void);
        int getStatus(int target, uint8_t *buff);
        int batCheck();
        void setJeanStructOrbit(int32_t o1,int32_t o2,int32_t o3,int32_t o4,int32_t o5,int32_t o6);
        void setJeanStructPoint(int32_t o1,int32_t o2,int32_t o3,int32_t o4,int32_t o5,int32_t o6,int32_t o7);


};

#endif