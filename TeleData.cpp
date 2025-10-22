#include "TeleData.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;


vector<vector<string> > contentR;
void readData();
int tel_count;
int userPos;
struct ob telstruc_orbit;
struct o_pointer telstruc_pointers;


double arr[40][6] = {{3506.2,0,0,0.001,1.68,0.0002},

{3505.894601,48.4308935,0.005765583,-0.022186614,1.679839856,0.000199981},
{3504.920818,96.85255491,0.011530066,-0.045368417,1.679359466,0.000199924},
{3503.278853,145.2557533,0.017292352,-0.068540981,1.678558937,0.000199828},
{3500.969037,193.6312589,0.02305134,-0.091699872,1.677438439,0.000199695},
{3497.991827,241.9698506,0.028805935,-0.114840665,1.675998204,0.000199524},
{3494.34781,290.2623168,0.034555038,-0.137958943,1.674238524,0.000199314},
{3490.03769,338.4994725,0.040297556,-0.161050317,1.672159744,0.000199067},
{3485.062283,386.6721887,0.046032403,-0.18411046,1.669762254,0.000198781},
{3479.422567,434.7712429,0.051758481,-0.207134926,1.667046544,0.000198458},
{3473.119649,482.7874277,0.057474694,-0.230119273,1.664013163,0.000198097},
{3466.15476,530.7115653,0.063179948,-0.253059076,1.660662722,0.000197698},
{3458.529254,578.5345077,0.068873156,-0.275949929,1.656995887,0.000197261},
{3450.244608,626.2471366,0.074553231,-0.298787448,1.653013383,0.000196787},
{3441.302422,673.8403633,0.080219091,-0.321567268,1.648715991,0.000196276},
{3431.70442,721.305129,0.085869658,-0.344285041,1.644104551,0.000195727},
{3421.452449,768.6324046,0.091503858,-0.366936441,1.639179961,0.00019514},
{3410.54848,815.8131906,0.097120618,-0.389517161,1.633943175,0.000194517},
{3398.994607,862.8385172,0.102718871,-0.412022913,1.628395207,0.000193857},
{3386.793046,909.6994445,0.108297553,-0.43444943,1.622537127,0.000193159},
{3373.946137,956.3870622,0.113855603,-0.456792462,1.616370062,0.000192425},
{3360.456346,1002.89249,0.119391963,-0.479047781,1.609895199,0.000191654},
{3346.326223,1049.206919,0.124905586,-0.501211214,1.603113754,0.000190847},
{3331.558409,1095.321613,0.13039543,-0.523278622,1.59602698,0.000190003},
{3316.155782,1141.227716,0.135860442,-0.545245727,1.588636294,0.000189123},
{3300.121341,1186.916425,0.141299574,-0.567108275,1.580943166,0.000188208},
{3283.458196,1232.37899,0.146711785,-0.588862047,1.572949119,0.000187256},
{3266.169571,1277.606724,0.152096039,-0.610502855,1.564655727,0.000186269},
{3248.258799,1322.590995,0.157451309,-0.632026544,1.556064614,0.000185246},
{3229.729326,1367.323233,0.162776575,-0.653428992,1.547177456,0.000184188},
{3210.584709,1411.794924,0.168070824,-0.67470611,1.537995979,0.000183095},
{3190.828619,1455.997613,0.173333049,-0.695853843,1.528521958,0.000181967},
{3170.464835,1499.922904,0.17856225,-0.716868168,1.518757223,0.000180804},
{3149.497251,1543.562459,0.183757436,-0.737745093,1.508703651,0.000179608},
{3127.92987,1586.907999,0.188917619,-0.758480663,1.498363171,0.000178377},
{3105.76681,1629.951304,0.194041822,-0.779070952,1.487737763,0.000177112},
{3083.012298,1672.684211,0.199129073,-0.79951207,1.476829458,0.000175813}};


int getStatus(int target, uint8_t *buff);

        struct telData tel;
        struct cdhData cdh;
        struct propsData props;
        struct adcsData adcs;
        struct commsData comms;
        struct payloadData payload;
        struct powerData power;
        struct structuralData structural;
        struct thermalData thermal;
        int counter, vecSize;

int TeleData::batCheck(){
    if (power.volt1 > 5.0 && power.volt2 > 5.0){
        return 0;
    }
    return 1;
}

