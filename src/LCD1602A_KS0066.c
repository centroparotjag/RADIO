
//LCD1602A_KS0066
//Author: Oleksander Lupul

#include "header.h"

void TRANSMITH_COMMAND_KS0066 (uint8_t cmd){
	uint16_t i;

	RS_0; E_0;
	for (i = 0; i < 200; ++i) {}				// 200delay 46mkS

	//--------- Transfer of most significant half-byte (1111 0000) ------------
	if ((cmd & 0b10000000) == 0b10000000) {D7_1;}
	else 								  {D7_0;}

	if ((cmd & 0b01000000) == 0b01000000) {D6_1;}
	else 								  {D6_0;}

	if ((cmd & 0b00100000) == 0b00100000) {D5_1;}
	else 								  {D5_0;}

	if ((cmd & 0b00010000) == 0b00010000) {D4_1;}
	else 								  {D4_0;}

	for (i = 0; i < 7; ++i) {}					// 7 delay 2mkS
	E_1; 										// ENABLE (read/write)
	for (i = 0; i < 7; ++i) {}					// 7 delay 2mkS
	E_0; 										// DISABLE (read/write)

	//--------- Transfer of last significant half-byte (0000 1111) ------------
	if ((cmd & 0b00001000) == 0b00001000) {D7_1;}
	else 								  {D7_0;}

	if ((cmd & 0b00000100) == 0b00000100) {D6_1;}
	else 								  {D6_0;}

	if ((cmd & 0b00000010) == 0b00000010) {D5_1;}
	else 								  {D5_0;}

	if ((cmd & 0b00000001) == 0b00000001) {D4_1;}
	else 								  {D4_0;}

	for (i = 0; i < 7; ++i) {}					// 7 delay 2mkS
	E_1; 										// ENABLE (read/write)
	for (i = 0; i < 7; ++i) {}					// 7 delay 2mkS
	E_0; 										// DISABLE (read/write)


	for (i = 0; i < 200; ++i) {}				// 200delay 46mkS
}


void TRANSMITH_DATA_KS0066 (uint8_t DATA){
	uint32_t i;

	RS_1; E_0;
	for (i = 0; i < 200; ++i) {}				// 200delay 46mkS

	//--------- Transfer of most significant half-byte (1111 0000) ------------
	if ((DATA & 0b10000000) == 0b10000000) {D7_1;}
	else 								  {D7_0;}

	if ((DATA & 0b01000000) == 0b01000000) {D6_1;}
	else 								  {D6_0;}

	if ((DATA & 0b00100000) == 0b00100000) {D5_1;}
	else 								  {D5_0;}

	if ((DATA & 0b00010000) == 0b00010000) {D4_1;}
	else 								  {D4_0;}

	for (i = 0; i < 7; ++i) {}					// 7 delay 2mkS
	E_1; 										// ENABLE (read/write)
	for (i = 0; i < 7; ++i) {}					// 7 delay 2mkS
	E_0; 										// DISABLE (read/write)

	//--------- Transfer of last significant half-byte (0000 1111) ------------
	if ((DATA & 0b00001000) == 0b00001000) {D7_1;}
	else 								  {D7_0;}

	if ((DATA & 0b00000100) == 0b00000100) {D6_1;}
	else 								  {D6_0;}

	if ((DATA & 0b00000010) == 0b00000010) {D5_1;}
	else 								  {D5_0;}

	if ((DATA & 0b00000001) == 0b00000001) {D4_1;}
	else 								  {D4_0;}

	for (i = 0; i < 7; ++i) {}					// 7 delay 2mkS
	E_1; 										// ENABLE (read/write)
	for (i = 0; i < 7; ++i) {}					// 7 delay 2mkS
	E_0; 										// DISABLE (read/write)


	for (i = 0; i < 200; ++i) {}				// 200delay 46mkS
}



