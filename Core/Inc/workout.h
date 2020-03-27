/*
 * workout.h
 *
 *  Created on: 2019. 12. 11.
 *      Author: issacs
 */

#ifndef WORKOUT_H_
#define WORKOUT_H_

#include "ble_cmd.h"
#include "time.h"
#define __100ms 	0
#define __1000ms	1
#define _100mS	100
#define _1s			1000
#define maxTime	900
#define maxCnt	maxTime
#define maxDay	2
#define totalBuffSize maxCnt * maxDay

typedef struct {
	uint32_t timeStamp;
	uint32_t distExercised;
	uint32_t timeExercised;
//	void (*reset)(void);
} exerciseReport;

typedef struct {
	uint32_t acumulatedLEDMove;
	uint32_t MoveTick;

} dataExercise;

typedef exerciseReport *exReport_handle_t;

extern exerciseReport *exReport;
extern dataExercise *exData;
extern time_t timestamp;
void initExercise(void);
void resetReport(void);
void resetExercise(void);
void loadDataFromFlash(exerciseReport *exReport, uint8_t day_index);
void writeDataToFlash(exerciseReport *exReport, uint8_t day_index);
uint16_t arcLength(uint32_t ledMoved);
uint16_t acumulateAngle(uint16_t degree);
uint32_t get_acumulatedDegree(void);
void amountOfExercise(dataExercise *exData);
size_t getExerciseData(exerciseReport **exReport, uint16_t n);
#endif /* WORKOUT_H_ */