TeleData::TeleData(){
    counter=0;
    // userPos = 0;
    tel_count = 0;
    // readData();
    memset(&tel,0,69);
    memset(&cdh,0,16);
    memset(&props,0,8);
    memset(&adcs,0,16);
    memset(&comms,0,32);
    memset(&payload,0,8);
    memset(&power,0,16);
    memset(&structural,0,2);
    memset(&thermal,0,12);

}
int TeleData::getStatus(int target, uint8_t *buff){

    char uni[] = "GETSTATUS REPORT\n\n";
    char line[] = "=====================\n\n";
    char coreTemp[] = "Core Temperature (C): ";
    char coreTemp2[] = "Core2 Temperature (C): ";
    char coreTemp3[] = "Core3 Temperature (C): ";
    char coreTemp4[] = "Core4 Temperature (C): ";
    char tankTemp[] = "Tank Temperature (C): ";
    char tankPres[] = "Tank Pressure (MPa): ";

    
    char e1[] = "e1: ";
    char e2[] = "e2: ";
    char e3[] = "e3: ";
    char eta[] = "eta: ";
    char amp1T[] = "Amplifier1 Temperature (C): ";
    char amp2T[] = "Amplifier2 Temperature (C): ";
    char gx1[] = "Gimble1 X: ";
    char gy1[] = "Gimble1 Y: ";
    char gz1[] = "Gimble1 Z: ";
    char gx2[] = "Gimble2 X: ";
    char gy2[] = "Gimble2 Y: ";
    char gz2[] = "Gimble2 Z: ";
    char posx[] = "Spacecraft Position X (km): ";
    char posy[] = "Spacecraft Position Y (km): ";
    char posz[] = "Spacecraft Position Z (km): ";
    char velx[] = "Spacecraft Velocity X (km): ";
    char vely[] = "Spacecraft Velocity Y (km): ";
    char velz[] = "Spacecraft Velocity Z (km): ";
    
    char lens[] = "Lens Temperature (C): ";
    char v1[] = "Voltage1 (V): ";
    char v2[] = "Voltage2 (V): ";
    char w1[] = "Watt1 (W): ";
    char w2[] = "Watt2 (W): ";
    char d1[] = "Deploy1: ";
    char d2[] = "Deploy2: ";
    char b1t[] = "Bus Temperature1 (C): ";
    char b2t[] = "Bus Temperature2 (C): ";
    char b3t[] = "Bus Temperature3 (C): ";
    switch(target){
        case CDH:
            return sprintf((char *)buff, "%s%s%s%f\n%s%f\n%s%f\n%s%f\n%s",
                    uni,line,coreTemp,cdh.core1Temp,coreTemp2,cdh.core2Temp,
                    coreTemp3,cdh.core3Temp,coreTemp4,cdh.core4Temp,line)+1;
        case PROPS:
            return sprintf((char *)buff, "%s%s%s%f\n%s%f\n%s",
                    uni,line,tankTemp,props.tempTank,tankPres,props.pressTank,line)+1;
        case ADCS:
            cout<<userPos<<"userpos"<<endl;
            if (userPos){
                cout<<"I MADE IR IN"<<endl;
                setQuat(telstruc_pointers.e1,telstruc_pointers.e2,telstruc_pointers.e3,telstruc_pointers.eta);
                setAVData(telstruc_pointers.ax,telstruc_pointers.ay,telstruc_pointers.az);
                userPos = 0;
                return sprintf((char *)buff, "%s%s%s%f\n%s%f\n%s%f\n%s%f\n%s",
                    uni,line,e1,tel.e1,e2,tel.e2,e3,tel.e3,eta,tel.eta,line)+1;
            }       
            return sprintf((char *)buff, "%s%s%s%f\n%s%f\n%s%f\n%s%f\n%s",
                    uni,line,e1,adcs.e1,e2,adcs.e2,e3,adcs.e3,eta,adcs.eta,line)+1;
        case COMMS:
            return sprintf((char *)buff, "%s%s%s%f\n%s%f\n%s%f\n%s%f\n%s%f\n%s%f\n%s%f\n%s%f\n%s",
                    uni,line,amp1T,comms.ampTemp1,amp2T,comms.ampTemp2,gx1,comms.gX1,gy1,comms.gY1,
                    gz1,comms.gZ1,gx2,comms.gX2,gy2,comms.gY2,gz2,comms.gZ2,line)+1;
        case PAYLOAD:
            return sprintf((char *)buff, "%s%s%s%f\n%s",
                    uni,line,lens,payload.lensTemp,line)+1;
        case POWER:
            return sprintf((char *)buff, "%s%s%s%f\n%s%f\n%s%f\n%s%f\n%s",
                    uni,line,v1,power.volt1,v2,power.volt2,w1,power.watt1,w2,power.watt2,line)+1;
        case STRUCT:
            return sprintf((char *)buff, "%s%s%s%i\n%s%i\n%s",
                    uni,line,d1,structural.deploy1,d2,structural.deploy2,line)+1;
        case THERMAL:
            return sprintf((char *)buff, "%s%s%s%f\n%s%f\n%s%f\n%s",
                    uni,line,b1t,thermal.busTemp1,b2t,thermal.busTemp2,b3t,thermal.busTemp3,line)+1;
        case ORBITS:
            // cout<<userPos<<"userpos"<<endl;
            // cout<<telstruc_orbit.x<<"should be 0"<<endl;
            // cout<<telstruc_orbit.y<<"should be 0"<<endl;
            // cout<<telstruc_orbit.z<<"should be 0"<<endl;
            // cout<<telstruc_orbit.vx<<"should be 0"<<endl;
            // cout<<telstruc_orbit.vy<<"should be 0"<<endl;
            // cout<<telstruc_orbit.vz<<"should be 0"<<endl;
            if (userPos){
                // cout<<"I MADE IR IN"<<endl;
                setPosData(telstruc_orbit.x,telstruc_orbit.y,telstruc_orbit.z);
                setVelData(telstruc_orbit.vx,telstruc_orbit.vx,telstruc_orbit.vz);
                userPos = 0;
                // return sprintf((char *)buff, "%s%s%s%f\n%s%f\n%s%f\n%s%f\n%s%f\n%s%f\n%s",
                //     uni,line,posx,telstruc_orbit.x,posy,telstruc_orbit.y,posz,telstruc_orbit.z,velx,
                //     telstruc_orbit.vx,vely,telstruc_orbit.vx,velz,telstruc_orbit.vz,line)+1;
            }       
            return sprintf((char *)buff, "%s%s%s%f\n%s%f\n%s%f\n%s%f\n%s%f\n%s%f\n%s",
                    uni,line,posx,tel.spaceCraftPosx,posy,tel.spaceCraftPosy,posz,tel.spaceCraftPosz,velx,
                    tel.spaceCraftVelx,vely,tel.spaceCraftVely,velz,tel.spaceCraftVelz,line)+1;
        default:
        return 0;

    }
}

