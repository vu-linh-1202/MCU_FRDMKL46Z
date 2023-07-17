#ifndef _Peripheral_H
#define _Peripheral_H
#include "MKL46Z4.h"

typedef enum
{   NOT_DETECT,
    SW1_FALLING_EDGE,
    SW1_RISING_EDGE,
    SW3_FALLING_EDGE,
    SW3_RISING_EDGE
}SW_STATUS;

#define RED_LED_ON            GPIOD->PCOR |= (1 << 5)
#define RED_LED_OFF           GPIOD->PSOR |= (1 << 5)
#define GREEN_LED_ON          GPIOE->PCOR |= (1 << 29)
#define GREEN_LED_OFF         GPIOE->PSOR |= (1 << 29)
#define GREEN_LED_TOGGLE      GPIOD->PTOR |= (1 << 5)
#define RED_LED_TOGGLE        GPIOE->PTOR |= (1 << 29)
#define RED_LED_SET           GPIOE->PDOR |= (1 << 29)
#define RED_LED_CLEAR         GPIOE->PDOR &= ~(1 << 29)
/*#define SW1                 GPIOC->PDIR&(1<<3)*/
#define SW1_IN                   (*(volatile unsigned int *)0x400FF090&(1<<3))
/*#define SW3                 GPIOC->PDIR&(1<<12)*/
#define SW3_IN                   (*(volatile unsigned int *)0x400FF090&(1 <<12))

void initLed();
SW_STATUS detect_key_pressed(int32_t *int32_old_status_SW1,int32_t *int32_old_status_SW3);

#endif