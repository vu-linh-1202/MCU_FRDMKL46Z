#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Srecord.h"

__ramfunc int Convert2ChartoHex(char a1, char a2)
{
    unsigned char u8_hight_byte = 0;
    unsigned char u8_low_byte = 0;
    unsigned char u8_hex = 0;
    
    if ((a1 >= '0') && (a1 <= '9'))
    {
        u8_hight_byte = a1 - '0';
    }
    else if ((a1 >= 'A') && (a1 <= 'F'))
    {
        u8_hight_byte = a1 - 'A' + 10;
    }
    
    if ((a2 >= '0') && (a2 <= '9'))
    {
        u8_low_byte = a2 - '0';
    }
    else if ((a2 >= 'A') && (a2 <= 'F'))
    {
        u8_low_byte = a2 - 'A' + 10;
    }
    
    u8_hex = u8_hight_byte * 16 + u8_low_byte;
    return u8_hex;
}

uint32_t hex_to_int(char *hex_string, uint32_t length)
{
    uint32_t result = 0;
    for (int i = 0; i < length; i++)
    {
        result = result << 4;
        if (hex_string[i] >= '0' && hex_string[i] <= '9')
        {
            result |= hex_string[i] - '0';
        }
        else if (hex_string[i] >= 'A' && hex_string[i] <= 'F')
        {
            result |= hex_string[i] - 'A' + 10;
        }
        else if (hex_string[i] >= 'a' && hex_string[i] <= 'f')
        {
            result |= hex_string[i] - 'a' + 10;
        }
    }
    return result;
}


uint16_t Calculate_Check_Sum(uint8_t * srec_line)
{
    uint16_t crc = 0xFFFF;
    uint8_t len = *srec_line;
    uint8_t i;
    uint8_t j;
    for(i = 0; i < len; i++)
    {
        crc ^= srec_line[i];
        for(j = 0; j < 8; j++)
        {
            if(crc & 0x0001)
            {
                crc = (crc >> 1) ^ 0xA001;
            }
            else
            {
                crc = (crc >> 1);
            }
        }
    }
    return crc;
}




