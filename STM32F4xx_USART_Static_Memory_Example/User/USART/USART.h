/****************************************************************
 * Copyright (C) 2016, XinLi, all right reserved.
 * File name:    USART.h
 * Date:         2016.03.22
 * Description:  USART Driver
*****************************************************************/

#ifndef __USART_H
#define __USART_H

/****************************************************************
 *                        Header include
*****************************************************************/
#include "stm32f4xx.h"

/****************************************************************
 *                       Macro definition
*****************************************************************/
#define USART1_BufferSize_Tx    (100)
#define USART1_BufferSize_Rx    (100)

#define USART2_BufferSize_Tx    (0)
#define USART2_BufferSize_Rx    (0)

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
#define USART3_BufferSize_Tx    (0)
#define USART3_BufferSize_Rx    (0)
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
#define USART4_BufferSize_Tx    (0)
#define USART4_BufferSize_Rx    (0)

#define USART5_BufferSize_Tx    (0)
#define USART5_BufferSize_Rx    (0)
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */

#define USART6_BufferSize_Tx    (0)
#define USART6_BufferSize_Rx    (0)

#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
#define USART7_BufferSize_Tx    (0)
#define USART7_BufferSize_Rx    (0)

#define USART8_BufferSize_Tx    (0)
#define USART8_BufferSize_Rx    (0)
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */

#if defined(STM32F413_423xx)
#define USART9_BufferSize_Tx    (0)
#define USART9_BufferSize_Rx    (0)

#define USART10_BufferSize_Tx   (0)
#define USART10_BufferSize_Rx   (0)
#endif /* STM32F413_423xx */

/********************* USART1 Configuration *********************/

#define RCC_AHB1Periph_USART1_Tx    RCC_AHB1Periph_GPIOA
#define RCC_AHB1Periph_USART1_Rx    RCC_AHB1Periph_GPIOA

#define GPIO_USART1_Tx              GPIOA
#define GPIO_USART1_Rx              GPIOA

#define GPIO_Pin_USART1_Tx          GPIO_Pin_9
#define GPIO_Pin_USART1_Rx          GPIO_Pin_10

#define GPIO_PinSource_USART1_Tx    GPIO_PinSource9
#define GPIO_PinSource_USART1_Rx    GPIO_PinSource10

/****************************************************************/

/********************* USART2 Configuration *********************/

#define RCC_AHB1Periph_USART2_Tx    RCC_AHB1Periph_GPIOA
#define RCC_AHB1Periph_USART2_Rx    RCC_AHB1Periph_GPIOA

#define GPIO_USART2_Tx              GPIOA
#define GPIO_USART2_Rx              GPIOA

#define GPIO_Pin_USART2_Tx          GPIO_Pin_2
#define GPIO_Pin_USART2_Rx          GPIO_Pin_3

#define GPIO_PinSource_USART2_Tx    GPIO_PinSource2
#define GPIO_PinSource_USART2_Rx    GPIO_PinSource3

/****************************************************************/

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
/********************* USART3 Configuration *********************/

#define RCC_AHB1Periph_USART3_Tx    RCC_AHB1Periph_GPIOB
#define RCC_AHB1Periph_USART3_Rx    RCC_AHB1Periph_GPIOB

#define GPIO_USART3_Tx              GPIOB
#define GPIO_USART3_Rx              GPIOB

#define GPIO_Pin_USART3_Tx          GPIO_Pin_10
#define GPIO_Pin_USART3_Rx          GPIO_Pin_11

#define GPIO_PinSource_USART3_Tx    GPIO_PinSource10
#define GPIO_PinSource_USART3_Rx    GPIO_PinSource11

/****************************************************************/
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
/********************* USART4 Configuration *********************/

#define RCC_AHB1Periph_USART4_Tx    RCC_AHB1Periph_GPIOC
#define RCC_AHB1Periph_USART4_Rx    RCC_AHB1Periph_GPIOC

#define GPIO_USART4_Tx              GPIOC
#define GPIO_USART4_Rx              GPIOC

#define GPIO_Pin_USART4_Tx          GPIO_Pin_10
#define GPIO_Pin_USART4_Rx          GPIO_Pin_11

#define GPIO_PinSource_USART4_Tx    GPIO_PinSource10
#define GPIO_PinSource_USART4_Rx    GPIO_PinSource11

/****************************************************************/

/********************* USART5 Configuration *********************/

#define RCC_AHB1Periph_USART5_Tx    RCC_AHB1Periph_GPIOC
#define RCC_AHB1Periph_USART5_Rx    RCC_AHB1Periph_GPIOD

#define GPIO_USART5_Tx              GPIOC
#define GPIO_USART5_Rx              GPIOD

#define GPIO_Pin_USART5_Tx          GPIO_Pin_12
#define GPIO_Pin_USART5_Rx          GPIO_Pin_2

#define GPIO_PinSource_USART5_Tx    GPIO_PinSource12
#define GPIO_PinSource_USART5_Rx    GPIO_PinSource2

