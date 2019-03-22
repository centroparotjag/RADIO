//Author: Oleksander Lupul

#include "header.h"

unsigned char ATTACH;


void UTX_BYTE(char ch){
	//Wait for buffer to be empty
  while ((USART2->ISR & USART_ISR_TXE) == 0){}			//Nop
  USART2->TDR = (ch);//Send character
}



int SER_GetChar(void){

  if (USART2->ISR & USART_ISR_RXNE)
    return (USART2->RDR);

  return (-1);
}


//--------------- UART TRANSMIT STRING -----------------------------------------
void UTX_STRING (char BUFFER[], unsigned char PERENOS_STR){

//    RCSTAbits.CREN = 0;         // Receive Enable bit:  1-EN / 0-DIS
//    RCREG;

    unsigned int i;
    i=0;


    while (BUFFER[i] != 0) {
        UTX_BYTE(BUFFER[i]);
        i++;
    }

    if (PERENOS_STR){UTX_BYTE(0x0A); UTX_BYTE(0x0D);}
    else {}

 //   RCSTAbits.CREN = 1;         // Receive Enable bit:  1-EN / 0-DIS
}


//==============================================================================
//==============================================================================
//  "AT/"   "HELP/" "MEMR/" "UPOW/"
//  "RAUSB/"
//==============================================================================
//==============================================================================


void URX_COMAND (char str []){
//    unsigned char i, k;
//    unsigned char DUMP [32];
//    char display[32];

//------------------------ RECCONECT CC-PD --------------------------------------
    if (str[16]=='R'&&str[17]=='E'&&str[18]=='C'&&str[19]=='O'&&
        str[20]=='N'&&str[21]=='N'&&str[22]=='E'&&str[23]=='C'&&str[24]=='T'){


    	UTX_STRING ("AT/RECCONECT-OK", 1);
    }
    else {}


//------------------------ DATA CONECTING  -----------------------------------
        if (str[16]=='C'&&str[17]=='O'&&str[18]=='N'&&str[19]=='F'&&
        	str[20]=='I'&&str[21]=='G'&&str[22]=='-'&&str[23]=='D'&&str[24]=='A'&&
        	str[25]=='T'&&str[26]=='A'){

			 extern unsigned long PACK3_WORD0, PACK3_WORD1, PACK3_WORD2,PACK3_WORD3,
								  PACK4_WORD0, PACK4_WORD1, PACK4_WORD2,PACK4_WORD3,
								  PACK5_WORD0, PACK5_WORD1, PACK5_WORD2,PACK5_WORD3;

			 PACK3_WORD0 = PACK3_WORD1 = PACK3_WORD2 = PACK3_WORD3 = PACK4_WORD0 =
			 PACK4_WORD1 = PACK4_WORD2 = PACK4_WORD3 = PACK5_WORD0 = PACK5_WORD1 =
			 PACK5_WORD2 = PACK5_WORD3 = 0;

			 PACK3_WORD0 = (str[32]<<24) | (str[33]<<16) | (str[34]<<8) | str[35];
			 PACK3_WORD1 = (str[36]<<24) | (str[37]<<16) | (str[38]<<8) | str[39];
			 PACK3_WORD2 = (str[40]<<24) | (str[41]<<16) | (str[42]<<8) | str[43];
			 PACK3_WORD3 = (str[44]<<24) | (str[45]<<16) | (str[46]<<8) | str[47];

			 PACK4_WORD0 = (str[48]<<24) | (str[49]<<16) | (str[50]<<8) | str[51];
			 PACK4_WORD1 = (str[52]<<24) | (str[53]<<16) | (str[54]<<8) | str[55];
			 PACK4_WORD2 = (str[56]<<24) | (str[57]<<16) | (str[58]<<8) | str[59];
			 PACK4_WORD3 = (str[60]<<24) | (str[61]<<16) | (str[62]<<8) | str[63];

			 PACK5_WORD0 = (str[64]<<24) | (str[65]<<16) | (str[66]<<8) | str[67];
			 PACK5_WORD1 = (str[68]<<24) | (str[69]<<16) | (str[70]<<8) | str[71];
			 PACK5_WORD2 = (str[72]<<24) | (str[73]<<16) | (str[74]<<8) | str[75];
			 PACK5_WORD3 = (str[76]<<24) | (str[77]<<16) | (str[78]<<8) | str[79];


			 UTX_STRING ("AT/CONFIG-DATA-OK", 1);




        }
        else {}
}


//===============================================================================
//===========================  INTERRUPT UART ===================================
//===============================================================================
char RX_BUF[90];
unsigned char COUNT_DATA;

