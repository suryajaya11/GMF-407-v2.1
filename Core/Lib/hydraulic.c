/*
 * hydraulic.c
 *
 *  Created on: Nov 28, 2022
 *      Author: Surya
 */

#include "hydraulic.h"

void set_pins(uint32_t data){
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, (data >> 0) & 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, (data >> 1) & 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, (data >> 2) & 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, (data >> 3) & 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, (data >> 4) & 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, (data >> 5) & 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (data >> 6) & 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (data >> 7) & 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (data >> 8) & 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (data >> 9) & 1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, (data >> 10) & 1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, (data >> 11) & 1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, (data >> 12) & 1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, (data >> 13) & 1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, (data >> 14) & 1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, (data >> 15) & 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, (data >> 16) & 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, (data >> 17) & 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, (data >> 18) & 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, (data >> 19) & 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, (data >> 20) & 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, (data >> 21) & 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, (data >> 22) & 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, (data >> 23) & 1);
}

