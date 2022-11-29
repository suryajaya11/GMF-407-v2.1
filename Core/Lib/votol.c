/*
 * votol.c
 *
 *  Created on: Nov 16, 2022
 *      Author: Surya
 */

#include "votol.h"
#include "radio.h"
#include "dac.h"
#include "usart.h"
#include "tim.h"
#include "can_stuff.h"

int16_t speed_motor[2];

float pwm_filtered[2];
float dac_high[2] = {1.65, 1.65};
float dac_low[2] = {0.9, 0.9};

uint8_t reverse[2];
uint8_t brake[2];
uint8_t park[2];

votol_t data_votol[2];

uint8_t open_loop[2];

void start_votol(){
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);

  CAN_GeneralSetup(&hcan1);
//  CAN_GeneralSetup(&hcan2);
}

void kontrol_votol(){
	if(frame_active == 0){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);		//L PARK
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, 0);		//R PARK
		return;
	}

	speed_motor[0] = map_val(channels[2], 172, 1810, -100, 100);
	speed_motor[1] = map_val(channels[1], 172, 1810, -100, 100);
	brake[0] = map_val(channels[8], 172, 1810, 0, 1);
	brake[1] = map_val(channels[9], 172, 1810, 0, 1);
	park[0] = map_val(channels[6], 172, 1810, 0, 1);
	park[1] = map_val(channels[7], 172, 1810, 0, 1);

	for(int i = 0; i < 2; i++){
		if(speed_motor[i] >= 0){
			pwm_filtered[i] = map_float(speed_motor[i], 0, 100, dac_low[i], dac_high[i]);
			reverse[i] = 0;
		}
		else {
			reverse[i] = 1;
			if(data_votol[i].reverse || open_loop[i])
				 pwm_filtered[i] = map_float(speed_motor[i], 0, -100, dac_low[i], dac_high[i]);
			else pwm_filtered[i] = 0.9;
		}
	}

	//reverse pin
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, reverse[0]);		//L REV
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, reverse[1]);			//R REV

	//brake
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, brake[0]);		//L BRAKE
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, brake[1]);		//R BRAKE

	//analog
	uint16_t pwm_val;

	if(pwm_filtered[0] > 5) pwm_filtered[0] = 5;
	if(pwm_filtered[0] < 0) pwm_filtered[0] = 0;

	pwm_val = pwm_filtered[0] * 91.7 - 51.3;
	if(pwm_val > 499) pwm_val = 499;
	if(pwm_val < 0) pwm_val = 0;

	TIM4->CCR1 = pwm_val;

	if(pwm_filtered[1] > 5) pwm_filtered[1] = 5;
	if(pwm_filtered[1] < 0) pwm_filtered[1] = 0;

	pwm_val = pwm_filtered[1] * 91.7 - 51.3;
	if(pwm_val > 499) pwm_val = 499;
	if(pwm_val < 0) pwm_val = 0;

	TIM4->CCR2 = pwm_val;

	//park
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, park[0]);		//L PARK
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, park[1]);		//R PARK

	/*
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, relay[0]);		//SEPARATE	COM+1 & COM+2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, relay[1]);		//L FRWD
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, relay[2]);		//L REV
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, relay[3]);		//L STR
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, relay[4]);		//L PARK
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, relay[5]);		//L BRAKE
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, relay[6]);		//R FRWD
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, relay[7]);			//R REV
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, relay[8]);			//R STR
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, relay[9]);		//R PARK
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, relay[10]);		//R BRAKE

	TIM4->CCR1 = pwm[0];
	TIM4->CCR2 = pwm[1];
	*/
}

uint32_t can_terima[2];

void callback_can_votol(CAN_HandleTypeDef* hcan, CAN_RxHeaderTypeDef* header, uint8_t* data){
	uint8_t i = 0;

	if(hcan->Instance == CAN2) i = 0;
	if(hcan->Instance == CAN1) i = 1;

	if(header->StdId == 0 		  &&
	   header->ExtId == 270929954 &&
	   header->IDE   == 4		  &&
	   header->RTR == 0 &&
	   header->DLC == 8)
	{
		data_votol[i].bat = ((data[5] << 8) + (data[4])) * 0.1;
		data_votol[i].current = ((int16_t)(data[7] << 8) + (data[6])) * 0.1;
		can_terima[0]++;
	}

	if(header->StdId == 1022 	&&
	   header->ExtId == 0 		&&
	   header->IDE   == 0		&&
	   header->RTR == 0 		&&
	   header->DLC == 8)
	{
		data_votol[i].rpm = (data[0] << 8) + (data[1]);
		data_votol[i].temp = data[2] - 50;
		data_votol[i].ext_temp = data[3] - 50;

		data_votol[i].speed_mode = (data[6] & 0b11);
		data_votol[i].reverse = (data[6] >> 2) & 1;
		data_votol[i].park 	= (data[6] >> 3) & 1;
		data_votol[i].brake_votol = (data[6] >> 4) & 1;
		data_votol[i].antitheft = (data[6] >> 5) & 1;
		data_votol[i].side_stand = (data[6] >> 6) & 1;
		data_votol[i].regen = (data[6] >> 7) & 1;
		can_terima[1]++;
	}
}
