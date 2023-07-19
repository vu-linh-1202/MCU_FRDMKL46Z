#ifndef _Srecord_H_
#define _Srecord_H_

__ramfunc uint8_t Convert2ChartoHex(uint8_t c1, uint8_t c2);
uint32_t hex_to_int(char *hex_string, uint32_t length);
uint16_t Calculate_Check_Sum(uint8_t * srec_line);
#endif