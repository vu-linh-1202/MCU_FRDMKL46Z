#ifndef _UART_H_
#define _UART_H_
#include "MKL46Z4.h"
#include "MKL46Z4.h"
#include "CLOCK.h"
#include "Peripheral.h"
#include "UART.h"
#define buffer_max 100

void uart0_control(CONTROL_S control);
void uart0_set_baurate(int32_t int32_baurate, int32_t int32_frequency);
void UART0_setclk(CHOOSE_CLOCK_PERIPHERAL select);
void uart0_transmission(CONTROL_S control);
void uart0_receive(CONTROL_S control);
uint8_t Uart0_Receive_Data();
void Uart_String_Transmission(uint8_t *str);
void Uart_Put_Data(uint8_t data);
void uart_init();
void clear_Str(uint8_t *p);

#endif




