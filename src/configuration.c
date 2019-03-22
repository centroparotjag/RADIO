/*
 * configuration.c
 *
 *  Created on: 5 июн. 2018 г.
 *  Author: Oleksander Lupul
 */

#include "header.h"

void configuration (void){


	//=================================== CONFIG GPIO =======================================================
	//-------------------- Clock IO port enable register ------------------------------------------------
	RCC -> AHBENR = RCC_AHBENR_GPIOAEN		// port A, C, B clock enabled
				 |  RCC_AHBENR_GPIOBEN
				 |  RCC_AHBENR_GPIOCEN
				 |  RCC_AHBENR_GPIOFEN;

   //------------------------- SPEED GPIO --------------------------------------------------------------
	GPIOA -> OSPEEDR = 0xFFFFFFFF;			// ALL GPIOA Very high speed
	GPIOB -> OSPEEDR = 0xFFFFFFFF;			// ALL GPIOB Very high speed
	GPIOC -> OSPEEDR = 0xFFFFFFFF;			// ALL GPIOC Very high speed
	GPIOF -> OSPEEDR = 0xFFFFFFFF;			// ALL GPIOF Very high speed

	//------------------------ GPIO MODE ----------------------------------------------------------------
	GPIOA -> MODER = 0xFFFFFFFF;			// ALL GPIOA 11: Analog mode
	GPIOB -> MODER = 0xFFFFFFFF;			// ALL GPIOB 11: Analog mode
	GPIOC -> MODER = 0xFFFFFFFF;			// ALL GPIOC 11: Analog mode
	GPIOF -> MODER = 0xFFFFFFFF;			// ALL GPIOF 11: Analog mode

	//-------------------------- Pull_UP / Pull-down ----------------------------------------------------
	GPIOB -> PUPDR   = 0x00;
	GPIOB -> PUPDR  |= 	GPIO_PUPDR_PUPDR10_0;					// PB10 (BUTTON)- 01: Pull-up

	GPIOA -> PUPDR   = 0x00;
	GPIOA -> PUPDR  |= 	GPIO_PUPDR_PUPDR2_0						// PA4  (TX)	- 01: Pull-up
					|	GPIO_PUPDR_PUPDR3_0;					// PA5  (RX)	- 01: Pull-up

	GPIOF -> PUPDR   = 0x00;
	GPIOF -> PUPDR  |= 	GPIO_PUPDR_PUPDR6_0						// PF6  (SCL)	- 01: Pull-up
					|	GPIO_PUPDR_PUPDR7_0;					// PF7  (SDA)	- 01: Pull-up


	//-------------------- GPIO  OUTPUT MODE ------------------------------------------------------------
	GPIOA -> MODER &= ~GPIO_MODER_MODER15_1;  		// PA15 (E) 	- 01: General purpose output mode
	GPIOB -> MODER &= ~GPIO_MODER_MODER3_1;  		// PB3  (D4) 	- 01: General purpose output mode
	GPIOB -> MODER &= ~GPIO_MODER_MODER4_1;  		// PB4  (D5) 	- 01: General purpose output mode
	GPIOB -> MODER &= ~GPIO_MODER_MODER5_1;  		// PB5  (D6) 	- 01: General purpose output mode
	GPIOB -> MODER &= ~GPIO_MODER_MODER6_1;  		// PB6  (D7) 	- 01: General purpose output mode
	GPIOB -> MODER &= ~GPIO_MODER_MODER7_1;  		// PB7  (RS) 	- 01: General purpose output mode
	GPIOB -> MODER &= ~GPIO_MODER_MODER1_1;  		// PB1  (LED) 	- 01: General purpose output mode
	GPIOB -> MODER &= ~GPIO_MODER_MODER12_1;  		// PB12 (SHD) 	- 01: General purpose output mode

	//------------------- GPIO INPUT MODE ----------------------------------------------------------------
	GPIOB -> MODER &= ~GPIO_MODER_MODER10_1 & ~GPIO_MODER_MODER10_0;  	// PB10 (BUTTON) - 00: Input mode
	GPIOA -> MODER &= ~GPIO_MODER_MODER4_1 & ~GPIO_MODER_MODER4_0;  	// PA4 (ENC_F_BUT) - 00: Input mode
	GPIOB -> MODER &= ~GPIO_MODER_MODER2_1 & ~GPIO_MODER_MODER2_0;  	// PB2 (ENC_V_BUT) - 00: Input mode

	//----------------- ALTERNATE FINCTION MODE ----------------------------------------------------------
	GPIOA -> MODER &= ~GPIO_MODER_MODER2_0;  		// PA2 (U2TX) 		- 10: Alternate function mode
	GPIOA -> MODER &= ~GPIO_MODER_MODER3_0;  		// PA3 (U2RX) 		- 10: Alternate function mode
	GPIOA -> MODER &= ~GPIO_MODER_MODER6_0;  		// PA6 (TIM3_CH1) 	- 10: Alternate function mode
	GPIOA -> MODER &= ~GPIO_MODER_MODER7_0;  		// PA7 (TIM3_CH2) 	- 10: Alternate function mode
	GPIOA -> MODER &= ~GPIO_MODER_MODER8_0;  		// PA8 (TIM1_CH2) 	- 10: Alternate function mode
	GPIOA -> MODER &= ~GPIO_MODER_MODER9_0;  		// PA9 (TIM1_CH1) 	- 10: Alternate function mode
	GPIOB -> MODER &= ~GPIO_MODER_MODER9_0;  		// PB9 (TIM17_CH1) 	- 10: Alternate function mode
	GPIOF -> MODER &= ~GPIO_MODER_MODER6_0;  		// PF6 (SCL) 		- 10: Alternate function mode
	GPIOF -> MODER &= ~GPIO_MODER_MODER7_0;  		// PF7 (SDA) 		- 10: Alternate function mode

	//--------------- GPIO set alternate function -------------------------------------------------------
	GPIOA-> AFR[1] = 0x00000022;			// AFRH - PA9 (TIM1_CH1), PA8 (TIM1_CH2)
	GPIOA-> AFR[0] = 0x11001100;			// AFRL - PA7 (TIM3_CH2), PA6 (TIM3_CH1), PA3 (U2RX), PA2 (U2TX)

	GPIOB-> AFR[1] = 0x00000020;			// AFRH -TIM17_CH1
	GPIOB-> AFR[0] = 0x00000000;			// AFRL -

	GPIOF-> AFR[1] = 0x00000000;			// AFRH -
	GPIOF-> AFR[0] = 0x11000000;			// AFRL - PF7 (SDA), PF6 (SCL)

	//=================================== CONFIG RCC (SYSCLK=48MHz) ========================================================================
	RCC->CR |= RCC_CR_HSION;					// Internal High Speed clock enable
	while (!(RCC->CR & RCC_CR_HSIRDY));			// Wait until it's stable (Internal high-speed clock ready flag)

	RCC->CFGR |= RCC_CFGR_PLLMUL12;				// PLL input clock*12			(8E6 / 2) * 12 = 48MHz

	RCC->CR |= RCC_CR_PLLON;					// PLL enable
	while (!(RCC->CR & RCC_CR_PLLRDY));			// Wait PLL to stabilize

	RCC->CFGR|=RCC_CFGR_SW_PLL;					// PLL selected as system clock (SYSCLK)


	//=================================== CONFIG I2C2 ==================================================================================
	RCC-> APB1ENR |= RCC_APB1ENR_I2C2EN;			// I2C2 clock enable
	I2C2 -> CR1 = 0x00;
//	I2C2->TIMINGR = (uint32_t)0x00B01A4B;			// 400 kHz example
	I2C2->TIMINGR = (uint32_t)0x00003630;			// 400 kHz
	I2C2 -> CR1 = I2C_CR1_PE;						// Peripheral I2C enable

	//FP6, FP7 alternate mode 0 or 1

	//=================================  CONFIG UART ==================================================================================
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;	 		//  USART2 clock enabled

	USART2->BRR = 417;				        		 // PCLK/BAUD=48000000/115200=416,666
	USART2->CR1	= 0;
	USART2->CR3 = 0;
	USART2->CR2 = 0;

	USART2->CR1 |= USART_CR1_TE;			// enable TX
	// USART2->CR1 |= USART_CR1_RE;			// enable RX
	// USART2->CR1 |= USART_CR1_RXNEIE;		// Enable Interrupt
	USART2->CR1 |= USART_CR1_UE;			// enable USART2


	//=================================  CONFIG ENCODERS "FREQUENCY & VOLUME" =========================================================
	//----------- TIM1 (VOLUMEF) --------------
	RCC-> APB2ENR |= RCC_APB2ENR_TIM1EN;							// TIM1 clock enable
	TIM1->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0; 			// Configure TI1FP1 on TI1 (CC1S = 01) & TI1FP2 on TI2 (CC2S = 01)
	TIM1->CCER  &= (uint16_t)(~(TIM_CCER_CC1P | TIM_CCER_CC2P)); 	// Configure TI1FP1 and TI1FP2 non inverted (CC1P = CC2P = 0, reset	value)
	TIM1->SMCR  |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; 				// 011: Encoder mode 3
	//TIM1->ARR = 100;												// RESET COUNTER
	TIM1->CR1   |= TIM_CR1_CEN; 									// Counter enable
	TIM1->CNT=1000;
	// READ TIM1->CNT;		// тут читати зміну лічильника при обертанні енкодера (зміна енкодера на 1 позіцію - дає зміну лічильника на 4)

	//----------- TIM3 (REQUENCY) -----------------
	RCC-> APB1ENR |= RCC_APB1ENR_TIM3EN;							// Timer 3 clock enable
	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0; 			// Configure TI1FP1 on TI1 (CC1S = 01) & TI1FP2 on TI2 (CC2S = 01)
	TIM3->CCER  &= (uint16_t)(~(TIM_CCER_CC1P | TIM_CCER_CC2P)); 	// Configure TI1FP1 and TI1FP2 non inverted (CC1P = CC2P = 0, reset	value)
	TIM3->SMCR  |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; 				// 011: Encoder mode 3
	//TIM3->ARR = 100;												// RESET COUNTER
	TIM3->CR1   |= TIM_CR1_CEN; 									// Counter enable
	TIM3->CNT=1000;
	// READ TIM3->CNT;		// тут читати зміну лічильника при обертанні енкодера (зміна енкодера на 1 позіцію - дає зміну лічильника на 4)


	//=================================  CONFIG PWM (-V0 generator) TIM17_CH1 - F=125kHz, DUTY=50% =====================================
	RCC-> APB2ENR |= RCC_APB2ENR_TIM17EN;							// TIM17 clock enable

	TIM17->PSC  = 5; 												// Set prescaler fCK_PSC / (PSC[15:0] + 1);    F = 48e6 / (47+1) = 1MHz
	TIM17->ARR  = 8; 												// TIM auto-reload register,                   Far = 1e6 / 8 = 125kHz
	TIM17->CCR1 = 4; 												// TIM capture/compare register 1,             duty cycle = 50%

	TIM17->CCMR1 |= TIM_CCMR1_OC1M_2 |  TIM_CCMR1_OC1M_1;			// PWM mode 1 (non inverted),
	TIM17->CCMR1 |= TIM_CCMR1_OC1PE;								// Output Compare 1 Preload enable
	TIM17->CCER  |= TIM_CCER_CC1E; 									// Capture/Compare 1 output enable (TIM17_CH1 -  PB9)
	TIM17->BDTR  |= TIM_BDTR_MOE; 									// Main Output enable
	TIM17->CR1   |= TIM_CR1_CEN;									// Counter enable
	TIM17->EGR   |= TIM_EGR_UG; 									// Force update generation


	//==================================== CONFIG TIM15 =======================================================
	RCC -> APB2ENR |= RCC_APB2ENR_TIM15EN;		// Timer 2 clock enable
	TIM15 -> CNT = 0;							// TIM counter register 16 bit
	TIM15 -> PSC = 48000;						// Preskaler register		T=1/(SYS_CLK/PSC/ARR)=1/(48000000/48000/2)=0.002s
	TIM15 -> ARR = 50;							// TIM auto-reload register ms 100

	TIM15 -> DIER |= TIM_DIER_UIE;				// Update interrupt enable
	TIM15 -> CR1  |= TIM_CR1_CEN;				// Counter enable

	NVIC_SetPriority(TIM15_IRQn,2); 			// Set priority

	//========================================================================================================

//				#ifdef DEBUG
//
//				# else
//			  	//  enabled protection in RELEASE
//			  		ReadOutProtection_ENABLED ();			   // LOCKed read ALL FLASH
//			  	#endif


	RS_0; E_0; D4_0; D5_0; D6_0; D7_0; LED_OFF; SHD_ON;

	//=================================  CONFIG LCD1602A (driver KS0066) =====================================
	INIT_LCD1602A_KS0066 ();

	//================================= RESET STATE ===========================================================
	ENCODER_FREQ (); 		// reset init. state encoder
	ENCODER_VOLUME (); 		// reset init. state encoder

	CONFIG_RDA5807M ();

}
