/*
 * ArduinoWrapper.h
 *
 *  Created on: Mar 27, 2013
 *      Author: zoellner
 */

#ifndef ARDUINOWRAPPER_H_
#define ARDUINOWRAPPER_H_

#define MAX(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a > _b ? _a : _b; })

#define MIN(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a < _b ? _a : _b; })

//Standard Libraries
#include <stm32f4xx_hal.h>

//TODO functions that need wrapper: millis(), Serial.print

#define millis() HAL_GetTick()
#define delay(x) HAL_Delay(x)
#define I2CDEV_DEFAULT_WRITE_TIMEOUT     100

#endif /* ARDUINOWRAPPER_H_ */
