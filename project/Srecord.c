#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Srecord.h"

__ramfunc uint8_t Convert2ChartoHex(uint8_t c1, uint8_t c2)
{
    uint8_t c;

    if ((c1 >= '0') && (c1 <= '9')) {
        c = c1 - '0';
    } else {
        c = c1 - 'A' + 10;
    }

    c <<= 4;

    if ((c2 >= '0') && (c2 <= '9')) {
        c |= c2 - '0';
    } else {
        c |= c2 - 'A' + 10;
    }

    return c;
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




