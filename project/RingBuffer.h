#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_
#include "MKL46Z4.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stdbool.h"
#include "Srecord.h"
#include "Flash.h"
#include "UART.h"

#define MAX_LINE_SREC    50
#define QUEUE_SIZE       10

typedef enum
{
    TRUE =  1,
    FALSE
}BOOL;

extern volatile BOOL queue_overflow_flag;
extern volatile BOOL driver_update_flag;

__ramfunc uint8_t next_index(uint8_t ui8_index);
void set_queue_head(uint8_t head);
void set_queue_tail(uint8_t tail);
uint8_t get_queue_head();
uint8_t get_queue_tail();
void Handle_Queue_Overflow();
__ramfunc void Push_Circular_Queue(uint8_t data);
void Pop_Circular_Queue(uint32_t* add_restart, uint32_t* msp_restart);

#endif /* _RING_BUFFER_H_ */