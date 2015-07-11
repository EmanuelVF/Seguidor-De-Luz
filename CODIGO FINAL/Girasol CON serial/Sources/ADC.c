/*
 * ADC.c
 *
 *  Created on: Nov 4, 2014
 *      Author: JuanPablo
 */
#include "ADC.h"

/***
 ADCPRESCALER :
 	 	 	 	 00 prescaler = 1
 	 	 	 	 01 prescaler = 2
 	 	 	 	 10 prescaler = 4
 	 	 	 	 11 prescaler = 8
 	 	 	 	 
MODE:
		00 DIFF 0	8bit
		00 DIFF 1	9bit diff complento a 2
		01 DIFF 0	12bit
		01 DIFF 1	13bit diff complento a 2
		10 DIFF 0	10bit
		10 DIFF 1	11bit diff complento a 2
		11 DIFF 0	16bit
		11 DIFF 1	16bit diff complento a 2
		
ADICLK:
		00	Bus Clock
		01	Alternate clock 2
		10	Alternate clock
		11	Asynchronous clock
		
AVGS:
		00 4 samples averaged.
		01 8 samples averaged.
		10 16 samples averaged.
		11 32 samples averaged.

ADCH:
		00000 When DIFF=0, DADP0 is selected as input; when DIFF=1, DAD0 is selected as input.
		00001 When DIFF=0, DADP1 is selected as input; when DIFF=1, DAD1 is selected as input.
		00010 When DIFF=0, DADP2 is selected as input; when DIFF=1, DAD2 is selected as input.
		00011 When DIFF=0, DADP3 is selected as input; when DIFF=1, DAD3 is selected as input.
		00100 When DIFF=0, AD4 is selected as input; when DIFF=1, it is reserved.
		00101 When DIFF=0, AD5 is selected as input; when DIFF=1, it is reserved.
		00110 When DIFF=0, AD6 is selected as input; when DIFF=1, it is reserved.
		00111 When DIFF=0, AD7 is selected as input; when DIFF=1, it is reserved.
		01000 When DIFF=0, AD8 is selected as input; when DIFF=1, it is reserved.
		01001 When DIFF=0, AD9 is selected as input; when DIFF=1, it is reserved.
		01010 When DIFF=0, AD10 is selected as input; when DIFF=1, it is reserved.
		01011 When DIFF=0, AD11 is selected as input; when DIFF=1, it is reserved.
		01100 When DIFF=0, AD12 is selected as input; when DIFF=1, it is reserved.
		01101 When DIFF=0, AD13 is selected as input; when DIFF=1, it is reserved.
		01110 When DIFF=0, AD14 is selected as input; when DIFF=1, it is reserved.
		01111 When DIFF=0, AD15 is selected as input; when DIFF=1, it is reserved.
		10000 When DIFF=0, AD16 is selected as input; when DIFF=1, it is reserved.
		10001 When DIFF=0, AD17 is selected as input; when DIFF=1, it is reserved.
		10010 When DIFF=0, AD18 is selected as input; when DIFF=1, it is reserved.
		10011 When DIFF=0, AD19 is selected as input; when DIFF=1, it is reserved.
		10100 When DIFF=0, AD20 is selected as input; when DIFF=1, it is reserved.
		10101 When DIFF=0, AD21 is selected as input; when DIFF=1, it is reserved.
		10110 When DIFF=0, AD22 is selected as input; when DIFF=1, it is reserved.
		10111 When DIFF=0, AD23 is selected as input; when DIFF=1, it is reserved.
		11000 Reserved.
		11001 Reserved.
		11010 When DIFF=0, Temp Sensor (single-ended) is selected as input; when DIFF=1, Temp Sensor
			 (differential) is selected as input.
		11011 When DIFF=0,Bandgap (single-ended) is selected as input; when DIFF=1, Bandgap (differential)
			  is selected as input.
		11100 Reserved.
		11101 When DIFF=0,VREFSH is selected as input; when DIFF=1, -VREFSH (differential) is selected as
			  input. Voltage reference selected is determined by SC2[REFSEL].
		11110 When DIFF=0,VREFSL is selected as input; when DIFF=1, it is reserved. Voltage reference
			  selected is determined by SC2[REFSEL].
		11111 Module is disabled.

CONVERSION VALUE: ADC1_R(index)
DRIVER STATUS:	(ADC_SC2 & ADC_SC2_ADACT_MASK) --> "0" WHEN Conversion not in progress, "1" WHEN Conversion in progress;
CALIBRATION STATUS: (ADC_SC3 & ADC_SC3_CALF_MASK) --> "0" WHEN Completed normally, "1" WHEN Calibration failed;
		
 */
