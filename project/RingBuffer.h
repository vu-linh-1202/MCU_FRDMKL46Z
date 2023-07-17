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
    TRUE,
    FALSE
}BOOL;

extern volatile BOOL queue_overflow_flag;
extern volatile BOOL driver_update_flag;
extern uint8_t srec_lines_pushed;
extern uint32_t add_restart;
extern uint32_t msp_restart;


__ramfunc void Push_Circular_Queue(uint8_t data);
void handle_queue_overflow();
void Pop_Circular_Queue();
__ramfunc uint8_t Next_Index_Queue(uint8_t ui8_index);

#endif /* _RING_BUFFER_H_ */