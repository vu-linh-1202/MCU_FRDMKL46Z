#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_
#include "MKL46Z4.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stdbool.h"
#include "Srecord.h"
#include "String_Handle.h"
#include "Flash.h"
#include "UART.h"

/****************************** START DEFINITION **************************************/
#define MAX_LINE_SREC    50
#define QUEUE_SIZE       10

typedef enum
{
    TRUE =  1,
    FALSE
}BOOL;

/****************************** END DEFINITION **************************************/

extern volatile BOOL queue_overflow_flag;

__ramfunc void Push_Circular_Queue(uint8_t data);
__ramfunc uint8_t next_index(uint8_t ui8_index);
__ramfunc void get_index_queue_head();
void set_queue_head(uint8_t head);
void set_queue_tail(uint8_t tail);
void get_index_queue_tail();
void Handle_Queue_Overflow();
uint8_t get_queue_head();
uint8_t get_queue_tail();
uint8_t *get_srec_queue_address();

#endif /* _RING_BUFFER_H_ */