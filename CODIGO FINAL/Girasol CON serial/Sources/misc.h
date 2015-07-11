/*
 * MISC.h
 *
 *  Created on: Jun 1, 2014
 *      Author: Juan Pablo
 */

#ifndef MISC_H_
#define MISC_H_

#include "derivative.h"

typedef unsigned char           bool;
typedef signed char             int8_t;
typedef signed short int        int16_t;
typedef signed long int         int32_t;
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned long int       uint32_t;

#define ISR_t void __attribute__ ((interrupt))

#define FOREVER for(;;)

#define  FALSE  0x00
#define  TRUE   0x01
#define  NULL   0x00

#define __EI() do {__asm("CPSIE f");} while(0)
#define __DI() do {__asm("CPSID f");} while(0)


#endif /* MISC_H_ */
