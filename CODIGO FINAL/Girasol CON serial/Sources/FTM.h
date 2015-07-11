/*
 * FTM.h
 *
 *  Created on: Nov 2, 2014
 *      Author: JuanPablo
 */

#ifndef FTM_H_
#define FTM_H_

#include "misc.h"

#define RISINGEDGE		0x00
#define FALLINGEDGE		0x01
#define ANYEDGE			0x02

#define TOGGLEOUTPUT	0x00
#define CLEAROUTPUT		0x01
#define SETOUTPUT		0x02

#define HIGHTRUE		0x00
#define LOWTRUE			0x01

/*
 * FTM.c
 *
 *  Created on: Nov 2, 2014
 *      Author: JuanPablo
 */
#include "FTM.h"

void FTM_init  (FTM_MemMapPtr base);
void FTM_PWMCenterAligned(FTM_MemMapPtr base, bool outputTrue);
void FTM_PWMEdgeAligned(FTM_MemMapPtr base, bool outputTrue);
void FTM_setPeriod(FTM_MemMapPtr base, unsigned int period);
void FTM_setDuty(FTM_MemMapPtr base, unsigned int duty);
void FTM_setPrescaler(FTM_MemMapPtr base, bool prescaler);
void FTM_enable(FTM_MemMapPtr base);
void FTM_disable(FTM_MemMapPtr base);
ISR_t FTM_IRQHandler(void);
#endif /* FTM_H_ */
