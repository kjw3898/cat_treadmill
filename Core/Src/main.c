/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "power.h"
#include "ws2812b.h"
#include "usart.h"
#include "mpu6050_dmp.h"
#include "mdbt42q.h"
#include "max17043.h"
#include "flash_if.h"
#include "circular_buffer.h"
#include <time.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define F(x) x

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

float Roll_before = 0.0f;
float cal_ledPos_before = 0.0f;

uint32_t ledmove;
uint8_t Stable_state = 0;
uint8_t beforeDirectionMove = 0;
uint8_t tickCounter = 0;
int16_t out_ledPos;
int16_t out_ledPos2;
uint8_t directionMove = 0;
uint8_t running_mode = STAT_SLEEP;
RTC_DateTypeDef rtc_date;
RTC_TimeTypeDef rtc_time;
uint32_t last_moved_tick;
uint32_t first_moved_tick;
uint32_t bat_previous_time;
uint16_t accumulate_ledmove = 0;
uint16_t before_accumulate_ledmove = 0;
time_t timestamp;
dataExercise *exData;
uint32_t now_Tick = 0;
uint32_t beforeTick = 0;
uint32_t gapTick = 0;
int16_t speedAdjustTarget = 0;
int16_t speedAdjustTargetArray[5] = { 0, };
int8_t speedAdjustTargetArrayCounter = 0;
int8_t speedAdjust = 0;
uint8_t ledPos_before;
uint8_t Cal_done = 0;
uint8_t mpuInterrupt = 0;
struct tm currTime;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_RTC_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int fd, char *str, int len) {
	for (int i = 0; i < len; i++) {
		HAL_UART_Transmit(&huart1, (uint8_t*) &str[i], 1, 0xFFFF);
	}
	return len;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_I2C1_Init();
	MX_USART1_UART_Init();
	MX_SPI1_Init();
	MX_RTC_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */

	initLEDMOSI();
	targetLedPos = (uint8_t) ((LED_TOTAL / 360.0f) * roundf(targetAnglel));
	uint8_t temp[1] = { 0 };
	//	 Flag_Show=1;
	uint8_t count = 0;

	while (temp[0] != 0x68) {
		if (HAL_I2C_IsDeviceReady(&hi2c1, 0x68 << 1, 10, 100) == HAL_OK) {
			printf("I2C Address is 0x%02X(7bit value) ready\r\n", 0x68);

		}
		if (count == 10) {

			printf("MPU6050  error\r\n");
			NVIC_SystemReset();

		}
		if (HAL_I2C_Mem_Read(&hi2c1, 0x68 << 1, 0x75, I2C_MEMADD_SIZE_8BIT,
				temp, 1, 100) != HAL_OK) {

			HAL_I2C_ClearBusyFlagErrata_2_14_7(&hi2c1);
			__HAL_RCC_I2C1_FORCE_RESET();
			HAL_Delay(100);
			__HAL_RCC_I2C1_RELEASE_RESET();
			count++;
		} else {
			break;
		}

	}
	HAL_Delay(200);

	DMP_Init();

