#ifndef PCKFMT
#define PCKFMT




class packetFormatter {
    public:
        packetFormatter();
        void makeComPacket(uint8_t *buff, int socket, Instruction *i);
        int sendTelPacket(uint8_t *buff, int socket, uint8_t version, uint8_t pktType, uint8_t dataHeader,
                    uint8_t processID, int8_t category, uint8_t grpNum, uint16_t seqCount,
                    uint8_t serv, uint8_t subserv, int64_t time, uint8_t *telData, uint16_t dataLen);
        
};

#endif