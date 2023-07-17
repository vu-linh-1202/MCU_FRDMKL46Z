#include "RingBuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

volatile BOOL queue_overflow_flag = FALSE;
volatile BOOL driver_update_flag = FALSE;
uint8_t srec_queue[QUEUE_SIZE][MAX_LINE_SREC];
uint8_t queue_head = 0;
uint8_t queue_tail = 0;
uint32_t add_restart;
uint32_t msp_restart;
volatile int16_t count = 1;
uint8_t srec_lines_pushed = 0;

/* Hàm này dùng để tính toán chỉ số của phần tử tiếp theo trong mảng vòng tròn srec_queue.
 Nếu chỉ số hiện tại của phần tử là QUEUE_SIZE - 1 thì hàm sẽ trả về chỉ số của phần tử đầu tiên trong mảng.
  Nếu không, hàm sẽ trả về chỉ số của phần tử tiếp theo. */
__ramfunc uint8_t Next_Index_Queue(uint8_t index)
{
  return (index == QUEUE_SIZE - 1) ? 0 : index + 1;
}


__ramfunc void Push_Circular_Queue(uint8_t data)
{
  static uint8_t prev_data = 0;
  
  srec_queue[queue_head][count] = data;  
  if ((data != '\n') || (prev_data != '\r'))
  {
    count++;
  }
  else
  {
    if (Next_Index_Queue(queue_head) != queue_tail)
    {         
      srec_queue[queue_head][0] = count - 2; 
      srec_lines_pushed++;
    }
    else
    {
      queue_overflow_flag = TRUE;
    }   
    
    count = 1;    
    queue_head = Next_Index_Queue(queue_head);
  }
  prev_data = data;  
}


void handle_queue_overflow()
{
  Uart_String_Transmission("Error: Queue overflow detected. Failed to update driver.\r\n");
  queue_overflow_flag = FALSE;
}


void Pop_Circular_Queue()
{
  static uint32_t flash_address;
  static uint8_t data_flash[4];
  uint8_t u8_i;
  uint8_t* th_byte_value = 0;
  uint8_t* srec_line = srec_queue[queue_tail];
  
  flash_address = 0;

  for ( u8_i = 4; u8_i < 8; u8_i += 2)
  {
    flash_address *= 256;
    flash_address += Convert2ChartoHex(*(srec_line + u8_i + 1), *(srec_line + u8_i + 2));
  }

  if (flash_address >= 0xa000)
  {
    for( u8_i = 0; u8_i < 4; u8_i++)
    {
       data_flash[u8_i] = 0; 
    }

    if ((*srec_line == 'S') && (*(srec_line + 1) == '9'))
    {
      add_restart = flash_address;
      driver_update_flag = TRUE;      
    }
    else 
    {
      for ( u8_i = 0; u8_i < 4; u8_i++)
      {
        th_byte_value = srec_line + u8_i*8;
        data_flash[0] = Convert2ChartoHex(*(th_byte_value + 9), *(th_byte_value+ 10));
        data_flash[1] = Convert2ChartoHex(*(th_byte_value + 11), *(th_byte_value + 12));
        data_flash[2] = Convert2ChartoHex(*(th_byte_value + 13), *(th_byte_value + 14));
        data_flash[3] = Convert2ChartoHex(*(th_byte_value + 15), *(th_byte_value + 16));
  
        Flash_Write(u8_i*4+flash_address, data_flash[0], data_flash[1], data_flash[2], data_flash[3]);
      }  
    }

    if (flash_address == 0xa000)
    {
      msp_restart = (data_flash[0] | (data_flash[1] << 8) | (data_flash[2] << 16) | (data_flash[3] << 24));
    }
  }

  queue_tail = Next_Index_Queue(queue_tail);
}

