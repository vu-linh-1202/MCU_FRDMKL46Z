#ifndef _FLASH_H_
#define _FLASH_H_
#include "MKL46Z4.h"

#define FTFA_FSTAT_CLEAR_ERROR          0x30U
#define START_COMMAND                   0x80U
#define CMD_PROGRAM_LONGWORD            0x06U
#define CMD_ERASE_SECTOR                0x09U

void Flash_Write(uint32_t Address, uint8_t u8i_data1,uint8_t u8i_data2,uint8_t u8i_data3,uint8_t u8i_data4);
void Flash_Erase(uint32_t Address);

#endif /* _FLASH_H_ */