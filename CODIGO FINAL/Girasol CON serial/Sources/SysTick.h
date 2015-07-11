
#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "misc.h"

ISR_t SysTick_Handler (void);

void SysTick_init (void);

#endif
