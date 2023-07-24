#include "RingBuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/****************************** START DECLARE VARIABLE**************************/
volatile BOOL queue_overflow_flag = FALSE;
volatile BOOL driver_update_flag = FALSE;
uint8_t srec_queue[QUEUE_SIZE][MAX_LINE_SREC];
uint8_t length_queue[QUEUE_SIZE];
uint32_t add_restart;
uint32_t msp_restart;
uint8_t queue_head = 0;
uint8_t queue_tail = 0;
uint8_t srec_length = 0;
static uint8_t push_line=0;

/****************************** END DECLARE VARIABLE**************************/


/****************************** CODE FOR CIRCULAR QUEUE**************************/
__ramfunc uint8_t next_index(uint8_t ui8_index)
{
  if (ui8_index == QUEUE_SIZE - 1)
  {
    ui8_index = 0;
  }
  else
  {
    ui8_index++;
  }
  return ui8_index;
}

__ramfunc void Push_Circular_Queue(uint8_t data)
{
  static uint8_t count = 0;
   
  if (data != '\n')
  {
    srec_queue[queue_head][count] = data;
    count++;
  }
  else
  {
    if (next_index(queue_head) != queue_tail)
    {         
      length_queue[queue_head]= count - 1;
      count = 0;
      push_line++;
      queue_head = next_index(queue_head);
    }
    else
    {
      queue_overflow_flag = TRUE;
    }   
  }
}

void Handle_Queue_Overflow()
{
  Uart_String_Transmission("Error: Queue overflow detected. Failed to update driver.\r\n");
  queue_overflow_flag = FALSE;
}


void Pop_Circular_Queue()
{
  static uint32_t flash_address;
  static uint8_t data_flash[4];
  uint32_t u8_i;
  uint8_t* th_byte_value = 0;
  uint8_t* srec_line = srec_queue[queue_tail];
  
  flash_address = 0;
  for ( u8_i = 4; u8_i < 8; u8_i += 2)
  {
    flash_address = (flash_address << 8) | Convert2ChartoHex(*(srec_line + u8_i + 0), *(srec_line + u8_i + 1));
  }

  if (flash_address >= 0xa000)
  {
    for( u8_i = 0; u8_i < 4; u8_i++)
    {
       data_flash[u8_i] = 0; 
    }

    if ((*(srec_line) == 'S') && (*(srec_line + 1) == '9'))
    {
      add_restart = flash_address;
      driver_update_flag = TRUE;      
    }
    else 
    {
      for ( u8_i = 0; u8_i < 4; u8_i++)
      {
        th_byte_value = srec_line + u8_i*8;
        data_flash[0] = Convert2ChartoHex(*(th_byte_value + 8), *(th_byte_value + 9));
        data_flash[1] = Convert2ChartoHex(*(th_byte_value + 10), *(th_byte_value + 11));
        data_flash[2] = Convert2ChartoHex(*(th_byte_value + 12), *(th_byte_value + 13));
        data_flash[3] = Convert2ChartoHex(*(th_byte_value + 14), *(th_byte_value + 15));
  
        Flash_Write(u8_i*4 + flash_address, *((uint32_t*)data_flash));
      }  
    }

    if (flash_address == 0xa000)
    {
      msp_restart = (data_flash[0] | (data_flash[1] << 8) | (data_flash[2] << 16) | (data_flash[3] << 24));
    }
  }
   queue_tail= next_index(queue_tail);
}