	set_wakeup();
	//max17043_init();
	ble_module_init();
	uart_recv_int_enable();
	test_led_rgb();
	//FLASH_If_Init();
	initExercise();
	Cal_done = 1;
	printf("Booting LittleCat Board!!!!\r\n\n");
	first_moved_tick = bat_previous_time = last_moved_tick = HAL_GetTick();
	//HAL_TIM_Base_Start_IT(&htim11);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		if (mpuInterrupt) {
			mpuInterrupt = 0;
			while (MPU6050_getFIFOCount() > 42)
				Read_DMP();
			if (Cal_done) {
				out_ledPos = cal_ledPos - targetLedPos - speedAdjust;
				out_ledPos2 = cal_ledPos - targetLedPos;
				if (out_ledPos < 0)
					out_ledPos = LED_TOTAL + out_ledPos;
				if (out_ledPos2 < 0)
					out_ledPos2 = LED_TOTAL + out_ledPos2;
			}
			if (ledPos_before_inLED != out_ledPos
					|| out_ledPos2 != ledPos_before_inLED2)
				led_update();
		}

		process();
		now_Tick = HAL_GetTick();
		gapTick = now_Tick - beforeTick;
		if (gapTick > 124) {
			tickCounter++;
			beforeTick = now_Tick;
			if (ledPos_before != (uint8_t) (out_ledPos)) {

				if (get_running_mode() == STAT_SLEEP) {
					printf("wake up\r\n");
					DMP_Wake();
					set_wakeup();
					first_moved_tick = now_Tick;

				}
				last_moved_tick = now_Tick;
				ledmove = abs(cal_ledPos_before - cal_ledPos);
				directionMove = (cal_ledPos_before - cal_ledPos) < 0;
				if ((beforeDirectionMove == directionMove) && Cal_done)
					accumulate_ledmove += ledmove;
				beforeDirectionMove = directionMove;
				ledPos_before = out_ledPos;
				cal_ledPos_before = cal_ledPos;

			} else if ((now_Tick - last_moved_tick)
					> 5000&& get_running_mode() != STAT_SLEEP) {
				if (accumulate_ledmove > 30) {
					exData->MoveTick = last_moved_tick - first_moved_tick;
					exData->acumulatedLEDMove = accumulate_ledmove;
					amountOfExercise(exData);
					cmd_process(GET_MOVE_DATA, 0x01);
				}

				accumulate_ledmove = 0;
				printf("go to sleep\r\n");

				clear_led();
				set_sleep();

			} else if ((now_Tick - bat_previous_time)
					> 120000&& get_running_mode() == STAT_SLEEP) { // low bat check

			//printf("power check\r\n");
				if (get_bat_val() <= 10) {
					/* Record cat movement information. */
					//////////////////////////////////////
					DMP_Sleep();
					clear_led();
					ble_disable();
					set_sleep();
					HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
					DMP_Off();
					MAX17043_setSleep();
					//HAL_TIM_Base_Stop_IT(&htim11);
					printf("power off\r\n");
					HAL_PWR_EnterSTANDBYMode();          //power off
				}
				bat_previous_time = now_Tick;
			}
			if (rand_led_mode) {
				random_led();
			}
			speedAdjustTargetArray[speedAdjustTargetArrayCounter++] =
					((uint16_t) ((accumulate_ledmove - before_accumulate_ledmove)
							* (125 / (float) gapTick)) >> 1);
			if (speedAdjustTargetArrayCounter > 4) {
				speedAdjustTargetArrayCounter = 0;
			}
			speedAdjustTarget = (speedAdjustTargetArray[0]
					+ speedAdjustTargetArray[1] + speedAdjustTargetArray[2]
					+ speedAdjustTargetArray[3] + speedAdjustTargetArray[4])
					/ 5;
			if (directionMove)
				speedAdjustTarget = -speedAdjustTarget;
			if (speedAdjustTarget > speedAdjust) {
				speedAdjust++;
			} else if (speedAdjustTarget < speedAdjust) {
				speedAdjust--;
			}
			before_accumulate_ledmove = accumulate_ledmove;
		}

		if (tickCounter > 7) {
			if (get_running_mode() == STAT_RUNNING) {

				//printf("get tick= %ld\r\n", now_Tick);
				while (HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN))
					;
				while (HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN))
					;
				//				char msg[22]; //YYYY-MM-DD HH:mm:SS
				//				sprintf(msg, "%04d-%02d-%02d %02d:%02d:%02d\r\n",
				//						rtc_date.Year + 2000, rtc_date.Month, rtc_date.Date,
				//						rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
				currTime.tm_year = rtc_date.Year; // In fact: 2000 + 18 - 1900
				currTime.tm_mday = rtc_date.Date;
				currTime.tm_mon = rtc_date.Month;
				currTime.tm_hour = rtc_time.Hours;
				currTime.tm_min = rtc_time.Minutes;
				currTime.tm_sec = rtc_time.Seconds;

				timestamp = mktime(&currTime);
				//				printf("%ld ", (uint32_t) timestamp);
				//				printf("time=  %s", msg);
				//				printf("ledPos= %d\r\n", out_ledPos);
				//
				printf("ledMoveAccum= %d\r\n", accumulate_ledmove);
				printf("lastMoveTick= %lu\r\n", first_moved_tick);

				//			printf("Bat= %ld \r\n", get_bat_val());
				//			printf("BatV= %ld \r\n", (MAX17043_getVCell()));
				HAL_GPIO_TogglePin(MCU_RUN_LED_GPIO_Port, MCU_RUN_LED_Pin);
			}
			tickCounter = 0;
		}
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE
			| RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 84;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 400000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void) {

	/* USER CODE BEGIN RTC_Init 0 */

	/* USER CODE END RTC_Init 0 */

	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };

	/* USER CODE BEGIN RTC_Init 1 */

	/* USER CODE END RTC_Init 1 */
	/** Initialize RTC Only
	 */
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&hrtc) != HAL_OK) {
		Error_Handler();
	}

	/* USER CODE BEGIN Check_RTC_BKUP */

	/* USER CODE END Check_RTC_BKUP */

	/** Initialize RTC and set the Time and Date
	 */
	sTime.Hours = 0;
	sTime.Minutes = 0;
	sTime.Seconds = 0;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
	sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	sDate.Month = RTC_MONTH_JANUARY;
	sDate.Date = 1;
	sDate.Year = 0;

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN RTC_Init 2 */

	/* USER CODE END RTC_Init 2 */

}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/** 
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA2_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Stream0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
	/* DMA2_Stream3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC,
			BLE_WAKE_UP_Pin | BLE_UART2_PD_Pin | BLE_RESET_Pin
					| BLE_FLASHED_DEFAULT_Pin | LED_3V3_PWR_EN_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(MCU_RUN_LED_GPIO_Port, MCU_RUN_LED_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : MPU6050_INT1_X_Pin BATT_ALRT_INT_Pin */
	GPIO_InitStruct.Pin = MPU6050_INT1_X_Pin | BATT_ALRT_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : BLE_WAKE_UP_Pin BLE_RESET_Pin BLE_FLASHED_DEFAULT_Pin */
	GPIO_InitStruct.Pin = BLE_WAKE_UP_Pin | BLE_RESET_Pin
			| BLE_FLASHED_DEFAULT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : BLE_UART2_PD_Pin LED_3V3_PWR_EN_Pin */
	GPIO_InitStruct.Pin = BLE_UART2_PD_Pin | LED_3V3_PWR_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : MCU_RUN_LED_Pin */
	GPIO_InitStruct.Pin = MCU_RUN_LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(MCU_RUN_LED_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == MPU6050_INT1_X_Pin) {

		mpuInterrupt = 1;

	}
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)

