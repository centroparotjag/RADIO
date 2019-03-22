/*
 * MENU.c
 *
 *  Created on: 12 июн. 2018 г.
 *  Author: Oleksander Lupul
 */


#include "header.h"

uint16_t FREQUENCY_RECEIVE;
uint8_t  NUMF, NUMV;
uint16_t NUM_V, NUM_F;

//=====================================================================
#define DAT0	__DATE__[0]
#define DAT1	__DATE__[1]
#define DAT2	__DATE__[2]
#define DAT4	__DATE__[4]
#define DAT5	__DATE__[5]
#define DAT9	__DATE__[9]
#define DAT10	__DATE__[10]

#define TIME0	__TIME__[0]
#define TIME1	__TIME__[1]
#define TIME3	__TIME__[3]
#define TIME4	__TIME__[4]


void MENU_STARTUP (uint8_t SET){
	uint8_t YEAR, DAY, MONTH, MIN, HOUR;
	uint8_t S10, S9, S8, S7, S6, S5, S4, S3, S2, S1;

	YEAR = (DAT9-0x30)*10+ (DAT10-0x30);
	if (DAT4 == 0x20)   {	DAY  = (DAT5 -0x30);	}
	else {	DAY  = (DAT4-0x30)*10+ (DAT5 -0x30);	}

	if (DAT0=='J' && DAT1=='a'					) 	MONTH=1;
	if (DAT0=='F' 								) 	MONTH=2;
	if (DAT0=='M' && DAT1=='a' && DAT2=='r'		)	MONTH=3;
	if (DAT0=='A' && DAT1=='p'					) 	MONTH=4;
	if (DAT0=='M' && DAT1=='a' && DAT2=='y'		)	MONTH=5;
	if (DAT0=='J' && DAT1=='u' && DAT2=='n'		)	MONTH=6;
	if (DAT0=='J' && DAT1=='u' && DAT2=='l'		)	MONTH=7;
	if (DAT0=='A' && DAT1=='u'					) 	MONTH=8;
	if (DAT0=='S' 								) 	MONTH=9;
	if (DAT0=='O' 								) 	MONTH=10;
	if (DAT0=='N' 								) 	MONTH=11;
	if (DAT0=='D' 								) 	MONTH=12;
	else {}

	HOUR = (TIME0-0x30)*10+ (TIME1-0x30);
	MIN  = (TIME3-0x30)*10+ (TIME4-0x30);


	S1 = MIN % 10;
	S2 = MIN / 10;

	S3 = HOUR % 10;
	S4 = HOUR / 10;

	S5 = YEAR % 10;
	S6 = YEAR / 10;

	S7 = MONTH % 10;
	S8 = MONTH / 10;

	S9 = DAY % 10;
	S10= DAY / 10;

	//====================================================
	if (SET==0){
		InstallationCursor (1, 1);			// UP string
		//           "----------------"
		PrintString ("  FM RDS RADIO  ");
	}
	else {
		InstallationCursor (1, 1);			// MENU=4
		//           "----------------"
		PrintString ("FIRMWARE VERSION");
	}





	InstallationCursor (1, 2);			// DOWN string
	//           "----------------"
	//PrintString (" 13.06.18 20:46 ");

	TRANSMITH_DATA_KS0066 (' ');
	TRANSMITH_DATA_KS0066 (S10+ 0x30);
	TRANSMITH_DATA_KS0066 (S9 + 0x30);
	TRANSMITH_DATA_KS0066 ('-');
	TRANSMITH_DATA_KS0066 (S8+ 0x30);
	TRANSMITH_DATA_KS0066 (S7 + 0x30);
	TRANSMITH_DATA_KS0066 ('-');
	TRANSMITH_DATA_KS0066 (S6+ 0x30);
	TRANSMITH_DATA_KS0066 (S5 + 0x30);
	TRANSMITH_DATA_KS0066 (' ');
	TRANSMITH_DATA_KS0066 (S4+ 0x30);
	TRANSMITH_DATA_KS0066 (S3 + 0x30);
	TRANSMITH_DATA_KS0066 (':');
	TRANSMITH_DATA_KS0066 (S2+ 0x30);
	TRANSMITH_DATA_KS0066 (S1 + 0x30);
	TRANSMITH_DATA_KS0066 (' ');
}







