#include "RingBuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/****************************** START DECLARE VARIABLE**************************/
volatile BOOL queue_overflow_flag = FALSE;
uint8_t srec_queue[QUEUE_SIZE][MAX_LINE_SREC];
uint8_t length_queue[QUEUE_SIZE];
uint8_t queue_head = 0;
uint8_t queue_tail = 0;
static uint8_t push_line=0;

/****************************** END DECLARE VARIABLE**************************/


/****************************** CODE FOR CIRCULAR QUEUE**************************/

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
            get_index_queue_head();
	    }
	    else
	    {
            queue_overflow_flag = TRUE;
	    }   
    }
}

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

void set_queue_head(uint8_t head)
{
    queue_head = head;
}

void set_queue_tail(uint8_t tail)
{
    queue_tail = tail;
}

uint8_t get_queue_head()
{
    return queue_head;
}

uint8_t get_queue_tail()
{
    return queue_tail;
}
__ramfunc void get_index_queue_head()
{
    queue_head = next_index(queue_head);
}

void get_index_queue_tail()
{
    queue_tail = next_index(queue_tail);
}

uint8_t *get_srec_queue_address()
{
    uint8_t* pointer;
    pointer = &srec_queue[queue_tail][0];
    return pointer;
}

