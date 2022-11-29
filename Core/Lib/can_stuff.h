/*
 * CAN_Setup.h
 *
 *  Created on: Jun 29, 2021
 *      Author: ismarintan
 *
 *  ====================================================================================
 *  								Panduan Penggunaan
 *  				 Fungsi Insialisasi RxFilter dan Interrupt CAN BUS
 *  ====================================================================================
 * 		1. Aktifkan CANx_RX0 Interrupt pada Iniialisasi NVIC
 * 		2. Letakkan Handler Penerimaan pada Fungsi Callback
 *   	   void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
 *
 *
 *
 */

#ifndef INC_CAN_SETUP_H_
#define INC_CAN_SETUP_H_



#include "main.h"
#include "can.h"

typedef struct{
	CAN_RxHeaderTypeDef rx_header;
	uint8_t data[8];
	uint32_t frame_received;
	uint32_t time_recv;
} can_debug_t;

typedef struct{
	uint16_t data_uint;
	int16_t data_int;
	float data_float;
	uint8_t list_num_0;
	uint8_t list_num_1;
	uint8_t data0;
	uint8_t data1;
} satuin_data_t;

void CAN_GeneralSetup(CAN_HandleTypeDef *hcan);
void can_kirim(uint16_t id, uint8_t data[8]);
void can_list_extid();
void votol_can_req();

CAN_TxHeaderTypeDef Tx_Header_Msg;


#endif /* INC_CAN_SETUP_H_ */
