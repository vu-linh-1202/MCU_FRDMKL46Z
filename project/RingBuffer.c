#include "RingBuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

volatile BOOL queue_overflow_flag = FALSE;
volatile BOOL driver_update_flag = FALSE;
uint8_t srec_queue[QUEUE_SIZE][MAX_LINE_SREC];
uint8_t queue_push_index = 0;
uint8_t queue_pop_index = 0;
uint8_t srec_lines_recorded = 0;
uint32_t add_restart;
uint32_t msp_restart;
volatile int16_t count = 1;
uint8_t srec_lines_pushed = 0;

__ramfunc uint8_t next_index(uint8_t index)
{
  return (index == QUEUE_SIZE - 1) ? 0 : index + 1;
}


 void push_srec_line(uint8_t data)
{
  static uint8_t prev_data = 0;
  
  srec_queue[queue_push_index][count] = data;  
  if ((data != '\n') || (prev_data != '\r'))
  {
    count++;
  }
  else
  {
    if (next_index(queue_push_index) != queue_pop_index)
    {         
      srec_queue[queue_push_index][0] = count - 2; 
      srec_lines_pushed++;
    }
    else
    {
      queue_overflow_flag = TRUE;
    }   
    
    count = 1;    
    queue_push_index = next_index(queue_push_index);
  }
  prev_data = data;  
}


/*
__ramfunc void push_srec_line(uint8_t data)
{
  static uint8_t prev_data = 0;
  static uint8_t* srec_ptr = NULL;
  uint8_t* queue_ptr = srec_queue[queue_push_index];
  
  if (srec_ptr == NULL)
  {
    srec_ptr = queue_ptr;
  }
  
  *srec_ptr = data;  
  if ((data != '\n') || (prev_data != '\r'))
  {
    srec_ptr++;
  }
  else
  {
    if (next_index(queue_push_index) != queue_pop_index)
    {         
      srec_queue[queue_push_index][0] = srec_ptr - queue_ptr - 1; 
      srec_lines_pushed++;
    }
    else
    {
      queue_overflow_flag = TRUE;
    }   
    
    srec_ptr = NULL;
    queue_push_index = next_index(queue_push_index);
  }
  prev_data = data;  
}
*/

void handle_queue_overflow()
{
  Uart_String_Transmission("Fail to updated driver");
}
/*
void process_srec_line()
{
  uint8_t i, k, temp;
  static uint32_t flash_address;
  static uint8_t data_flash[4];
    
  flash_address = 0;

  for (i = 4; i < 8; i += 2)
  {
    flash_address *= 256;
    flash_address += (Convert2ChartoHex(srec_queue[queue_pop_index][i+1], srec_queue[queue_pop_index][i+2]));
  }

  if (flash_address >= 0xa000)
  {
    data_flash[0] = 0;
    data_flash[1] = 0;
    data_flash[2] = 0;
    data_flash[3] = 0;
    if ((srec_queue[queue_pop_index][i+1] == 'S') && (srec_queue[queue_pop_index][i+1] == '9'))
    {
      add_restart = flash_address;
      driver_update_flag = TRUE;      
    }
    else 
    {
      for (i = 0; i < 4; i++)
      {
        data_flash[0] = Convert2ChartoHex(srec_queue[queue_pop_index][i*8+9], srec_queue[queue_pop_index][i*8+10]);
        data_flash[1] = Convert2ChartoHex(srec_queue[queue_pop_index][i*8+11], srec_queue[queue_pop_index][i*8+12]);
        data_flash[2] = Convert2ChartoHex(srec_queue[queue_pop_index][i*8+13], srec_queue[queue_pop_index][i*8+14]);
        data_flash[3] = Convert2ChartoHex(srec_queue[queue_pop_index][i*8+15], srec_queue[queue_pop_index][i*8+16]);
  
        Flash_Write(i*4+flash_address, data_flash[0], data_flash[1], data_flash[2], data_flash[3]);
      }  
    }
    if (flash_address == 0xa000)
    {
      msp_restart = (data_flash[0] | (data_flash[1] << 8) | (data_flash[2] << 16) | (data_flash[3] << 24));
    }
  }
  queue_pop_index = next_index(queue_pop_index);
}
*/

void process_srec_line()
{
  static uint32_t flash_address;
  static uint8_t data_flash[4];
  uint8_t* srec_line = srec_queue[queue_pop_index];
  
  flash_address = 0;

  for (uint8_t i = 4; i < 8; i += 2)
  {
    flash_address *= 256;
    flash_address += Convert2ChartoHex(*(srec_line + i + 1), *(srec_line + i + 2));
  }

  if (flash_address >= 0xa000)
  {
    data_flash[0] = 0;
    data_flash[1] = 0;
    data_flash[2] = 0;
    data_flash[3] = 0;

    if ((*srec_line == 'S') && (*(srec_line + 1) == '9'))
    {
      add_restart = flash_address;
      driver_update_flag = TRUE;      
    }
    else 
    {
      for (uint8_t i = 0; i < 4; i++)
      {
        data_flash[0] = Convert2ChartoHex(*(srec_line + i*8 + 9), *(srec_line + i*8 + 10));
        data_flash[1] = Convert2ChartoHex(*(srec_line + i*8 + 11), *(srec_line + i*8 + 12));
        data_flash[2] = Convert2ChartoHex(*(srec_line + i*8 + 13), *(srec_line + i*8 + 14));
        data_flash[3] = Convert2ChartoHex(*(srec_line + i*8 + 15), *(srec_line + i*8 + 16));
  
        Flash_Write(i*4+flash_address, data_flash[0], data_flash[1], data_flash[2], data_flash[3]);
      }  
    }

    if (flash_address == 0xa000)
    {
      msp_restart = (data_flash[0] | (data_flash[1] << 8) | (data_flash[2] << 16) | (data_flash[3] << 24));
    }
  }

  queue_pop_index = next_index(queue_pop_index);
}