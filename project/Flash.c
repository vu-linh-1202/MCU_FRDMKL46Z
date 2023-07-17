#include "FLASH.h"


void Flash_Write(uint32_t add, uint8_t u8i_data1,uint8_t u8i_data2,uint8_t u8i_data3,uint8_t u8i_data4)
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

        FTFA->FCCOB0 =0X06;
        
        FTFA->FCCOB7 =u8i_data1;
        FTFA->FCCOB6 =u8i_data2;
        FTFA->FCCOB5 =u8i_data3;
        FTFA->FCCOB4 =u8i_data4;
        
        __disable_irq();
        /* Clear CCIF Flag to launch command */
        FTFA->FSTAT = START_COMMAND;
        while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
        __enable_irq();
    
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
    __disable_irq();
    FTFA->FSTAT = START_COMMAND;
    while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
    __enable_irq();
}
