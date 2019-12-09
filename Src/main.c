/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mdbt42q.h"
#include "ws2812b.h"
#include "power.h"
#include <stdio.h>
#include <string.h>
#include "ema_filter.h"
#include "dwt_stm32_delay.h"
#include "ble_cmd.h"
//#include "pid.h"

#ifdef USE_DMP
#include "mpu6050_dmp.h"
#else
#include "mpu6050.h"
#endif
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_GREEN_TOGGLE	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint32_t last_time=0, mpu_last_time=0, led_last_time=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//float ledPos = 0;
float ledPos_before = 0.0f;
float pidControl = 0.0f;
uint8_t ledPosUser = 0;
uint8_t led_control_mode = 0; // default(0) : Auto(Gyro), Manual(1) : User Select
uint8_t auto_time_off_mode = 0;
uint32_t ntime_auto_off_mode = 0;
uint32_t time_cnt = 0;
uint8_t running_mode = 0;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
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
  MX_TIM10_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_TIM1_Init();
  MX_TIM11_Init();
  MX_SPI1_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

  vt100SetCursorPos( 0, 0);
  printf("Booting LittleCat Board!!!!221\r\n\n");

  power_en();
  ble_gpio_init();
  initLEDMOSI();
  time_setup();

  DWT_Delay_Init();
#ifdef USE_DMP
//  pidData = (PidData_t *)calloc(1, sizeof(PidData_t));
  //! Initialization of PID factors
//  PID_Init(K_P * SCALING_FACTOR, K_I * SCALING_FACTOR , K_D * SCALING_FACTOR , pidData);


  Cal_Filter = (MovingFilter_t *)calloc(4, sizeof(MovingFilter_t));
  EMA_FILTER_Init(EMA_Alpha, Cal_Filter);

  DMP_Init();
//  HAL_Delay(1000);
//  printf("Calibration ready\r\n");
  // Waiting the device status until the stable state
//  for(register int i=0; i<2000; i++) {
//	  Read_DMP();
//	  HAL_Delay(5);
//	  if( (i%100) == 0 ) HAL_UART_Transmit(&huart1, (uint8_t *)&".", 1, 100);
//  }

////  run_self_test2();
//  printf("\r\nCalibration start\r\n");
////   Calibration of the mpu6050
//  for(register int i=0; i<2000; i++)
//  {
//	  Read_DMP();
//	  DEMA_Filter( Roll, 		&Cal_Filter[0] );
//	  DEMA_Filter( Pitch, 		&Cal_Filter[1] );
//	  DEMA_Filter( Yaw,	 		&Cal_Filter[2] );
//	  DEMA_Filter( Roll_reverse, &Cal_Filter[3] );

//	  vt100SetCursorPos( 3, 0);
//	  vt100ClearLinetoEnd();
//	  printf("\rRoll : %f\r\n", Roll);
//	  printf("\rDEMA : %f\r\n", Cal_Filter[0].DEMA);
//	  HAL_Delay(5);
//	  if( (i%100) == 0 ) HAL_UART_Transmit(&huart1, (uint8_t *)&".", 1, 100);
//  }
//  base_roll			= Cal_Filter[0].DEMA;
//  base_pitch		= Cal_Filter[1].DEMA;
//  base_yaw			= Cal_Filter[2].DEMA;
//  base_roll_reverse	= Cal_Filter[3].DEMA;
//  printf("\r\nCalibration is done.\r\n");
//  HAL_Delay(2000);
  Cal_done = 1;
  targetAnglel -= base_roll;
  if (targetAnglel < 0) targetAnglel = 360.0 + targetAnglel;
  targetLedPos = (LED_TOTAL / 360.0f) * roundf(targetAnglel);
  ledPos =  (LED_TOTAL / 360.0f) * roundf(Roll);
  ledPos = ledPos - targetLedPos;
  if (ledPos < 0) ledPos = LED_TOTAL + ledPos;

  printf("\r\nCalibration is done.\r\n");
