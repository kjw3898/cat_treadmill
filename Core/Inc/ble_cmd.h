/**
  ******************************************************************************
  * File Name          : power.h
  * Description        : This file provides code for the configuration
  *                      of the power instances.
  ******************************************************************************
 */



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BLE_CMD_H
#define __BLE_CMD_H
#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>

#define LED_NORMAL 0x01
#define LED_RANDOM 0x02

uint8_t get_status(void);
uint16_t get_degree(void);
uint32_t get_run_time(void);


extern uint8_t running_mode;
extern bool led_rand_mode;

#ifdef __cplusplus
}
#endif
#endif /*__ BLE_CMD_H */
