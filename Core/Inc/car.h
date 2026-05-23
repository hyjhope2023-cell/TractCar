#ifndef __CAR_H
#define __CAR_H

#include "stm32f1xx_hal.h"
#include "motor.h"
#include "PID.h"


enum Direction{
	stop = 0,
	forward = 1,
	back = 2,
	left = 3,
	right = 4,
	left_move = 5,
	right_move = 6,
	left_forward = 7,
	left_back = 8,
	right_forward = 9,
	right_back = 10,
    DIR_MAX_COUNT = 11,
};


typedef struct Car* CarPosition;
struct Car{
	uint16_t speed;
	enum Direction direction;
	PID_Controller pid;
	// function definition
    CarPosition (*createCar)(void);
	void (*STOP)(void);
	void (*FORWARD)(uint16_t speed);
	void (*BACK)(uint16_t speed);
	void (*LEFT)(uint16_t speed);
	void (*RIGHT)(uint16_t speed);
	void (*LEFT_MOVE)(uint16_t speed);
	void (*RIGHT_MOVE)(uint16_t speed);
	void (*LEFT_FORWARD)(uint16_t speed);
	void (*LEFT_BACK)(uint16_t speed);
	void (*RIGHT_FORWARD)(uint16_t speed);
	void (*RIGHT_BACK)(uint16_t speed);
	void (*MOVE)(enum Direction dir,uint16_t speed);
    void (*TRACK)(CarPosition p);
	void (*TRACK_PID)(CarPosition p);

};

// function definition
CarPosition createCar(void);
/*
void STOP(void);
void FORWARD(uint16_t speed);
void BACK(uint16_t speed);
void LEFT(uint16_t speed);
void RIGHT(uint16_t speed);
void LEFT_MOVE(uint16_t speed);
void RIGHT_MOVE(uint16_t speed);
void LEFT_FORWARD(uint16_t speed);
void LEFT_BACK(uint16_t speed);
void RIGHT_FORWARD(uint16_t speed);
void RIGHT_BACK(uint16_t speed);
void MOVE(enum Direction dir,uint16_t speed);

*/
#endif
