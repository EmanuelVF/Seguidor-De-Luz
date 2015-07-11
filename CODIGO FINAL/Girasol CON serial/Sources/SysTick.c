
#include "SysTick.h"
#include "ADC.h"
#include "LDR.h"
#include "Quanser.h"

ISR_t SysTick_Handler(void)
{
	//if(ADC_convCompl())
	//ADC_PISR();
	
	QUANSER_PISR();
	LDR_PISR();
	
}

void SysTick_init(void) 
{
  SYST_CSR = 0x00;
  SYST_RVR = SysTick_RVR_RELOAD(274999U); // 0.5ms 
  SYST_CVR = SysTick_CVR_CURRENT(0);
  SYST_CSR = SysTick_CSR_CLKSOURCE_MASK |
		     SysTick_CSR_TICKINT_MASK |
		     SysTick_CSR_ENABLE_MASK;
}
