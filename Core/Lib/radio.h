/*
 * radio.h
 *
 *  Created on: Oct 12, 2022
 *      Author: Surya
 */

#ifndef INC_RADIO_H_
#define INC_RADIO_H_

#include "main.h"
#include "usart.h"

extern uint8_t serial_byte;
extern uint16_t channels[16];
extern uint8_t frame_active;

void start_radio(UART_HandleTypeDef* huart);
void parse_frame();
void clear_buf_changed();
int32_t map_val(float val, float in_min, float in_max, float out_min, float out_max);
float map_float(float val, float in_min, float in_max, float out_min, float out_max);
void reset_ctr_radio();

#endif /* INC_RADIO_H_ */
