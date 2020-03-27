/**
  ******************************************************************************
  * File Name          : WS2812B.h
  * Description        : This file provides code for the configuration
  *                      of the WS2812B instances.
  ******************************************************************************
 */



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ws2812b_H
#define __ws2812b_H
#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "main.h"



#define LED_NO    LED_TOTAL

#define LED_BUFFER_LENGTH (LED_NO*12)

void SystemClock_Config(void);
void encode_byte( uint8_t data, int16_t buffer_index );
void generate_ws_buffer( uint8_t RData,uint8_t GData,uint8_t BData, uint16_t led_no );
void Send_2812(void);
void clear_led(void);
void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
void initLEDMOSI(void);
void test_led_rgb(void);
void led_update(void);
void set_led_position(uint8_t pos);
void set_led_col(uint32_t data);
void set_rand_led_mode(void);
void dis_rand_led_mode(void);
void random_led(void);
uint8_t pos_move_check(uint8_t pos);
extern uint8_t red;
extern uint8_t green;
extern uint8_t blue;
extern int16_t out_ledPos;

extern int16_t out_ledPos2;
extern uint8_t ledPos_before_inLED;

extern uint8_t ledPos_before_inLED2;
extern uint16_t accumulate_ledmove;
extern uint8_t rand_led_mode;
#ifdef __cplusplus
}
#endif
#endif /*__ ws2812b_H */
