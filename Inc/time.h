/*
 * time.h
 *
 *  Created on: 2019. 11. 13.
 *      Author: issacs
 */

#ifndef TIME_H_
#define TIME_H_

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif



  void time_setup();
  volatile uint32_t time_ms();
  void time_SysTick_Handler();


#ifdef	__cplusplus
}
#endif

#endif /* TIME_H_ */
