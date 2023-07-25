#include "UART.h"


uint8_t g_str[buffer_max];
uint8_t number_of_elements=0;

void uart0_control(CONTROL_S control)
{
  SIM->SCGC4 &= SIM_SCGC4_UART0_MASK;
  SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;/*ENABLE UART*/
}

void uart0_set_baurate(int32_t int32_baurate, int32_t int32_frequency)
{
  static int32_t u32_sbr;//sbr:1-8191
  static int32_t u32_osr;//ors:4-32
  static int32_t u32_temp_sbr;
  static int32_t u32_temp_osr;
  static int32_t u32_min_diff;
  static int32_t u32_temp_diff;
  static int32_t u32_div;
  int8_t result=0;

  u32_div = (int32_frequency / int32_baurate);
  u32_temp_osr = 4;
  u32_temp_sbr = u32_div/(u32_temp_osr+1);
  u32_osr = u32_temp_osr;
  u32_sbr = u32_temp_sbr;
  u32_min_diff = int32_frequency-int32_baurate * u32_temp_sbr  *(u32_temp_osr + 1);
  for (u32_temp_osr = 4; u32_temp_osr <= 32; u32_temp_osr++)
  {
      u32_temp_sbr = u32_div / (u32_temp_osr + 1);
      u32_temp_diff = int32_frequency - int32_baurate * u32_temp_sbr * (u32_temp_osr + 1);
      if ((u32_temp_diff <= u32_min_diff)&&(u32_min_diff >= 0))
      {
        if ((u32_temp_sbr > 1)&&(u32_temp_sbr < 8191))
        {
          if((u32_temp_diff == u32_min_diff) && (u32_temp_sbr < u32_temp_osr))
          {
            ;
          }
          else
          {
            u32_min_diff=u32_temp_diff;
            u32_osr=u32_temp_osr;
            u32_sbr=u32_temp_sbr;
            result=1;
          }
        }
      }

      u32_temp_sbr=u32_div/(u32_temp_osr+1)+1;
      u32_temp_diff=int32_baurate*u32_temp_sbr*(u32_temp_osr+1)-int32_frequency;
      if ((u32_temp_diff<u32_min_diff)&&(u32_min_diff>0))
      {
        if ((u32_temp_sbr > 1)&&(u32_temp_sbr < 8191))
        {
          if((u32_temp_diff == u32_min_diff) && (u32_temp_sbr < u32_temp_osr))
          {
            ;
          }
          else
          {
            u32_min_diff=u32_temp_diff;
            u32_osr=u32_temp_osr;
            u32_sbr=u32_temp_sbr;
            result=1;
          }
          
        }
      }
  }
  if (result==1)
  {
    UART0->BDL = UART0_BDL_SBR(u32_sbr%256);
    UART0->BDH = UART0_BDH_SBR(u32_sbr/256);
    UART0->C4 = UART0_C4_OSR(u32_osr);
  }
  else
  {
    false_setup_baudrate();
  }
   
}

void false_setup_baudrate()
{
  False_Setup_Uart();
  while(1);
}

void UART0_setclk(CHOOSE_CLOCK_PERIPHERAL select)
{
  SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_UART0SRC(select);
}

void uart0_transmission(CONTROL_S control)
{
  UART0->C2 &= ~UART0_C2_TE_MASK;
  UART0->C2 |= UART0_C2_TE(control);/*cho phep truyen*/
}

void uart0_receive(CONTROL_S control)
{
  UART0->C2 &= ~UART0_C2_RE_MASK;
  UART0->C2 |= UART0_C2_RE(control);/*cho pheps nhan*/
}

uint8_t Uart0_Receive_Data()
{
  /* Wait Data Register is full */
  while ((UART0->S1&UART0_S1_RDRF_MASK)!=UART0_S1_RDRF_MASK);
  
  /* Read and Return Data Register */
  return UART0->D;
}

void Uart_String_Transmission(uint8_t *str)
{
  while(*str!=0)
  {
    Uart_Put_Data(*str);
    str++;
  }
}

void Uart_Put_Data(uint8_t data)
{
  UART0->D = data;
  while( (UART0->S1&UART0_S1_TDRE_MASK) != UART0_S1_TDRE_MASK);
}

void clear_Str(uint8_t *str1)
{
  uint16_t i;
  number_of_elements=0;
  for (i=0 ;i < buffer_max; i++)
  {
    *str1=0;
    str1++;
  }
}

void uart_init()
{
  uart0_control(ENABLE);
  uart0_set_baurate(BAUDRATE_921600,FREQUENCY_CLOCK);
  UART0->C2 |= UART_C2_RIE_MASK;/* enable rie interurrupt*/
  UART0_setclk(MCG_PLL_FLLCLK);
  uart0_transmission(ENABLE);
  uart0_receive(ENABLE);
}