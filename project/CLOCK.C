#include "MKL46Z4.h"
#include "CLOCK.h"
#include "Peripheral.h"
#include <math.h>

/******************** Start Config Clock for System ******************************/
void Internal_Referrence_Clock_4MHz_Divide(int8_t ui8_div)
{
    MCG->SC &= ~MCG_SC_FCRDIV_MASK;
    MCG->SC |= MCG_SC_FCRDIV(log2f((float_t)(ui8_div)));
}

void Choose_Internal_Clock(CHOOSE_INTERNAL_CLOCK choose_clock)
{
    MCG->C2 &= ~MCG_C2_IRCS_MASK;
    MCG->C2 |= choose_clock;
    MCG->C1 |= MCG_C1_IRCLKEN_MASK; /* Turn on Clock Gate - CG for Internal Reference Clock (IRC)*/
}

/******************** End Config Clock for System ******************************/

/******************** Start Config FLL, PLL ***********************************/

/****** FLL, PLL Clock Select
 * Controls whether the PLL or FLL output is selected as the MCG source when CLKS[1:0]=00. If the PLLS
 * bit is cleared and PLLCLKEN 0 is not set, the PLL is disabled in all modes. If the PLLS is set, the FLL isdisabled in all modes.
 * 0 FLL is selected.
 * 1 PLL is selected (PRDIV 0 need to be programmed to the correct divider to generate a PLL reference
 * clock in the range of 2–4 MHz prior to setting the PLLS bit).
*/
void Choose_FLL_PLL(CHOOSE_FLL_PLL_CLOCK choose)
{
    MCG->C6 &= ~MCG_C6_PLLS_MASK;
    MCG->C6 |= MCG_C6_PLLS(choose);
}

/* FLL Internal Referrence Clock Select
 * Selects the reference clock source for the FLL.
 * 0 External reference clock is selected.
 * 1 The slow internal reference clock is selected.
 */
void Source_Clock_for_FLL(CLOCK_SOURCE_FLL source_clock)
{
    MCG->C1 &= ~MCG_C1_IREFS_MASK;
    MCG->C1 |= MCG_C1_IREFS(source_clock);
}

/* FLL External Reference Divider 
 * Selects the amount to divide down the external reference clock for the FLL. The resulting frequency must
 * be in the range 31.25 kHz to 39.0625 kHz (This is required when FLL/DCO is the clock source for MCGOUTCLK 
*/
void OSC_OSCCLK_DIV(OSC_OSCCLK_FRDIV div)
{
    MCG->C1 &= ~MCG_C1_FRDIV_MASK;
    MCG->C1 |= MCG_C1_FRDIV(div);
}

/* PLL External Reference Divider
 * Divide Factor 1 - 24
 * The resulting frequency must be in the range of 2 MHz to 4 MHz. After the PLL
 * is enabled (by setting either PLLCLKEN 0 or PLLS), the PRDIV 0 value must not
 * be changed when LOCK0 is zero
*/
void PLL_Source_Divider(int8_t int8_frdiv_0)
{
    MCG->C5 &= ~MCG_C5_PRDIV0_MASK;
    MCG->C5 |= MCG_C5_PRDIV0(int8_frdiv_0 - 1);
}

/* PLL Clock Enable 
 * Enables the PLL independent of PLLS and enables the PLL clock for use as MCGPLLCLK. (PRDIV 0
 * needs to be programmed to the correct divider to generate a PLL reference clock in the range of 2 - 4 MHz
 * range prior to setting the PLLCLKEN 0 bit)
*/
void PLL_Clock_Control(CONTROL_CLOCK control)
{
    MCG->C5 &= ~MCG_C5_PLLCLKEN0_MASK;
    MCG->C5 |= MCG_C5_PLLCLKEN0(control);
    /*The External Oscillator is being used as the reference clock, the OSCINIT 0 bit should be checked to make sure it is set.*/
    while ((MCG->S & MCG_S_OSCINIT0_MASK) != 0);    
}

/*VCO 0 Divider - PLL VCO Divide Factor 24-55
Selects the amount to divide the VCO output of the PLL. The VDIV 0 bits establish the multiplication factor
(M) applied to the reference clock frequency. After the PLL is enabled (by setting either PLLCLKEN 0 or
PLLS), the VDIV 0 value must not be changed when LOCK 0 is zero*/
void Multiply_Out_PLL(int8_t int8_Vdiv_0)
{
    MCG->C6 &= ~MCG_C6_VDIV0_MASK;
    MCG->C6 |= MCG_C6_VDIV0(int8_Vdiv_0 - 24);
}

/* Config Divide */
void MCGOUTCLK_divide_OUTDIV1_OUTDIV4(OUTDIV1 outdiv1, OUTDIV4 outdiv4)
{
    SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV4_MASK;
    SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(outdiv1);
    SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV4(outdiv4);
}

/* Core Clock, platform clock, and system clock*/
void MCGOUTCLK_Clock(CHOOSE_CLOCK_SOURCE choose_clock_source)
{
    MCG->C1 &= ~MCG_C1_CLKS_MASK;
    MCG->C1 |= MCG_C1_CLKS(choose_clock_source);
}

/******************** End Config FLL, PLL *************************************/

/******************** Start Config Clock Peripheral  **************************/

