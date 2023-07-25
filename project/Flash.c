#include "FLASH.h"


void Flash_Write(uint32_t add, uint32_t data)
{
    
        /* Wait Previous Command Complete? */
        while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
        
        /* Check Error from Previous Command */
        if ( ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) == FTFA_FSTAT_ACCERR_MASK) \
            || ((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) == FTFA_FSTAT_FPVIOL_MASK) )
        {
            FTFA->FSTAT = FTFA_FSTAT_CLEAR_ERROR;
        }
        
        /* Write Command and Parameter to FCCOB Registers */
        FTFA->FCCOB1 =add>>16;
        FTFA->FCCOB2 =(add&0x00ff00)>>8;
        FTFA->FCCOB3 =add&0x0000ff;

        FTFA->FCCOB0 = CMD_PROGRAM_LONGWORD;
        
        FTFA->FCCOB7 =data & 0xFF;
        FTFA->FCCOB6 =(data>>8) & 0xFF;
        FTFA->FCCOB5 =(data>>16) & 0xFF;
        FTFA->FCCOB4 =(data>>24) & 0xFF;
        
        /* Clear CCIF Flag to launch command */
        FTFA->FSTAT = START_COMMAND;
        while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
}


void Flash_Erase(uint32_t Address)
{
    
    /* Wait Previous Command Complete? */
    while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
    
    /* Check Error from Previous Command */
    if ( ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) == FTFA_FSTAT_ACCERR_MASK) \
        || ((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) == FTFA_FSTAT_FPVIOL_MASK) )
    {
        FTFA->FSTAT = FTFA_FSTAT_CLEAR_ERROR;
    }
    
    /* Write Command and Parameter to FCCOB Registers */
    FTFA->FCCOB0 = CMD_ERASE_SECTOR;
    
    FTFA->FCCOB1 = (uint8_t)(Address >> 16);
    FTFA->FCCOB2 = (uint8_t)(Address >> 8);
    FTFA->FCCOB3 = (uint8_t)Address;
    
    /* Clear CCIF Flag to launch command */
    FTFA->FSTAT = START_COMMAND;
    while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
}

/*
void Flash_Read(uint32_t address, uint8_t* data)
{
    uint32_t* flash_address = (uint32_t*)address;
    uint32_t data_flash = *flash_address;
    data[0] = (data_flash & 0XFF);
    data[1] = ((data_flash >> 8) & 0xFF);
    data[2] = ((data_flash >> 16) & 0xFF);
    data[3] = ((data_flash >> 24) & 0xFF);
}

void Flash_Copy(uint32_t dest_addr, uint32_t src_addr, uint32_t size)
{
    uint32_t i;

    // Erase the destination area
    Flash_Erase(dest_addr);

    // Copy the data from source to destination
    for (i = 0; i < size; i += 4)
    {
        uint32_t data = Flash_Read(src_addr + i);
        Flash_Write(dest_addr + i, data);
    }
}
*/