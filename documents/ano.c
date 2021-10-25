/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "assignment.h"

int main(void)
{
  /*
   *  DO NOT WRITE TO THE WHOLE REGISTER!!!
   *  Write to the bits, that are meant for change.
   */
   
  //Systick init
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);	

  /*
   * TASK - configure MCU peripherals so that button state can be read and LED will blink.
   * Button must be connected to the GPIO port A and its pin 3.
   * LED must be connected to the GPIO port A and its pin 4.
   *
   * In header file "assignment.h" define macros for MCU registers access and LED blink application.
   * Code in this file must use these macros for the peripherals setup.
   * Code of the LED blink application is already given so just the macros used in the application must be defined.
   */


  /* Enable clock for GPIO port A*/

  RCC_AHBENR_REG |= (uint32_t)(1 << 17);//type your code for GPIOA clock enable here:

  /* GPIOA pin 3 and 4 setup */

  /*GPIO MODER register*/
  //Set mode for pin 4
  GPIOA_MODER_REG &= ~(uint32_t)(0x3 << 8);
  GPIOA_MODER_REG |= (uint32_t)(1 << 8);
  //Set mode for pin 3
  GPIOA_MODER_REG &= ~(uint32_t)(0x3 << 6);

  /*GPIO OTYPER register*/
  GPIOA_OTYPER_REG &= ~(1 << 4);

  /*GPIO OSPEEDR register*/
  //Set Low speed for GPIOA pin 4
  GPIOA_OSPEEDER_REG &= ~(0x3 << 8);

  /*GPIO PUPDR register, reset*/
  //Set pull up for GPIOA pin 3 (input)
  GPIOA_PUPDR_REG |= (1 << 6);
  //Set no pull for GPIOA pin 4
  GPIOA_PUPDR_REG &= ~(0x3 << 8);
  // Inicializacia parametrov
  volatile char state = 1;
  uint8_t pin_state = 0;
  volatile char Button = 0;  // V ako stave je tlacitko
  uint8_t countPressed = 0; // Kolko krat bola detekcia stlacenia tlacitka
  uint8_t countReleas = 0; // Kolko krat bola detekcia pustenia tlacitka

  EDGE_TYPE edgeDetect(uint8_t pin_state, uint8_t samples) {
  	  if(pin_state==0) {
  		  if(Button==0)

  	  		{
  	  			countPressed++;
  	  			countReleas=0;
  	  			if(countPressed > samples)  // Detekcia rise, pocita kolko krat bolo detegovane stlacenie
  	  			{
  	  				Button=1;
  	  				return RISE;
  	  			}
  	  			else
  	  			{
  	  				countPressed++;
  	  				countReleas=0;
  	  				//return RISE;
  	  			}
  	  		}
  	  }
  	  	  else
  	  	  {
  	  		  if(Button==1)
  	  		  {
  	  			  countReleas++;
  	  			  countPressed = 0;
  	  			  if(countReleas > samples)
  	  			  {
  	  				  Button=0;
  	  				  return FALL;
  	  			  }
  	  			  else
  	  			  {
  	  				  countReleas++;
  	  				  countPressed = 0;
  	  				  return NONE;
  	  			  }
  	  		  }
  	  	  }
  	  return NONE;
  }

  while (1)
  {
	  pin_state=GPIOA->IDR & GPIO_IDR_3;
	  if (edgeDetect(pin_state, 150) == RISE) {
	  	  if (state == 1) {
	  	  state = 0;
	  	  }
	  	  else {
	  	  state = 1;
	  	  }
	    }
	    switch (state) {
	  	  case 1:
	  		  LED_ON;
	  		  break;
	  	  case 0:
	  		  LED_OFF;
	  		  break;
	    }

}
}
