/*
 * ADC.h
 *
 *  Created on: Nov 4, 2014
 *      Author: JuanPablo
 */

#ifndef ADC_H_
#define ADC_H_

#include "misc.h"

void ADC_init(ADC_MemMapPtr base);
void ADC_calib (ADC_MemMapPtr base);
void ADC_enableAverage(ADC_MemMapPtr base, uint8_t avge);
void ADC_disableAverage(ADC_MemMapPtr base);
void ADC_initConversion(ADC_MemMapPtr base, uint8_t channel);
uint8_t ADC_convCompl(ADC_MemMapPtr base);
uint8_t ADC_result(ADC_MemMapPtr base);
void ADC_PISR(void);
ISR_t ADC1_IRQHandler(void);
#endif /* ADC_H_ */
