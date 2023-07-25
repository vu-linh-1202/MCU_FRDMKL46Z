#include "String_Handle.h"


/************************************* CODE FOR PROCESS STRING FOR ERROR OR SUCCESSFUL ***********************************/

void Handle_Checksum_False()
{
    Uart_String_Transmission("Error: Checksum False.\r\n");
} 

void Handle_Queue_Overflow()
{
    Uart_String_Transmission("Error: Queue overflow detected. Failed to update driver.\r\n");
    queue_overflow_flag = FALSE;
} 

void Update_Driver_Successful()
{
    Uart_String_Transmission("Update Driver Successful");
}

void False_Setup_Uart()
{
    Uart_String_Transmission("Error: False Setup Uart");
}

