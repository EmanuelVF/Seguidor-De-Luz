/*
 * GPIO.c
 *
 *  Created on: Jun 7, 2015
 *      Author: Juan
 */

#include "GPIO.h"

//Tabla de puertos
static volatile uint32_t MYPORT_BASE[2] = {PORTC_BASE_PTR,PORTC_BASE_PTR};
static volatile uint32_t MYPORT_NUM[2] = {2,3};
static volatile uint32_t MYPORT_REG[2] = {PTC_BASE_PTR, PTC_BASE_PTR};


void GPIO_init (uint8_t port)
{
	PORT_PCR_REG((PORT_MemMapPtr)MYPORT_BASE[port],MYPORT_NUM[port]) = PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x01);
}

void GPIO_write (uint8_t port, bool value)
{
	if (value)
		GPIO_PDOR_REG((GPIO_MemMapPtr)MYPORT_REG[port]) |=  GPIO_PDOR_PDO(1<<MYPORT_NUM[port]);
	else
		GPIO_PDOR_REG((GPIO_MemMapPtr)MYPORT_REG[port]) &=  ~GPIO_PDOR_PDO(1<<MYPORT_NUM[port]);
}

void GPIO_set(uint8_t port)
{
	GPIO_PSOR_REG((GPIO_MemMapPtr)MYPORT_REG[port]) |=  GPIO_PSOR_PTSO(1<<MYPORT_NUM[port]);
}

void GPIO_clear(uint8_t port)
{
	GPIO_PCOR_REG((GPIO_MemMapPtr)MYPORT_REG[port]) |=  GPIO_PCOR_PTCO(1<<MYPORT_NUM[port]);
}

void GPIO_toggle (uint8_t port)
{
	GPIO_PTOR_REG((GPIO_MemMapPtr)MYPORT_REG[port]) |=  GPIO_PTOR_PTTO(1<<MYPORT_NUM[port]);
}

void GPIO_purpose(uint8_t port, bool io)
{
	if(io == GPIOINPUT)
		GPIO_PDDR_REG((GPIO_MemMapPtr)MYPORT_REG[port]) &=  ~GPIO_PDDR_PDD(1<<MYPORT_NUM[port]);
	else
		GPIO_PDDR_REG((GPIO_MemMapPtr)MYPORT_REG[port]) |=  GPIO_PDDR_PDD(1<<MYPORT_NUM[port]);
}
