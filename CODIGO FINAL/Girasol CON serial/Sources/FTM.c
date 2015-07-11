/*
 * FTM.c
 *
 *  Created on: Nov 2, 2014
 *      Author: JuanPablo
 */
#include "FTM.h"

void FTM_init  (FTM_MemMapPtr base)
{
	// Habilito el pin PTC1 como salida del canal 0
	PORTC_PCR1 = (PORT_PCR_ISF_MASK |
	              PORT_PCR_MUX(0x04));
	PORTB_PCR18 = (PORT_PCR_ISF_MASK |
		              PORT_PCR_MUX(0x03));
	
	// Habilito la llave general de las interrupciones
	//NVICISER1 |= NVIC_ISER_SETENA(0x0400);
	
	// Habilito el clock gating
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK;
	
	// Deshabilito la proteccion de escritura
	FTM_MODE_REG(base) 		= FTM_MODE_WPDIS_MASK;
	
	// Apago los flag de interrupciones
	FTM_STATUS_REG(base) 	= (FTM0_STATUS &
				       0x00);
	
	// Deshabilito los clocks y borro la configuracion
	FTM_SC_REG(base) 		= 0x00;
	
	// Configuro el canal 0 como nada
	FTM_CnSC_REG(base,0) 		= 0x00;
	
	// Configuro la salida inicial en alto (los LEDs son activo-bajo)
	FTM_MODE_REG(base) 		= FTM_MODE_INIT_MASK;
	
	// Configuro el PWM a duty 25% y reinicio el contador
	FTM_CnV_REG(base,0) 		= 0x0000;
	FTM_MOD_REG(base) 	    = 0x0000;
	FTM_CNTIN_REG(base) 		= 0x0000;
	FTM_CNT_REG(base) 		= 0x0000;
	
	FTM_PWMLOAD_REG(base) 	= (FTM_PWMLOAD_LDOK_MASK | 
					   FTM_PWMLOAD_CH0SEL_MASK);
	
	// Habilito las interrupciones y los clocks, y configuro el prescaler
	FTM_SC_REG(base) 		= (FTM_SC_TOIE_MASK |
			   		   FTM_SC_CLKS(0x01) |
			           FTM_SC_PS(0x0F));
	// Habilito los contadores del modulo
	FTM_CONF_REG(base) 		= FTM_CONF_BDMMODE(0x03);
}

void FTM_PWMCenterAligned(FTM_MemMapPtr base, bool outputTrue){
	FTM_CnSC_REG(base,0) 	&=	~FTM_CnSC_MSA_MASK;
	FTM_CnSC_REG(base,0) 	|=	FTM_CnSC_MSB_MASK;
	FTM_SC_REG(base) |= FTM_SC_CPWMS_MASK;
	FTM_COMBINE_REG(base) &=	~FTM_COMBINE_COMBINE0_MASK;
	FTM_COMBINE_REG(base) &=	~FTM_COMBINE_DECAPEN0_MASK;
	
	if(outputTrue == HIGHTRUE){
		FTM_CnSC_REG(base,0) 	|=	FTM_CnSC_ELSB_MASK;
		FTM_CnSC_REG(base,0) 	&=	~FTM_CnSC_ELSA_MASK;
	}
	else{
		FTM_CnSC_REG(base,0) 	|=	FTM_CnSC_ELSA_MASK;
		FTM_CnSC_REG(base,0) 	&=	~FTM_CnSC_ELSB_MASK;
	}
}

void FTM_PWMEdgeAligned(FTM_MemMapPtr base, bool outputTrue){
	FTM_setPeriod(base, 0x0000);
	FTM_setDuty(base, 0x0000);
	FTM_CnSC_REG(base,0) 	&=	~FTM_CnSC_MSA_MASK;
	FTM_CnSC_REG(base,0) 	|=	FTM_CnSC_MSB_MASK;
	FTM_SC_REG(base) &=	~FTM_SC_CPWMS_MASK;
	FTM_COMBINE_REG(base) &=	~FTM_COMBINE_COMBINE0_MASK;
	FTM_COMBINE_REG(base) &=	~FTM_COMBINE_DECAPEN0_MASK;
	if(outputTrue == HIGHTRUE){
		FTM_CnSC_REG(base,0) 	|=	FTM_CnSC_ELSB_MASK;
		FTM_CnSC_REG(base,0) 	&=	~FTM_CnSC_ELSA_MASK;
	}
	else{
		FTM_CnSC_REG(base,0) 	|=	FTM_CnSC_ELSA_MASK;
		FTM_CnSC_REG(base,0) 	&=	~FTM_CnSC_ELSB_MASK;
	}
	
	FTM_SC_REG(base) &=	~FTM_SC_CPWMS_MASK;
}

void FTM_setPeriod(FTM_MemMapPtr base, unsigned int period){
	FTM_MOD_REG(base) = period-1;
}

void FTM_setDuty(FTM_MemMapPtr base, unsigned int duty){
	FTM_CnV_REG(base,0) = duty;
}

void FTM_setPrescaler(FTM_MemMapPtr base, bool prescaler){
	FTM_SC_REG(base) |= FTM_SC_PS(prescaler);
}

void FTM_enable(FTM_MemMapPtr base){
	if(base == FTM0_BASE_PTR)
		SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	else
		SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK;
}

void FTM_disable(FTM_MemMapPtr base){
	if(base == FTM0_BASE_PTR)
		SIM_SCGC6 &= ~SIM_SCGC6_FTM0_MASK;
	else
		SIM_SCGC6 &= ~SIM_SCGC6_FTM2_MASK;
}

ISR_t FTM_IRQHandler(void){

}

