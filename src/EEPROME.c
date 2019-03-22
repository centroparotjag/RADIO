/*
 * EEPROME.c
 *
 *  Created on: 13 θών. 2018 γ.
 *  Author: Oleksander Lupul
 */

#include "header.h"

#define EE_ADR		0xA0

extern 	unsigned char BUF_READ_I2C[28];
extern 	uint16_t FREQUENCY_RECEIVE, NUM_V;

//------------- WRITE FREQUENCY VOLUME ------------------------
void EE_WR_FREQ_VOL(uint16_t FREQ, uint8_t VOL) {
	uint16_t FREQ_H, FREQ_L;

	FREQ_H = FREQ >> 8;
	FREQ_L = FREQ & 0xFF;

	START_I2C(EE_ADR);			// START
	TXB_I2C(0x00);				// Address MSB
	TXB_I2C(0x00);				// Address LSB
	TXB_I2C(FREQ_H );			// FREQ_H
	TXB_I2C(FREQ_L);		    // FREQ_L
	TXB_I2C(VOL);				// VOLUME
	STOP_I2C();
}



//------------- READ FREQUENCY VOLUME ------------------------


void EE_READ_FREQ_VOL(void) {

	START_I2C(EE_ADR);					// START
	TXB_I2C(0x00);						// Address MSB
	TXB_I2C(0x00);						// Address LSB
	STOP_I2C();

	I2C_Read(EE_ADR,  3);

	NUM_V = BUF_READ_I2C[2];			// VOLUME

	FREQUENCY_RECEIVE = BUF_READ_I2C[0];
	FREQUENCY_RECEIVE = FREQUENCY_RECEIVE << 8;
	FREQUENCY_RECEIVE |= BUF_READ_I2C[1];

	//=========== NEW EEPROME CORRECT (FIRST ON) ============
	if (NUM_V > 15) {NUM_V = 1;}									// volume=1. min.
	else {}

	if (FREQUENCY_RECEIVE > 1150) {FREQUENCY_RECEIVE = 1000;}		// 100.0MHz
	else {}


}





//------------- READ WORK TIME ------------------------

uint32_t EE_READ_WORK_TIME (void) {
	uint32_t TIME;
	START_I2C(EE_ADR);					// START
	TXB_I2C(0x00);						// Address MSB
	TXB_I2C(0x10);						// Address LSB
	STOP_I2C();

	I2C_Read(EE_ADR,  4);

	TIME = BUF_READ_I2C[0] << 24;
	TIME = TIME | (BUF_READ_I2C[1] << 16);
	TIME = TIME | (BUF_READ_I2C[2] << 8);
	TIME = TIME |  BUF_READ_I2C[3];

	return TIME;
}



//------------- WRITE WORK TIME ------------------------
void EEPROME_INCREMENT_WORK_TIME (void) {
	uint32_t T3, T2, T1, T0, TIME;

	TIME = EE_READ_WORK_TIME ();

	TIME = TIME + 1;

	T3 =  TIME >> 24;
	T2 =  TIME & 0x00FF0000; 		T2 = T2 >> 16;
	T1 = (TIME & 0x0000FF00); 		T1 = T1 >> 8;
	T0 =  TIME & 0x000000FF;


	START_I2C(EE_ADR);					// START
	TXB_I2C(0x00);						// Address MSB
	TXB_I2C(0x10);						// Address LSB
	TXB_I2C( T3 );
	TXB_I2C( T2 );
	TXB_I2C( T1 );
	TXB_I2C( T0 );
	STOP_I2C();

}



//=========================================================================
void PRINT_LCD_16B_HEX_DIGIT (uint16_t DIGIT);

void EEPROME_CLEAR_DEFAULT (void){
	uint32_t i, k;

	InstallationCursor (1, 1);			// UP string
	PrintString ("FM24CL64 default");


	LED_ON;

	START_I2C(EE_ADR);					// START
	TXB_I2C(0x00);						// Address MSB
	TXB_I2C(0x00);						// Address LSB

	for (i = 0; i < 129; ++i) {

		for (k = 0; k < 20000; ++k) {}
		PRINT_LCD_16B_HEX_DIGIT (i);
		TXB_I2C(0xFF);

	}

	STOP_I2C();

	PrintString (" OK");
	for (k = 0; k < 5000000; ++k) {}


	LED_OFF;
	Clear ();
}






void PRINT_LCD_16B_HEX_DIGIT (uint16_t DIGIT){
	uint16_t  D3, D2, D1, D0;

	 D3 =  DIGIT >> 12;
	 D2 = (DIGIT & 0x0F00) >> 8;
	 D1 = (DIGIT & 0x00F0) >> 4;
	 D0 =  DIGIT & 0x000F;


	InstallationCursor (1, 2);			// UP string
	PrintString ("write 0x");

	if (D3 < 10) {	TRANSMITH_DATA_KS0066 (D3 + 0x30);	}
	else 		  {	TRANSMITH_DATA_KS0066 (D3 + 0x37);	}

	if (D2 < 10) {	TRANSMITH_DATA_KS0066 (D2 + 0x30);	}
	else 		  {	TRANSMITH_DATA_KS0066 (D2 + 0x37);	}

	if (D1 < 10) {	TRANSMITH_DATA_KS0066 (D1 + 0x30);	}
	else 		  {	TRANSMITH_DATA_KS0066 (D1 + 0x37);	}

	if (D0 < 10) {	TRANSMITH_DATA_KS0066 (D0 + 0x30);	}
	else 		  {	TRANSMITH_DATA_KS0066 (D0 + 0x37);	}


}







