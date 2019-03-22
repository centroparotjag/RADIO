/*
 * RDA5807M.c
 *
 *  Created on: 11 июн. 2018 г.
 *  Author: Oleksander Lupul
 */


#include "header.h"

uint16_t REG_0, REG_2, REG_3, REG_4, REG_5, REG_6, REG_7, REG_A, REG_B, REG_C, REG_D, REG_E, REG_F;
extern 	uint16_t  NUM_V;

uint8_t S8, S7, S6, S5, S4, S3, S2, S1, COUNTER;
uint8_t T8, T7, T6, T5, T4, T3, T2, T1;



void CONFIG_RDA5807M (void){
	uint32_t i;


	//----------- Required Settings ------------------------------------------
	REG_2 &= 0xDFFF;			// Stereo
	REG_2  = 0xD00D;			// Audio Output _ON / NO_MUTE / Bass boost enabled/ RDS_EN / New Demodulate Method Enable /Power Up Enable
	REG_3  = 0x0000;			// Channel Spacing 00 = 100KHz
	REG_4  = 0x0000;			// AFC enable / SOFTMUTE disable
	REG_5  = 0x0870 | NUM_V;		// LNAN / 3.0mA -> see https://pichobby.lg.ua/downloads/cxemu/sound/24_02_17/RDA5807_prog.pdf
	REG_6  = 0x0000;			//
	REG_7  = 0x0000;			//
	//NUM_V = REG_5 & 0x0F;


	//-------------------- SOFT RESET RDA5807 -----------------------------------
	START_I2C(0x22);					// CHIP ADDRES
	TXB_I2C(0x02);						// REG ADDRES
	TXB_I2C(0x00);						// REGISTER BIT <15:8>
	TXB_I2C(0x01);						// REGISTER BIT <7:0>
	STOP_I2C();

	for (i = 0; i < 500000; ++i) {}		// 135 mS pause

	//------------------- WRITE REG 02h to 07h ---------------------------------
	START_I2C(0x22);					// CHIP ADDRES
	TXB_I2C(0x02);						// REG ADDRES 02h

	TXB_I2C(REG_2 >> 8);				// REGISTER 02H BIT <15:8>
	TXB_I2C(REG_2 & 0x00FF);			// REGISTER 02L BIT <7:0>

	TXB_I2C(REG_3 >> 8);				// REGISTER 03H BIT <15:8>
	TXB_I2C(REG_3 & 0x00FF);			// REGISTER 03L BIT <7:0>

	TXB_I2C(REG_4 >> 8);				// REGISTER 04H BIT <15:8>
	TXB_I2C(REG_4 & 0x00FF);			// REGISTER 04L BIT <7:0>

	TXB_I2C(REG_5 >> 8);				// REGISTER 05H BIT <15:8>
	TXB_I2C(REG_5 & 0x00FF);			// REGISTER 05L BIT <7:0>

	STOP_I2C();



}




//====================================================================

uint8_t RDSR, STC, SF, RDSS, ST, RSSI, FM_TRUE, FM_READY, ABCD_E, BLERA, BLERB, BAND;
uint16_t READCHAN;




void READ_ALL_REGISTER_RDA5807M (void){
	extern unsigned char BUF_READ_I2C[28];

	//------------ REED STATUS AND REGISTER ---------------------
	START_I2C(0x22);					// CHIP ADDRES
	TXB_I2C(0x02); 						// REGISTER
	STOP_I2C();
	I2C_Read(0x22,  28);

	//------------ DECODE STATUS / REGISTER --------------------
		BAND  = (BUF_READ_I2C[0x01] & 0x0C ) >> 2;
		RDSR =  BUF_READ_I2C[0x10] >> 7;
		STC  = (BUF_READ_I2C[0x10] & 0x40 ) >> 6;
		SF   = (BUF_READ_I2C[0x10] & 0x20 ) >> 5;
		RDSS = (BUF_READ_I2C[0x10] & 0x10 ) >> 4;
		ST   = (BUF_READ_I2C[0x10] & 0x04 ) >> 2;
		READCHAN = (BUF_READ_I2C[0x10] & 0x03 ) <<8; 	READCHAN = READCHAN + BUF_READ_I2C[0x11];
		RSSI = BUF_READ_I2C[0x12] >> 1;
		FM_TRUE = BUF_READ_I2C[0x12] & 0x01;
		FM_READY = (BUF_READ_I2C[0x13] & 0x40 ) >> 6;
		ABCD_E = (BUF_READ_I2C[0x13] & 0x08 ) >> 4;
		BLERA = (BUF_READ_I2C[0x13] & 0x0C ) >> 2;
		BLERB =  BUF_READ_I2C[0x13] & 0x03;


}









