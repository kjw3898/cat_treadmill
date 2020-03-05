/**
  ******************************************************************************
  * File Name          : power.c
  * Description        : This file provides code for the configuration
  *                      of the power instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "power.h"
#include <stdbool.h>
#include "main.h"
uint8_t bat_val = 0;
uint8_t power_mode = POWR_PERFORMANCE; //POWR_SAVE, POWR_SAVE, POWR_PERFORMANCE

void led_power_off(void)
{
  HAL_GPIO_WritePin(LED_3V3_PWR_EN_GPIO_Port, LED_3V3_PWR_EN_Pin, GPIO_PIN_RESET); // LED_3V3_PWR_nEN, High Enable, Low Disable
}



void led_power_on(void)
{
  HAL_GPIO_WritePin(LED_3V3_PWR_EN_GPIO_Port, LED_3V3_PWR_EN_Pin, GPIO_PIN_SET); // LED_3V3_PWR_nEN, High Enable, Low Disable, Device : U3
}

/* power Enable */
void power_en(void)
{
  // power Controler
  led_power_on(); 
}

/* power Disable */
void power_dis(void)
{
  // power Controler
  led_power_off();
  HAL_GPIO_WritePin(MCU_RUN_LED_GPIO_Port, MCU_RUN_LED_Pin, GPIO_PIN_SET); // led off
}

uint8_t get_power_mode(void)
{
  return power_mode;
}

void set_power_mode(uint8_t mode)
{
  power_mode = mode;
}


void set_wakeup(void) 
{
  ble_enable();
  power_en();
  running_mode = STAT_RUNNING;
  HAL_TIM_Base_Start_IT(&htim11);
}

void set_sleep(void) 
{
  power_dis();
  running_mode = STAT_SLEEP;
  HAL_TIM_Base_Stop_IT(&htim11);
}
uint8_t get_running_mode(void)
{
	return running_mode;
}
