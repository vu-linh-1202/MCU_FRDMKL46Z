#include "MKL46Z4.h" 
#include "TPM.h"

void Set_TPM0_Value(TPM_CHANNEL channel,CHOOSE_CLOCK_MODE mod,PS_DIVIDE div,\
int32_t val_mod,int8_t percent)
{
    int32_t val;
    val=val_mod*percent/100;
    TPM0->SC &= ~TPM_SC_CMOD_MASK;
    TPM0->SC &= ~TPM_SC_PS_MASK;
    TPM0->SC |= TPM_SC_PS(div);
    TPM0->MOD &=TPM_MOD_MOD_MASK;
    TPM0->MOD |= TPM_MOD_MOD(val_mod);
    TPM0->CONF |= TPM_CONF_DBGMODE(3);
    TPM0->CONTROLS[channel].CnV = val;
    TPM0->SC |= TPM_SC_CMOD(mod);
}
