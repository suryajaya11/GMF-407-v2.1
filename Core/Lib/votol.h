/*
 * votol.h
 *
 *  Created on: Nov 16, 2022
 *      Author: Surya
 */

#ifndef LIB_VOTOL_H_
#define LIB_VOTOL_H_

#include "main.h"

typedef struct{
	uint8_t buf[24];

	int16_t rpm;
	float bat;
	float current;

	int16_t temp;
	int16_t ext_temp;

	uint8_t speed_mode;
	bool reverse;
	bool park;
	bool brake_votol;
	bool antitheft;
	bool side_stand;
	bool regen;
} votol_t;

void start_votol();
void kontrol_votol();
void callback_uart_votol(UART_HandleTypeDef* huart);
void callback_can_votol(CAN_HandleTypeDef* hcan, CAN_RxHeaderTypeDef* header, uint8_t* data);

#endif /* LIB_VOTOL_H_ */
