#ifndef _FLASH_H_
#define _FLASH_H_
#include "MKL46Z4.h"

#define FTFA_FSTAT_CLEAR_ERROR          0x30U
#define START_COMMAND                   0x80U
#define CMD_PROGRAM_LONGWORD            0x06U
#define CMD_ERASE_SECTOR                0x09U


void Flash_Write(uint32_t add, uint32_t data);
void Flash_Erase(uint32_t Address);
void Flash_Read(uint32_t address, uint8_t* data);

#endif /* _FLASH_H_ */