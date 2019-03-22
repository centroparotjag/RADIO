/*
 * header.h
 *
 *  Created on: 5 θών. 2018 γ.
 *      Author: qwerty
 */

#ifndef HEADER_H_
#define HEADER_H_


#include "stm32f0xx.h"
#include <stdio.h>

void INIT_LCD1602A_KS0066 (void);
void InstallationCursor (unsigned char x, unsigned char y);
void TRANSMITH_DATA_KS0066 (uint8_t DATA);
void PrintString (char *sign );
void Clear (void);
void SYMBOL5x8_PROGRAM (uint8_t *CGRAM_DATA, uint8_t ADDRES_0_7);
void VOLUME_SKALE (uint8_t VOLUME);
void FREQ_SKALE (uint16_t FREQ);


void configuration (void);
void UTX_BYTE(char ch);
void UTX_STRING (char BUFFER[], unsigned char PERENOS_STR);
void START_I2C(uint32_t ADRES_SLAVE);
void TXB_I2C(unsigned char Data);
void STOP_I2C(void);
void I2C_Read_Reg(uint32_t Device,  uint32_t Register, uint32_t numBytes);
void I2C_Read(uint32_t Device,  uint32_t numBytes);

uint8_t ENCODER_VOLUME (void);
uint8_t VOLUME_BUTTON (void);
uint8_t ENCODER_FREQ (void);
uint8_t FREQUENCY_BUTTON (void);
uint8_t BUTTON (void);

void CONFIG_RDA5807M (void);
void VOLUME_RDA5807M(uint8_t VOLUME);
void SEEK (uint8_t DIRECTLY);
void TUNE (uint16_t F);
void READ_FREQ (void);
void FREQ_TUNE_SET (uint16_t FREQ);
void LCD_RSSI_ST_FMTRUE (void);

void MENU_STARTUP (uint8_t SET);
void MENU_FREQ_1 (void);
void MENU_VOLUME_1 (void);
void MENU_SEEK_1 (void);


void EE_WR_FREQ_VOL(uint16_t FREQ, uint8_t VOL);
void EE_READ_FREQ_VOL(void);
uint32_t  EE_READ_WORK_TIME (void);
void EEPROME_INCREMENT_WORK_TIME (void);
void EEPROME_CLEAR_DEFAULT (void);

void READ_LCD_WORK_TIME (void);


void REG_DISPLAYED (uint16_t REG);

void READ_ALL_REGISTER_RDA5807M (void);
uint8_t RDS_DECODE (void);

//------------ GPIO (output mode) DEFINE -------------------------------------------------------
	#define	D7_1					GPIOB->ODR |=GPIO_ODR_6;
	#define	D7_0					GPIOB->BSRR|=GPIO_BSRR_BR_6;

	#define	D6_1					GPIOB->ODR |=GPIO_ODR_5;
	#define	D6_0					GPIOB->BSRR|=GPIO_BSRR_BR_5;

	#define	D5_1					GPIOB->ODR |=GPIO_ODR_4;
	#define	D5_0					GPIOB->BSRR|=GPIO_BSRR_BR_4;

	#define	D4_1					GPIOB->ODR |=GPIO_ODR_3;
	#define	D4_0					GPIOB->BSRR|=GPIO_BSRR_BR_3;

	#define	E_1						GPIOA->ODR |=GPIO_ODR_15;
	#define	E_0						GPIOA->BSRR|=GPIO_BSRR_BR_15;

	#define	RS_1					GPIOB->ODR |=GPIO_ODR_7;
	#define	RS_0					GPIOB->BSRR|=GPIO_BSRR_BR_7;

	#define	LED_OFF					GPIOB->ODR |=GPIO_ODR_1;
	#define	LED_ON					GPIOB->BSRR|=GPIO_BSRR_BR_1;

	#define	SHD_ON					GPIOB->ODR |=GPIO_ODR_12;
	#define	SHD_OFF					GPIOB->BSRR|=GPIO_BSRR_BR_12;





#endif /* HEADER_H_ */