void INIT_LCD1602A_KS0066 (void){
	uint32_t i;

	RS_0; E_0; D7_0; D6_0; D5_0; D4_0;

	for (i = 0; i < 200000; ++i) {}				// 200000 delay 54mS

	//================ AFTER POWER RESET ============================
	D7_0; D6_0; D5_1; D4_0;
	for (i = 0; i < 400; ++i){}	E_1; for (i = 0; i < 20; ++i){} E_0;
	for (i = 0; i < 400; ++i) {}				// 200delay 46mkS
	//===============================================================

	D7_0; D6_0; D5_1; D4_0;
	for (i = 0; i < 20; ++i){}	E_1; for (i = 0; i < 20; ++i){} E_0;
	for (i = 0; i < 400; ++i) {}				// 200delay 46mkS

	D7_1; D6_1; D5_0; D4_0;
	for (i = 0; i < 20; ++i){}	E_1; for (i = 0; i < 20; ++i){} E_0;
	for (i = 0; i < 400; ++i) {}				// 200delay 46mkS



//	for (i = 0; i < 300; ++i) {}				// 200 delay 46mkS
	TRANSMITH_COMMAND_KS0066 (0x0C);
	for (i = 0; i < 300; ++i) {}				// 200 delay 46mkS
	TRANSMITH_COMMAND_KS0066 (0x01);			// clear display
	for (i = 0; i < 7000; ++i) {}				// 6000delay 1.63mS
	TRANSMITH_COMMAND_KS0066 (0x06);			// Increment mode
	for (i = 0; i < 300; ++i) {}				// 200delay 46mkS


	//------------- WRITE CPEC SYMBOL ------------------------ https://www.quinapalus.com/hd44780udg.html
	uint8_t S0[8]={0x0,0x0,0xa,0x0,0x11,0xe,0x0,0x0};
	uint8_t S1[8]={0x0,0xe,0x15,0x17,0x11,0xe,0x0,0x0};
	uint8_t S2[8]={0x4,0xa,0xa,0xa,0x11,0x1f,0xe,0x0};
	uint8_t S3[8]={0x0,0x0,0xa,0x15,0x11,0xa,0x4,0x0};
	uint8_t S4[8]={0x0,0x1,0x3,0x16,0x1c,0x8,0x0,0x0};
	uint8_t S5[8]={0x1,0x3,0xf,0xf,0xf,0x3,0x1,0x0};
	uint8_t S6[8]={0x1f,0x11,0xa,0x4,0xa,0x1f,0x1f,0x0};
	uint8_t S7[8]={0x3,0x4,0x1e,0x8,0x1e,0x8,0x7,0x0};

	SYMBOL5x8_PROGRAM (S0, 0);
	SYMBOL5x8_PROGRAM (S1, 1);
	SYMBOL5x8_PROGRAM (S2, 2);
	SYMBOL5x8_PROGRAM (S3, 3);
	SYMBOL5x8_PROGRAM (S4, 4);
	SYMBOL5x8_PROGRAM (S5, 5);
	SYMBOL5x8_PROGRAM (S6, 6);
	SYMBOL5x8_PROGRAM (S7, 7);

}


/****************************** DISPLAY POSITION DDRAM ADDRES *******************************************
|	1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10  |  11  |  12  |  13  |  14  |  15  |  16  | = X
=========================================================================================================
|	00 |  01 |  02 |  03 |  04 |  05 |  06 |  07 |  08 |  09  |  0A  |  0B  |  0C  |  0D  |  0E  |  0F  | Y=1
---------------------------------------------------------------------------------------------------------
|	40 |  41 |  42 |  43 |  44 |  45 |  46 |  47 |  48 |  49  |  4A  |  4B  |  4C  |  4D  |  4E  |  4F  | Y=2
=========================================================================================================
****************************** 2-Line by 16-Character Display ******************************************/

