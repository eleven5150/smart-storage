#ifndef INC_RFID_H_
#define INC_RFID_H_

#include "rc522.h"
#include "stdio.h"
#include "retarget.h"
#include "string.h"
#include "stdlib.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "pb_common.h"
#include "LedStripData.pb.h"

#define MAX_DATA_SIZE 48

typedef uint8_t rfidStatus_t;

rfidStatus_t RFID_ReadFullMem(void);
rfidStatus_t RFID_ReadSectorData(uint8_t sectorNum, uint8_t *pData);
void RFID_CloseConnect(void);
rfidStatus_t RFID_PrepareConnect(uint8_t *pSerialNum);
rfidStatus_t RFID_WriteSectorData(uint8_t sectorNum, uint8_t *pData);
rfidStatus_t RFID_ResetAllSectorsData(void);
rfidStatus_t RFID_WriteBasicData(void);

typedef enum
{
    RESISTOR = 1,
    CAPACITOR,
    BJT
} eComponenets;

typedef struct
{
    size_t messageLength;
    uint8_t status;
    uint8_t Data[MAX_DATA_SIZE];
} Proto;

#endif /* INC_RFID_H_ */
