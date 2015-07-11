#include "App.h"
#include "UART.h"
#include "ADC.h"
#include "GPIO.h"
#include "SysTick.h"
#include "LDR.h"
#include "Quanser.h"


uint16_t getConstValue(uint8_t constNum);
uint16_t getConstValue4(uint8_t constNum);

uint8_t updateConstants;
uint16_t k1;
uint16_t k2;
uint16_t k3;
uint16_t k4;

void App_init (void)
{
	k1 = 20;
	k2 = 5;
	k3 = 1;
	k4 = 1.5;
	SysTick_init();
	LDR_init();
	QUANSER_init();
	UART_init();
}

void App_run (void)
{
	uint8_t dataBufferL[30];	
	uint8_t idx = 0;
	uint16_t servoPos = 0;
	
	
	
	for(;;){
		
		
		if(UART_isDataAvailable()){
			UART_readDataBuffer(dataBufferL);
			if(dataBufferL[0] == 11) {
				
				for(idx=0; idx<4; idx++) {
					UART_writeByte(LDR_returnValue(idx));
				}	
				
				servoPos = LDR_getServo();
				
				for(idx = 0; idx<3; idx++) {
					UART_writeByte(servoPos%10);
					servoPos = servoPos/10;
				}
				
			} else {
				
				k1 = dataBufferL[0]-'0';// servoProporcional 
				k2 = dataBufferL[1]-'0';	// quanserMagnification
				k3 = dataBufferL[2]-'0';	// QuanserProporcional
				k4 = (dataBufferL[3] -'0') / 10;	//QuanserVelocidad
			}
			
		}
		
	}
}

uint16_t getConstValue(uint8_t constNum) {
	uint16_t constToReturn;
	
	switch(constNum) {
	case 1:
		constToReturn = k1;
		break;
	case 2:
		constToReturn = k2;
		break;
	case 3:
		constToReturn = k3;
		break;
	case 4:
		constToReturn = k4;
		break;	
	}
	
	return constToReturn;
	
}

uint16_t getConstValue4(uint8_t constNum) {

	return k4;
	
}


