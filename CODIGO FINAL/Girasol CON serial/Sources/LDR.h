/*
 * LDR.h
 *
 *  Created on: Jun 18, 2015
 *      Author: Juan
 */

#ifndef LDR_H_
#define LDR_H_

/*
 * LDR.c
 *
 *  Created on: Jun 18, 2015
 *      Author: Juan
 */
#include "misc.h"

#define LDRMAX 4

void LDR_init(void);
void LDR_PISR(void);
uint8_t LDR_returnValue(uint8_t pos);
uint16_t LDR_getServo(void);

#endif /* LDR_H_ */