void MENU_FREQ_1 (void){
	uint32_t i;

	NVIC_DisableIRQ(TIM15_IRQn); 				// Disable Interrupt

		if( NUMF == 0 ) {}
		else {

			for (i = 0; i < 30000; ++i) {}

			if( NUMF == 1 ) {
				if (FREQUENCY_RECEIVE>=1080){FREQUENCY_RECEIVE=650;}
				else {FREQUENCY_RECEIVE += 1;}
				FREQ_TUNE_SET (FREQUENCY_RECEIVE);
				}
			else {}

			if( NUMF == 2 ){
				if (FREQUENCY_RECEIVE>=651){
					FREQUENCY_RECEIVE -= 1;
					FREQ_TUNE_SET (FREQUENCY_RECEIVE);
				}
				else {
					FREQUENCY_RECEIVE=1080;
				}
			}
			else {}

			FREQ_SKALE (FREQUENCY_RECEIVE);
			InstallationCursor (7, 1);
			PrintString ("          ");
		}


		NVIC_EnableIRQ(TIM15_IRQn); 				// Enable Interrupt

}



extern uint64_t LIC_OUT_MENU;


void MENU_VOLUME_1 (void){
	uint32_t i;
	NVIC_DisableIRQ(TIM15_IRQn); 				// DisableIRQ Interrupt



	if( NUMV == 0 ) {}
	else {

		for (i = 0; i < 100000; ++i) {}
		LIC_OUT_MENU=0;

		if( NUMV == 1 ) {				//+
			if (NUM_V>=15){NUM_V=15;}
			else {NUM_V++;}
		}
		else {							//-
			if (NUM_V>=1){NUM_V--;}
			else {}
			}
		VOLUME_RDA5807M(NUM_V);
		VOLUME_SKALE (NUM_V);
	}

	TIM15 -> CNT = 0;							// TIM counter register 16 bit
	TIM15->SR &= ~TIM_SR_UIF; 					// —брасываем флаг UIF

//	NVIC_EnableIRQ(TIM15_IRQn); 				// Enable Interrupt

}




uint8_t SEEK_COMPLETE (void){
	extern uint8_t  STC;
	extern uint16_t READCHAN;
	extern 	uint16_t FREQUENCY_RECEIVE;
	extern uint16_t REG_2;
	uint32_t F, i;
	uint32_t KEY, ANIME;
	KEY = 0; ANIME = 0;
	uint16_t  SEEK_REG2;

	extern uint8_t S8, S7, S6, S5, S4, S3, S2, S1;
	S8=S7=S6=S5=S4=S3=S2=S1=0;

	InstallationCursor (7, 1);
	PrintString (" SEEK  ");
	InstallationCursor (15, 1);
	PrintString ("  ");

	uint8_t SYMBOL1[8]={0x0,0x7,0x8,0x10,0x10,0x10,0x0,0x0};
	uint8_t SYMBOL2[8]={0x0,0x0,0x0,0x3,0x4,0x4,0x0,0x0};
	uint8_t SYMBOL3[8]={0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0};

	SYMBOL5x8_PROGRAM (SYMBOL1, 5);
	SYMBOL5x8_PROGRAM (SYMBOL2, 6);
	SYMBOL5x8_PROGRAM (SYMBOL3, 7);


//=========== SEEK KOMAND ===============================
	if (FREQUENCY_RECEIVE < 760){
		SEEK_REG2 = REG_2 | 0x0380;			// 0x0380-SEEK UP;  0x0180-SEEK DOWN;
		START_I2C(0x22);					// CHIP ADDRES
		TXB_I2C(0x03);						// REG ADDRES 02h
		TXB_I2C(0x00);						// REGISTER 03H BIT <15:8>
		TXB_I2C(0x08);						// REGISTER 03L BIT <7:0>  10 = 76Ц108 MHz (world wide)
		STOP_I2C();
	}
	else {	}


	SEEK_REG2 = REG_2 | 0x0380;			// 0x0380-SEEK UP;  0x0180-SEEK DOWN;
	START_I2C(0x22);					// CHIP ADDRES
	TXB_I2C(0x02);						// REG ADDRES 02h
	TXB_I2C(SEEK_REG2 >> 8);			// REGISTER 02H BIT <15:8>
	TXB_I2C(SEEK_REG2 & 0x00FF);		// REGISTER 02L BIT <7:0>
	STOP_I2C();



//==============================================================
	while (KEY < 150 ){						// 15 sec auto exit
		for (i = 0; i < 300000; ++i) {}	// 5000000

		READ_ALL_REGISTER_RDA5807M ();

		if ( STC == 1) { break; }
		else { }

		KEY++;

		F = READCHAN + 760;

		FREQUENCY_RECEIVE = F;

		FREQ_SKALE (F);

		//----------- ANIMATION ----------------------
		if (ANIME <= 1) {
			InstallationCursor (15, 1);
			TRANSMITH_DATA_KS0066 (7);
		} else {}


		if (ANIME == 2) {
			InstallationCursor (15, 1);
			TRANSMITH_DATA_KS0066 (6);
		} else {}

		if (ANIME == 3) {
			InstallationCursor (15, 1);
			TRANSMITH_DATA_KS0066 (5);
			ANIME = 0;
		} else {}


		ANIME++;
	}
	//=====================================================


	InstallationCursor (7, 1);
	PrintString ("          ");


	READ_ALL_REGISTER_RDA5807M ();


	F = READCHAN + 760;
	FREQUENCY_RECEIVE = F;

	FREQ_SKALE (F);

return 0;

}


