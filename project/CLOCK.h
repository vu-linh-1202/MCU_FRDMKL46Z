#ifndef _CLOCK_H_
#define _CLOCK_H_
#include "MKL46Z4.h"

/****************************** START DEFINITION **************************************/

#define check_FCRDIV_FCRDIV4 (((MCG->SC & MCG_SC_FCRDIV_MASK) == MCG_C1_FRDIV(2))\
&& ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) == SIM_CLKDIV1_OUTDIV4(1)))

/* Choose clock source by FLL, PLL, Internal Referrence Clock, External Referrnce Clock*/
typedef enum
{
    FLL_PLL,
    INTERNAL_REFERRENCE_CLOCK,
    EXTERNAL_REFERRENCR_CLOCK
}CHOOSE_CLOCK_SOURCE;

typedef enum
{
    SLOW_INTERNAL_REFERRENCE_CLOCK,
    FAST_INTERNAL_REFERRENCE_CLOCK
}CHOOSE_INTERNAL_CLOCK;

typedef enum
{
    DISABLE_CLOCK,
    ENABLE_CLOCK
}CONTROL_CLOCK;

typedef enum
{
    DISABLE,
    ENABLE
}CONTROL_S;

/*Clock 1 Output Divider value
 * Sets the divide value for the core/system clock, as well as the bus/flash clocks. At the end of reset, it is
 * loaded with 0000 (divide by one), 0001 (divide by two), 0011 (divide by four), or 0111 (divide by eight)
 * depending on the setting of the FTFA_FOPT[LPBOOT]
 */
typedef enum
{
    OUTDIV1_1,
    OUTDIV1_2,
    OUTDIV1_3,
    OUTDIV1_4,
    OUTDIV1_5,
    OUTDIV1_6,
    OUTDIV1_7,
    OUTDIV1_8,
    OUTDIV1_9,
    OUTDIV1_10,
    OUTDIV1_11,
    OUTDIV1_12,
    OUTDIV1_13,
    OUTDIV1_14,
    OUTDIV1_15,
    OUTDIV1_16
}OUTDIV1;

/*Clock 4 Output Divider value
 * Sets the divide value for the bus and flash clock and is in addition to the System clock divide ratio. At the
 * end of reset, it is loaded with 0001 (divide by two)
*/
typedef enum
{
    OUTDIV4_1,
    OUTDIV4_2,
    OUTDIV4_3,
    OUTDIV4_4,
    OUTDIV4_5,
    OUTDIV4_6,
    OUTDIV4_7,
    OUTDIV4_8
}OUTDIV4;

/*PLL/FLL clock select
 * Selects the MCGPLLCLK or MCGFLLCLK clock for various peripheral clocking options.
 * 0 MCGFLLCLK clock
 * 1 MCGPLLCLK clock with fixed divide by two*/
typedef enum
{
    CHOOSE_FLL,
    CHOOSE_PLL_DIV_2
}CHOOSE_PLL_2_FLL_CLOCK;

typedef enum
{
    FLL,
    PLL
} CHOOSE_FLL_PLL_CLOCK;

/*Config at SIM_SCGC6
 * TPM0 Clock Gate Control
 * TPM1 Clock Gate Control
 * TMP2 Clock Gate Control
 */
typedef enum
{
    TPM0_0 = SIM_SCGC6_TPM0_MASK,
    TPM1_1 = SIM_SCGC6_TPM1_MASK,
    TPM2_2 = SIM_SCGC6_TPM2_MASK
}CHOOSE_TPM_X;

/*Frequency Range Select
 * Selects the frequency range for the crystal oscillator or external clock source. See the Oscillator (OSC)
 * chapter for more details and the device data sheet for the frequency ranges used.
 * 00 Encoding 0 — Low frequency range selected for the crystal oscillator .
 * 01 Encoding 1 — High frequency range selected for the crystal oscillator .
 * 1X Encoding 2 — Very high frequency range selected for the crystal oscillator .*/
typedef enum
{
    LOW_FREQUENCY,
    HIGHT_FREQUENCY,
    VERY_HIGHT_FREQUENCY
}FREQUENCY_RANGER;

typedef enum
{
    EXTERNAL_CLOCK,
    OSC
}CHOOSE_EXTERNAL_CLOCK;

typedef enum
{
    DISABLE_PERIPHERAL,
    MCG_PLL_FLLCLK,
    OSCERCLK,
    MCGIRCLK
} CHOOSE_CLOCK_PERIPHERAL;

typedef enum
{
    FLL_FRDIV,
    FLL_SLOW_INTERNAL_REFERRENCR_CLOCK
}CLOCK_SOURCE_FLL;

typedef enum
{
    FRDIV_32,
    FRDIV_64,
    FRDIV_128,
    FRDIV_256,
    FRDIV_512,
    FRDIV_1024,
    FRDIV_1280,
    FRDIV_1536
}OSC_OSCCLK_FRDIV;

/****************************** END DEFINITION **************************************/

/******************************* FUNCTION *******************************************/

/******************** Start Config Clock for System ******************************/
void Internal_Referrence_Clock_4MHz_Divide(int8_t ui8_div);
void Choose_Internal_Clock(CHOOSE_INTERNAL_CLOCK choose_clock);

/******************** Start Config FLL, PLL ***********************************/
void Choose_FLL_PLL(CHOOSE_FLL_PLL_CLOCK choose);
void Source_Clock_for_FLL(CLOCK_SOURCE_FLL source_clock);
void OSC_OSCCLK_DIV(OSC_OSCCLK_FRDIV div);
void PLL_Source_Divider(int8_t int8_frdiv_0);
void PLL_Clock_Control(CONTROL_CLOCK control);
void Multiply_Out_PLL(int8_t int8_Vdiv_0);
void MCGOUTCLK_divide_OUTDIV1_OUTDIV4(OUTDIV1 outdiv1, OUTDIV4 outdiv4);
void MCGOUTCLK_Clock(CHOOSE_CLOCK_SOURCE choose_clock_source);

/******************** Start Config Clock Peripheral  **************************/
void  Choose_PLLCLK2_FLLCLK(CHOOSE_PLL_2_FLL_CLOCK choose);
void Choose_Clock_TPM(CHOOSE_CLOCK_PERIPHERAL clock);

/******************** Start Config Clock OSC *********************************/
void Choose_Frequency_Range(FREQUENCY_RANGER frequence_range);
void Choose_OSCCLK(CHOOSE_EXTERNAL_CLOCK choose_clock);
void OSCCLK_Clock_Gate(CONTROL_CLOCK control);

void Clock_Gate_Control_TPM(CONTROL_CLOCK control, CHOOSE_TPM_X tpm);
void Init_IRC_4MHz_TPM();
void Init_Slow_IRC_TPM();
void Init_OSC_TPM();
void Init_OSC();
void Init_FLL();
void OSC_CLOCK_FLL();

/******************** Start Config Clock Gate for Bus Clock, Flash Clock ***********************************/
void Clock_Gate_PIT(CONTROL_CLOCK control);
void Clock_Gate_MCGIRCLK(CONTROL_CLOCK control);

/******************** EXAMPLE ***************/
void OSCCLK_Source_FLL();
void init_clock();

#endif /*_CLOCK_H_*/

