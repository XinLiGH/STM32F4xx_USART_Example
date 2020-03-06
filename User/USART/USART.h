/**
  ******************************************************************************
  * @file    USART.h
  * @author  XinLi
  * @version v3.10
  * @date    24-October-2017
  * @brief   Header file for USART.c module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>Copyright &copy; 2017 XinLi</center></h2>
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <https://www.gnu.org/licenses/>.
  *
  ******************************************************************************
  */

#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Header includes -----------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdbool.h>

/* Macro definitions ---------------------------------------------------------*/
/******************************************************************************/
/*                              USART1 Configure                              */
/******************************************************************************/
#define USART1_BufferSize_Tx            (4096)
#define USART1_BufferSize_Rx            (4096)

#define USART1_DMA_BufferSize_Tx        (4096)
#define USART1_DMA_BufferSize_Rx        (4096)

#define USART1_IRQ_PreemptionPriority   (0)
#define USART1_IRQ_SubPriority          (0)

#define USART1_Tx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOA
#define USART1_Rx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOA

#define USART1_Tx_GPIO                  GPIOA
#define USART1_Rx_GPIO                  GPIOA

#define USART1_Tx_GPIO_Pin              GPIO_Pin_9
#define USART1_Rx_GPIO_Pin              GPIO_Pin_10

#define USART1_Tx_GPIO_PinSource        GPIO_PinSource9
#define USART1_Rx_GPIO_PinSource        GPIO_PinSource10

/******************************************************************************/
/*                              USART2 Configure                              */
/******************************************************************************/
#define USART2_BufferSize_Tx            (4096)
#define USART2_BufferSize_Rx            (4096)

#define USART2_DMA_BufferSize_Tx        (4096)
#define USART2_DMA_BufferSize_Rx        (4096)

#define USART2_IRQ_PreemptionPriority   (0)
#define USART2_IRQ_SubPriority          (0)

#define USART2_Tx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOA
#define USART2_Rx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOA

#define USART2_Tx_GPIO                  GPIOA
#define USART2_Rx_GPIO                  GPIOA

#define USART2_Tx_GPIO_Pin              GPIO_Pin_2
#define USART2_Rx_GPIO_Pin              GPIO_Pin_3

#define USART2_Tx_GPIO_PinSource        GPIO_PinSource2
#define USART2_Rx_GPIO_PinSource        GPIO_PinSource3

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
/******************************************************************************/
/*                              USART3 Configure                              */
/******************************************************************************/
#define USART3_BufferSize_Tx            (4096)
#define USART3_BufferSize_Rx            (4096)

#define USART3_DMA_BufferSize_Tx        (4096)
#define USART3_DMA_BufferSize_Rx        (4096)

#define USART3_IRQ_PreemptionPriority   (0)
#define USART3_IRQ_SubPriority          (0)

#define USART3_Tx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOB
#define USART3_Rx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOB

#define USART3_Tx_GPIO                  GPIOB
#define USART3_Rx_GPIO                  GPIOB

#define USART3_Tx_GPIO_Pin              GPIO_Pin_10
#define USART3_Rx_GPIO_Pin              GPIO_Pin_11

#define USART3_Tx_GPIO_PinSource        GPIO_PinSource10
#define USART3_Rx_GPIO_PinSource        GPIO_PinSource11
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
/******************************************************************************/
/*                              USART4 Configure                              */
/******************************************************************************/
#define USART4_BufferSize_Tx            (4096)
#define USART4_BufferSize_Rx            (4096)

#define USART4_DMA_BufferSize_Tx        (4096)
#define USART4_DMA_BufferSize_Rx        (4096)

#define USART4_IRQ_PreemptionPriority   (0)
#define USART4_IRQ_SubPriority          (0)

#define USART4_Tx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOC
#define USART4_Rx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOC

#define USART4_Tx_GPIO                  GPIOC
#define USART4_Rx_GPIO                  GPIOC

#define USART4_Tx_GPIO_Pin              GPIO_Pin_10
#define USART4_Rx_GPIO_Pin              GPIO_Pin_11

#define USART4_Tx_GPIO_PinSource        GPIO_PinSource10
#define USART4_Rx_GPIO_PinSource        GPIO_PinSource11

/******************************************************************************/
/*                              USART5 Configure                              */
/******************************************************************************/
#define USART5_BufferSize_Tx            (4096)
#define USART5_BufferSize_Rx            (4096)

#define USART5_DMA_BufferSize_Tx        (4096)
#define USART5_DMA_BufferSize_Rx        (4096)

