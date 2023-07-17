#include "MKL46Z4.h" 
#include "PIT.h" 
#include "CLOCK.h"

void init_PIT()
{
    enable_clock_PIT_TIMER();
    Clock_Gate_PIT(ENABLE_CLOCK);    
    load_val_PIT(PIT_CHANNEL_0,2000000);/* load value for PIT*/
    enable_PIT(PIT_CHANNEL_0);
    NVIC_EnableIRQ(PIT_IRQn);
}


void load_val_PIT (PIT_CHANNEL channel,int32_t PIT_val )
{
  PIT->CHANNEL[channel].LDVAL = PIT_val;
}

void disable_PIT (PIT_CHANNEL channel)
{
    PIT->CHANNEL[channel].TCTRL =0;
    /* TEN=0 :timer  (n) channel disable, TIE=0:disable interrupt*/
}

void enable_PIT (PIT_CHANNEL channel)
{
    PIT->CHANNEL[channel].TCTRL =3;
    /* TEN=1 :timer (n) channel enable, TIE=1:enable interrupt */
}

void enable_clock_PIT_TIMER()
{
    PIT->MCR &= ~PIT_MCR_MDIS_MASK;
    /* MDIS=0 clock for PIT enable*/
}

void disable_clock_PIT_TIMER()
{
    PIT->MCR |= PIT_MCR_MDIS_MASK;
}

void enable_clock_gate_PIT()
{
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
    /* PIT clock gate control*/
}

void disable_clock_gate_PIT()
{
    SIM->SCGC6 &= ~SIM_SCGC6_PIT_MASK;
}

void clear_flag_PIT(PIT_CHANNEL channel)
{
    PIT->CHANNEL[channel].TFLG |= 1;
}
