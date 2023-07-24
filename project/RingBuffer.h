#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_
#include "MKL46Z4.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
extern uint32_t add_restart;
extern uint32_t msp_restart;
extern uint8_t queue_head;
extern uint8_t queue_tail;
extern uint8_t srec_length;
extern uint8_t srec_queue[QUEUE_SIZE][MAX_LINE_SREC];


__ramfunc void Push_Circular_Queue(uint8_t data);
__ramfunc uint8_t next_index(uint8_t ui8_index);
void Handle_Queue_Overflow();
void Pop_Circular_Queue();

#endif /* _RING_BUFFER_H_ */