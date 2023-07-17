#include "MKL46Z4.h"
#include "Peripheral.h"

void initLed()
{
    /* Enable clock for PORTD, PORTE */
    SIM->SCGC5 |= (1 << 11);/* Set bit 11 enable clock portC*/
    SIM->SCGC5 |= (1 << 12);/* Set bit 12 enable clock portD*/
    SIM->SCGC5 |= (1 << 13);/* Set bit 13 enable clock portE*/
    SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK; /* Set bit 9 enable clock portA*/

    /* Configure multiplex of PTD5 and PTE29 and PTC3 as GPIO */
    PORTD->PCR[5]  |= PORT_PCR_MUX(1);
    PORTE->PCR[29] |= PORT_PCR_MUX(1);
    PORTC->PCR[3]  |= PORT_PCR_MUX(1);
    PORTC->PCR[12]  |= PORT_PCR_MUX(1);
    PORTA->PCR[1]  |=PORT_PCR_MUX(2);/*UART*/
    PORTA->PCR[2]  |=PORT_PCR_MUX(2);/* UART*/
    PORTC->PCR[3]  |= PORT_PCR_IRQC(10);
    PORTC->PCR[3]  |=3;
    PORTC->PCR[12] |=3;
    /* Configure PTD5 and PTE29 as output, PTC3 as input  */
    GPIOD->PDDR |= (1 << 5);
    GPIOE->PDDR |= (1 << 29);
    GPIOC->PDDR &= (~(1 << 3));
    /* Clear PTD5 and PTE29 */
    RED_LED_OFF;
    GREEN_LED_OFF;
}

SW_STATUS detect_key_pressed(int32_t *int32_old_status_SW1, int32_t *int32_old_status_SW3)
{
    static int32_t last_edge_SW1 = -1;
    static int32_t last_edge_SW3 = -1;
    int32_t current_edge_SW1 = SW1_IN;
    int32_t current_edge_SW3 = SW3_IN;

    if (*int32_old_status_SW1 != current_edge_SW1)
    {
        last_edge_SW1 = current_edge_SW1;
        *int32_old_status_SW1 = current_edge_SW1;
        return current_edge_SW1 == 0 ? SW1_FALLING_EDGE : SW1_RISING_EDGE;
    }

    if (*int32_old_status_SW3 != current_edge_SW3)
    {
        last_edge_SW3 = current_edge_SW3;
        *int32_old_status_SW3 = current_edge_SW3;
        return current_edge_SW3 == 0 ? SW3_FALLING_EDGE : SW3_RISING_EDGE;
    }

    return NOT_DETECT;
}
