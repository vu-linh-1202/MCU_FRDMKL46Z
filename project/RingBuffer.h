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

extern uint8_t queue_push_index;
extern uint8_t queue_pop_index;
extern uint8_t srec_lines_recorded;
extern volatile BOOL queue_overflow_flag;
extern volatile BOOL driver_update_flag;
extern uint32_t add_restart;
extern uint32_t msp_restart;
extern uint8_t srec_lines_pushed;



__ramfunc void push_srec_line(uint8_t data);
void handle_queue_overflow();
void process_srec_line();
__ramfunc uint8_t next_index(uint8_t ui8_index);

#endif /* _RING_BUFFER_H_ */