{
//frame error occur by floating You should connect all serial port
	printf("UART ERROR CODE : %lu, UART handler : %X \r\n", huart->ErrorCode,
			(unsigned int) huart->Instance);

}
void HAL_I2C_ClearBusyFlagErrata_2_14_7(I2C_HandleTypeDef *hi2c) {

	static uint8_t resetTried = 0;
	if (resetTried == 1) {
		return;
	}
	uint32_t SDA_PIN = MPU6050_SDA_Pin;
	uint32_t SCL_PIN = MPU6050_SCL_Pin;
	GPIO_InitTypeDef GPIO_InitStruct;

	// 1
	__HAL_I2C_DISABLE(hi2c);

	// 2
	GPIO_InitStruct.Pin = SDA_PIN | SCL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WRITE_ODR(GPIOB, SDA_PIN);
	HAL_GPIO_WRITE_ODR(GPIOB, SCL_PIN);

	// 6
	GPIO_InitStruct.Pin = SCL_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// 8
	GPIO_InitStruct.Pin = SDA_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WRITE_ODR(GPIOB, SDA_PIN);

	// 10
	GPIO_InitStruct.Pin = SCL_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WRITE_ODR(GPIOB, SCL_PIN);

	// 12

	// 13
	hi2c->Instance->CR1 |= I2C_CR1_SWRST;

	// 14
	hi2c->Instance->CR1 ^= I2C_CR1_SWRST;

	// 15
	__HAL_I2C_ENABLE(hi2c);

	resetTried = 1;
}

void HAL_GPIO_WRITE_ODR(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	/* Check the parameters */
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	GPIOx->ODR |= GPIO_Pin;
}
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)

{

	if (hi2c->Instance == hi2c1.Instance)

	{

		//I2C_MasterTransmit_BTF(hi2c);

	}

}
/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM5 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM5) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */
	if (htim->Instance == TIM11) {

	}
	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	printf("main Error_handler \r\n");
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
