/*
 * UART.h
 *
 *  Created on: Nov 8, 2014
 *      Author: JuanPablo
 */

#ifndef UART_H_
#define UART_H_
#include "misc.h"

//#define BAUDRATELOW 0x8B
//#define BAUDRATEHIGH 0x02
#define BAUDRATELOW 0x31
#define BAUDRATEHIGH 0x00
#define BAUDRATEFINEADJUST 0x00
//Poner 108 y 11
void UART_writeByte(uint8_t data);
void UART_init(void);
uint8_t UART_isDataAvailable(void);
void UART_readDataBuffer(uint8_t *dataStore);
#endif /* UART_H_ */