//------------- UART TX interrupt ----------------------
void USART2_IRQHandler(void){
		unsigned char i;


		RX_BUF[COUNT_DATA] += USART2->RDR;

		COUNT_DATA ++;

		//-------------- CHECK "AT/INVERTEBRATE/" sequence ----------------------------------------------------
    	if (COUNT_DATA ==1 && (RX_BUF[0]!='A') ){COUNT_DATA=0; RX_BUF[0]=0;}
    	else {}
    	if (COUNT_DATA ==2 && (RX_BUF[0]!='A' || RX_BUF[1]!='T') ){COUNT_DATA=0; RX_BUF[0]=0; RX_BUF[1]=0; 	}
    	else{}
    	if (COUNT_DATA >=3 && (RX_BUF[0]!='A' || RX_BUF[1]!='T' || RX_BUF[2]!='/') ){COUNT_DATA=0; RX_BUF[0]=0; RX_BUF[1]=0; RX_BUF[2]=0;}
    	else {}

    	if (COUNT_DATA >=16 && (RX_BUF[0]!='A' || RX_BUF[1]!='T' || RX_BUF[2]!='/' || RX_BUF[3]!='I' ||
    							RX_BUF[4]!='N' || RX_BUF[5]!='V' || RX_BUF[6]!='E' || RX_BUF[7]!='R' ||
    							RX_BUF[8]!='T' || RX_BUF[9]!='E' || RX_BUF[10]!='B'|| RX_BUF[11]!='R'||
    							RX_BUF[12]!='A'|| RX_BUF[13]!='T'|| RX_BUF[14]!='E'|| RX_BUF[15]!='/') ){
    		COUNT_DATA=0;
    		for (i=0; i<16; ++i) {RX_BUF[i]= 0;}		// CLEAR BUFER RX

    	}
    	else {}




		//---------------- END Line ----------------------------------------
		if (RX_BUF[3]=='I' && RX_BUF[4]=='N'  && RX_BUF[5]=='V' &&
		    RX_BUF[6]=='E' && RX_BUF[7]=='R'  && RX_BUF[8]=='T' && RX_BUF[9]=='E' &&
		    RX_BUF[10]=='B'&& RX_BUF[11]=='R' && RX_BUF[12]=='A'&& RX_BUF[13]=='T'&&
		    RX_BUF[14]=='E'&& RX_BUF[15]=='/' && RX_BUF[COUNT_DATA-4] == 0x0A && RX_BUF[COUNT_DATA-3] == 0x0D &&
		    RX_BUF[COUNT_DATA-2] == 0x0A && RX_BUF[COUNT_DATA-1] == 0x0D){
		}
		else {}




		//---------------- OVERFLOW BUFFER RESET ---------------------------
		if (COUNT_DATA>90){
		    COUNT_DATA=0;
		   for (i=0; i<90; ++i){RX_BUF[i]= 0;}		// CLEAR BUFER RX
		}
		else {}

}




//=====================================================================================
//--------------- UTX_BYTE uint16_t to DEC transmith (54321 - GRADE)-------------------
void TRANSMITH_DEC (uint16_t NUMBER, uint8_t GRADE){
	uint16_t NUM, NUM5, NUM4, NUM3, NUM2, NUM1;

	NUM = NUMBER;

	//---- extraction grade ----
	NUM1 = NUM % 10;
	NUM /= 10;
	NUM2 = NUM % 10;
	NUM /= 10;
	NUM3 = NUM % 10;
	NUM /= 10;
	NUM4 = NUM % 10;
	NUM5 = NUM / 10;

	//---------- GRADE 0 (авто.) нулі крім молодшого розряду не передаються ()-----
	if (GRADE==0)	{

		if ( NUM5!=0 ) { UTX_BYTE(NUM5+0x30); UTX_BYTE(NUM4+0x30); UTX_BYTE(NUM3+0x30); UTX_BYTE(NUM2+0x30); UTX_BYTE(NUM1+0x30); }
		else {
			if (NUM4!=0) { UTX_BYTE(NUM4+0x30); UTX_BYTE(NUM3+0x30); UTX_BYTE(NUM2+0x30); UTX_BYTE(NUM1+0x30); }
			else {
				if (NUM3!=0) {UTX_BYTE(NUM3+0x30); UTX_BYTE(NUM2+0x30); UTX_BYTE(NUM1+0x30); }
				else {
					if (NUM2!=0) { UTX_BYTE(NUM2+0x30); UTX_BYTE(NUM1+0x30); }
					else { UTX_BYTE(NUM1+0x30); }
				}
			}
		}
	}
	else {}

	//-------------------- GRADE 5 передаються 5 розрядів --------------------------
	if (GRADE==5) {	UTX_BYTE(NUM5+0x30); UTX_BYTE(NUM4+0x30); UTX_BYTE(NUM3+0x30); UTX_BYTE(NUM2+0x30); UTX_BYTE(NUM1+0x30); }
	else {}

	//-------------------- GRADE 4 передаються 4 розряда ---------------------------
	if (GRADE==4) {	UTX_BYTE(NUM4+0x30); UTX_BYTE(NUM3+0x30); UTX_BYTE(NUM2+0x30); UTX_BYTE(NUM1+0x30); }
	else {}

	//-------------------- GRADE 3 передаються 3 розряда ---------------------------
	if (GRADE==3) {	UTX_BYTE(NUM3+0x30); UTX_BYTE(NUM2+0x30); UTX_BYTE(NUM1+0x30); }
	else {}

	//-------------------- GRADE 2 передаються 2 розряда ---------------------------
	if (GRADE==2) {	UTX_BYTE(NUM2+0x30); UTX_BYTE(NUM1+0x30); }
	else {}

}
//=====================================================================================