//============================================================================================
//================================ FREQUENCY TUNE SET ========================================
//============================================================================================
void FREQ_TUNE_SET (uint16_t FREQ){
	uint16_t FREQ_REG_3, FREQ_REG_7, CHAN;

	//====================== 50...65 MHz ========================
	if (FREQ <= 650) {
		FREQ_REG_7 = 0;						// 0 = 50~76 MHz, [9]-65M_50M MODE

		START_I2C(0x22);					// CHIP ADDRES
		TXB_I2C(0x07);						// REG ADDRES 02h
		TXB_I2C(FREQ_REG_7 >> 8);			// REGISTER 02H BIT <15:8>
		TXB_I2C(FREQ_REG_7 & 0x00FF);		// REGISTER 02L BIT <7:0>
		STOP_I2C();


		//CHAN = (FREQ - 5000) / 5;			// BAND=3 Frequency=Channel Spacing (kHz)xCHAN+65.0MHz
		CHAN = FREQ - 500;			// BAND=3 Frequency=Channel Spacing (kHz)xCHAN+65.0MHz

		FREQ_REG_3 = 0x003F;

		FREQ_REG_3 |= (CHAN << 6)			// [15:6] CHAN
				   |  0x10					// [4] tune enabled
				   |  0x0C;					// [3:2] BAND 11 = 50-65MHz


		START_I2C(0x22);					// CHIP ADDRES
		TXB_I2C(0x03);						// REG ADDRES 03h
		TXB_I2C(FREQ_REG_3 >> 8);			// REGISTER 03H BIT <15:8>
		TXB_I2C(FREQ_REG_3 & 0x00FF);		// REGISTER 03L BIT <7:0>
		STOP_I2C();




	}
	else {}


	//====================== 65...76 MHz ========================
	if (FREQ > 650 && FREQ <= 760) {


		FREQ_REG_7 = 0x0200;				// 1 = 65~76 MHz, [9]-65M_50M MODE

		START_I2C(0x22);					// CHIP ADDRES
		TXB_I2C(0x07);						// REG ADDRES 02h
		TXB_I2C(FREQ_REG_7 >> 8);			// REGISTER 02H BIT <15:8>
		TXB_I2C(FREQ_REG_7 & 0x00FF);		// REGISTER 02L BIT <7:0>
		STOP_I2C();

		//CHAN = (FREQ - 6500) / 5;			// BAND=3 Frequency=Channel Spacing (kHz)xCHAN+65.0MHz
		CHAN = FREQ - 650;					// BAND=3 Frequency=Channel Spacing (kHz)xCHAN+65.0MHz

		FREQ_REG_3 = REG_3;

		FREQ_REG_3 &= 0x003F;
		FREQ_REG_3 |= (CHAN << 6)			// [15:6] CHAN
				   |  0x10					// [4] tune enabled
				   |  0x0C;					// [3:2] BAND 11 = 65 –76 MHz （ East Europe）


		START_I2C(0x22);					// CHIP ADDRES
		TXB_I2C(0x03);						// REG ADDRES 03h
		TXB_I2C(FREQ_REG_3 >> 8);			// REGISTER 03H BIT <15:8>
		TXB_I2C(FREQ_REG_3 & 0x00FF);		// REGISTER 03L BIT <7:0>
		STOP_I2C();


	}
	else {}


	//====================== 76...115 MHz ========================
	if (FREQ > 760) {

		FREQ_REG_3 = REG_3;

		//CHAN = (FREQ - 7600) / 5;			// BAND=3 Frequency=Channel Spacing (kHz)xCHAN+65.0MHz
		CHAN = FREQ - 760;					// BAND=3 Frequency=Channel Spacing (kHz)xCHAN+65.0MHz

		FREQ_REG_3 &= 0x003F;
		FREQ_REG_3 |= (CHAN << 6)			// [15:6] CHAN
				   |  0x10					// [4] tune enabled
				   |  0x08;					// [3:2] BAND 10 = 76–108 MHz (world wide)


		START_I2C(0x22);					// CHIP ADDRES
		TXB_I2C(0x03);						// REG ADDRES 03h
		TXB_I2C(FREQ_REG_3 >> 8);			// REGISTER 03H BIT <15:8>
		TXB_I2C(FREQ_REG_3 & 0x00FF);		// REGISTER 03L BIT <7:0>
		STOP_I2C();
	}
	else {}

	S8=S7=S6=S5=S4=S3=S2=S1=0;
}





