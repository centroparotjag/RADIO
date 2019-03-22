/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  * @Author: Oleksander Lupul
  ******************************************************************************
*/


#include "header.h"


extern 	uint8_t  NUMF, NUMV;
extern 	uint16_t FREQUENCY_RECEIVE, NUM_V;

uint8_t  MENU;
uint32_t LIC_OUT_MENU;
uint8_t  VOLUME_EE;
uint16_t FREQ_EE, TIME;

int main(void)
{

	MENU=1; NUMF=3;				// start menu 1 FREQUENCY
	TIM3->CNT = 55;

	configuration ();

	//===================================================
	//=========== DEFAULT RESET EE =====================
	//===================================================
	if ( BUTTON() && VOLUME_BUTTON () && FREQUENCY_BUTTON() ) {		// ALL BUTTONS PUSH
		EEPROME_CLEAR_DEFAULT ();
	}
	else {}
	//===================================================
	//===================================================
	//===================================================

	uint32_t i;
	for ( i = 0; i < 10000; ++i) {}
	SHD_OFF;		// ON AMPLIFIER

	for ( i = 0; i < 3000000; ++i) {}
	EE_READ_FREQ_VOL();
	FREQ_TUNE_SET (FREQUENCY_RECEIVE);

	for (i = 0; i < 10000; ++i) {}

	VOLUME_RDA5807M(NUM_V);							// NUM_V

	TIM15 -> CNT = 0;							// TIM counter register 16 bit
	TIM15->SR &= ~TIM_SR_UIF; 					// —брасываем флаг UIF
	NVIC_EnableIRQ(TIM15_IRQn); 				// Enable Interrupt
	TIME=0;
	LIC_OUT_MENU=0;


	//=========================================================================================
	while (1){

		    //=============== MENU 1 FREQUENCY ===========================
			if ( MENU == 1 || 0 != NUMF )   { MENU_FREQ_1(); MENU = 1;}
			else {}
			NUMF = ENCODER_FREQ ();



			//=============== MENU 2 VOLUME ==============================
			if ( MENU == 2 || 0 != NUMV )   {
				MENU_VOLUME_1();
				MENU = 2;
				LIC_OUT_MENU++;

				if (LIC_OUT_MENU > 400000){
					MENU = 1; NUMF=3;
					InstallationCursor (0, 2);
					PrintString ("                ");
					LIC_OUT_MENU =0;
				} else {}
			}
			else {}
			NUMV = ENCODER_VOLUME ();


			//============== MENU TIME_WORK ============================
			if ( VOLUME_BUTTON () && MENU != 3){
				MENU = 3;
				LIC_OUT_MENU =0;
				READ_LCD_WORK_TIME ();
			}
			else {}

			if (MENU == 3){
				LIC_OUT_MENU++;
				if (LIC_OUT_MENU > 400000){
					Clear ();
					LIC_OUT_MENU=0;
					MENU=1; NUMF=3;				// return to menu 1 FREQUENCY
				}
				else {}
			}
			else {}


			//============== MENU VPO ============================
			if ( FREQUENCY_BUTTON() && MENU != 4){
				MENU = 4;
				LIC_OUT_MENU =0;
				MENU_STARTUP (1);
			}
			else {}

			if (MENU == 4){
				LIC_OUT_MENU++;
				if (LIC_OUT_MENU > 400000){
					Clear ();
					LIC_OUT_MENU=0;
					MENU=1; NUMF=3;				// return to menu 1 FREQUENCY
				}
				else {}
			}
			else {}


			//================ BUTTON CENTRAL ===========================
			if ( BUTTON() ){
				Clear ();
				LCD_RSSI_ST_FMTRUE ();
				MENU_SEEK_1 ();
				LCD_RSSI_ST_FMTRUE ();
				while (BUTTON()){}
			}
			else {}


	}
}




//=======================================================================================================
//============================== INTERRUPTION EVERY 1S TIM6 =============================================
//=======================================================================================================

void TIM15_IRQHandler (void){

	TIM15->SR &= ~TIM_SR_UIF; //—брасываем флаг UIF

	if (MENU == 1){ 		  // MENU=1 FREQUENCY
		READ_ALL_REGISTER_RDA5807M ();
		LCD_RSSI_ST_FMTRUE ();
		RDS_DECODE ();
	}
	else {}

	//------------ REWRITEN in EEPROME FREQUENCY and VOLUME --------------------------
	TIME++;					// +0.05sec

	if (TIME>1175){									// 1200 - 60 sec  (1175 поправка)
		//LED_OFF;
		NVIC_DisableIRQ(TIM15_IRQn); 				// Disable Interrupt
		TIME=0;

		//---------------- UPDATE WORK TIME ------------------------------------------
		EEPROME_INCREMENT_WORK_TIME ();
		//---------------- FREQUENCY and VOLUME EE update ----------------------------
		if ((VOLUME_EE != NUM_V) ||	(FREQ_EE != FREQUENCY_RECEIVE)	 ){
			VOLUME_EE = NUM_V; FREQ_EE = FREQUENCY_RECEIVE;
			EE_WR_FREQ_VOL(FREQUENCY_RECEIVE, NUM_V);		// written eeprome
		}
		else {}

		NVIC_EnableIRQ(TIM15_IRQn); 				// Enable Interrupt
	}
	else {}
}