int TeleData::sendTEL(uint8_t *buff, int t_count){
    tel_count = t_count;
    char line[] = "=====================\n\n";
    // MAN,1,0 0 0 0 0 0
    // GETSTATUS,1,ORBITS,0 0 0
    // char posx[] = "Spacecraft Position X (km): ";
    // char posy[] = "Spacecraft Position Y (km): ";
    // char posz[] = "Spacecraft Position Z (km): ";
    // maybe add velocity here

    // change to user deigned parameters no kizzy
    if (userPos){
        setPosData(telstruc_orbit.x,telstruc_orbit.y,telstruc_orbit.z);
        setVelData(telstruc_orbit.vx,telstruc_orbit.vy,telstruc_orbit.vz);
        userPos = 0;
    }
    
    int amount = 0;
    for (int i = 0; i<9; i++){
        amount = (getStatus(i,buff+amount)-1) + amount;
    }
    // amount = sprintf((char *) buff+amount, "%s%s%f\n%s%f\n%s%f\n\n", line, posx,
    //         tel.spaceCraftPosx,posy, tel.spaceCraftPosy,posz,tel.spaceCraftPosz) + amount;
    return amount+1;
}

void setCDH(void){
    cdh.core1Temp = 4*sin(counter)+50;
    cdh.core2Temp = 5*sin(counter)+55;
    cdh.core3Temp = 4*sin(counter)+49;
    cdh.core4Temp = 3*sin(counter)+60;
}

void setProps(void){
    props.pressTank = 200*sin(counter)+200000;
    props.tempTank = 20*sin(counter)-20;
    //printf("New Pressure: %f\n", props.pressTank);
}

void setADCS(void){
    adcs.e1 = 5*sin(counter)+300;
    adcs.e2 = 234*sin(counter)-14;
    adcs.e3 = 2*sin(counter)+ 89;
    adcs.eta = 19*sin(counter);
}

void setComms(){
    comms.ampTemp1 = 4*sin(counter)+50;
    comms.ampTemp2 = 4*sin(counter)+20;
    comms.gX1 = 2*sin(counter)+ 89;
    comms.gY1 = 7.9*sin(counter)+ 9;
    comms.gZ1 = 1.3*sin(counter)+ 49;
    comms.gX2 = 23*sin(counter)-1;
    comms.gY2 = 1*sin(counter);
    comms.gZ2 = 8*sin(counter) +98;
}
void setPayload(){
    payload.lensTemp = 20*sin(counter)-20;
}