//============================================================================================
//============================================================================================
//============================================================================================








void TUNE (uint16_t F){
	uint16_t FREQ_REG_3, CHAN;

	if (F < 870) {F=870;}
	else {}
	if (F > 1150) {F=1150;}
	else {}


	CHAN = F - 870;

	FREQ_REG_3 = REG_3;

	FREQ_REG_3 &= 0x003F;
	FREQ_REG_3 |= (CHAN << 6)		// [15:6] CHAN
	           |  0x10;				// [4] tune enabled

	START_I2C(0x22);				// CHIP ADDRES
	TXB_I2C(0x03);					// REG ADDRES 03h
	TXB_I2C(FREQ_REG_3 >> 8);		// REGISTER 03H BIT <15:8>
	TXB_I2C(FREQ_REG_3 & 0x00FF);	// REGISTER 03L BIT <7:0>
	STOP_I2C();

}




void VOLUME_RDA5807M(uint8_t VOLUME){

	REG_5 = REG_5 & 0xFFF0;
	REG_5 = REG_5 + VOLUME;

	START_I2C(0x22);					// CHIP ADDRES
	TXB_I2C(0x05); 						// 05H
	TXB_I2C(REG_5  >> 8);     			// 05H
	TXB_I2C(REG_5  & 0x00FF); 			// 05L
	STOP_I2C();

	if (VOLUME == 0) {
		REG_2 = REG_2 & 0xBFFF;				// Mute enable

		START_I2C(0x22);					// CHIP ADDRES
		TXB_I2C(0x02); 						// 05H
		TXB_I2C(REG_2 >> 8);     // 02H
		TXB_I2C(REG_2 & 0x00FF); // 02L
		STOP_I2C();
	}
	else {
		REG_2 = REG_2 | 0x4000;				// Mute disable

		START_I2C(0x22);					// CHIP ADDRES
		TXB_I2C(0x02); 						// 05H
		TXB_I2C(REG_2 >> 8);     // 02H
		TXB_I2C(REG_2 & 0x00FF); // 02L
		STOP_I2C();

	}



}


//-------------- READ ------------------------------------------

