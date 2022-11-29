/*
 * CAN_Setup.c
 *
 *  Created on: Jun 29, 2021
 *      Author: ismarintan
 *
 */



#include "can_stuff.h"
#include "can.h"
#include "votol.h"

#define list_len 10
CAN_FilterTypeDef sFilterConfig;
CAN_TxHeaderTypeDef Tx_Header_Msg;
CAN_RxHeaderTypeDef Rx_Header_Msg;
uint32_t CAN_TxMailbox;

satuin_data_t satuin_data;


void CAN_GeneralSetup(CAN_HandleTypeDef *hcan){
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;

	sFilterConfig.FilterIdHigh = 0X0000;
	sFilterConfig.FilterIdLow = 0X0000;

	sFilterConfig.FilterMaskIdHigh = 0X0000;
	sFilterConfig.FilterMaskIdLow = 0X0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;

	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 0;

	HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
	HAL_CAN_Start(hcan);
	HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

	Tx_Header_Msg.DLC = 8;
	Tx_Header_Msg.IDE = CAN_ID_STD;
	Tx_Header_Msg.RTR = CAN_RTR_DATA;
}


uint32_t can_kepanggil = 0;
uint8_t CAN_RxData[8];
uint8_t CAN_TxData[8];

extern void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

can_debug_t list[list_len];
uint16_t list_length = 0;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	can_kepanggil++;
	memset(CAN_RxData, 0, 8);

	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &Rx_Header_Msg, CAN_RxData);
//	callback_can_votol(hcan, &Rx_Header_Msg, CAN_RxData);

	return;

//reverse can stuff
	can_list_extid();

//	satuin_data.data_uint = (list[satuin_data.list_num_0].data[satuin_data.data0] << 8) +
//							(list[satuin_data.list_num_1].data[satuin_data.data1]);
//	memcpy(&satuin_data.data_int, &satuin_data.data_uint, 2);
//	satuin_data.data_float = satuin_data.data_uint / 10.0;
}



void can_list_extid(){
	uint16_t listed = 0;
	uint16_t write_to = 0;
	for(uint16_t i = 0; i <= list_length; i++){
		if(Rx_Header_Msg.ExtId == list[i].rx_header.ExtId &&
		   Rx_Header_Msg.StdId == list[i].rx_header.StdId &&
		   Rx_Header_Msg.IDE   == list[i].rx_header.IDE &&
		   Rx_Header_Msg.DLC   == list[i].rx_header.DLC &&
		   (memcmp(list[i].data, CAN_RxData, Rx_Header_Msg.DLC) == 0 || Rx_Header_Msg.StdId != 1023)){
			listed = 1;
			write_to = i;
			break;
		}
	}

	if(!listed){
		write_to = list_length;
		if(list_length < list_len - 1) list_length++;
	}

	if(list_length < list_len){
		list[write_to].rx_header.StdId = Rx_Header_Msg.StdId;
		list[write_to].rx_header.ExtId = Rx_Header_Msg.ExtId;
		list[write_to].rx_header.DLC = Rx_Header_Msg.DLC;
		list[write_to].rx_header.IDE = Rx_Header_Msg.IDE;
		list[write_to].rx_header.RTR = Rx_Header_Msg.RTR;
		list[write_to].frame_received++;
		list[write_to].time_recv = HAL_GetTick();

		memcpy(list[write_to].data, CAN_RxData, 8);
		Rx_Header_Msg.StdId = 0;
		Rx_Header_Msg.ExtId = 0;
		Rx_Header_Msg.DLC = 0;
		Rx_Header_Msg.IDE = 0;
		Rx_Header_Msg.RTR = 0;
	}

}

void can_kirim(uint16_t id, uint8_t data[8]){
	Tx_Header_Msg.DLC = 8;
	Tx_Header_Msg.StdId = id;//0x1FF;
	Tx_Header_Msg.IDE = CAN_ID_STD;
	Tx_Header_Msg.RTR = CAN_RTR_DATA;

	for(int i = 0; i < 8; i++) CAN_TxData[i] = data[i];

	HAL_CAN_AddTxMessage(&hcan1, &Tx_Header_Msg, CAN_TxData, &CAN_TxMailbox);
}

void votol_can_req(CAN_HandleTypeDef* hcan){
	//c0 c1 (controller 1 & 2) sama
	Tx_Header_Msg.StdId = 1023;
	Tx_Header_Msg.ExtId = 0;
	Tx_Header_Msg.DLC = 8;
	Tx_Header_Msg.IDE = CAN_ID_STD;

	Tx_Header_Msg.RTR = CAN_RTR_DATA;

	CAN_TxData[0] = 9;
	CAN_TxData[1] = 85;
	CAN_TxData[2] = 170;
	CAN_TxData[3] = 170;
	CAN_TxData[4] = 0;
	CAN_TxData[5] = 170;
	CAN_TxData[6] = 0;
	CAN_TxData[7] = 0;

	HAL_CAN_AddTxMessage(hcan, &Tx_Header_Msg, CAN_TxData, &CAN_TxMailbox);

	CAN_TxData[0] = 0;
	CAN_TxData[1] = 24;
	CAN_TxData[2] = 170;
	CAN_TxData[3] = 5;
	CAN_TxData[4] = 210;
	CAN_TxData[5] = 0;
	CAN_TxData[6] = 32;
	CAN_TxData[7] = 51;

	HAL_CAN_AddTxMessage(hcan, &Tx_Header_Msg, CAN_TxData, &CAN_TxMailbox);
}

//motor 0

//id std = 0
//id ext = 270929954
//IDE 	= 4
//RTR = 0
//DLC = 8

//data[4] [5] -> battry voltage =  ([5] << 8 + [4])/10.0
//data[6] [7] -> battery current = ([7] << 8 + [6])/10.0




//----------------

//id std = 1022
//id ext = 0
//IDE = 0
//rtr = 0
//DLC = 8

// data [0] [1] -> rpm ([0] << 8 + [1]
// data[2]: converts into Dec - 50 is the Controller temp
// data[3]: converts into Dec - 50 is the External temp
// data[4] [5] -> converts into integer is the temperature coefficient ([7] << 8 + [8])
//data[6] ->
	//bit0~1= 0:L, 1:M, 2:H, 3:S
	//bit2=R
	//bit3=P
	//bit4=brake
	//bit5=antitheft
	//bit6=side stand
	//bit7=regen
//data[7] bit status (0=IDLE, 1=INIT, 2=START, 3=RUN, 4=STOP, 5=BRAKE, 6=WAIT, 7=FAULT)


//motor 1

//votol ngirim terus ?
//10 26 10 22	-> isi tegangan + arus
//10 26 10 23
//10 08 55 a0
//10 09 55 a0

//komputer yang req
//1023 ask  -> datanya ngga berubah
//1022 rep	-> isi rpm
