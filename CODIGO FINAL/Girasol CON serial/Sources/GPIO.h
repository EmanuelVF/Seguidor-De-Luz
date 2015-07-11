/*
 * GPIO.h
 *
 *  Created on: Jun 7, 2015
 *      Author: Juan
 */

#ifndef GPIO_H_
#define GPIO_H_

/*
 * GPIO.c
 *
 *  Created on: Jun 7, 2015
 *      Author: Juan
 */

#include "GPIO.h"
#include "misc.h"

#define GPIOINPUT 0
#define GPIOOUT 1

void GPIO_init (uint8_t port);
void GPIO_write (uint8_t port, bool value);
void GPIO_set (uint8_t port);
void GPIO_clear (uint8_t port);
void GPIO_toggle (uint8_t port);
void GPIO_untoggle (uint8_t port);
void GPIO_purpose(uint8_t port, bool io);




#endif /* GPIO_H_ */