#define USART5_IRQ_PreemptionPriority   (0)
#define USART5_IRQ_SubPriority          (0)

#define USART5_Tx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOC
#define USART5_Rx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOD

#define USART5_Tx_GPIO                  GPIOC
#define USART5_Rx_GPIO                  GPIOD

#define USART5_Tx_GPIO_Pin              GPIO_Pin_12
#define USART5_Rx_GPIO_Pin              GPIO_Pin_2

#define USART5_Tx_GPIO_PinSource        GPIO_PinSource12
#define USART5_Rx_GPIO_PinSource        GPIO_PinSource2
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */

/******************************************************************************/
/*                              USART6 Configure                              */
/******************************************************************************/
#define USART6_BufferSize_Tx            (4096)
#define USART6_BufferSize_Rx            (4096)

#define USART6_DMA_BufferSize_Tx        (4096)
#define USART6_DMA_BufferSize_Rx        (4096)

#define USART6_IRQ_PreemptionPriority   (0)
#define USART6_IRQ_SubPriority          (0)

#define USART6_Tx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOC
#define USART6_Rx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOC

#define USART6_Tx_GPIO                  GPIOC
#define USART6_Rx_GPIO                  GPIOC

#define USART6_Tx_GPIO_Pin              GPIO_Pin_6
#define USART6_Rx_GPIO_Pin              GPIO_Pin_7

#define USART6_Tx_GPIO_PinSource        GPIO_PinSource6
#define USART6_Rx_GPIO_PinSource        GPIO_PinSource7

#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
/******************************************************************************/
/*                              USART7 Configure                              */
/******************************************************************************/
#define USART7_BufferSize_Tx            (4096)
#define USART7_BufferSize_Rx            (4096)

#define USART7_DMA_BufferSize_Tx        (4096)
#define USART7_DMA_BufferSize_Rx        (4096)

#define USART7_IRQ_PreemptionPriority   (0)
#define USART7_IRQ_SubPriority          (0)

#define USART7_Tx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOE
#define USART7_Rx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOE

#define USART7_Tx_GPIO                  GPIOE
#define USART7_Rx_GPIO                  GPIOE

#define USART7_Tx_GPIO_Pin              GPIO_Pin_8
#define USART7_Rx_GPIO_Pin              GPIO_Pin_7

#define USART7_Tx_GPIO_PinSource        GPIO_PinSource8
#define USART7_Rx_GPIO_PinSource        GPIO_PinSource7

/******************************************************************************/
/*                              USART8 Configure                              */
/******************************************************************************/
#define USART8_BufferSize_Tx            (4096)
#define USART8_BufferSize_Rx            (4096)

#define USART8_DMA_BufferSize_Tx        (4096)
#define USART8_DMA_BufferSize_Rx        (4096)

#define USART8_IRQ_PreemptionPriority   (0)
#define USART8_IRQ_SubPriority          (0)

#define USART8_Tx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOE
#define USART8_Rx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOE

#define USART8_Tx_GPIO                  GPIOE
#define USART8_Rx_GPIO                  GPIOE

#define USART8_Tx_GPIO_Pin              GPIO_Pin_1
#define USART8_Rx_GPIO_Pin              GPIO_Pin_0

#define USART8_Tx_GPIO_PinSource        GPIO_PinSource1
#define USART8_Rx_GPIO_PinSource        GPIO_PinSource0
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */

#if defined(STM32F413_423xx)
/******************************************************************************/
/*                              USART9 Configure                              */
/******************************************************************************/
#define USART9_BufferSize_Tx            (4096)
#define USART9_BufferSize_Rx            (4096)

#define USART9_DMA_BufferSize_Tx        (4096)
#define USART9_DMA_BufferSize_Rx        (4096)

#define USART9_IRQ_PreemptionPriority   (0)
#define USART9_IRQ_SubPriority          (0)

#define USART9_Tx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOG
#define USART9_Rx_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOG

#define USART9_Tx_GPIO                  GPIOG
#define USART9_Rx_GPIO                  GPIOG

#define USART9_Tx_GPIO_Pin              GPIO_Pin_1
#define USART9_Rx_GPIO_Pin              GPIO_Pin_0

#define USART9_Tx_GPIO_PinSource        GPIO_PinSource1
#define USART9_Rx_GPIO_PinSource        GPIO_PinSource0

/******************************************************************************/
/*                              USART10 Configure                             */
/******************************************************************************/
#define USART10_BufferSize_Tx           (4096)
#define USART10_BufferSize_Rx           (4096)

