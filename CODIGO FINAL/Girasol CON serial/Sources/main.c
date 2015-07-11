/*
 * main.c
 *
 *  Created on: Jun 1, 2014
 *      Author: Juan Pablo
 */

#include "LDM.h"
#include "App.h"

int main(void){
	__LDM_init();
	
	__DI();
	
	/**************************/
	/* Program-specific setup */
	/**************************/
	App_init();               //
	/**************************/
	
	__EI();
	
	/**************************/
	/* Program-specific loop  */
	/**************************/
	App_run();                //
	/**************************/

	FOREVER;
}