void setPower(){
    power.volt1 = 1*sin(counter+200) + 20;
    power.volt2 = 1*sin(counter) + 20;
    power.watt1 = 1*sin(counter)+300;
    power.watt2 = 1.2*sin(counter)+300;
}

void setStruct(){
    structural.deploy1 = 1;
    structural.deploy2 = 1;
}
void setThermal(){
    thermal.busTemp1 = 5*sin(counter)+35;
    thermal.busTemp2 = 4*sin(counter)+15;
    thermal.busTemp3 = 3*sin(counter)+30;
}

void TeleData::update(void){
    counter++;
    //printf("UPDATED\n\n%i", counter);
    setCDH();
    setProps();
    setADCS();
    setComms();
    setPayload();
    setPower();
    setStruct();
    setThermal();
    setPosData(arr[tel_count][0],arr[tel_count][1],arr[tel_count][2]);
    setVelData(arr[tel_count][3],arr[tel_count][4],arr[tel_count][5]);
    //printf("THIS IS THE TEMP: %f\n\n", props.tempTank);
}
void TeleData::setPosData(float posx, float posy, float posz){
    
    tel.spaceCraftPosx = posx;
    tel.spaceCraftPosy = posy;
    tel.spaceCraftPosz = posz;
}
void TeleData::setVelData(float velx, float vely, float velz){
    tel.spaceCraftVelx = velx;
    tel.spaceCraftVely = vely;
    tel.spaceCraftVelz = velz;
}
void TeleData::setAVData(float avelx, float avely, float avelz){
    tel.spaceCraftAVx = avelx;
    tel.spaceCraftAVy = avely;
    tel.spaceCraftAVz = avelz;
}
void TeleData::setQuat(float e1, float e2, float e3, float eta){
    tel.e1 = e1;
    tel.e2 = e2;
    tel.e3 = e3;
    tel.eta = eta;
}
void TeleData::setNew(void){
    tel.volt1 = 28;
    tel.volt2 = 28;
    tel.pressTank= 23;
    tel.tempTank = 143;
    tel.spaceCraftPosx = 542;
    tel.spaceCraftPosy = 2123;
    tel.spaceCraftPosz = 32;
    tel.spaceCraftVelx = 12;
    tel.spaceCraftVely = 165;
    tel.spaceCraftVelz = 21;
    tel.spaceCraftAVx = 0;
    tel.spaceCraftAVy = 0;
    tel.spaceCraftAVz = 0;
    tel.e1 = 1;
    tel.e2 = 2;
    tel.e3 = 3;
    tel.eta = 1;
    tel.deploymentStatus = 21;

    update();
}

void TeleData::setJeanStructOrbit(int32_t o1,int32_t o2,int32_t o3,int32_t o4,int32_t o5,int32_t o6){
    telstruc_orbit.x = o1;
    telstruc_orbit.y = o2;
    telstruc_orbit.z = o3;
    telstruc_orbit.vx = o4;
    telstruc_orbit.vy = o5;
    telstruc_orbit.vz = o6;
    // cout<<"CHANGED IT INTO EHRE"<<endl;
    userPos = 1;
    // cout<<userPos<<endl;
}

void TeleData::setJeanStructPoint(int32_t o1,int32_t o2,int32_t o3,int32_t o4,int32_t o5,int32_t o6,int32_t o7)
{
    telstruc_pointers.e1 = o1;
    telstruc_pointers.e2 = o2;
    telstruc_pointers.e3 = o3;
    telstruc_pointers.eta = o4;
    telstruc_pointers.ax = o5;
    telstruc_pointers.ay = o6;
    telstruc_pointers.az = o7;
    userPos = 1;
    // cout<<userPos<<endl;

}

void readData(){
    string fname = "state1.txt";
	// fname = "InterplanetaryV";
	vector<string> row;
	string line, word;
    

	fstream file (fname, ios::in);
	if(file.is_open())
	{
		while(getline(file, line)){
			row.clear();

			stringstream str(line);

			while(getline(str, word, ','))
				row.push_back(word);
			contentR.push_back(row);
		}
	}
    file.close();
    cout<<"CONTENT IS +BIG DADDY: "<<contentR[1][0]<<endl;
    cout<<"float"<<stof(contentR[1][0])<<endl;
    cout<<"LENGTH OF THIS DATA IS NO KIZZY"<<contentR.size()<<endl;
    vecSize = contentR.size();
}