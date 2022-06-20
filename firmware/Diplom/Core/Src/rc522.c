// Mifare RC522 RFID Card reader 13.56 MHz

#include "stm32f0xx_hal.h"
#include "rc522.h"
#include "stdlib.h"


extern SPI_HandleTypeDef hspi1;

// RC522
extern uint8_t MFRC522_Check(uint8_t* id);
extern uint8_t MFRC522_Compare(uint8_t* CardID, uint8_t* CompareID);
extern void MFRC522_WriteRegister(uint8_t addr, uint8_t val);
extern uint8_t MFRC522_ReadRegister(uint8_t addr);
extern void MFRC522_SetBitMask(uint8_t reg, uint8_t mask);
extern void MFRC522_ClearBitMask(uint8_t reg, uint8_t mask);
extern uint8_t MFRC522_Request(uint8_t reqMode, uint8_t* TagType);
extern uint8_t MFRC522_ToCard(uint8_t command, uint8_t* sendData, uint8_t sendLen, uint8_t* backData, uint16_t* backLen);
extern uint8_t MFRC522_Anticoll(uint8_t* serNum);
extern void MFRC522_CalulateCRC(uint8_t* pIndata, uint8_t len, uint8_t* pOutData);
extern uint8_t MFRC522_SelectTag(uint8_t* serNum);
extern uint8_t MFRC522_Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t* Sectorkey, uint8_t* serNum);
extern uint8_t MFRC522_Read(uint8_t blockAddr, uint8_t* recvData);
extern uint8_t MFRC522_Write(uint8_t blockAddr, uint8_t* writeData);
extern void MFRC522_Init(void);
extern void MFRC522_Reset(void);
extern void MFRC522_AntennaOn(void);
extern void MFRC522_AntennaOff(void);
extern void MFRC522_Halt(void);

uint8_t SPI1SendByte(uint8_t data) {
	unsigned char writeCommand[1];
	unsigned char readValue[1];
	writeCommand[0] = data;
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&writeCommand, (uint8_t*)&readValue, 1, 10);
	return readValue[0];
}

void SPI1_WriteReg(uint8_t address, uint8_t value) {
	cs_reset();
	SPI1SendByte(address);
	SPI1SendByte(value);
	cs_set();
}

uint8_t SPI1_ReadReg(uint8_t address) {
	uint8_t	val;

	cs_reset();
	SPI1SendByte(address);
	val = SPI1SendByte(0x00);
	cs_set();
	return val;
}

void MFRC522_WriteRegister(uint8_t addr, uint8_t val) {
	addr = (addr << 1) & 0x7E;															// Address format: 0XXXXXX0
  SPI1_WriteReg(addr, val);
}

uint8_t MFRC522_ReadRegister(uint8_t addr) {
	uint8_t val;

	addr = ((addr << 1) & 0x7E) | 0x80;
	val = SPI1_ReadReg(addr);
	return val;	
}

uint8_t MFRC522_Check(uint8_t* id) {
	uint8_t status;
	status = MFRC522_Request(PICC_REQIDL, id);							// Find cards, return card type
	if (status == MI_OK) status = MFRC522_Anticoll(id);			// Card detected. Anti-collision, return card serial number 4 bytes
	MFRC522_Halt();																					// Command card into hibernation 
	return status;
}

uint8_t MFRC522_Compare(uint8_t* CardID, uint8_t* CompareID) {
	uint8_t i;
	for (i = 0; i < 5; i++) {
		if (CardID[i] != CompareID[i]) return MI_ERR;
	}
	return MI_OK;
}

void MFRC522_SetBitMask(uint8_t reg, uint8_t mask) {
	MFRC522_WriteRegister(reg, MFRC522_ReadRegister(reg) | mask);
}

void MFRC522_ClearBitMask(uint8_t reg, uint8_t mask){
	MFRC522_WriteRegister(reg, MFRC522_ReadRegister(reg) & (~mask));
}

uint8_t MFRC522_Request(uint8_t reqMode, uint8_t* respData) {
	uint8_t status;  
	uint16_t backBits;																			// The received data bits
	uint8_t* pSendData;
	pSendData = (uint8_t *) malloc(sizeof(uint8_t)*16);

	MFRC522_WriteRegister(MFRC522_REG_BIT_FRAMING, 0x07);		// TxLastBists = BitFramingReg[2..0]
	pSendData[0] = reqMode;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, pSendData, 1, respData, &backBits);
	if ((status != MI_OK) || (backBits != 0x10)) status = MI_ERR;
	free(pSendData);
	return status;
}

