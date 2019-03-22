/*
 * RDS.c
 *
 *  Created on: 14 θών. 2018 γ.
 *  Author: Oleksander Lupul
 */


#include "header.h"


//=============================================================
 uint8_t BITE_MASK (uint16_t WORD, uint16_t MASK){
	 if ((WORD & MASK) == MASK){return 0x31;}
	 else {return 0x30;}
 }

void REG_DISPLAYED (uint16_t REG){
	uint16_t B15, B14, B13, B12, B11, B10, B9, B8,
	         B7,  B6,  B5,  B4,  B3,  B2,  B1, B0;


//	START_I2C(0x22);					// CHIP ADDRES
//	TXB_I2C(REG); 						// REG For Read And Displayed
//	STOP_I2C();
//	I2C_Read(0x22,  2);



	//========== BITE DECODE OF WORD =======
	B15 = BITE_MASK (REG, 0x8000);
	B14 = BITE_MASK (REG, 0x4000);
	B13 = BITE_MASK (REG, 0x2000);
	B12 = BITE_MASK (REG, 0x1000);
	B11 = BITE_MASK (REG, 0x0800);
	B10 = BITE_MASK (REG, 0x0400);
	B9  = BITE_MASK (REG, 0x0200);
	B8  = BITE_MASK (REG, 0x0100);

	B7  = BITE_MASK (REG, 0x80);
	B6  = BITE_MASK (REG, 0x40);
	B5  = BITE_MASK (REG, 0x20);
	B4  = BITE_MASK (REG, 0x10);
	B3  = BITE_MASK (REG, 0x08);
	B2  = BITE_MASK (REG, 0x04);
	B1  = BITE_MASK (REG, 0x02);
	B0  = BITE_MASK (REG, 0x01);


	//========= BYTE to BYTE DISPLAYED =======

	InstallationCursor (1, 2);
	TRANSMITH_DATA_KS0066 (B15);
	TRANSMITH_DATA_KS0066 (B14);
	TRANSMITH_DATA_KS0066 (B13);
	TRANSMITH_DATA_KS0066 (B12);
	TRANSMITH_DATA_KS0066 (B11);
	TRANSMITH_DATA_KS0066 (B10);
	TRANSMITH_DATA_KS0066 (B9);
	TRANSMITH_DATA_KS0066 (B8);

	TRANSMITH_DATA_KS0066 (B7);
	TRANSMITH_DATA_KS0066 (B6);
	TRANSMITH_DATA_KS0066 (B5);
	TRANSMITH_DATA_KS0066 (B4);
	TRANSMITH_DATA_KS0066 (B3);
	TRANSMITH_DATA_KS0066 (B2);
	TRANSMITH_DATA_KS0066 (B1);
	TRANSMITH_DATA_KS0066 (B0);

//	PrintString (char *sign );


}
