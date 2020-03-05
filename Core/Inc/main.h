/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern uint8_t running_mode;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim11;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MPU6050_INT1_X_Pin GPIO_PIN_13
#define MPU6050_INT1_X_GPIO_Port GPIOC
#define MPU6050_INT1_X_EXTI_IRQn EXTI15_10_IRQn
#define BLE_WAKE_UP_Pin GPIO_PIN_0
#define BLE_WAKE_UP_GPIO_Port GPIOC
#define BLE_UART2_PD_Pin GPIO_PIN_1
#define BLE_UART2_PD_GPIO_Port GPIOC
#define BLE_RESET_Pin GPIO_PIN_2
#define BLE_RESET_GPIO_Port GPIOC
#define BLE_FLASHED_DEFAULT_Pin GPIO_PIN_3
#define BLE_FLASHED_DEFAULT_GPIO_Port GPIOC
#define BLE_UART2_CTS_Pin GPIO_PIN_0
#define BLE_UART2_CTS_GPIO_Port GPIOA
#define BLE_UART2_RTS_Pin GPIO_PIN_1
#define BLE_UART2_RTS_GPIO_Port GPIOA
#define BLE_UART2_TX_Pin GPIO_PIN_2
#define BLE_UART2_TX_GPIO_Port GPIOA
#define BLE_UART2_RX_Pin GPIO_PIN_3
#define BLE_UART2_RX_GPIO_Port GPIOA
#define DEBUG_TX_Pin GPIO_PIN_9
#define DEBUG_TX_GPIO_Port GPIOA
#define DEBUG_RX_Pin GPIO_PIN_10
#define DEBUG_RX_GPIO_Port GPIOA
#define MCU_RUN_LED_Pin GPIO_PIN_11
#define MCU_RUN_LED_GPIO_Port GPIOA
#define LED_3V3_PWR_EN_Pin GPIO_PIN_10
#define LED_3V3_PWR_EN_GPIO_Port GPIOC
#define BATT_ALRT_INT_Pin GPIO_PIN_12
#define BATT_ALRT_INT_GPIO_Port GPIOC
#define BATT_ALRT_INT_EXTI_IRQn EXTI15_10_IRQn
#define MPU6050_SCL1_Pin GPIO_PIN_8
#define MPU6050_SCL1_GPIO_Port GPIOB
#define MPU6050_SDA1_Pin GPIO_PIN_9
#define MPU6050_SDA1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define STAT_SLEEP 0x01
#define STAT_RUNNING 0x02
#define LED_TOTAL	212
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
