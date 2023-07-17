#ifndef _PIT_H_
#define _PIT_H_
#include "MKL46Z4.h"
typedef enum{PIT_CHANNEL_0,PIT_CHANNEL_1}PIT_CHANNEL;

void init_PIT();
void load_val_PIT (PIT_CHANNEL channel,int32_t PIT_val );
void disable_PIT (PIT_CHANNEL channel);
void enable_PIT (PIT_CHANNEL channel);
void enable_clock_PIT_TIMER();
void disable_clock_PIT_TIMER();
void enable_clock_gate_PIT();
void disable_clock_gate_PIT();
void clear_flag_PIT(PIT_CHANNEL channel);

#endif