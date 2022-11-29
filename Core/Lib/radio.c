/*
 * radio.c
 *
 *  Created on: Oct 12, 2022
 *      Author: Surya
 */

#include "radio.h"

#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


uint8_t serial_byte;
uint8_t serial_ptr;
uint8_t serial_buf[50];
uint8_t serial_buf_channels[25];
uint8_t serial_buf_changed[25];
uint8_t serial_buf_lq[25];
int8_t clear_buf = 0;
uint8_t sync = 0;

uint32_t unsync_rate = 0;
uint16_t wait_to_sync = 0;

uint16_t channels[16];
uint16_t sync_val = 26;
uint8_t frame_size;

uint32_t ctr_active = 0;
uint8_t frame_active = 0;

void start_radio(UART_HandleTypeDef* huart){
	HAL_UART_Receive_DMA(huart, &serial_byte, 1);
}

void parse_frame(){
	if(serial_ptr > 49){
		serial_ptr = 0;
		sync = 0;
		memset(serial_buf, 0, 50);
		unsync_rate++;
	}

	if(serial_byte == 0xc8 && sync == 0){
//		memset(serial_buf, 0, 50);
		serial_buf[serial_ptr] = serial_byte;
		serial_ptr = 1;
		sync = 1;
		return;
	}

	if(sync == 1){
		if(serial_byte > 50){
			serial_ptr = 0;
			sync = 0;
			return;
		}
		frame_size = serial_byte;
		serial_buf[serial_ptr] = serial_byte;
		serial_ptr++;
		sync = 2;
		return;
	}

	if(sync == 2){
		serial_buf[serial_ptr] = serial_byte;
		serial_ptr++;
		if(serial_ptr >= frame_size + 1){
			sync = 0;
			serial_ptr = 0;
			//frame_size = 0;

			if(frame_size == 24){
				ctr_active++;
				frame_active = 1;
				for(int i = 0; i < 25; i++){
					if(serial_buf_channels[i] == serial_buf[i]) continue;

					for(int bit = 0; bit < 8; bit++){
						if(((serial_buf_channels[i] >> bit) & 1) != ((serial_buf[i] >> bit) & 1)) serial_buf_changed[i] |= 1 << bit;
					}
				}
				memcpy(serial_buf_channels, serial_buf, 25);
			//11 bit
			//analog
				channels[0] = (serial_buf[3] + ((serial_buf[4] & 0b111) << 8)) >> 0;
				channels[1] = (serial_buf[4] >> 3) + (((serial_buf[5] & 0b111111) << 5) >> 0);
				channels[2] = (serial_buf[5] >> 6) + ((serial_buf[6] & 0xff) << 2) + ((serial_buf[7] & 0b1) << 10);
				channels[3] = (serial_buf[7] >> 1) + ((serial_buf[8] & 0b1111) << 7);

			//switches
				//sw E
				channels[4] = (serial_buf[8] >> 4) + ((serial_buf[9] & 0b1111111) << 4);

				//sw F
				channels[5] = (serial_buf[9] >> 7) + (serial_buf[10] << 1) + ((serial_buf[11] & 0b11) << 9);

				//sw B
				channels[6] = (serial_buf[11] >> 2) + ((serial_buf[12] & 0b11111) << 6);

				//sw C
				channels[7] = (serial_buf[12] >> 5) + ((serial_buf[13] << 3));

				//sw(button) A
				channels[8] = (serial_buf[14]) + ((serial_buf[15] & 0b111) << 8);

				//sw(button) D
				channels[9] = (serial_buf[15] >> 3) + ((serial_buf[16] & 0b111111) << 5);

				//sw(button) G
				channels[10] = ((serial_buf[16]) >> 6) + (serial_buf[17] << 2) + ((serial_buf[18] & 1) << 10);

				//sw(button) H
				channels[11] = (serial_buf[18] >> 1) + ((serial_buf[19] & 0b1111) << 7);

				//potensio S1 (kiri)
				channels[12] = (serial_buf[19] >> 4) + ((serial_buf[20] & 0b1111111) << 4);

				//potensio S2 (kanan)
				//ga ada berubah??
			}

			if(serial_buf[2] == 0x14){
				memcpy(serial_buf_lq, serial_buf, serial_buf[1] + 1);
			}
		}

	}
}

void clear_buf_changed(){
	memset(serial_buf_changed, 0, 25);
}

int32_t map_val(float val, float in_min, float in_max, float out_min, float out_max){
	float normalized = ((float)val - in_min)/((float)in_max - in_min);	//to 0-1
	float denormalized = (normalized)*(out_max-out_min)+out_min;

	return (int32_t) denormalized;
}

float map_float(float val, float in_min, float in_max, float out_min, float out_max){
	float normalized = ((float)val - in_min)/((float)in_max - in_min);	//to 0-1
	float denormalized = (normalized)*(out_max-out_min)+out_min;

	return denormalized;
}

void reset_ctr_radio(){
	if(ctr_active == 0) frame_active = 0;
	else ctr_active = 0;
}
