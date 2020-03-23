/**
  ******************************************************************************
  * File Name          : ble_cmd.c
  * Description        : This file provides code for the configuration
  *                      of the ble_cmd instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ble_cmd.h"
#include "ws2812b.h"
#include "power.h"
#include "usart.h"
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

uint32_t run_time = 0;
bool led_rand_mode = false;

uint8_t get_status(void)
{
  return running_mode;
}

uint16_t get_degree(void)
{
  return accumulate_ledmove;
	//return abs(speedAdjust);
}

uint32_t get_run_time(void)
{
  return run_time;
}

