#include "rfid.h"

uint8_t BLOCK_KEY[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

rfidStatus_t RFID_PrepareConnect(uint8_t *pSerialNum)
{

    rfidStatus_t status = MI_ERR;
    if (pSerialNum == NULL)
    {
        return status;
    }

    MFRC522_Init();
    HAL_Delay(20);

    uint8_t *pSakBuff;
    pSakBuff = (uint8_t *) malloc(sizeof(*pSakBuff)*16);

    status = MFRC522_Request(PICC_REQIDL, pSakBuff);
    if (status == MI_OK)
    {
        switch(pSakBuff[0])
        {
        case 0x44:
          DEBUG_PRINT(DEBUG_PRINT_TRACE, "Card type: MF1S500yX\r\n");
          break;
        case 0x04:
          DEBUG_PRINT(DEBUG_PRINT_TRACE, "Card type: MF1S503yX\r\n");
          break;
        case 0x42:
          DEBUG_PRINT(DEBUG_PRINT_TRACE, "Card type: MF1S700yX\r\n");
          break;
        case 0x02:
          DEBUG_PRINT(DEBUG_PRINT_TRACE, "Card type: MF1S703yX\r\n");
          break;
        case 0x08:
          DEBUG_PRINT(DEBUG_PRINT_TRACE, "Card type: MF1S50yyX/V1\r\n");
          break;
        default:
          DEBUG_PRINT(DEBUG_PRINT_TRACE, "Unknown card type\r\n");
        }
        DEBUG_PRINT(DEBUG_PRINT_TRACE, "SAK: 0x%02X 0x%02X\r\n", pSakBuff[1], pSakBuff[0]);
    }
    free(pSakBuff);

    // Anti-collision, return the card's 4-byte serial number
    status = MFRC522_Anticoll(pSerialNum);
    if (status == MI_OK)
    {
      DEBUG_PRINT
      (
          DEBUG_PRINT_TRACE,
          "SN: 0x%02X 0x%02X 0x%02X 0x%02X\r\n",
          pSerialNum[0],
          pSerialNum[1],
          pSerialNum[2],
          pSerialNum[3]
      );
    }

    // Election card, return capacity
    uint8_t sizeRC;
    sizeRC = MFRC522_SelectTag(pSerialNum);
    if (sizeRC != 0)
    {
      DEBUG_PRINT(DEBUG_PRINT_TRACE, "CS: %d\r\n", sizeRC);
    }

    return status;
}

void RFID_CloseConnect(void)
{
    MFRC522_Halt();
    MFRC522_AntennaOff();
}

rfidStatus_t RFID_ReadFullMem(void)
{
    rfidStatus_t status = MI_ERR;

    uint8_t *pSerialNum;
    pSerialNum = (uint8_t *) malloc(sizeof(*pSerialNum)*16);

    status = RFID_PrepareConnect(pSerialNum);
    if (status == MI_OK)
    {
        DEBUG_PRINT(DEBUG_PRINT_INFO, "[DEBUG] Card memory dump\r\n");
        // Card reader
        uint8_t *pCardBuff;
        pCardBuff = (uint8_t *) malloc(sizeof(*pCardBuff)*16);

        for (int sectorNum = 1;sectorNum<=16;sectorNum++)
        {
            int blockNum = sectorNum * 4 - 1;
            DEBUG_PRINT(DEBUG_PRINT_INFO, "Sector number 0x%02X\r\n", sectorNum - 1);
            for (int k = 4;k>0;k--)
            {
                status = MFRC522_Auth(PICC_AUTHENT1A, blockNum, BLOCK_KEY, pSerialNum);
                if (status == MI_OK)
                {
                    status = MFRC522_Read(blockNum, pCardBuff);
                    if (status == MI_OK)
                    {
                        DEBUG_PRINT(DEBUG_PRINT_INFO, "0x%02X   ", blockNum);
                        for (int j = 0;j<16;j++)
                        {
                            DEBUG_PRINT(DEBUG_PRINT_INFO, "%02X ", pCardBuff[j]);
                        }
                        DEBUG_PRINT(DEBUG_PRINT_INFO,"\r\n");
                    }
                }
                blockNum--;
            }
        }
        free(pCardBuff);
    }

    free(pSerialNum);

    RFID_CloseConnect();

    return status;
}

rfidStatus_t RFID_ReadSectorData(uint8_t sectorNum, uint8_t *pData)
{
    rfidStatus_t status = MI_ERR;

    if (sectorNum < 1 || sectorNum > 15)
    {
        DEBUG_PRINT(DEBUG_PRINT_ERROR, "[ERROR] Card sector must be between [1..15]\r\n");
        return status;
    }

    if (pData == NULL)
    {
        DEBUG_PRINT(DEBUG_PRINT_ERROR, "[ERROR] Empty data\r\n");
        return status;
    }

    memset(pData, 0, 48);

    uint8_t *pSerialNum;
    pSerialNum = (uint8_t *) malloc(sizeof(*pSerialNum)*16);

    status = RFID_PrepareConnect(pSerialNum);
    if (status == MI_OK)
    {
        uint8_t blockNum = (sectorNum + 1) * 4 - 2;

        DEBUG_PRINT(DEBUG_PRINT_INFO, "Sector number 0x%02X\r\n", sectorNum);
        for(uint8_t idx = 3; idx>0 ;idx--)
        {
            status = MFRC522_Auth(PICC_AUTHENT1A, blockNum, BLOCK_KEY, pSerialNum);
            if (status == MI_OK)
            {
                status = MFRC522_Read(blockNum, pData);
                if (status == MI_OK)
                {
                    DEBUG_PRINT(DEBUG_PRINT_INFO, "0x%02X   ", blockNum);

                    for (int idx = 0;idx<16;idx++)
                    {
                        DEBUG_PRINT(DEBUG_PRINT_INFO, "%02X ", pData[idx]);
                    }
                    DEBUG_PRINT(DEBUG_PRINT_INFO,"\r\n");
                }
            }
            blockNum--;
            pData += 16;
        }
    }
    free(pSerialNum);

    RFID_CloseConnect();

    return status;
}

rfidStatus_t RFID_WriteSectorData(uint8_t sectorNum, uint8_t *pData)
{
    rfidStatus_t status = MI_ERR;
    if (pData == NULL)
    {
        DEBUG_PRINT(DEBUG_PRINT_ERROR, "[ERROR] Empty data\r\n");
        return status;
    }

    if (sectorNum < 1 || sectorNum > 15)
    {
        DEBUG_PRINT(DEBUG_PRINT_ERROR, "[ERROR] Card sector must be between [1..15]\r\n");
        return status;
    }

    uint8_t *pSerialNum;
    pSerialNum = (uint8_t *) malloc(sizeof(*pSerialNum)*16);

    status = RFID_PrepareConnect(pSerialNum);
    if (status == MI_OK)
    {
        uint8_t blockNum = (sectorNum + 1) * 4 - 2;
        for (uint8_t idx = 3; idx>0 ;idx--)
        {
            status = MFRC522_Auth(PICC_AUTHENT1A, blockNum, BLOCK_KEY, pSerialNum);
            if (status == MI_OK)
            {
                status = MFRC522_Write(blockNum, pData);
                if (status != MI_OK)
                {
                    DEBUG_PRINT(DEBUG_PRINT_ERROR, "[ERROR] Error with writing sector %02X\r\n", sectorNum);
                    return status;
                }
            }
            blockNum--;
            pData += 16;
        }
        DEBUG_PRINT(DEBUG_PRINT_INFO, "[DEBUG] Sector %02X sucessfully written\r\n", sectorNum);
    }
    free(pSerialNum);

    RFID_CloseConnect();


    return status;
}

rfidStatus_t RFID_ResetAllSectorsData(void)
{
    rfidStatus_t status = MI_ERR;
    uint8_t resetData[16] = {0};

    uint8_t *pSerialNum;
    pSerialNum = (uint8_t *) malloc(sizeof(*pSerialNum)*16);

    status = RFID_PrepareConnect(pSerialNum);
    if (status == MI_OK)
    {
        for (int sectorNum = 2;sectorNum<=16;sectorNum++)
        {
            int blockNum = sectorNum * 4 - 2;
            for (int k = 3;k>0;k--)
            {
                status = MFRC522_Auth(PICC_AUTHENT1A, blockNum, BLOCK_KEY, pSerialNum);
                if (status == MI_OK)
                {
                    status = MFRC522_Write(blockNum, &resetData);
                    if (status != MI_OK)
                    {
                        DEBUG_PRINT(DEBUG_PRINT_ERROR, "[ERROR] Error with resetting all sectors data at %02X sector\r\n", sectorNum - 1);
                        return status;
                    }
                }
                blockNum--;
            }
        }
        DEBUG_PRINT(DEBUG_PRINT_INFO, "All data sectors reseted\r\n");
    }
    free(pSerialNum);

    RFID_CloseConnect();


    return status;
}

rfidStatus_t RFID_WriteBasicData(void)
{
    rfidStatus_t status = MI_ERR;
    Proto TxProto = {0, 0, {0}};
    Item_t TxComponent = Item_t_init_default;
    Resistor_t TxResistor = Resistor_t_init_default;

    TxComponent.type = RESISTOR;
    TxResistor.partNumber = 2;
    TxResistor.amount = 3;
    TxResistor.maxVoltage = 4;
    TxResistor.package = 5;
    TxResistor.power = 6;
    TxResistor.resistance = 7;
    TxResistor.tolerance = 8;
    pb_ostream_t TxStream = pb_ostream_from_buffer(TxProto.Data, sizeof(TxProto.Data));
    TxProto.status = pb_encode(&TxStream, Item_t_fields, &TxComponent);
    TxProto.status = pb_encode(&TxStream, Resistor_t_fields, &TxResistor);
    TxProto.messageLength = TxStream.bytes_written;

    status = RFID_WriteSectorData(1, TxProto.Data);
    return status;
}