void InstallationCursor (unsigned char x, unsigned char y)		// x-номер €чейки, Y-номер строки
{
	uint32_t i;
	unsigned char adr;
	x--;
	if (y > 1) {adr = x + 64;}
	else { adr = x;}

	adr = adr|0x80;
	TRANSMITH_COMMAND_KS0066 (adr); 			// Set DDRAM Address

	for (i = 0; i < 200; ++i) {}				// 200 delay 46mkS
}


void PrintString (char *sign )
{
          unsigned char i;
          for (i=0; i<40; i++)
          {
                  if (sign[i] == 0x00) {break;} 			// end strig-exit
                  else {TRANSMITH_DATA_KS0066 (sign[i]);}

           }

}


void Clear (void)
{
	uint16_t i;
	TRANSMITH_COMMAND_KS0066 (0x01);
	for (i = 0; i < 6000; ++i) {}				// 6000delay 1.63mS
}


//================= Program graphical characters 0-7 ========================

void SYMBOL5x8_PROGRAM (uint8_t *CGRAM_DATA, uint8_t ADDRES_0_7){

	uint8_t CGRAM_addres, i;

	CGRAM_addres = ADDRES_0_7 * 8;

	TRANSMITH_COMMAND_KS0066 (CGRAM_addres | 0x40);			// address 64-127

	for (i = 0; i < 8; ++i) {	TRANSMITH_DATA_KS0066 ( CGRAM_DATA[i] );	}		//data 5x8

}




//============= VOLUME SKALE ========================================
void FREQ_SKALE (uint16_t FREQ){
	uint16_t F, S4, S3, S2, S1;

	uint8_t SYMBOL0[8]={0x1f,0x11,0x17,0x11,0x17,0x17,0x17,0x1f};		// F
	uint8_t SYMBOL1[8]={0x10,0x18,0x1E,0x1E,0x1E,0x18,0x10,0x00};		// DINAMIK L
	uint8_t SYMBOL2[8]={0x10,0x1F,0x10,0x00,0x16,0x15,0x0D,0x00};		// ST
	uint8_t SYMBOL3[8]={0x1, 0x3, 0xf, 0xf, 0xf, 0x3, 0x1, 0x0};		// DINAMIK R
//    uint8_t SYMBOL4[8]={0x4,0x15,0xe, 0x4, 0x4, 0x4, 0x4, 0x4};		// ANTENNA
//    uint8_t SYMBOL4[8]={0x15,0x1f,0x0e, 0x04, 0x04, 0x04, 0x04, 0x04};	// ANTENNA2
    uint8_t SYMBOL4[8]={0x15,0x15,0x0e, 0x04, 0x04, 0x04, 0x04, 0x04};	// ANTENNA3

	SYMBOL5x8_PROGRAM (SYMBOL0, 0);
	SYMBOL5x8_PROGRAM (SYMBOL1, 1);
	SYMBOL5x8_PROGRAM (SYMBOL2, 2);
	SYMBOL5x8_PROGRAM (SYMBOL3, 3);
	SYMBOL5x8_PROGRAM (SYMBOL4, 4);


	//--------FREQUENCY DECADE -----
	F = FREQ;
	S1 = F % 10;
	F /= 10;
	S2 = F % 10;
	F /= 10;
	S3 = F % 10;
	F /= 10;
	S4 = F;


	//------- LCD FREQUENCY ------
	InstallationCursor (1, 1);
	if (S4 == 1){TRANSMITH_DATA_KS0066 (S4 + 0x30);}
	else {TRANSMITH_DATA_KS0066 (' ');}

	TRANSMITH_DATA_KS0066 (S3 + 0x30);
	TRANSMITH_DATA_KS0066 (S2 + 0x30);
	TRANSMITH_DATA_KS0066 ('.');			// "DOT
	TRANSMITH_DATA_KS0066 (S1 + 0x30);
//	TRANSMITH_DATA_KS0066 (S1 + 0x30);
//	TRANSMITH_DATA_KS0066 ('F');				// F
	TRANSMITH_DATA_KS0066 (0);				// F - invert
//	PrintString ("          ");







//
//	InstallationCursor (1, 2);
//	PrintString ("                ");


}