#define USART10_DMA_BufferSize_Tx       (4096)
#define USART10_DMA_BufferSize_Rx       (4096)

#define USART10_IRQ_PreemptionPriority  (0)
#define USART10_IRQ_SubPriority         (0)

#define USART10_Tx_RCC_AHB1Periph_GPIO  RCC_AHB1Periph_GPIOE
#define USART10_Rx_RCC_AHB1Periph_GPIO  RCC_AHB1Periph_GPIOE

#define USART10_Tx_GPIO                 GPIOE
#define USART10_Rx_GPIO                 GPIOE

#define USART10_Tx_GPIO_Pin             GPIO_Pin_3
#define USART10_Rx_GPIO_Pin             GPIO_Pin_2

#define USART10_Tx_GPIO_PinSource       GPIO_PinSource3
#define USART10_Rx_GPIO_PinSource       GPIO_PinSource2
#endif /* STM32F413_423xx */

/* Type definitions ----------------------------------------------------------*/
typedef enum
{
  USART_BaudRate1200   = 1200,
  USART_BaudRate2400   = 2400,
  USART_BaudRate4800   = 4800,
  USART_BaudRate9600   = 9600,
  USART_BaudRate19200  = 19200,
  USART_BaudRate38400  = 38400,
  USART_BaudRate57600  = 57600,
  USART_BaudRate115200 = 115200,
  USART_BaudRate230400 = 230400,
  USART_BaudRate460800 = 460800,
  USART_BaudRate921600 = 921600
}USART_BaudRate;

typedef enum
{
  USART_DataBits8 = USART_WordLength_8b,
  USART_DataBits9 = USART_WordLength_9b
}USART_DataBits;

typedef enum
{
  USART_StopBits1 = USART_StopBits_1,
  USART_StopBits2 = USART_StopBits_2
}USART_StopBits;

typedef enum
{
  USART_ParityNo   = USART_Parity_No,
  USART_ParityEven = USART_Parity_Even,
  USART_ParityOdd  = USART_Parity_Odd
}USART_Parity;

/* Variable declarations -----------------------------------------------------*/
/* Variable definitions ------------------------------------------------------*/
/* Function declarations -----------------------------------------------------*/
void USART_Configure(USART_TypeDef *USARTx, USART_BaudRate BaudRate, USART_DataBits DataBits, USART_StopBits StopBits, USART_Parity Parity);
void USART_Unconfigure(USART_TypeDef *USARTx);

uint32_t USART_SetSendData(USART_TypeDef *USARTx, const void *Data, uint32_t Length);
uint32_t USART_GetReceiveData(USART_TypeDef *USARTx, void *Data, uint32_t Length);

void USART_SetSendDataFinishCallback(USART_TypeDef *USARTx, void (*Function)(void));
void USART_SetReceiveDataFinishCallback(USART_TypeDef *USARTx, void (*Function)(void));

void USART_ClearSendDataFinishCallback(USART_TypeDef *USARTx);
void USART_ClearReceiveDataFinishCallback(USART_TypeDef *USARTx);

uint64_t USART_GetSendDataCount(USART_TypeDef *USARTx);
uint64_t USART_GetReceiveDataCount(USART_TypeDef *USARTx);

void USART_ClearSendDataCount(USART_TypeDef *USARTx);
void USART_ClearReceiveDataCount(USART_TypeDef *USARTx);

uint32_t USART_GetUsedSendBufferSize(USART_TypeDef *USARTx);
uint32_t USART_GetUsedReceiveBufferSize(USART_TypeDef *USARTx);
uint32_t USART_GetUnusedSendBufferSize(USART_TypeDef *USARTx);
uint32_t USART_GetUnusedReceiveBufferSize(USART_TypeDef *USARTx);

float USART_GetSendBufferUnilizationRatio(USART_TypeDef *USARTx);
float USART_GetReceiveBufferUnilizationRatio(USART_TypeDef *USARTx);

void USART_ClearSendBuffer(USART_TypeDef *USARTx);
void USART_ClearReceiveBuffer(USART_TypeDef *USARTx);

bool USART_IsSendBufferEmpty(USART_TypeDef *USARTx);
bool USART_IsReceiveBufferEmpty(USART_TypeDef *USARTx);
bool USART_IsSendBufferFull(USART_TypeDef *USARTx);
bool USART_IsReceiveBufferFull(USART_TypeDef *USARTx);
bool USART_IsSendingData(USART_TypeDef *USARTx);

/* Function definitions ------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __USART_H */
