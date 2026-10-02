#ifndef INC_MODBUSRTU_MASTER_H_
#define INC_MODBUSRTU_MASTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "main.h"

#define SLAVEID    1
#define NUMBER_OF_REGISTER 10
#define NUMBER_OF_COIL 16
#define BUFFERSIZE (NUMBER_OF_REGISTER * 2 + 5)

extern uint16_t ModbusRegister[NUMBER_OF_REGISTER];
extern bool ModbusCoil[NUMBER_OF_COIL];

enum
{
	ReadCoil = 0x01,
	ReadHoldingRegister = 0x03,
	WriteSingleCoil = 0x05,
	WriteSingleRegister = 0x06,
	WriteMultipleCoils = 0x0F,
	WriteMultipleRegisters = 0x10,
};

typedef enum
{
	uartOkState,
	uartErrState,
}uartState_t;

typedef enum
{
	crcOk,
	crcErr,
}crcState_t;

typedef enum
{
	funcOk,
	funcErr,
}funcState_t;

typedef enum
{
	coilOff = 0x00,
	coilOn = 0xFF,
}coilState_t;

/*Data transmit and receive func*/
funcState_t sendMessage(char *msg, uint16_t len);
funcState_t takeMessage(char *msg, uint16_t len, uint8_t SlaveID);

/****************************************** MODBUS Command Function ******************************************************/
funcState_t Modbus_ReadHoldingRegister(uint8_t SlaveID, uint16_t StartAddress, uint16_t NumberOfReg);
funcState_t Modbus_WriteSingleRegister(uint8_t SlaveID, uint16_t RegAddress, uint16_t RegValue);

funcState_t Modbus_ReadCoil(uint8_t SlaveID, uint16_t StartAddress, int16_t NumberOfCoil);
funcState_t Modbus_WriteSingleCoil(uint8_t SlaveID, uint16_t RegAddress, coilState_t CoilState);

funcState_t Modbus_WriteMultipleCoils(uint8_t SlaveID, uint16_t StartAddress, uint16_t NumberOfCoils, char *coilValue);
funcState_t Modbus_WriteMultipleRegisters(uint8_t SlaveID, uint16_t StartAddress, uint16_t NumberOfReg, uint16_t *regValue);
/**************************************************************************************************************************/
uint8_t findByte(int16_t NumberOfCoil);
uint16_t MODBUS_CRC16(char *buf, uint8_t len );

#ifdef __cplusplus
}
#endif

#endif /* INC_MODBUSRTU_MASTER_H_ */
