/*
 * Quanser.c
 *
 *  Created on: Jun 22, 2015
 *      Author: Juan
 */
/*
 * LDR.c
 *
 *  Created on: Jun 18, 2015
 *      Author: Juan
 */

#include "LDR.h"
#include "FTM.h"
#include "ADC.h"
#include "GPIO.h"
#include "Quanser.h"

#define VELMAX 2
#define POSITIVO 0
#define NEGATIVO 1

static volatile uint8_t vel_counter;
static volatile uint8_t adc_channels[2] = {0,1};
static volatile uint8_t prom_down = 0;
static volatile uint8_t prom_up = 0;
static volatile uint8_t index_int = 0;
static volatile int16_t integrativo = 0;

uint16_t quanserProporcional = 1;
double quanserVelocidad = 1.5;
uint16_t quanserMagnification = 5;

void QUANSER_init(void){
	ADC_init(ADC0_BASE_PTR);
	FTM_init(FTM2_BASE_PTR);
	FTM_PWMEdgeAligned(FTM2_BASE_PTR, HIGHTRUE);
	FTM_setPeriod(FTM2_BASE_PTR ,0x02FD);			//poner frecuencia 50Hz
	FTM_setDuty(FTM2_BASE_PTR, 0x0000);	//duty 2-12% corresponden a 0 y 180
	GPIO_init (0);
	GPIO_write(0,1);
	GPIO_purpose(0, GPIOOUT);
	GPIO_init (1);
	GPIO_write(1,0);
	GPIO_purpose(1, GPIOOUT);
	
	//VALUE = D*MOD -> MOD=FREQ 2% -> 0x106; 12% -> 0x2D0 */
}

void QUANSER_PISR(void){
	  static int u = 0, last_u = 0, correction_u = 0, x2=0;
	  static int newvel[2] = {0,0};
	  static int newprom_izq=0, newprom_ri=0;
	  static uint8_t signo_u = POSITIVO;
	  
	  
	  if(ADC_convCompl(ADC0_BASE_PTR)){
	  		newvel[vel_counter] = ADC_result(ADC0_BASE_PTR);
	  		vel_counter++;
	  	}else{
	  		ADC_initConversion(ADC0_BASE_PTR, adc_channels[vel_counter%VELMAX]);
	  }
	  if((vel_counter == VELMAX)){
		  vel_counter = 0;
		  newprom_izq= (LDR_returnValue(2)+LDR_returnValue(3))/2;
		  newprom_ri= (LDR_returnValue(0)+LDR_returnValue(1))/2;
		  if(newvel[1]>5)
			 newvel[0] = 0;
		    
		  if(newvel[1] < 10)
			  newvel[1] = 0;  
		  if(newvel[0] < 10)
			  newvel[0] = 0;  
		  x2 = abs(newvel[1]-newvel[0]);
		  if(x2<50)
			  x2 = 0;
		  quanserProporcional = getConstValue(3);
		  quanserVelocidad = getConstValue(4);
		  quanserMagnification = getConstValue(2);
		  u = quanserProporcional * abs(newprom_izq-newprom_ri) - quanserVelocidad*x2;
		  /*
		  if(abs(u)>5 && last_u == u)
		  {
			  correction_u += 4;

		      if(u < 0)
		      {
		        if(signo_u == POSITIVO)
		        {
		          signo_u = NEGATIVO;
		          correction_u = 0;
		        }
		        u -= correction_u;
		      }
		      else
		      {
		        if( signo_u == NEGATIVO)
		        {
		          signo_u = POSITIVO;
		          correction_u = 0;
		        }
		        u += correction_u;
		      }
		    }
		    
		    
		    last_u = u;
		    */
		  
		  
		  if(u>20){
			  if(LDR_getServo() < 0x220){	//Si paso la mitad, gira en el otro sentido
				  if(newprom_izq < newprom_ri){
					GPIO_write(0, 1);
					GPIO_write(1, 0);
					FTM_setDuty(FTM2_BASE_PTR, quanserMagnification*u);
				  }
				  else{
					GPIO_write(0, 0);
					GPIO_write(1, 1);
					FTM_setDuty(FTM2_BASE_PTR, quanserMagnification*u);
				  }
			  }
			  else{
				  if(newprom_izq > newprom_ri){
					GPIO_write(0, 1);
					GPIO_write(1, 0);
					FTM_setDuty(FTM2_BASE_PTR, quanserMagnification*u);
				  }
				  else{
					GPIO_write(0, 0);
					GPIO_write(1, 1);
					FTM_setDuty(FTM2_BASE_PTR, quanserMagnification*u);
				  }
			  }
		  }
		  else{
			  FTM_setDuty(FTM2_BASE_PTR, 0);
		  }
	  }
}