//============= VOLUME SKALE ========================================
void VOLUME_SKALE (uint8_t VOLUME){
	//------------- WRITE CPEC SYMBOL ------------------------ https://www.quinapalus.com/hd44780udg.html
	uint8_t S0[8]={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1f};
	uint8_t S1[8]={0x0,0x0,0x0,0x0,0x0,0x0,0x1f,0x1f};
	uint8_t S2[8]={0x0,0x0,0x0,0x0,0x0,0x1f,0x1f,0x1f};
	uint8_t S3[8]={0x0,0x0,0x0,0x0,0x1f,0x1f,0x1f,0x1f};
	uint8_t S4[8]={0x0,0x0,0x0,0x1f,0x1f,0x1f,0x1f,0x1f};
	uint8_t S5[8]={0x0,0x0,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f};
	uint8_t S6[8]={0x0,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f};
	uint8_t S7[8]={0x0,0x1,0x3,0xf,0xf,0xf,0x3,0x1};

	SYMBOL5x8_PROGRAM (S0, 0);
	SYMBOL5x8_PROGRAM (S1, 1);
	SYMBOL5x8_PROGRAM (S2, 2);
	SYMBOL5x8_PROGRAM (S3, 3);
	SYMBOL5x8_PROGRAM (S4, 4);
	SYMBOL5x8_PROGRAM (S5, 5);
	SYMBOL5x8_PROGRAM (S6, 6);
	SYMBOL5x8_PROGRAM (S7, 7);


	//---------------------------------
	InstallationCursor (1, 1);
	PrintString ("     VOLUME     ");

	InstallationCursor (1, 2);
	TRANSMITH_DATA_KS0066 (7);


	if (VOLUME==0){PrintString ("     MUTE      ");}
	else {}

	if (VOLUME>=1){	TRANSMITH_DATA_KS0066 (0);}
	else {TRANSMITH_DATA_KS0066 (0x20);}
	if (VOLUME>=2){	TRANSMITH_DATA_KS0066 (1);}
	else {TRANSMITH_DATA_KS0066 (0x20);}
	if (VOLUME>=3){	TRANSMITH_DATA_KS0066 (1);}
	else {TRANSMITH_DATA_KS0066 (0x20);}
	if (VOLUME>=4){	TRANSMITH_DATA_KS0066 (2);}
	else {TRANSMITH_DATA_KS0066 (0x20);}
	if (VOLUME>=5){	TRANSMITH_DATA_KS0066 (2);}
	else {TRANSMITH_DATA_KS0066 (0x20);}
	if (VOLUME>=6){	TRANSMITH_DATA_KS0066 (3);}
	else {TRANSMITH_DATA_KS0066 (0x20);}
	if (VOLUME>=7){	TRANSMITH_DATA_KS0066 (3);}
	else {TRANSMITH_DATA_KS0066 (0x20);}
	if (VOLUME>=8){	TRANSMITH_DATA_KS0066 (4);}
	else {TRANSMITH_DATA_KS0066 (0x20);}
	if (VOLUME>=9){	TRANSMITH_DATA_KS0066 (4);}
	else {TRANSMITH_DATA_KS0066 (0x20);}
	if (VOLUME>=10){TRANSMITH_DATA_KS0066 (5);}
	else {TRANSMITH_DATA_KS0066 (0x20);}
	if (VOLUME>=11){TRANSMITH_DATA_KS0066 (5);}
	else {PrintString (" ");}
	if (VOLUME>=12){TRANSMITH_DATA_KS0066 (6);}
	else {PrintString (" ");}
	if (VOLUME>=13){TRANSMITH_DATA_KS0066 (6);}
	else {PrintString (" ");}
	if (VOLUME>=14){TRANSMITH_DATA_KS0066 (0xFF);}
	else {PrintString (" ");}
	if (VOLUME>=15){TRANSMITH_DATA_KS0066 (0xFF);}
	else {PrintString (" ");}


}