uint8_t MFRC522_ToCard(uint8_t command, uint8_t* sendData, uint8_t sendLen, uint8_t* backData, uint16_t* backLen) {
	uint8_t status = MI_ERR;
	uint8_t irqEn = 0x00;
	uint8_t waitIRq = 0x00;
	uint8_t lastBits;
	uint8_t FIFOLevel;
    uint8_t IRQAnsw;
	uint16_t i;

	switch (command) {
		case PCD_AUTHENT: {
			irqEn = 0x12;
			waitIRq = 0x10;
			break;
		}
		case PCD_TRANSCEIVE: {
			irqEn = 0x77;
			waitIRq = 0x30;
			break;
		}
		default:
		break;
	}

	MFRC522_WriteRegister(MFRC522_REG_COMM_IE_N, irqEn | 0x80);
	MFRC522_ClearBitMask(MFRC522_REG_COMM_IRQ, 0x80);
	MFRC522_SetBitMask(MFRC522_REG_FIFO_LEVEL, 0x80);
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, PCD_IDLE);

	// Writing data to the FIFO
	for (i = 0; i < sendLen; i++)
	    MFRC522_WriteRegister(MFRC522_REG_FIFO_DATA, sendData[i]);
	//DEBUG_PRINT(DEBUG_PRINT_TRACE, "[DEBUG] cmdReq -> 0x%X\r\n", MFRC522_ReadRegister(MFRC522_REG_FIFO_DATA));

	// Execute the command
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, command);

	if (command == PCD_TRANSCEIVE)
	    MFRC522_SetBitMask(MFRC522_REG_BIT_FRAMING, 0x80);		// StartSend=1,transmission of data starts

	// Waiting to receive data to complete
    i = 2000;   // i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms
    do {
        // CommIrqReg[7..0]
        // Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
        IRQAnsw = MFRC522_ReadRegister(MFRC522_REG_COMM_IRQ);
        i--;
    } while ((i!=0) && !(IRQAnsw&0x01) && !(IRQAnsw&waitIRq));



	MFRC522_ClearBitMask(MFRC522_REG_BIT_FRAMING, 0x80);																// StartSend=0

	if (i != 0)  {
		if (!(MFRC522_ReadRegister(MFRC522_REG_ERROR) & 0x1B)) {
			status = MI_OK;
			if (IRQAnsw & irqEn & 0x01)
			    status = MI_NOTAGERR;
			if (command == PCD_TRANSCEIVE) {
				FIFOLevel = MFRC522_ReadRegister(MFRC522_REG_FIFO_LEVEL);
				lastBits = MFRC522_ReadRegister(MFRC522_REG_CONTROL) & 0x07;
				if (lastBits)
				    *backLen = (FIFOLevel-1)*8+lastBits;
				else
				    *backLen = FIFOLevel*8;
				if (FIFOLevel == 0)
				    FIFOLevel = 1;
//			    DEBUG_PRINT(DEBUG_PRINT_TRACE, "[DEBUG] queueSize -> %d\r\n", FIFOLevel);
				if (FIFOLevel > MFRC522_MAX_LEN)
				    FIFOLevel = MFRC522_MAX_LEN;
				for (i = 0; i < FIFOLevel; i++)
				{
				    backData[i] = MFRC522_ReadRegister(MFRC522_REG_FIFO_DATA);		// Reading the received data in FIFO
				}
//			    DEBUG_PRINT(DEBUG_PRINT_TRACE, "[DEBUG] IRQ reg: 0x%X\r\n", IRQAnsw);
			}
		} else status = MI_ERR;
	}

	return status;
}

uint8_t MFRC522_Anticoll(uint8_t* serNum) {
	uint8_t status;
	uint8_t i;
	uint8_t serNumCheck = 0;
	uint16_t unLen;
    uint8_t* pSendData;

    pSendData = (uint8_t *) malloc(sizeof(pSendData)*16);
	MFRC522_WriteRegister(MFRC522_REG_BIT_FRAMING, 0x00);												// TxLastBists = BitFramingReg[2..0]
	pSendData[0] = PICC_ANTICOLL;
	pSendData[1] = 0x20;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, pSendData, 2, serNum, &unLen);
	if (status == MI_OK) {
		// Check card serial number
		for (i = 0; i < 4; i++) serNumCheck ^= serNum[i];
		if (serNumCheck != serNum[i]) status = MI_ERR;
	}
	free(pSendData);
	return status;
} 

void MFRC522_CalculateCRC(uint8_t*  pIndata, uint8_t len, uint8_t* pOutData) {
	uint8_t i, n;

	MFRC522_ClearBitMask(MFRC522_REG_DIV_IRQ, 0x04);													// CRCIrq = 0
	MFRC522_SetBitMask(MFRC522_REG_FIFO_LEVEL, 0x80);													// Clear the FIFO pointer
	// Write_MFRC522(CommandReg, PCD_IDLE);

	// Writing data to the FIFO	
	for (i = 0; i < len; i++) MFRC522_WriteRegister(MFRC522_REG_FIFO_DATA, *(pIndata+i));
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, PCD_CALCCRC);

	// Wait CRC calculation is complete
	i = 0xFF;
	do {
		n = MFRC522_ReadRegister(MFRC522_REG_DIV_IRQ);
		i--;
	} while ((i!=0) && !(n&0x04));																						// CRCIrq = 1

	// Read CRC calculation result
	pOutData[0] = MFRC522_ReadRegister(MFRC522_REG_CRC_RESULT_L);
	pOutData[1] = MFRC522_ReadRegister(MFRC522_REG_CRC_RESULT_M);
}