/****************************************************************/
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */

/********************* USART6 Configuration *********************/

#define RCC_AHB1Periph_USART6_Tx    RCC_AHB1Periph_GPIOC
#define RCC_AHB1Periph_USART6_Rx    RCC_AHB1Periph_GPIOC

#define GPIO_USART6_Tx              GPIOC
#define GPIO_USART6_Rx              GPIOC

#define GPIO_Pin_USART6_Tx          GPIO_Pin_6
#define GPIO_Pin_USART6_Rx          GPIO_Pin_7

#define GPIO_PinSource_USART6_Tx    GPIO_PinSource6
#define GPIO_PinSource_USART6_Rx    GPIO_PinSource7

/****************************************************************/

#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
/********************* USART7 Configuration *********************/

#define RCC_AHB1Periph_USART7_Tx    RCC_AHB1Periph_GPIOE
#define RCC_AHB1Periph_USART7_Rx    RCC_AHB1Periph_GPIOE

#define GPIO_USART7_Tx              GPIOE
#define GPIO_USART7_Rx              GPIOE

#define GPIO_Pin_USART7_Tx          GPIO_Pin_8
#define GPIO_Pin_USART7_Rx          GPIO_Pin_7

#define GPIO_PinSource_USART7_Tx    GPIO_PinSource8
#define GPIO_PinSource_USART7_Rx    GPIO_PinSource7

/****************************************************************/

/********************* USART8 Configuration *********************/

#define RCC_AHB1Periph_USART8_Tx    RCC_AHB1Periph_GPIOE
#define RCC_AHB1Periph_USART8_Rx    RCC_AHB1Periph_GPIOE

#define GPIO_USART8_Tx              GPIOE
#define GPIO_USART8_Rx              GPIOE

#define GPIO_Pin_USART8_Tx          GPIO_Pin_1
#define GPIO_Pin_USART8_Rx          GPIO_Pin_0

#define GPIO_PinSource_USART8_Tx    GPIO_PinSource1
#define GPIO_PinSource_USART8_Rx    GPIO_PinSource0

/****************************************************************/
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */

#if defined(STM32F413_423xx)
/********************* USART9 Configuration *********************/

#define RCC_AHB1Periph_USART9_Tx    RCC_AHB1Periph_GPIOG
#define RCC_AHB1Periph_USART9_Rx    RCC_AHB1Periph_GPIOG

#define GPIO_USART9_Tx              GPIOG
#define GPIO_USART9_Rx              GPIOG

#define GPIO_Pin_USART9_Tx          GPIO_Pin_1
#define GPIO_Pin_USART9_Rx          GPIO_Pin_0

#define GPIO_PinSource_USART9_Tx    GPIO_PinSource1
#define GPIO_PinSource_USART9_Rx    GPIO_PinSource0

/****************************************************************/

/******************** USART10 Configuration *********************/

#define RCC_AHB1Periph_USART10_Tx    RCC_AHB1Periph_GPIOE
#define RCC_AHB1Periph_USART10_Rx    RCC_AHB1Periph_GPIOE

#define GPIO_USART10_Tx              GPIOE
#define GPIO_USART10_Rx              GPIOE

#define GPIO_Pin_USART10_Tx          GPIO_Pin_3
#define GPIO_Pin_USART10_Rx          GPIO_Pin_2

#define GPIO_PinSource_USART10_Tx    GPIO_PinSource3
#define GPIO_PinSource_USART10_Rx    GPIO_PinSource2

/****************************************************************/
#endif /* STM32F413_423xx */

#define BAUD_RATE_2400      ((uint32_t)2400)
#define BAUD_RATE_4800      ((uint32_t)4800)
#define BAUD_RATE_9600      ((uint32_t)9600)
#define BAUD_RATE_19200     ((uint32_t)19200)
#define BAUD_RATE_38400     ((uint32_t)38400)
#define BAUD_RATE_57600     ((uint32_t)57600)
#define BAUD_RATE_115200    ((uint32_t)115200)
#define BAUD_RATE_230400    ((uint32_t)230400)
#define BAUD_RATE_460800    ((uint32_t)460800)
#define BAUD_RATE_921600    ((uint32_t)921600)

/****************************************************************
 *                       Type definition
*****************************************************************/


/****************************************************************
 *                     Structure definition
*****************************************************************/



#ifdef __cplusplus
 extern "C" {
#endif  /* __cplusplus */

/****************************************************************
 *                     Variable declaration
*****************************************************************/


/****************************************************************
 *                     Function declaration
*****************************************************************/
void USART_Configuration(USART_TypeDef* USARTx, uint32_t BaudRate, uint16_t USART_IT);
void USART_Unconfigure(USART_TypeDef* USARTx);
uint16_t USART_SetSendData(USART_TypeDef* USARTx, void* Buffer, uint16_t BufferSize);
uint16_t USART_GetReceiveData(USART_TypeDef* USARTx, void* Buffer, uint16_t BufferSize);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif 	/* __USART_H */
