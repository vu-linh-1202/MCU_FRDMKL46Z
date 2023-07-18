#ifndef _Srecord_H_
#define _Srecord_H_

__ramfunc int Convert2ChartoHex(char a1, char a2);
uint32_t hex_to_int(char *hex_string, uint32_t length);
uint16_t Calculate_Check_Sum(uint8_t * srec_line);
#endif