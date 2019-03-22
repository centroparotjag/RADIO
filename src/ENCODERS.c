/*
 * ENCODERS.c
 *
 *  Created on: 6 θών. 2018 γ.
 *  Author: Oleksander Lupul
 */

#include "header.h"

//======== ENCODER "VOLUME" STATE ================
uint16_t ENC_VOL;

uint8_t ENCODER_VOLUME (void){
	uint16_t E;
	E = TIM1->CNT;

	if (ENC_VOL-3 > E ) {ENC_VOL=100; TIM1->CNT=100;  return 1;}		// encoder +
	else {}
	if (ENC_VOL+3 < E ) {ENC_VOL=100; TIM1->CNT=100;  return 2;}		// encoder -
	else {}

	return 0;
}


//======== ENCODER "FREQUENCY" STATE ================
uint16_t ENC_FREQ;

uint8_t ENCODER_FREQ (void){
	uint16_t E;
	E = TIM3->CNT;

	if (ENC_FREQ-3 > E ) {ENC_FREQ=100; TIM3->CNT=100;  return 2;}		// encoder -
	else {}
	if (ENC_FREQ+3 < E ) {ENC_FREQ=100; TIM3->CNT=100;  return 1;}		// encoder +
	else {}

	return 0;
}



//===================== BUTTONS ENCODERS ==========================
uint8_t VOLUME_BUTTON (void){
	if ((GPIOA->IDR & GPIO_IDR_4)==GPIO_IDR_4)	{return 0;}
	else {return 1;}			//ENCODER VOLUME BUTTON PUSH
}

uint8_t FREQUENCY_BUTTON (void){
	if ((GPIOB->IDR & GPIO_IDR_2)==GPIO_IDR_2)	{return 0;}
	else {return 1;}			//ENCODER FREQUENCY BUTTON PUSH
}


//==================== BUTTON ====================================

uint8_t BUTTON (void){
	if ((GPIOB->IDR & GPIO_IDR_10)==GPIO_IDR_10)	{return 0;}
	else {return 1;}			// BUTTON PUSH
}