void  Choose_PLLCLK2_FLLCLK(CHOOSE_PLL_2_FLL_CLOCK choose)
{
    SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL(choose);
}

/* Include source: MCGIRCLK,  OSCERCLK, MCGFLLCLK, MCGPLLCLK/2 */
void Choose_Clock_TPM(CHOOSE_CLOCK_PERIPHERAL clock)
{
    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(clock);
}

/******************** End Config Clock Peripheral *****************************/

/******************** Start Config Clock OSC *********************************/

void Choose_Frequency_Range(FREQUENCY_RANGER frequence_range)
{
    MCG->C2 &= ~MCG_C2_RANGE_MASK;
    MCG->C2 |= MCG_C2_RANGE(frequence_range); // khoang tan so thach anh la *MHz
}

void Choose_OSCCLK(CHOOSE_EXTERNAL_CLOCK choose_clock)
{
    MCG->C2 &=~MCG_C2_EREFS_MASK;
    MCG->C2 |= MCG_C2_EREFS(choose_clock);/*chon thach anh thay vi clock ngoai tu chan chip*/
}

/* Enable OSC External referrence clock OSCERCLK*/
void OSCCLK_Clock_Gate(CONTROL_CLOCK control)
{
    OSC0 -> CR &= ~OSC_CR_ERCLKEN_MASK;
    OSC0 -> CR |= OSC_CR_ERCLKEN(control);
}

void Clock_Gate_Control_TPM(CONTROL_CLOCK control, CHOOSE_TPM_X tpm)
{
    SIM->SCGC6 &= ~tpm;
    if(control ==  ENABLE_CLOCK)
    {
        SIM->SCGC6 |= tpm;
    }
}

/*CLOCK 4Mhz/128=125kHz FOR tpm*/
void Init_IRC_4MHz_TPM()
{
    Internal_Referrence_Clock_4MHz_Divide(32);
    Choose_Internal_Clock(FAST_INTERNAL_REFERRENCE_CLOCK);
    Clock_Gate_MCGIRCLK(ENABLE_CLOCK);
}

void Init_Slow_IRC_TPM()
{
    Choose_Internal_Clock(SLOW_INTERNAL_REFERRENCE_CLOCK);
    Clock_Gate_MCGIRCLK(ENABLE_CLOCK);
}

void Init_OSC_TPM()
{
    Choose_Frequency_Range(HIGHT_FREQUENCY);
    OSCCLK_Clock_Gate(ENABLE_CLOCK);
    Choose_OSCCLK(OSC);
}

void Init_OSC()
{
    MCGOUTCLK_divide_OUTDIV1_OUTDIV4(OUTDIV1_1, OUTDIV4_1);
    Choose_OSCCLK(OSC);
    MCGOUTCLK_Clock(EXTERNAL_REFERRENCR_CLOCK);
}

void Init_FLL()
{
    MCGOUTCLK_divide_OUTDIV1_OUTDIV4(OUTDIV1_1, OUTDIV4_1);
    Choose_Frequency_Range(HIGHT_FREQUENCY);
    Choose_OSCCLK(OSC);
    OSC_OSCCLK_DIV(FRDIV_256);
    Source_Clock_for_FLL(FLL_FRDIV);
    Choose_FLL_PLL(FLL);
    MCGOUTCLK_Clock(FLL_PLL);
}

void OSC_CLOCK_FLL()
{
    Choose_Frequency_Range(HIGHT_FREQUENCY);
    Choose_OSCCLK(OSC);
    Source_Clock_for_FLL(FLL_FRDIV);
}
/******************** End Config Clock OSC ***********************************/

/******************** Start Config Clock Gate for Bus Clock, Flash Clock ***********************************/

/* Clock Gate for Bus, Flash*/
void Clock_Gate_PIT(CONTROL_CLOCK control)
{
    SIM->SCGC6 &= ~SIM_SCGC6_PIT_MASK;
    SIM->SCGC6 |= SIM_SCGC6_PIT(control);
}

/* Clock Gate for MCGIRCLK */
void Clock_Gate_MCGIRCLK(CONTROL_CLOCK control)
{
    MCG->C1 &= MCG_C1_IRCLKEN_MASK;
    MCG->C1 |= MCG_C1_IRCLKEN(control);
}

/******************** End Config Clock Gate for Bus Clock, Flash Clock ***********************************/

/******************** EXAMPLE ***************/

void OSCCLK_Source_FLL()
{
    Choose_Frequency_Range(HIGHT_FREQUENCY);
    Choose_OSCCLK(OSC);
    Source_Clock_for_FLL(FLL_FRDIV);
}

void init_clock()
{
    /* Clock for UART*/
    Choose_Frequency_Range(HIGHT_FREQUENCY);
    OSCCLK_Clock_Gate(ENABLE_CLOCK);
    Choose_OSCCLK(OSC);
    /*******************/
    MCGOUTCLK_divide_OUTDIV1_OUTDIV4(OUTDIV1_1, OUTDIV4_4);
    PLL_Source_Divider(2);
    Multiply_Out_PLL(24);
    PLL_Clock_Control(ENABLE_CLOCK);
    MCGOUTCLK_Clock(FLL_PLL);
    Choose_PLLCLK2_FLLCLK(CHOOSE_PLL_DIV_2);
    Choose_FLL_PLL(PLL);
}