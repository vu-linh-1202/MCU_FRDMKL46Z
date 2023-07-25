#include "MKL46Z4.h"
#include "CLOCK.h"
#include "PIT.h" 
#include "TPM.h"
#include "UART.h"
#include "Flash.h"
#include "Peripheral.h"
#include "Srecord.h"
#include "RingBuffer.h"
#include "String_Handle.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/****************************** START DEFINITION **************************************/
#define VECTOR_TABLE_SIZE         48
#define VECTOR_TABLE_ALIGNMENT   (0x80U)
#define BACKUP_FLASH_ADDRESS     (0xb000)
#define FLASH_ADDRESS_WRITE      (0xa000)
#define FLASH_ADDRESS_ERASE      (0xa400)

/****************************** END DEFINITION **************************************/

/****************************** START DECLARE PROTOTYPE *****************************/
void Move_Vector_Table();
void delay(unsigned long delayTime);
void HardFault_Handler();
void Process_Data_From_Queue();
/****************************** END DECLARE PROTOTYPE *****************************/

/****************************** START DECLARE VARIABLE**************************/
uint32_t Vector_Table_To_RAM[VECTOR_TABLE_SIZE] __attribute__(( aligned (VECTOR_TABLE_ALIGNMENT) ));
volatile BOOL driver_update_flag = FALSE;
static uint8_t pop_line=0;
uint32_t add_restart;
uint32_t msp_restart;


/****************************** END DECLARE VARIABLE**************************/


/************************************* CODE ***********************************/
void SetMSP(uint32_t value)
{
   __asm("MSR MSP, R0");
}
void SetPC(uint32_t value)
{
    __asm("BLX R0");
}

void main()
{
    
    Move_Vector_Table();
    initLed();
    init_clock();
    uart_init();
    Uart_String_Transmission("Hi Tera Term. ^-^ ^-^\r\n");
    NVIC_EnableIRQ(UART0_IRQn);
    Flash_Erase(FLASH_ADDRESS_WRITE);
    Flash_Erase(FLASH_ADDRESS_ERASE);

    
    while(1)
    {
        if(get_queue_head() != get_queue_tail())
        {
            Process_Data_From_Queue();
            pop_line++;
            if(queue_overflow_flag == TRUE)
            {
                Handle_Queue_Overflow();
            }
            if(driver_update_flag == TRUE)
            {
                Update_Driver_Successful();
                SetPC(add_restart + 4);
                SetMSP(msp_restart);
            }
        }
    }
}

void Process_Data_From_Queue()
{
    uint32_t u8_i;
    uint8_t* th_byte_value = 0;
    uint8_t* srec_line = get_srec_queue_address();
    uint32_t flash_address;
    uint8_t data_flash[4] = {0};
    
    flash_address = 0;
    for (u8_i = 4; u8_i < 8; u8_i += 2)
    {
        flash_address = (flash_address << 8) | Convert2ChartoHex(*(srec_line + u8_i + 0), *(srec_line + u8_i + 1));
    }
    if(flash_address >= FLASH_ADDRESS_WRITE)
    {
        for(u8_i = 0; u8_i < 4; u8_i++)
        {
            data_flash[u8_i]=0;
        }
        if ((*srec_line == 'S') && (*(srec_line + 1) == '9'))
        {
            add_restart = flash_address;
            driver_update_flag = TRUE;
        }
        else
        {
            for(u8_i = 0; u8_i < 4; u8_i++)
            {
                th_byte_value = srec_line + u8_i*8;
                data_flash[0] = Convert2ChartoHex(*(th_byte_value + 8),  *(th_byte_value + 9));
                data_flash[1] = Convert2ChartoHex(*(th_byte_value + 10), *(th_byte_value + 11));
                data_flash[2] = Convert2ChartoHex(*(th_byte_value + 12), *(th_byte_value + 13));
                data_flash[3] = Convert2ChartoHex(*(th_byte_value + 14), *(th_byte_value + 15));
                Flash_Write(u8_i*4 + flash_address, *((uint32_t*)data_flash));
            }
        }
        if (flash_address == FLASH_ADDRESS_WRITE)
        {
            msp_restart = (data_flash[0] | (data_flash[1] << 8) | (data_flash[2] << 16) | (data_flash[3] << 24));
        }
    }
    if(queue_overflow_flag == TRUE)
    {
        Handle_Queue_Overflow();
    }
    get_index_queue_tail();
}




__ramfunc void UART0_IRQHandler()
{
   Push_Circular_Queue(UART0->D);
}

void Move_Vector_Table()
{
    uint32_t* pSrc = (uint32_t*)0;
    uint32_t* pDest = Vector_Table_To_RAM;
    memcpy(pDest, pSrc, VECTOR_TABLE_SIZE * sizeof(uint32_t));

    __disable_irq();
    SCB->VTOR = (uint32_t)Vector_Table_To_RAM;
    __enable_irq();
}


void HardFault_Handler()
{
    while(1);
}

void delay(unsigned long delayTime)
{
    static unsigned long i;
    i=0;
    while ( i < delayTime)
    {
        i++;
    }
}