void LCD_RSSI_ST_FMTRUE (void){

	uint8_t SYMBOL5[8];
    uint8_t SYMBOL6[8];

    //-------------- DEFAULT RSSI BAR GRAPH (FULL) ---------------------------------
	SYMBOL5[0]=0x0C; SYMBOL5[1]=0x14; SYMBOL5[2]=0x1c; SYMBOL5[3]=0x14;
	SYMBOL5[4]=0x01; SYMBOL5[5]=0x03; SYMBOL5[6]=0x07; SYMBOL5[7]=0x0f;

	SYMBOL6[0]=0x02; SYMBOL6[1]=0x06; SYMBOL6[2]=0x0e; SYMBOL6[3]=0x1e;
	SYMBOL6[4]=0x1e; SYMBOL6[5]=0x1e; SYMBOL6[6]=0x1e; SYMBOL6[7]=0x1e;

	//-------------- LCD PRINT RSSI BAR GRAPH ---------------------------------------
    if (RSSI >= 40){ }
    else{
        if (RSSI >= 35){
        	SYMBOL6[0]=0x00; SYMBOL6[1]=0x04; SYMBOL6[2]=0x0c; SYMBOL6[3]=0x1c;
        	SYMBOL6[4]=0x1c; SYMBOL6[5]=0x1c; SYMBOL6[6]=0x1c; SYMBOL6[7]=0x1c;
        }
        else {
            if (RSSI >= 30){
            	SYMBOL6[0]=0x00; SYMBOL6[1]=0x00; SYMBOL6[2]=0x08; SYMBOL6[3]=0x18;
            	SYMBOL6[4]=0x18; SYMBOL6[5]=0x18; SYMBOL6[6]=0x18; SYMBOL6[7]=0x18;
            }
            else {
                if (RSSI >= 25){
                	SYMBOL6[0]=0x00; SYMBOL6[1]=0x00; SYMBOL6[2]=0x00; SYMBOL6[3]=0x10;
                	SYMBOL6[4]=0x10; SYMBOL6[5]=0x10; SYMBOL6[6]=0x10; SYMBOL6[7]=0x10;
                }
                else {
                    if (RSSI >= 20){
                    	SYMBOL6[0]=0x00; SYMBOL6[1]=0x00; SYMBOL6[2]=0x00; SYMBOL6[3]=0x00;
                    	SYMBOL6[4]=0x00; SYMBOL6[5]=0x00; SYMBOL6[6]=0x00; SYMBOL6[7]=0x00;
                    }
                    else {

                        if (RSSI >= 15){
							//SYMBOL5[0]=0x00; SYMBOL5[1]=0x00; SYMBOL5[2]=0x00; SYMBOL5[3]=0x00;
							SYMBOL5[4]=0x00; SYMBOL5[5]=0x02; SYMBOL5[6]=0x06; SYMBOL5[7]=0x0e;

							SYMBOL6[0]=0x00; SYMBOL6[1]=0x00; SYMBOL6[2]=0x00; SYMBOL6[3]=0x00;
							SYMBOL6[4]=0x00; SYMBOL6[5]=0x00; SYMBOL6[6]=0x00; SYMBOL6[7]=0x00;
                        }
                        else {
                            if (RSSI >= 10){
    							//SYMBOL5[0]=0x00; SYMBOL5[1]=0x00; SYMBOL5[2]=0x00; SYMBOL5[3]=0x00;
    							SYMBOL5[4]=0x00; SYMBOL5[5]=0x00; SYMBOL5[6]=0x04; SYMBOL5[7]=0x0C;

    							SYMBOL6[0]=0x00; SYMBOL6[1]=0x00; SYMBOL6[2]=0x00; SYMBOL6[3]=0x00;
    							SYMBOL6[4]=0x00; SYMBOL6[5]=0x00; SYMBOL6[6]=0x00; SYMBOL6[7]=0x00;
                            }
                            else {
                                if (RSSI >= 5){
									//SYMBOL5[0]=0x00; SYMBOL5[1]=0x00; SYMBOL5[2]=0x00; SYMBOL5[3]=0x00;
									SYMBOL5[4]=0x00; SYMBOL5[5]=0x00; SYMBOL5[6]=0x00; SYMBOL5[7]=0x08;

									SYMBOL6[0]=0x00; SYMBOL6[1]=0x00; SYMBOL6[2]=0x00; SYMBOL6[3]=0x00;
									SYMBOL6[4]=0x00; SYMBOL6[5]=0x00; SYMBOL6[6]=0x00; SYMBOL6[7]=0x00;
                                 }
                                else {
    								//SYMBOL5[0]=0x00; SYMBOL5[1]=0x00; SYMBOL5[2]=0x00; SYMBOL5[3]=0x00;
									SYMBOL5[4]=0x00; SYMBOL5[5]=0x00; SYMBOL5[6]=0x00; SYMBOL5[7]=0x00;

									SYMBOL6[0]=0x00; SYMBOL6[1]=0x00; SYMBOL6[2]=0x00; SYMBOL6[3]=0x00;
									SYMBOL6[4]=0x00; SYMBOL6[5]=0x00; SYMBOL6[6]=0x00; SYMBOL6[7]=0x00;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

	SYMBOL5x8_PROGRAM (SYMBOL5, 5);
	SYMBOL5x8_PROGRAM (SYMBOL6, 6);

	InstallationCursor (14, 1);
	TRANSMITH_DATA_KS0066 (4);			// ANTENNA
	TRANSMITH_DATA_KS0066 (5);
	TRANSMITH_DATA_KS0066 (6);

	//=============== FM TRUE ==========================
	if (FM_TRUE == 1 ){
			InstallationCursor (9, 1);
			TRANSMITH_DATA_KS0066 (1);			// D_L
			InstallationCursor (11, 1);
			TRANSMITH_DATA_KS0066 (3);			// D_R
			LED_ON;
	}
	else {
			InstallationCursor (9, 1);
			TRANSMITH_DATA_KS0066 (' ');
			InstallationCursor (11, 1);
			TRANSMITH_DATA_KS0066 (' ');
			LED_OFF;
	}

	//============= STEREO ==============================
	if (ST == 1){
			InstallationCursor (10, 1);
			TRANSMITH_DATA_KS0066 (2);			// ST
	}
	else {
			InstallationCursor (10, 1);
			TRANSMITH_DATA_KS0066 (' ');
	}
}


//==========================================     RDS      ======================================================

//--------------------------------------------------------------------------------------------------------------
uint8_t VALID_SYMBOL_ISO646 (uint8_t S){

	if (S=='0' || S=='1' || S=='2' || S=='3' || S=='4' || S=='5' || S=='6' || S=='7' || S=='8' || S=='9' ||
		S=='A' || S=='B' || S=='C' || S=='D' ||	S=='E' || S=='F' || S=='G' || S=='H' || S=='I' || S=='J' ||
		S=='K' || S=='L' || S=='M' || S=='N' || S=='O' || S=='P' || S=='Q' || S=='R' || S=='S' || S=='T' ||
		S=='U' || S=='V' || S=='W' || S=='X' || S=='Y' || S=='Z' ||
		S==' ' || S=='\''|| S==',' || S=='-' ||S=='.'  ||S=='/')	{	return S;	}
	else {	return 0;	}
}


//------------------------------------------------------------------------------------------------------------


uint8_t RDS_DECODE (void){
	extern uint8_t RDSR,  RDSS;
	extern unsigned char BUF_READ_I2C[28];
	uint16_t PI, BLOCK_2;
	uint8_t GTYPE,  PTY, OBMIN;		//, DI, APP B0,TP,

	NVIC_DisableIRQ(TIM15_IRQn); 		// Disable Interrupt

//=========================================================================================
//			InstallationCursor (1, 2);
//			PrintString ("               " );
			PI = BUF_READ_I2C[0x0E]; PI = PI + BUF_READ_I2C[0x0F];
			BLOCK_2 = BUF_READ_I2C[0x10]<<8; BLOCK_2 = BLOCK_2 + BUF_READ_I2C[0x11];
			//BLOCK_3 = BUF_READ_I2C[0x12]; BLOCK_3 = BLOCK_3 + BUF_READ_I2C[0x13];
			//BLOCK_4 = BUF_READ_I2C[0x14]; BLOCK_4 = BLOCK_4 + BUF_READ_I2C[0x15];
			//DI=(BUF_READ_I2C[0x17] & 0x04)>>2;

			GTYPE = BUF_READ_I2C[0x16] >> 4;
			//B0 = (BUF_READ_I2C[0x16] & 0x08) >> 3;
			//TP = (BUF_READ_I2C[0x16] & 0x04) >> 2;

			OBMIN = (BUF_READ_I2C[0x16] & 0x03) << 3; PTY = (BUF_READ_I2C[0x17] & 0xE0) >> 5;
			PTY = PTY | OBMIN;

			//APP = BUF_READ_I2C[0x17] & 0x0F;


//======================= RDS decoder not synchronized  ===============================================================
			if (RDSS == 0){
				InstallationCursor (1, 2);
				PrintString ("                ");
			}
			else {
				InstallationCursor (2, 2);
				TRANSMITH_DATA_KS0066 ('R');
				TRANSMITH_DATA_KS0066 (0xA5);
				TRANSMITH_DATA_KS0066 ('D');
				TRANSMITH_DATA_KS0066 (0xA5);
				TRANSMITH_DATA_KS0066 ('S');
			}

				//         test
//				InstallationCursor (1, 2);
//				TRANSMITH_DATA_KS0066 (DI+0x30);


//========================== Type 0 groups: Basic tuning and switching information ====================================
			if (GTYPE==0 && RDSS==1){		//RDSR==1 && && BLERA && BLERB



				//-------------- S8 -------------------------------------------------
				if ((BUF_READ_I2C[0x17] & 0x03) == 0 && S8==0){	S8=VALID_SYMBOL_ISO646 (BUF_READ_I2C[0x1A]);}	else {}
				//-------------- S7 -------------------------------------------------
				if ((BUF_READ_I2C[0x17] & 0x03) == 0 && S7==0){	S7=VALID_SYMBOL_ISO646 (BUF_READ_I2C[0x1B]);}	else {}
				//-------------- S6 -------------------------------------------------
				if ((BUF_READ_I2C[0x17] & 0x03) == 1 && S6==0){	S6=VALID_SYMBOL_ISO646 (BUF_READ_I2C[0x1A]);}	else {}
				//-------------- S5 -------------------------------------------------
				if ((BUF_READ_I2C[0x17] & 0x03) == 1 && S5==0){	S5=VALID_SYMBOL_ISO646 (BUF_READ_I2C[0x1B]);}	else {}
				//-------------- S4 -------------------------------------------------
				if ((BUF_READ_I2C[0x17] & 0x03) == 2 && S4==0){	S4=VALID_SYMBOL_ISO646 (BUF_READ_I2C[0x1A]);}	else {}
				//-------------- S3 -------------------------------------------------
				if ((BUF_READ_I2C[0x17] & 0x03) == 2 && S3==0){	S3=VALID_SYMBOL_ISO646 (BUF_READ_I2C[0x1B]);}	else {}
				//-------------- S2 -------------------------------------------------
				if ((BUF_READ_I2C[0x17] & 0x03) == 3 && S2==0){	S2=VALID_SYMBOL_ISO646 (BUF_READ_I2C[0x1A]);}	else {}
				//-------------- S1 -------------------------------------------------
				if ((BUF_READ_I2C[0x17] & 0x03) == 3 && S1==0){	S1=VALID_SYMBOL_ISO646 (BUF_READ_I2C[0x1B]);}	else {}




				//--------------------- LCD - RADIO STANCION --------------------------------
				if (S8!=0 && S7!=0 && S6!=0 && S5!=0 && S4!=0 && S3!=0 && S2!=0 && S1!=0 ){

					if (S8==T8 && S7==T7 && S6==T6 && S5==T5 && S4==T4 && S3==T3 && S2==T2 && S1==T1 ){ COUNTER++;}
					else {T8=S8; T7=S7; T6=S6; T5=S5; T4=S4; T3=S3; T2=S2; T1=S1; COUNTER=0;}

					if (COUNTER >= 2){

						InstallationCursor (9, 2);
						TRANSMITH_DATA_KS0066 (S8);
						TRANSMITH_DATA_KS0066 (S7);
						TRANSMITH_DATA_KS0066 (S6);
						TRANSMITH_DATA_KS0066 (S5);
						TRANSMITH_DATA_KS0066 (S4);
						TRANSMITH_DATA_KS0066 (S3);
						TRANSMITH_DATA_KS0066 (S2);
						TRANSMITH_DATA_KS0066 (S1);
						S8=0; S7=0; S6=0; S5=0; S4=0; S3=0; S2=0; S1=0;
						COUNTER=0;
					}
					else {}

				}
				else {}
				//----------------------------------------------------------------------


			}
			else {}
//=================================================================================================================





//========================== Type 4 groups: Version A - Clock Time and Data ====================================
//			uint8_t HOUR, MIN, LOS, OFFSET;
//			uint8_t NUM2, NUM1, NUM;
//
//			if (GTYPE==4 && RDSS==1 && RDSR==1 && B0==0){		//RDSR==1 && && BLERA && BLERB
//
//				LOS = (BUF_READ_I2C[0x1B] & 0x20) >> 5;
//				OFFSET = BUF_READ_I2C[0x1B] & 0x1F;
//
//				//--------------- UTC --------------------
//				OBMIN = (BUF_READ_I2C[0x19] & 0x01) << 4;
//				HOUR = OBMIN + (BUF_READ_I2C[0x1A] >> 4);
//
//				OBMIN = (BUF_READ_I2C[0x1A] & 0x0F) << 2;
//				MIN = OBMIN + ((BUF_READ_I2C[0x1B] & 0xC0) >> 6);
//
//				//-------- Local Time Offset -------------
////				if (LOS == 0){	HOUR = HOUR + OFFSET;}
////				else {HOUR = HOUR - OFFSET;}
//
//
//				//---- extraction grade ----
//				NUM = HOUR;
//				NUM1 = NUM % 10;
//				NUM /= 10;
//				NUM2 = NUM;
//
//				InstallationCursor (1, 2);
//				TRANSMITH_DATA_KS0066 (NUM2 + 0x30);
//				TRANSMITH_DATA_KS0066 (NUM1 + 0x30);
//				TRANSMITH_DATA_KS0066 (':');
//
//				NUM = MIN;
//				NUM1 = NUM % 10;
//				NUM /= 10;
//				NUM2 = NUM;
//
//				TRANSMITH_DATA_KS0066 (NUM2 + 0x30);
//				TRANSMITH_DATA_KS0066 (NUM1 + 0x30);
//
//
//			}
//			else {
////				InstallationCursor (1, 2);
////				PrintString ("     ");
//			}



			NVIC_EnableIRQ(TIM15_IRQn); 		// Enable Interrupt


	return 1;
}