#define ADCPRESCALER 00
#define ADCMODE 00
#define ADCCLOCK 00
#define ADCCHANNEL 00001

static volatile uint8_t adcChannel = 0x00;
static volatile uint8_t convCompl0 = 0;
static volatile uint8_t convCompl1 = 0;
static volatile uint8_t measADC0 = 0;
static volatile uint8_t measADC1 = 0;

void ADC_init(ADC_MemMapPtr base){
	// Habilito el pin PTB0 como simple ADC_SE8
	
	// Habilito la llave general de las interrupciones
	
	//Habilito el modulo
	if(base == ADC1_BASE_PTR){
		SIM_SCGC3 |= SIM_SCGC3_ADC1_MASK;
		NVICISER2 |= NVIC_ISER_SETENA(0x0200);
	}
	else{
		SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
		NVICISER1 |= NVIC_ISER_SETENA(0x80);
	}
	//Configuro el clock y el funcionamiento
	 
	ADC_CFG1_REG(base) = ADC_CFG1_ADLPC_MASK | ADC_CFG1_ADIV(ADCPRESCALER) | ADC_CFG1_MODE(ADCMODE) | ADC_CFG1_ADICLK(ADCCLOCK);
	//Seteo modo high speed convertion
	ADC_CFG2_REG(base) = ADC_CFG2_ADHSC_MASK;
	ADC_SC2_REG(base) = 0x00;
	ADC_SC1_REG(base,0) = ADC_SC1_ADCH(0x00);
	//
}

void ADC_calib (ADC_MemMapPtr base)
{
	uint16_t calib = 0x00;
	calib = (ADC_CLP0_REG(base) + ADC_CLP1_REG(base) + ADC_CLP2_REG(base) + ADC_CLP3_REG(base) + ADC_CLP4_REG(base) + ADC_CLPS_REG(base))/2;
	calib |= 1<<15;
	
	
	calib = 0x00;
	calib = (ADC_CLM0_REG(base) + ADC_CLM1_REG(base) + ADC_CLM2_REG(base) + ADC_CLM3_REG(base) + ADC_CLM4_REG(base) + ADC_CLMS_REG(base))/2;
	calib |= 1<<15;
	
	//Begins Calibration
	ADC_SC3_REG(base) |= ADC_MG_MG(calib) | ADC_PG_PG(calib);
	
}

void ADC_enableAverage(ADC_MemMapPtr base, uint8_t avge){
	ADC_SC3_REG(base) = ADC_SC3_AVGS(avge) | ADC_SC3_AVGE_MASK;
}

void ADC_disableAverage(ADC_MemMapPtr base){
	ADC_SC3_REG(base) &= ~ADC_SC3_AVGE_MASK;
}

void ADC_initConversion(ADC_MemMapPtr base, uint8_t channel){
	//Init conversion
	ADC_SC1_REG(base,0) = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(channel);
	if(base == ADC0_BASE_PTR)
		convCompl0 = 0;
	else
		convCompl1 = 0;
}

uint8_t ADC_convCompl(ADC_MemMapPtr base){
	if(base == ADC0_BASE_PTR)
		return convCompl0;
	else
		return convCompl1;
}

uint8_t ADC_result(ADC_MemMapPtr base){
	if(base == ADC0_BASE_PTR){
		convCompl0 = 0;
		return measADC0;
	}
	else{
		convCompl1 = 0;
		return measADC1;
	}
}

void ADC_PISR(void){
	//adcChannel = !adcChannel;	//Uso los canales 0 y 1
	//ADC_initConversion(ADC1_BASE_PTR, adcChannel);
}

ISR_t ADC1_IRQHandler(void){
	//Interruption is cleared upon reading of Rn register or SC1n is written
	//ADC_CompleteInterrupt();
	measADC1 = ADC1_RA;
	convCompl1 = 1;
}

ISR_t ADC0_IRQHandler(void){
	//Interruption is cleared upon reading of Rn register or SC1n is written
	//ADC_CompleteInterrupt();
	measADC0 = ADC0_RA;
	convCompl0 = 1;
}

