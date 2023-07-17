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


#define VECTOR_TABLE_SIZE         48
#define VECTOR_TABLE_ALIGNMENT   (0x100U)

void Move_Vector_Table();
void delay(unsigned long delayTime);
void HardFault_Handler();

uint32_t Vector_Table_To_RAM[VECTOR_TABLE_SIZE] __attribute__(( aligned (VECTOR_TABLE_ALIGNMENT) ));


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
    Uart_String_Transmission("Hi Tera Term\r\n");
    NVIC_EnableIRQ(UART0_IRQn);
    Flash_Erase(0xa000);
    Flash_Erase(0xa400);
    
    while(1)
    {
        if(queue_overflow_flag == TRUE)
        {
            handle_queue_overflow();
        }
        if(srec_lines_recorded != srec_lines_pushed)
        {
            process_srec_line();
            srec_lines_recorded++;
        }
        else
        {
            if(driver_update_flag ==  TRUE)
            {
                driver_update_flag =  FALSE;
                SetPC(add_restart);
                SetMSP(msp_restart);
                Uart_String_Transmission("Update Driver");
            }
        }
    }
}


__ramfunc void UART0_IRQHandler()
{
   push_srec_line(UART0->D);
}


void Move_Vector_Table()
{
    uint8_t  ui8_i;
    uint32_t* p; 
    p=0;
    for (ui8_i = 0; ui8_i < VECTOR_TABLE_SIZE; ui8_i++)
    {
      Vector_Table_To_RAM[ui8_i]=*p;
      p++;
    }
    __asm("CPSID   I");
    SCB->VTOR= (uint32_t)Vector_Table_To_RAM;
    __asm("CPSIE   I");
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