#else
  MPU6050_SelfTest(SelfTest);
  printf("x-axis self test: acceleration trim within : %7.2f%% of factory value \n\r", SelfTest[0]);
  printf("y-axis self test: acceleration trim within : %7.2f%% of factory value \n\r", SelfTest[1]);
  printf("z-axis self test: acceleration trim within : %7.2f%% of factory value \n\r", SelfTest[2]);
  printf("x-axis self test: gyration trim within     : %7.2f%% of factory value \n\r", SelfTest[3]);
  printf("y-axis self test: gyration trim within     : %7.2f%% of factory value \n\r", SelfTest[4]);
  printf("z-axis self test: gyration trim within     : %7.2f%% of factory value \n\r", SelfTest[5]);
  HAL_Delay(1);

  if(SelfTest[0] < 1.0f && SelfTest[1] < 1.0f && SelfTest[2] < 1.0f && SelfTest[3] < 1.0f && SelfTest[4] < 1.0f && SelfTest[5] < 1.0f)
  {
  	MPU6050_Reset();
  	MPU6050_Calibration(gyroBias, accelBias);
  	vt100SetCursorPos( 8, 0);
  	printf("MPU6050 bias\r\n");
  	printf("\r ax : %7.2f o/s\t  ay : %7.2f o/s\t  az : %7.2f o/s\r\n",(accelBias[0]), (accelBias[1]), (accelBias[2]));
  	printf("\r gx : %7.2f   g\t  gy : %7.2f   g\t  gz : %7.2f   g\r\n",(gyroBias[0]), (gyroBias[1]), (gyroBias[2]));
  	MPU6050_Init(0x03); //42hz
  }
  else
  	printf("\rMPU6050 Init failed!\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uart_recv_int_enable();
  HAL_Delay(1000);

  vt100ClearScreen();

//  HAL_TIM_Base_Start_IT(&htim10);
  HAL_TIM_Base_Start_IT(&htim11);
#endif

  



  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
#ifndef USE_DMP
  	if( MPU6050_ReadOneByte(MPU6050_RA_INT_STATUS) & 0x01) {  // check if data ready interrupt

  		MPU6050_ReadAccelData(&accelCount[0]);  // Read the x/y/z adc values
  		MPU6050_GetAres();

  		// Now we'll calculate the accleration value into actual g's
  		ax = (float)accelCount[0]*aRes - accelBias[0];  // get actual g value, this depends on scale being set
  		ay = (float)accelCount[1]*aRes - accelBias[1];
//  		az = (float)accelCount[2]*aRes - accelBias[2];

  		MPU6050_ReadGyroData(&gyroCount[0]);  // Read the x/y/z adc values
  		MPU6050_GetGres();

  		// Calculate the gyro value into actual degrees per second
  		gx = (float)gyroCount[0]*gRes - gyroBias[0];  // get actual gyro value, this depends on scale being set
  		gy = (float)gyroCount[1]*gRes - gyroBias[1];
//  		gz = (float)gyroCount[2]*gRes - gyroBias[2];

//  		tempCount = MPU6050_ReadTempData();  // Read the x/y/z adc values
//  		temperature = (tempCount) / 340. + 36.53; // Temperature in degrees Centigrade
  	}

  	Now = time_ms();
  	deltat = (float)((Now - lastUpdate) / 1000000.0f); // set integration time by time elapsed since last filter update
  	lastUpdate  = time_ms();


  	//    x_acc =  atan2(ay/sqrt(pow(ax,2) + pow(az,2)))*57.29577951;
  	//    y_acc = -atan2(-ax/sqrt(pow(ay,2) + pow(az,2)))*57.29577951;
  	x_acc =  atan2(ay, sqrt(pow(ax,2) + pow(az,2)));//*57.29577951;
  	y_acc =  atan2((-1)*ax, az);//*57.29577951;
  	z_acc = 0;

  	x_gyr = (gx)*deltat + last_x_angle;
  	y_gyr = (gy)*deltat + last_y_angle;
  	//z_gyr = (gz)*deltat + last_z_angle;

  	float alpha = 0.96;
  	x_fil = alpha*x_gyr + (1.0 - alpha)*x_acc;
  	y_fil = alpha*y_gyr + (1.0 - alpha)*y_acc;
  	//z_fil = z_gyr;  //Accelerometer doesn't give z-angle


  	last_x_angle = x_fil;
  	last_y_angle = y_fil;
  	//last_z_angle = z_fil;

  	deltat = time_ms() - count;
  	if (deltat > 500) { // update LCD once per half-second independent of read rate
//  		LED_GREEN_TOGGLE;

  		ledPos = roundf((LED_TOTAL / 360.0f) * Roll);

  		x_fil *= 180.0f / PI;
  		y_fil *= 180.0f / PI;
  		//z_fil *= 180.0f / PI;


  		if (x_fil < 0) x_fil = 360.0 + x_fil;
  		if (y_fil < 0) y_fil = 360.0 + y_fil;
  		//if (z_fil < 0) z_fil = 360.0 + z_fil;

  		vt100SetCursorPos( 20, 0);

		printf("\r x_acc \t\t: %d degree\n\r", accelCount[0]);
  		printf("\r x_acc \t\t: %d dgree\n\r", accelCount[1]);
  		printf("\r x_acc \t\t: %d degree\n\r", gyroCount[0]);
  		printf("\r x_acc \t\t: %d degree\n\r", gyroCount[1]);


  		printf("\r x_acc \t\t: %7.2f degree\n\r", x_acc);
  		printf("\r y_acc \t\t: %7.2f degree\n\n\r", y_acc);
//  		printf("\r z_acc \t\t: %7.2f degree\n\r", z_acc);

  		printf("\r x_gyro \t: %7.2f degree\n\r", x_gyr);
  		printf("\r y_gyro \t: %7.2f degree\n\\nr", y_gyr);
//  		printf("\r z_gyro \t: %7.2f degree\n\r", z_gyr);

  		printf("\r x_fil \t\t: %7.2f degree\n\r", x_fil);
  		printf("\r y_fil \t\t: %7.2f degree\n\n\r", y_fil);
//  		printf("\r z_fil \t\t: %7.2f degree\n\r", z_fil);

  		count = time_ms();
  	}
#else
	  if (ledPos_before != ledPos) {

		  setAllPixelColor(0, 0, 0);
//		  setPixelColor( (uint16_t)DEMA_Filter( ledPos, &Cal_Filter[0] ), 0, 50, 0 );
		  setPixelColor( (uint16_t)ledPos, 0, 50, 0 );
		  ledPos_before = ledPos;
//		  memset(buff, 0, sizeof(buff));
//		  sprintf(buff, "roll : %d, pos : %d\r\n", (uint16_t)Roll, (uint16_t)ledPos);
//		  HAL_UART_Transmit(&huart2, buff, strlen(buff), 100);
//		  vt100SetCursorPos(30, 0);
//		  printf("roll : %d, pos : %d\r\n", (uint16_t)Roll, (uint16_t)ledPos);


	  }
//	  DWT_Delay_us(1);
#endif
//    memset(i2c_rd_buff, 0, sizeof(i2c_rd_buff));
//    if (HAL_I2C_Master_Receive_DMA(&hi2c1,(uint16_t)(0x68<<1), i2c_rd_buff, 8) == HAL_OK) {
//      printf("i2c_rd_buff[0] : %02x\r\n", i2c_rd_buff[0]);
//      printf("i2c_rd_buff[1] : %02x\r\n", i2c_rd_buff[1]);
//      printf("i2c_rd_buff[2] : %02x\r\n", i2c_rd_buff[2]);
//      printf("i2c_rd_buff[3] : %02x\r\n", i2c_rd_buff[3]);
//      printf("i2c_rd_buff[4] : %02x\r\n", i2c_rd_buff[4]);
//      printf("i2c_rd_buff[5] : %02x\r\n", i2c_rd_buff[5]);
//      printf("i2c_rd_buff[6] : %02x\r\n", i2c_rd_buff[6]);
//      printf("i2c_rd_buff[7] : %02x\r\n", i2c_rd_buff[7]);
//    }
//    else{
//      printf("I2C ERROR\r\n");
//    }
//    memset(i2c_rd_buff, 0, sizeof(i2c_rd_buff));
process();
  }

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System 
  */
  HAL_RCC_EnableCSS();

}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* TIM1_UP_TIM10_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  /* I2C1_EV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
  /* I2C1_ER_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
}

/* USER CODE BEGIN 4 */

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
/*
  if (htim->Instance ==TIM10)
  {
    HAL_TIM_Base_Stop_IT(&htim10);
    Read_DMP();
    //mpu_last_time = time_ms();
    HAL_TIM_Base_Start_IT(&htim10);
  }
*/
//}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == MPU6050_INT1_X_Pin)
  {
    // To do
#ifdef USE_DMP
	  if(Cal_done) {
		  Read_DMP();
	  }
#endif
  }
}


void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	printf("I2C Interrupt\r\n");
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM5) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
