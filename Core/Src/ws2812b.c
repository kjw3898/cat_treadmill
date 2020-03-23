/**
 ******************************************************************************
 * File Name          : WS2812B.c
 * Description        : This file provides code for the configuration
 *                      of the WS2812B instances.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "ws2812b.h"
#include "power.h"
#include <stdlib.h>
const uint8_t leddata[256 * 4] = { // size = 256 * 3
		0X44, 0X44, 0X44,
				0X44, // 0
				0X44, 0X44, 0X44,
				0X47, // 1
				0X44, 0X44, 0X44, 0X74, 0X44, 0X44, 0X44, 0X77, 0X44, 0X44,
				0X47, 0X44, 0X44, 0X44, 0X47, 0X47, 0X44, 0X44, 0X47, 0X74,
				0X44, 0X44, 0X47, 0X77, 0X44, 0X44, 0X74, 0X44, 0X44, 0X44,
				0X74, 0X47, 0X44, 0X44, 0X74, 0X74, 0X44, 0X44, 0X74, 0X77,
				0X44, 0X44, 0X77, 0X44, 0X44, 0X44, 0X77, 0X47, 0X44, 0X44,
				0X77, 0X74, 0X44, 0X44, 0X77, 0X77, 0X44, 0X47, 0X44, 0X44,
				0X44, 0X47, 0X44, 0X47, 0X44, 0X47, 0X44, 0X74, 0X44, 0X47,
				0X44, 0X77, 0X44, 0X47, 0X47, 0X44, 0X44, 0X47, 0X47, 0X47,
				0X44, 0X47, 0X47, 0X74, 0X44, 0X47, 0X47, 0X77, 0X44, 0X47,
				0X74, 0X44, 0X44, 0X47, 0X74, 0X47, 0X44, 0X47, 0X74, 0X74,
				0X44, 0X47, 0X74, 0X77, 0X44, 0X47, 0X77, 0X44, 0X44, 0X47,
				0X77, 0X47, 0X44, 0X47, 0X77, 0X74, 0X44, 0X47, 0X77, 0X77,
				0X44, 0X74, 0X44, 0X44, 0X44, 0X74, 0X44, 0X47, 0X44, 0X74,
				0X44, 0X74, 0X44, 0X74, 0X44, 0X77, 0X44, 0X74, 0X47, 0X44,
				0X44, 0X74, 0X47, 0X47, 0X44, 0X74, 0X47, 0X74, 0X44, 0X74,
				0X47, 0X77, 0X44, 0X74, 0X74, 0X44, 0X44, 0X74, 0X74, 0X47,
				0X44, 0X74, 0X74, 0X74, 0X44, 0X74, 0X74, 0X77, 0X44, 0X74,
				0X77, 0X44, 0X44, 0X74, 0X77, 0X47, 0X44, 0X74, 0X77, 0X74,
				0X44, 0X74, 0X77, 0X77, 0X44, 0X77, 0X44, 0X44, 0X44, 0X77,
				0X44, 0X47, 0X44, 0X77, 0X44, 0X74, 0X44, 0X77, 0X44, 0X77,
				0X44, 0X77, 0X47, 0X44, 0X44, 0X77, 0X47, 0X47, 0X44, 0X77,
				0X47, 0X74, 0X44, 0X77, 0X47, 0X77, 0X44, 0X77, 0X74, 0X44,
				0X44, 0X77, 0X74, 0X47, 0X44, 0X77, 0X74, 0X74, 0X44, 0X77,
				0X74, 0X77, 0X44, 0X77, 0X77, 0X44, 0X44, 0X77, 0X77, 0X47,
				0X44, 0X77, 0X77, 0X74, 0X44, 0X77, 0X77, 0X77, 0X47, 0X44,
				0X44, 0X44, 0X47, 0X44, 0X44, 0X47, 0X47, 0X44, 0X44, 0X74,
				0X47, 0X44, 0X44, 0X77, 0X47, 0X44, 0X47, 0X44, 0X47, 0X44,
				0X47, 0X47, 0X47, 0X44, 0X47, 0X74, 0X47, 0X44, 0X47, 0X77,
				0X47, 0X44, 0X74, 0X44, 0X47, 0X44, 0X74, 0X47, 0X47, 0X44,
				0X74, 0X74, 0X47, 0X44, 0X74, 0X77, 0X47, 0X44, 0X77, 0X44,
				0X47, 0X44, 0X77, 0X47, 0X47, 0X44, 0X77, 0X74, 0X47, 0X44,
				0X77, 0X77, 0X47, 0X47, 0X44, 0X44, 0X47, 0X47, 0X44, 0X47,
				0X47, 0X47, 0X44, 0X74, 0X47, 0X47, 0X44, 0X77, 0X47, 0X47,
				0X47, 0X44, 0X47, 0X47, 0X47, 0X47, 0X47, 0X47, 0X47, 0X74,
				0X47, 0X47, 0X47, 0X77, 0X47, 0X47, 0X74, 0X44, 0X47, 0X47,
				0X74, 0X47, 0X47, 0X47, 0X74, 0X74, 0X47, 0X47, 0X74, 0X77,
				0X47, 0X47, 0X77, 0X44, 0X47, 0X47, 0X77, 0X47, 0X47, 0X47,
				0X77, 0X74, 0X47, 0X47, 0X77, 0X77, 0X47, 0X74, 0X44, 0X44,
				0X47, 0X74, 0X44, 0X47, 0X47, 0X74, 0X44, 0X74, 0X47, 0X74,
				0X44, 0X77, 0X47, 0X74, 0X47, 0X44, 0X47, 0X74, 0X47, 0X47,
				0X47, 0X74, 0X47, 0X74, 0X47, 0X74, 0X47, 0X77, 0X47, 0X74,
				0X74, 0X44, 0X47, 0X74, 0X74, 0X47, 0X47, 0X74, 0X74, 0X74,
				0X47, 0X74, 0X74, 0X77, 0X47, 0X74, 0X77, 0X44, 0X47, 0X74,
				0X77, 0X47, 0X47, 0X74, 0X77, 0X74, 0X47, 0X74, 0X77, 0X77,
				0X47, 0X77, 0X44, 0X44, 0X47, 0X77, 0X44, 0X47, 0X47, 0X77,
				0X44, 0X74, 0X47, 0X77, 0X44, 0X77, 0X47, 0X77, 0X47, 0X44,
				0X47, 0X77, 0X47, 0X47, 0X47, 0X77, 0X47, 0X74, 0X47, 0X77,
				0X47, 0X77, 0X47, 0X77, 0X74, 0X44, 0X47, 0X77, 0X74, 0X47,
				0X47, 0X77, 0X74, 0X74, 0X47, 0X77, 0X74, 0X77, 0X47, 0X77,
				0X77, 0X44, 0X47, 0X77, 0X77, 0X47, 0X47, 0X77, 0X77, 0X74,
				0X47, 0X77, 0X77, 0X77, 0X74, 0X44, 0X44, 0X44, 0X74, 0X44,
				0X44, 0X47, 0X74, 0X44, 0X44, 0X74, 0X74, 0X44, 0X44, 0X77,
				0X74, 0X44, 0X47, 0X44, 0X74, 0X44, 0X47, 0X47, 0X74, 0X44,
				0X47, 0X74, 0X74, 0X44, 0X47, 0X77, 0X74, 0X44, 0X74, 0X44,
				0X74, 0X44, 0X74, 0X47, 0X74, 0X44, 0X74, 0X74, 0X74, 0X44,
				0X74, 0X77, 0X74, 0X44, 0X77, 0X44, 0X74, 0X44, 0X77, 0X47,
				0X74, 0X44, 0X77, 0X74, 0X74, 0X44, 0X77, 0X77, 0X74, 0X47,
				0X44, 0X44, 0X74, 0X47, 0X44, 0X47, 0X74, 0X47, 0X44, 0X74,
				0X74, 0X47, 0X44, 0X77, 0X74, 0X47, 0X47, 0X44, 0X74, 0X47,
				0X47, 0X47, 0X74, 0X47, 0X47, 0X74, 0X74, 0X47, 0X47, 0X77,
				0X74, 0X47, 0X74, 0X44, 0X74, 0X47, 0X74, 0X47, 0X74, 0X47,
				0X74, 0X74, 0X74, 0X47, 0X74, 0X77, 0X74, 0X47, 0X77, 0X44,
				0X74, 0X47, 0X77, 0X47, 0X74, 0X47, 0X77, 0X74, 0X74, 0X47,
				0X77, 0X77, 0X74, 0X74, 0X44, 0X44, 0X74, 0X74, 0X44, 0X47,
				0X74, 0X74, 0X44, 0X74, 0X74, 0X74, 0X44, 0X77, 0X74, 0X74,
				0X47, 0X44, 0X74, 0X74, 0X47, 0X47, 0X74, 0X74, 0X47, 0X74,
				0X74, 0X74, 0X47, 0X77, 0X74, 0X74, 0X74, 0X44, 0X74, 0X74,
				0X74, 0X47, 0X74, 0X74, 0X74, 0X74, 0X74, 0X74, 0X74, 0X77,
				0X74, 0X74, 0X77, 0X44, 0X74, 0X74, 0X77, 0X47, 0X74, 0X74,
				0X77, 0X74, 0X74, 0X74, 0X77, 0X77, 0X74, 0X77, 0X44, 0X44,
				0X74, 0X77, 0X44, 0X47, 0X74, 0X77, 0X44, 0X74, 0X74, 0X77,
				0X44, 0X77, 0X74, 0X77, 0X47, 0X44, 0X74, 0X77, 0X47, 0X47,
				0X74, 0X77, 0X47, 0X74, 0X74, 0X77, 0X47, 0X77, 0X74, 0X77,
				0X74, 0X44, 0X74, 0X77, 0X74, 0X47, 0X74, 0X77, 0X74, 0X74,
				0X74, 0X77, 0X74, 0X77, 0X74, 0X77, 0X77, 0X44, 0X74, 0X77,
				0X77, 0X47, 0X74, 0X77, 0X77, 0X74, 0X74, 0X77, 0X77, 0X77,
				0X77, 0X44, 0X44, 0X44, 0X77, 0X44, 0X44, 0X47, 0X77, 0X44,
				0X44, 0X74, 0X77, 0X44, 0X44, 0X77, 0X77, 0X44, 0X47, 0X44,
				0X77, 0X44, 0X47, 0X47, 0X77, 0X44, 0X47, 0X74, 0X77, 0X44,
				0X47, 0X77, 0X77, 0X44, 0X74, 0X44, 0X77, 0X44, 0X74, 0X47,
				0X77, 0X44, 0X74, 0X74, 0X77, 0X44, 0X74, 0X77, 0X77, 0X44,
				0X77, 0X44, 0X77, 0X44, 0X77, 0X47, 0X77, 0X44, 0X77, 0X74,
				0X77, 0X44, 0X77, 0X77, 0X77, 0X47, 0X44, 0X44, 0X77, 0X47,
				0X44, 0X47, 0X77, 0X47, 0X44, 0X74, 0X77, 0X47, 0X44, 0X77,
				0X77, 0X47, 0X47, 0X44, 0X77, 0X47, 0X47, 0X47, 0X77, 0X47,
				0X47, 0X74, 0X77, 0X47, 0X47, 0X77, 0X77, 0X47, 0X74, 0X44,
				0X77, 0X47, 0X74, 0X47, 0X77, 0X47, 0X74, 0X74, 0X77, 0X47,
				0X74, 0X77, 0X77, 0X47, 0X77, 0X44, 0X77, 0X47, 0X77, 0X47,
				0X77, 0X47, 0X77, 0X74, 0X77, 0X47, 0X77, 0X77, 0X77, 0X74,
				0X44, 0X44, 0X77, 0X74, 0X44, 0X47, 0X77, 0X74, 0X44, 0X74,
				0X77, 0X74, 0X44, 0X77, 0X77, 0X74, 0X47, 0X44, 0X77, 0X74,
				0X47, 0X47, 0X77, 0X74, 0X47, 0X74, 0X77, 0X74, 0X47, 0X77,
				0X77, 0X74, 0X74, 0X44, 0X77, 0X74, 0X74, 0X47, 0X77, 0X74,
				0X74, 0X74, 0X77, 0X74, 0X74, 0X77, 0X77, 0X74, 0X77, 0X44,
				0X77, 0X74, 0X77, 0X47, 0X77, 0X74, 0X77, 0X74, 0X77, 0X74,
				0X77, 0X77, 0X77, 0X77, 0X44, 0X44, 0X77, 0X77, 0X44, 0X47,
				0X77, 0X77, 0X44, 0X74, 0X77, 0X77, 0X44, 0X77, 0X77, 0X77,
				0X47, 0X44, 0X77, 0X77, 0X47, 0X47, 0X77, 0X77, 0X47, 0X74,
				0X77, 0X77, 0X47, 0X77, 0X77, 0X77, 0X74, 0X44, 0X77, 0X77,
				0X74, 0X47, 0X77, 0X77, 0X74, 0X74, 0X77, 0X77, 0X74, 0X77,
				0X77, 0X77, 0X77, 0X44, 0X77, 0X77, 0X77, 0X47, 0X77, 0X77,
				0X77, 0X74, 0X77, 0X77, 0X77, 0X77, };

extern SPI_HandleTypeDef hspi1;
int16_t out_ledPos;
uint8_t ledPos_before_inLED = 0;
uint8_t red = 0;
uint8_t green = 50;
uint8_t blue = 0;
uint8_t rand_led_mode = 0;
uint8_t ws_buffer[LED_BUFFER_LENGTH];
uint16_t offset_opt = 0;
uint16_t index_opt = 0;
HAL_StatusTypeDef spi_status = HAL_OK;

void encode_byte(uint8_t data, int16_t buffer_index) {
	int index = data * 4;
	ws_buffer[buffer_index++] = leddata[index++];
	ws_buffer[buffer_index++] = leddata[index++];
	ws_buffer[buffer_index++] = leddata[index++];
	ws_buffer[buffer_index++] = leddata[index++];
}
void generate_ws_buffer(uint8_t RData, uint8_t GData, uint8_t BData,
		int16_t led_no) {
	//memset(ws_buffer, 0, sizeof(ws_buffer));
	//ws2812b
	//G--R--B
	//MSB first
	int offset = led_no * 12;
	encode_byte(GData, offset);
	encode_byte(RData, offset + 4);
	encode_byte(BData, offset + 8);
}
void Send_2812(void) {

	spi_status = HAL_SPI_Transmit_DMA(&hspi1, ws_buffer, LED_BUFFER_LENGTH);
//			if(spi_status){
//				printf("spi error : %d\r\n",spi_status);
//	}
}

void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
	generate_ws_buffer(r, g, b, n);
	Send_2812();
}
void setOnePixelOnlyOnColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
	memset(ws_buffer, 0x44, LED_BUFFER_LENGTH);
	generate_ws_buffer(r, g, b, n);
	Send_2812();
}
/**
 * initialize MOSI pin to LOW.  Without this, first time transmit for first LED might be wrong.
 *
 */
