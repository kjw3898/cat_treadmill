/**
  ******************************************************************************
  * File Name          : MDBT42Q-AT.c
  * Description        : This file provides code for the configuration
  *                      of the MDBT42Q-AT instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mdbt42q.h"
#include "usart.h"

void ble_module_init(void)
{
  int8_t data[20];
  char* p;
  memset(data, 0, sizeof(data));
  
  /* dummy command, You need to pass this command once to get At command data. */
  HAL_UART_Transmit(&huart2, (uint8_t *)"AT?NAME\r\n", sizeof("AT?NAME\r\n"), 1000);
  HAL_Delay(50);

  HAL_UART_Transmit(&huart2, (uint8_t *)"AT?NAME\r\n", sizeof("AT?NAME\r\n"), 1000);
  HAL_Delay(50);
  HAL_UART_Receive(&huart2 , (uint8_t *)&data, sizeof(data) , 1000);


  HAL_UART_Receive(&huart2 , (uint8_t *)&data, sizeof(data) , 1000);
  p = strstr((char*)data, "B612");
  if (p == NULL) 
  {
    HAL_UART_Transmit(&huart2, (uint8_t *)"AT+NAMEthe Little Cat-B612\r\n", sizeof("AT+NAMEthe Little Cat-B612\r\n"), 1000);
    HAL_Delay(100);
    HAL_UART_Transmit(&huart2, (uint8_t *)"AT+FLOWCONTROLEN\r\n", sizeof("AT+FLOWCONTROLEN\r\n"), 1000);
    HAL_Delay(100);
    HAL_UART_Transmit(&huart2, (uint8_t *)"AT+RESET\r\n", sizeof("AT+RESET\r\n"), 1000);
    HAL_Delay(100);
  }
  ble_enable();
}

void ble_disable(void)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)"AT+SLEEP\r\n", sizeof("AT+SLEEP\r\n"), 1000);
  //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET); // BLE_RESET Low : Disable
  //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); // BLE_FLASHED_DEFAULT Low
  //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); // BLE_UART2_PD(POWER DOWN) High : Power Down
  //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET); // BLE_WAKEUP Low : Disable
  //HAL_UART_Transmit(&huart2, (uint8_t *)"AT+SLEEP\r\n", sizeof("AT+SLEEP\r\n"), 1000);
  //HAL_Delay(100);
}

void ble_enable(void)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET); // BLE_RESET High
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); // BLE_FLASHED_DEFAULT Low
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); // BLE_UART2_PD(POWER DOWN) Low : Power Up
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET); // BLE_WAKEUP High
}