uint8_t MFRC522_SelectTag(uint8_t* serNum) {
	uint8_t i;
	uint8_t status;
	uint8_t size;
	uint16_t recvBits;
	uint8_t *pBackData;
    uint8_t* pSendData;

    pSendData = (uint8_t *) malloc(sizeof(pSendData)*16);
    pBackData = (uint8_t *) malloc(sizeof(pBackData)*16);
    pSendData[0] = PICC_SElECTTAG;
    pSendData[1] = 0x70;
	for (i = 0; i < 5; i++) pSendData[i+2] = *(serNum+i);
	MFRC522_CalculateCRC(pSendData, 7, &pSendData[7]);		//??
	status = MFRC522_ToCard(PCD_TRANSCEIVE, pSendData, 9, pBackData, &recvBits);
	if ((status == MI_OK) && (recvBits == 0x18)) size = pBackData[0]; else size = 0;
	free(pSendData);
	free(pBackData);
	return size;
}

uint8_t MFRC522_Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t* Sectorkey, uint8_t* serNum) {
	uint8_t status;
	uint16_t recvBits;
	uint8_t i;
    uint8_t *pBackData;
    uint8_t* pSendData;

    pSendData = (uint8_t *) malloc(sizeof(pSendData)*16);
    pBackData = (uint8_t *) malloc(sizeof(pBackData)*16);
	// Verify the command block address + sector + password + card serial number
    pSendData[0] = authMode;
    pSendData[1] = BlockAddr;
	for (i = 0; i < 6; i++)
	    pSendData[i+2] = *(Sectorkey+i);
	for (i=0; i<4; i++)
	    pSendData[i+8] = *(serNum+i);

	status = MFRC522_ToCard(PCD_AUTHENT, pSendData, 12, pBackData, &recvBits);
	if ((status != MI_OK) || (!(MFRC522_ReadRegister(MFRC522_REG_STATUS2) & 0x08)))
	    status = MI_ERR;
    free(pSendData);
    free(pBackData);
	return status;
}

uint8_t MFRC522_Read(uint8_t blockAddr, uint8_t* recvData) {
	uint8_t status;
	uint16_t unLen;

	recvData[0] = PICC_READ;
	recvData[1] = blockAddr;
	MFRC522_CalculateCRC(recvData,2, &recvData[2]);
	status = MFRC522_ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);
	if ((status != MI_OK) || (unLen != 0x90)) status = MI_ERR;
	return status;
}

uint8_t MFRC522_Write(uint8_t blockAddr, uint8_t* writeData) {
	uint8_t status;
	uint16_t recvBits;
	uint8_t i;
	uint8_t buff[18]; 

	buff[0] = PICC_WRITE;
	buff[1] = blockAddr;
	MFRC522_CalculateCRC(buff, 2, &buff[2]);
	status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);
	if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)) status = MI_ERR;
	if (status == MI_OK) {
		// Data to the FIFO write 16Byte
		for (i = 0; i < 16; i++) buff[i] = *(writeData+i);
		MFRC522_CalculateCRC(buff, 16, &buff[16]);
		status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);
		if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)) status = MI_ERR;
	}
	return status;
}

void MFRC522_Init(void) {
	MFRC522_Reset();
	MFRC522_WriteRegister(MFRC522_REG_T_MODE, 0x8D);
	MFRC522_WriteRegister(MFRC522_REG_T_PRESCALER, 0x3E);
	MFRC522_WriteRegister(MFRC522_REG_T_RELOAD_L, 30);           
	MFRC522_WriteRegister(MFRC522_REG_T_RELOAD_H, 0);
	MFRC522_WriteRegister(MFRC522_REG_RF_CFG, 0x70);				// 48dB gain	
	MFRC522_WriteRegister(MFRC522_REG_TX_AUTO, 0x40);
	MFRC522_WriteRegister(MFRC522_REG_MODE, 0x3D);
	MFRC522_AntennaOn();																		// Open the antenna
}

void MFRC522_Reset(void) {
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, PCD_RESETPHASE);
}

void MFRC522_AntennaOn(void) {
	uint8_t temp;

	temp = MFRC522_ReadRegister(MFRC522_REG_TX_CONTROL);
	if (!(temp & 0x03)) MFRC522_SetBitMask(MFRC522_REG_TX_CONTROL, 0x03);
}

void MFRC522_AntennaOff(void) {
	MFRC522_ClearBitMask(MFRC522_REG_TX_CONTROL, 0x03);
}

void MFRC522_Halt(void) {
	uint16_t unLen;
	uint8_t buff[16];

	buff[0] = PICC_HALT;
	buff[1] = 0;
	MFRC522_CalculateCRC(buff, 2, &buff[2]);
	MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &unLen);
}
