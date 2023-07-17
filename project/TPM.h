#ifndef _TPM_H_
#define _TPM_H_
#include "MKL46Z4.h"
#include "CLOCK.h"

typedef enum
{
    CHANNEL_0,
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3,
    CHANNEL_4,
    CHANNEL_5
}TPM_CHANNEL;

typedef enum
{
    DISABLE_TPM,
    TPM_COUNTER_INCREMENTS_ON_EVERY_CLOCK,
    TPM_COUNTER_INCREMENTS_ON_RISING,
    RESERVED
}CHOOSE_CLOCK_MODE;
typedef enum
{
    PS_DIV1=0,
    PS_DIV2=1,
    PS_DIV4=2,
    PS_DIV8=3,
    PS_DIV16=4,
    PS_DIV32=5,
    PS_DIV64=6,
    PS_DI128=7
}PS_DIVIDE;

void Set_TPM0_Value(TPM_CHANNEL channel,CHOOSE_CLOCK_MODE mod,PS_DIVIDE div, int32_t val_mod, int8_t percent);

#endif