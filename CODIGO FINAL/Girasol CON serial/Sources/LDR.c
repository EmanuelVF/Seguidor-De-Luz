/*
 * LDR.c
 *
 *  Created on: Jun 18, 2015
 *      Author: Juan
 */

#include "LDR.h"
#include "FTM.h"
#include "ADC.h"

#define ANGLE_0 0x106
#define ANGLE_180 0x2D0
#define ANGLE_WIDTH 0x1CA

static volatile uint8_t ldr[LDRMAX];
static volatile uint8_t ldr_counter;
static volatile uint8_t adc_channels[4] = {0,1,15,18};
static volatile uint8_t prom_down = 0;
static volatile uint8_t prom_up = 0;
static volatile uint8_t initial = 1;
static uint16_t mypos = 0;
uint16_t servoProporcional = 20;

void LDR_init(void){
	ADC_init(ADC1_BASE_PTR);
	FTM_init(FTM0_BASE_PTR);
	FTM_PWMEdgeAligned(FTM0_BASE_PTR, HIGHTRUE);
	FTM_setPeriod(FTM0_BASE_PTR ,0x1999);			//poner frecuencia 50Hz
	FTM_setDuty(FTM0_BASE_PTR, 0x106);	//duty 2-12% corresponden a 0 y 180
	
	//VALUE = D*MOD -> MOD=FREQ 2% -> 0x106; 12% -> 0x2D0 */
}

uint16_t LDR_getServo(void){
	return mypos;
}

uint16_t LDR_remapAngle(uint8_t myposition){
	return (0x106+(2*myposition));
}

uint8_t LDR_returnValue(uint8_t pos){
	return ldr[pos];
}

void LDR_PISR(void){
	static uint16_t pos = 0;
	uint8_t newprom_up = 0;
	uint8_t newprom_down = 0;
	
	servoProporcional = getConstValue(1);
	
	if(ADC_convCompl(ADC1_BASE_PTR)){
		ldr[ldr_counter] = ADC_result(ADC1_BASE_PTR);
		ldr_counter++;
	}else{
		ADC_initConversion(ADC1_BASE_PTR, adc_channels[ldr_counter%LDRMAX]);
	}
	if((ldr_counter == LDRMAX) & initial){
		initial = 0;
		prom_up= (ldr[0]+ldr[3])/2;
		prom_down= (ldr[1]+ldr[2])/2;
	}
	if((ldr_counter == LDRMAX) & !initial){
		ldr_counter = 0;
		newprom_up= (ldr[0]+ldr[3])/2;
		newprom_down= (ldr[1]+ldr[2])/2;
		if(abs(newprom_up-newprom_down)>10){
			if((newprom_up)<(newprom_down)){
					   if((pos+abs(newprom_up-newprom_down)/servoProporcional)<229){
						   pos=pos+abs(newprom_up-newprom_down)/servoProporcional;
						}
					}
			else{
				if((pos-abs(newprom_up-newprom_down)/servoProporcional)>0){
					pos=pos-abs(newprom_up-newprom_down)/servoProporcional;
				}
			}
			mypos = LDR_remapAngle(pos);
			FTM_setDuty(FTM0_BASE_PTR,mypos);
		}
	}
}
