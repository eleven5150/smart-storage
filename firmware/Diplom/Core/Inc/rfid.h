#ifndef INC_RFID_H_
#define INC_RFID_H_

#include "rc522.h"
#include "stdio.h"
#include "retarget.h"
#include "string.h"
#include "stdlib.h"

typedef uint8_t rfidStatus_t;

rfidStatus_t RFID_ReadFullMem(void);
rfidStatus_t RFID_ReadSectorData(uint8_t sectorNum);
void RFID_CloseConnect(void);
rfidStatus_t RFID_PrepareConnect(uint8_t *pSerialNum);
rfidStatus_t RFID_WriteSectorData(uint8_t sectorNum, uint8_t *pData);
rfidStatus_t RFID_ResetAllSectorsData(void);

struct testData_t
{
    uint8_t byte0;
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;
    uint8_t byte4;
    uint8_t byte5;
    uint8_t byte6;
    uint8_t byte7;
    uint8_t byte8;
    uint8_t byte9;
    uint8_t byteA;
    uint8_t byteB;
    uint8_t byteC;
    uint8_t byteD;
    uint8_t byteE;
    uint8_t byteF;
};

#endif /* INC_RFID_H_ */