void MENU_SEEK_1 (void){
	NVIC_DisableIRQ(TIM15_IRQn); 		// Disable Interrupt
	LED_OFF;
	SEEK_COMPLETE();
	NVIC_EnableIRQ(TIM15_IRQn); 				// Enable Interrupt
}




void READ_LCD_WORK_TIME (void){

	uint32_t H8, H7, H6, H5, H4, H3, H2, H1, M2, M1;
	uint32_t WORK_HOUR, WORK_MIN;
	uint32_t WORK_TIME;

	WORK_TIME = EE_READ_WORK_TIME ();

	if (WORK_TIME < 60){
		WORK_HOUR = 0;
		WORK_MIN  = WORK_TIME;
	}
	else {
		WORK_HOUR = WORK_TIME / 60;
		WORK_MIN  = WORK_HOUR * 60;
		WORK_MIN  = WORK_TIME - WORK_MIN;
	}

	//--------- DECADE HOUR -----------------
	H1 = WORK_HOUR % 10;
	WORK_HOUR /= 10;
	H2 = WORK_HOUR % 10;
	WORK_HOUR /= 10;
	H3 = WORK_HOUR % 10;
	WORK_HOUR /= 10;
	H4 = WORK_HOUR % 10;
	WORK_HOUR /= 10;
	H5 = WORK_HOUR % 10;
	WORK_HOUR /= 10;
	H6 = WORK_HOUR % 10;
	WORK_HOUR /= 10;
	H7 = WORK_HOUR % 10;
	H8 = WORK_HOUR / 10;

	//--------- DECADE MIN -----------------
	M1 = WORK_MIN % 10;
	M2 = WORK_MIN / 10;

	//-----------------------------------------------
	InstallationCursor (1, 1);
	PrintString ("  HOURS  METER  ");
	InstallationCursor (1, 2);
	PrintString ("             H:m");
	InstallationCursor (1, 2);

	TRANSMITH_DATA_KS0066 (H8+0x30);
	TRANSMITH_DATA_KS0066 (H7+0x30);
	TRANSMITH_DATA_KS0066 (H6+0x30);
	TRANSMITH_DATA_KS0066 (H5+0x30);
	TRANSMITH_DATA_KS0066 (H4+0x30);
	TRANSMITH_DATA_KS0066 (H3+0x30);
	TRANSMITH_DATA_KS0066 (H2+0x30);
	TRANSMITH_DATA_KS0066 (H1+0x30);
	TRANSMITH_DATA_KS0066 (':');
	TRANSMITH_DATA_KS0066 (M2+0x30);
	TRANSMITH_DATA_KS0066 (M1+0x30);
}



