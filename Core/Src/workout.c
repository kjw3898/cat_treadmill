/*
 * workout.c
 *
 *  Created on: 2019. 12. 11.
 *      Author: issacs
 */

#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include <math.h>
#include "flash_if.h"
#include "bitopr.h"
#include "circular_buffer.h"
#define radius 55

exerciseReport *exReport;
dataExercise *exData;
exerciseReport *exReport_temp;

/*! \brief
 *
 *
 */
void initExercise(void) {

	exReport = (exerciseReport*) malloc(sizeof(exerciseReport) * totalBuffSize);
	assert(exReport);

	for (int i = 0; i < totalBuffSize - 1; i++) {
		exReport[i].timeStamp = 0;
		exReport[i].distExercised = 0;
		exReport[i].timeExercised = 0;
	}

	cbuf = circular_buf_init(exReport, totalBuffSize);
	assert(cbuf);
	exData = (dataExercise*) malloc(sizeof(dataExercise));

//	printf("Buffer initialized. ");
//	print_buffer_status(cbuf);
//	exReport->reset = resetReport;

}

/*! \brief
 *
 *
 */
void resetReport(void) {
	for (int i = 0; i < totalBuffSize - 1; i++) {
		exReport[i].timeStamp = 0;
		exReport[i].distExercised = 0;
		exReport[i].timeExercised = 0;
	}
}

/*! \brief
 *
 *
 */
void loadDataFromFlash(exerciseReport *exReport, uint8_t day_index) {
	assert(exReport);

	day_index += 1;
	/* Load boot data from flash */
//	exReport->distExercised	= (*(uint32_t*)(ADDR_FLASH_SECTOR_4));
	memcpy(exReport, (uint32_t*) (ADDR_FLASH_SECTOR_6),
			sizeof(exerciseReport) * maxCnt * day_index);
}

/*! \brief
 *
 *
 */
void writeDataToFlash(exerciseReport *exReport, uint8_t day_index) {
	uint32_t ramsource;
	uint32_t offset = sizeof(exerciseReport) * maxCnt * day_index;

	assert(exReport);

	//지우기 전에 기존 데이터 로드
	//loadDataFromFlash(day_index);

//	if(FLASH_If_Erase_Range(ADDR_FLASH_SECTOR_6, ADDR_FLASH_SECTOR_7) == FLASHIF_OK)
//	{
	ramsource = (uint32_t) &exReport[0];

	if (FLASH_If_Write(ADDR_FLASH_SECTOR_6 + offset, (uint32_t*) ramsource,
			sizeof(exerciseReport) * maxCnt / 4) != FLASHIF_OK)
		;
	//return ;
//	}
//	else
//		return ;

	//return ;
}

/*! \brief
 *
 *
 */
uint16_t arcLength(uint32_t ledMoved) {
	return 2 * M_PI  * radius * ledMoved / LED_TOTAL;
}

/*! \brief
 *
 *
 */

void amountOfExercise(dataExercise *exData) {

	//1초가 되면 운동데이터를 기록한다
	exReport[cbuf->head].timeStamp = timestamp;
	exReport[cbuf->head].distExercised = exData->acumulatedLEDMove;
	exReport[cbuf->head].timeExercised = exData->MoveTick;

	//push the buffer
	circular_buf_put(cbuf, &exReport[cbuf->head]);

}
size_t getExerciseData(exerciseReport **exReport,uint16_t n){
	size_t size;
	if(n==0)
		size = circular_buf_size(cbuf);
	else
		size = n;
	if(size==0)
		return 0;
	*exReport =  (exerciseReport*) malloc(sizeof(exerciseReport) * size);
	circular_buf_get_range(*exReport,cbuf,size);
	return  sizeof(exerciseReport) *size;
}