void initLEDMOSI(void) {
	uint8_t buffer0[2] = { 0, 0 };
	HAL_SPI_Transmit(&hspi1, buffer0, 1, 100);
}

void random_led(void) {
	red = rand() % 56;
	green = rand() % 56;
	blue = rand() % 56;
}

void led_update() {

	memset(ws_buffer, 0x44, LED_BUFFER_LENGTH);  // for optimized
	offset_opt = out_ledPos * 12;
	index_opt = green *4;
	ws_buffer[offset_opt++] = leddata[index_opt++];
	ws_buffer[offset_opt++] = leddata[index_opt++];
	ws_buffer[offset_opt++] = leddata[index_opt++];
	ws_buffer[offset_opt++] = leddata[index_opt];
	index_opt = red *4;
	ws_buffer[offset_opt++] = leddata[index_opt++];
	ws_buffer[offset_opt++] = leddata[index_opt++];
	ws_buffer[offset_opt++] = leddata[index_opt++];
	ws_buffer[offset_opt++] = leddata[index_opt];
	index_opt = blue *4;
	ws_buffer[offset_opt++] = leddata[index_opt++];
	ws_buffer[offset_opt++] = leddata[index_opt++];
	ws_buffer[offset_opt++] = leddata[index_opt++];
	ws_buffer[offset_opt] = leddata[index_opt];
	Send_2812();
	ledPos_before_inLED = out_ledPos;

}

void set_led_col(uint32_t data) {
	uint8_t r;
	uint8_t g;
	uint8_t b;

	r = (data >> 16) & 0xff;
	g = (data >> 8) & 0xff;
	b = (data >> 0) & 0xff;
	red = r;
	green = g;
	blue = b;

	if (data == 0) {
		led_power_off();
	} else {
		led_power_on();
		HAL_Delay(1);
	}
	setOnePixelOnlyOnColor(out_ledPos, red, green, blue);
}

void set_rand_led_mode(void) {
	rand_led_mode = 1;
}

void dis_rand_led_mode(void) {
	rand_led_mode = 0;
}

void test_led_rgb(void) {
	uint8_t i;

	setOnePixelOnlyOnColor(0, 0, 0, 0);
	for (i = 0; i < LED_NO; i += 4) {

		setOnePixelOnlyOnColor(i, 0, 50, 0);
		HAL_Delay(40);
	}
	setOnePixelOnlyOnColor(0, 0, 0, 0);
}

/* USER CODE END 0 */
