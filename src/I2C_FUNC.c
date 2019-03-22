/*
 * I2C_FUNC.c
 *
 *  Created on: 6 θών. 2018 γ.
 *  Author: Oleksander Lupul
 */

#include "header.h"

//=======================================================================================================================
//================================================ I2C MASTER ===========================================================
//=======================================================================================================================

//----------------------- Start Condition -------------------------------------------------------------------------------
void START_I2C(uint32_t ADRES_SLAVE)
{
    I2C2->CR2 = 0x00000000;//Reset CR2 Register
    while((I2C2->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY);							//Check to see if the bus is busy
    I2C2->CR2 |= (0xFF<<16) | (ADRES_SLAVE);									//Set CR2 for 1-Byte Transfer, ADRESS_SLAVE
    I2C2->CR2 |= I2C_CR2_START;													//Start communication
//	while(I2C1->CR2 & I2C_CR2_START); 											//Wait for transfer to complete

}


//-------------------------- Write Data -------------------------------------------------------------------------------
void TXB_I2C(unsigned char Data)
{
	uint64_t i, filter;

	i=0; filter=50000;

	 if((I2C2->ISR & I2C_ISR_TXE) == I2C_ISR_TXE){								//Check Tx empty before writing to it
	    I2C2->TXDR = Data;
	 }
	 else {I2C2->ICR = 0xffff;	}												//ALL interrut I2C flag clear


	 while( !(I2C2->ISR & I2C_ISR_TXE) && i<filter)   {i++;}					//Wait for transfer to complete
}


//------------------------ Stop Condition -----------------------------------------------------------------------------
void STOP_I2C(void)
{

    I2C2->CR2 |= I2C_CR2_STOP;//Send Stop Condition
    while((I2C2->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY){} 						//Check to see if the bus is busy

	I2C2->CR1 &= ~I2C_CR1_PE;
	I2C2->ICR = 0xffff;															//ALL interrut I2C flag clear
	I2C2->CR1 |= I2C_CR1_PE;
}




unsigned char BUF_READ_I2C[28];

//======================================== READ =====================================================================

void I2C_Read_Reg(uint32_t Device,  uint32_t Register, uint32_t numBytes) {
	uint64_t i, k, filter;
	i=0; filter=100000; k=0;

	//-------------- TRANSMIT START + ADRES SLAVE + w(0) ------------------------------------------------------------
	I2C2->CR2 = 0;																//Reset CR2 Register

	while ((I2C2->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY);							//Check to see if the bus is busy


	I2C2->CR2 |= (1UL << 16) | (Device);										//Set CR2 for 1-byte transfer for Device

	I2C1->CR2 |= I2C_CR2_START;													//Start communication

	//------------- TRANSMIT REGISTER FOR READ ---------------------------------------------------------------------
	if ((I2C2->ISR & I2C_ISR_TXE) == I2C_ISR_TXE) {								//Check Tx empty before writing to it
		I2C2->TXDR = Register;
	}

	while (!(I2C2->ISR & I2C_ISR_TC)  && i<filter)	{i++;} 						//Wait for transfer to complete

	//--------------------- STOP BIT -------------------------------------------------------------------------------
	i=0;
	I2C2->CR2 |= I2C_CR2_STOP; //Send Stop Condition
	while ((I2C1->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY && i<filter)	{i++;}  	//Check to see if the bus is busy
	I2C2->ICR |= I2C_ICR_STOPCF; //Clear Stop bit flag


	//for (i = 0; i < 10; ++i) {}


	//------------ TRANSMIT START + ADRES SLAVE + r(1) -------------------------------------------------------------
	i=0;
	I2C2->CR2 = 0; 																//Clear CR2 for new configuration
	I2C2->CR2 |= I2C_CR2_AUTOEND | (numBytes<< 16) | I2C_CR2_RD_WRN | (Device); //Set CR2 for N-byte transfer, in read mode for Device
	I2C2->CR2 |= I2C_CR2_START; 												//Start communication
	while((I2C2->CR2 & I2C_CR2_START)  && i<filter)	{i++;} 						//Wait for transfer to complete


	//------------- READ Number of Times from buffer I2C_RX --------------------------------------------------------
	for (k = 0; k < numBytes; ++k) {
		i=0;

		while(((I2C2->ISR & I2C_ISR_RXNE) != (I2C_ISR_RXNE)) && i<filter){i++;} //white BUFF RX no EMPTY
		BUF_READ_I2C[k] =I2C1->RXDR; 			    							// read buffer
	}


	//--------------------- STOP BIT -------------------------------------------------------------------------------
	STOP_I2C();
}




//======================================== READ bez vkazivky registra =============================================

void I2C_Read(uint32_t Device,  uint32_t numBytes) {
	uint64_t i, k, filter;
	i=0; filter=100000; k=0;


	//------------ TRANSMIT START + ADRES SLAVE + r(1) -------------------------------------------------------------
	i=0;
	I2C2->CR2 = 0; 																//Clear CR2 for new configuration
	I2C2->CR2 |= I2C_CR2_AUTOEND | (numBytes<< 16) | I2C_CR2_RD_WRN | (Device); //Set CR2 for N-byte transfer, in read mode for Device
	I2C2->CR2 |= I2C_CR2_START; 												//Start communication
	while((I2C2->CR2 & I2C_CR2_START)  && i<filter)	{i++;} 						//Wait for transfer to complete


	//------------- READ Number of Times from buffer I2C_RX --------------------------------------------------------
	for (k = 0; k < numBytes; ++k) {
		i=0;

		while(((I2C2->ISR & I2C_ISR_RXNE) != (I2C_ISR_RXNE)) && i<filter){i++;} //white BUFF RX no EMPTY
		BUF_READ_I2C[k] =I2C2->RXDR; 			    							// read buffer
	}


	//--------------------- STOP BIT -------------------------------------------------------------------------------
	STOP_I2C();
}

