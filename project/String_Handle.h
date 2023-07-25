#ifndef __STRING_HANDLE_H__
#define __STRING_HANDLE_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "UART.h"
#include "RingBuffer.h"

void Handle_Checksum_False();
void Handle_Queue_Overflow();
void Update_Driver_Successful();

#endif  /*__STRING_HANDLE_H__*/