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
uint8_t onePixelData[12] = { 0x44, };
extern SPI_HandleTypeDef hspi1;
int16_t out_ledPos;
int16_t out_ledPos2;
uint8_t ledPos_before_inLED = 0;
uint8_t ledPos_before_inLED2 = 0;

uint8_t red = 0;
uint16_t total = 0;
float bright = 51;
uint8_t green = 50;
uint8_t blue = 0;
uint8_t rand_led_mode = 0;
uint8_t ws_buffer[LED_BUFFER_LENGTH];
uint16_t offset_opt = 0;
uint16_t index_opt = 0;
uint16_t offset = 0;
float adjust_bright = 0;

HAL_StatusTypeDef spi_status = HAL_OK;
void clear_led(void) {
	memset(ws_buffer, 0x44, LED_BUFFER_LENGTH);
	Send_2812();
}
void encode_byte(uint8_t data, int16_t buffer_index) {
	int index = data * 4;
	onePixelData[buffer_index++] = leddata[index++];
	onePixelData[buffer_index++] = leddata[index++];
	onePixelData[buffer_index++] = leddata[index++];
	onePixelData[buffer_index++] = leddata[index++];
}
void generate_ws_buffer(uint8_t RData, uint8_t GData, uint8_t BData,
		uint16_t led_no) {
	//memset(ws_buffer, 0, sizeof(ws_buffer));
	//ws2812b
	//G--R--B
	//MSB first
	offset = led_no * 12;
	encode_byte(GData, 0);
	encode_byte(RData, 4);
	encode_byte(BData, 8);
	memcpy(ws_buffer + offset, onePixelData, 12);
}
void Send_2812(void) {

	spi_status = HAL_SPI_Transmit_DMA(&hspi1, ws_buffer, LED_BUFFER_LENGTH);
//			if(spi_status){
//				printf("spi error : %d\r\n",spi_status);
//	}
}
void bright_adjust(void) {
	total = red + green + blue;
	adjust_bright = bright / (total + 1);
	red = red * adjust_bright;
	green = green * adjust_bright;
	blue = blue * adjust_bright;

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
	clear_led();
}

void random_led(void) {
	red = rand() % 255;
	green = rand() % 255;
	blue = rand() % 255;
	bright_adjust();
	encode_byte(green, 0);
	encode_byte(red, 4);
	encode_byte(blue, 8);
	memset(ws_buffer + ledPos_before_inLED * 12, 0x44, 12);  // for optimized
	memcpy(ws_buffer + out_ledPos * 12, onePixelData, 12);
	Send_2812();
}

void led_update() {

//	memset(ws_buffer + ledPos_before_inLED * 12, 0x44, 12);  // for optimized
//	memcpy(ws_buffer + out_ledPos * 12, onePixelData, 12);
		memset(ws_buffer, 0x44, LED_BUFFER_LENGTH);  // for optimized

		memcpy(ws_buffer + out_ledPos * 12, onePixelData, 4);
		memcpy(ws_buffer + out_ledPos2 * 12+4, onePixelData, 4);
	Send_2812();
	ledPos_before_inLED = out_ledPos;

	ledPos_before_inLED2 = out_ledPos2;

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
	bright_adjust();
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
	clear_led();

	for (uint8_t j = 0; j < 1; j++) {
		for (uint8_t i = 0; i < LED_NO; i++) {

			setOnePixelOnlyOnColor(i, red, green, blue);
			HAL_Delay(10);
		}
	}

	clear_led();
}

/* USER CODE END 0 */
