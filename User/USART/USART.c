/**
  ******************************************************************************
  * @file    USART.c
  * @author  XinLi
  * @version v3.10
  * @date    24-October-2017
  * @brief   USART driver.
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

/* Header includes -----------------------------------------------------------*/
#include "USART.h"
#include <string.h>

/* Macro definitions ---------------------------------------------------------*/
/* Type definitions ----------------------------------------------------------*/
/* Variable declarations -----------------------------------------------------*/
/* Variable definitions ------------------------------------------------------*/
#if (USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx) || (USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx)
static __IO bool USART1_RCC_Flag = false, USART1_GPIO_Flag = false, USART1_NVIC_Flag = false;

#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
static __IO bool USART1_DMA_Tx_Flag = false, USART1_SendData_Flag = false;
static __IO uint32_t USART1_SendData_P = NULL;
static uint8_t USART1_Buffer_Tx[USART1_BufferSize_Tx]         = {NULL};
static uint8_t USART1_DMA_Buffer_Tx[USART1_DMA_BufferSize_Tx] = {NULL};
static __IO void (*USART1_SendDataFinishCallback)(void) = NULL;
static __IO uint64_t USART1_SendDataCount = NULL;
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */

#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
static __IO bool USART1_DMA_Rx_Flag = false;
static __IO uint32_t USART1_ReceiveData_P = NULL;
static uint8_t USART1_Buffer_Rx[USART1_BufferSize_Rx]         = {NULL};
static uint8_t USART1_DMA_Buffer_Rx[USART1_DMA_BufferSize_Rx] = {NULL};
static __IO void (*USART1_ReceiveDataFinishCallback)(void) = NULL;
static __IO uint64_t USART1_ReceiveDataCount = NULL;
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
#endif /* (USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx) || (USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx) */

#if (USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx) || (USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx)
static __IO bool USART2_RCC_Flag = false, USART2_GPIO_Flag = false, USART2_NVIC_Flag = false;

#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
static __IO bool USART2_DMA_Tx_Flag = false, USART2_SendData_Flag = false;
static __IO uint32_t USART2_SendData_P = NULL;
static uint8_t USART2_Buffer_Tx[USART2_BufferSize_Tx]         = {NULL};
static uint8_t USART2_DMA_Buffer_Tx[USART2_DMA_BufferSize_Tx] = {NULL};
static __IO void (*USART2_SendDataFinishCallback)(void) = NULL;
static __IO uint64_t USART2_SendDataCount = NULL;
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */

#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
static __IO bool USART2_DMA_Rx_Flag = false;
static __IO uint32_t USART2_ReceiveData_P = NULL;
static uint8_t USART2_Buffer_Rx[USART2_BufferSize_Rx]         = {NULL};
static uint8_t USART2_DMA_Buffer_Rx[USART2_DMA_BufferSize_Rx] = {NULL};
static __IO void (*USART2_ReceiveDataFinishCallback)(void) = NULL;
static __IO uint64_t USART2_ReceiveDataCount = NULL;
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
#endif /* (USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx) || (USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx) */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
#if (USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx) || (USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx)
static __IO bool USART3_RCC_Flag = false, USART3_GPIO_Flag = false, USART3_NVIC_Flag = false;

#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
static __IO bool USART3_DMA_Tx_Flag = false, USART3_SendData_Flag = false;
static __IO uint32_t USART3_SendData_P = NULL;
static uint8_t USART3_Buffer_Tx[USART3_BufferSize_Tx]         = {NULL};
static uint8_t USART3_DMA_Buffer_Tx[USART3_DMA_BufferSize_Tx] = {NULL};
static __IO void (*USART3_SendDataFinishCallback)(void) = NULL;
static __IO uint64_t USART3_SendDataCount = NULL;
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */

#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
static __IO bool USART3_DMA_Rx_Flag = false;
static __IO uint32_t USART3_ReceiveData_P = NULL;
static uint8_t USART3_Buffer_Rx[USART3_BufferSize_Rx]         = {NULL};
static uint8_t USART3_DMA_Buffer_Rx[USART3_DMA_BufferSize_Rx] = {NULL};
static __IO void (*USART3_ReceiveDataFinishCallback)(void) = NULL;
static __IO uint64_t USART3_ReceiveDataCount = NULL;
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
#endif /* (USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx) || (USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx) */
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
#if (USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx) || (USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx)
static __IO bool USART4_RCC_Flag = false, USART4_GPIO_Flag = false, USART4_NVIC_Flag = false;

#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
static __IO bool USART4_DMA_Tx_Flag = false, USART4_SendData_Flag = false;
static __IO uint32_t USART4_SendData_P = NULL;
static uint8_t USART4_Buffer_Tx[USART4_BufferSize_Tx]         = {NULL};
static uint8_t USART4_DMA_Buffer_Tx[USART4_DMA_BufferSize_Tx] = {NULL};
static __IO void (*USART4_SendDataFinishCallback)(void) = NULL;
static __IO uint64_t USART4_SendDataCount = NULL;
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */

#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
static __IO bool USART4_DMA_Rx_Flag = false;
static __IO uint32_t USART4_ReceiveData_P = NULL;
static uint8_t USART4_Buffer_Rx[USART4_BufferSize_Rx]         = {NULL};
static uint8_t USART4_DMA_Buffer_Rx[USART4_DMA_BufferSize_Rx] = {NULL};
static __IO void (*USART4_ReceiveDataFinishCallback)(void) = NULL;
static __IO uint64_t USART4_ReceiveDataCount = NULL;
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
#endif /* (USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx) || (USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx) */

#if (USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx) || (USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx)
static __IO bool USART5_RCC_Flag = false, USART5_GPIO_Flag = false, USART5_NVIC_Flag = false;

#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
static __IO bool USART5_DMA_Tx_Flag = false, USART5_SendData_Flag = false;
static __IO uint32_t USART5_SendData_P = NULL;
static uint8_t USART5_Buffer_Tx[USART5_BufferSize_Tx]         = {NULL};
static uint8_t USART5_DMA_Buffer_Tx[USART5_DMA_BufferSize_Tx] = {NULL};
static __IO void (*USART5_SendDataFinishCallback)(void) = NULL;
static __IO uint64_t USART5_SendDataCount = NULL;
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */

#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
static __IO bool USART5_DMA_Rx_Flag = false;
static __IO uint32_t USART5_ReceiveData_P = NULL;
static uint8_t USART5_Buffer_Rx[USART5_BufferSize_Rx]         = {NULL};
static uint8_t USART5_DMA_Buffer_Rx[USART5_DMA_BufferSize_Rx] = {NULL};
static __IO void (*USART5_ReceiveDataFinishCallback)(void) = NULL;
static __IO uint64_t USART5_ReceiveDataCount = NULL;
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
#endif /* (USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx) || (USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx) */
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */

#if (USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx) || (USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx)
static __IO bool USART6_RCC_Flag = false, USART6_GPIO_Flag = false, USART6_NVIC_Flag = false;

#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
static __IO bool USART6_DMA_Tx_Flag = false, USART6_SendData_Flag = false;
static __IO uint32_t USART6_SendData_P = NULL;
static uint8_t USART6_Buffer_Tx[USART6_BufferSize_Tx]         = {NULL};
static uint8_t USART6_DMA_Buffer_Tx[USART6_DMA_BufferSize_Tx] = {NULL};
static __IO void (*USART6_SendDataFinishCallback)(void) = NULL;
static __IO uint64_t USART6_SendDataCount = NULL;
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */

#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
static __IO bool USART6_DMA_Rx_Flag = false;
static __IO uint32_t USART6_ReceiveData_P = NULL;
static uint8_t USART6_Buffer_Rx[USART6_BufferSize_Rx]         = {NULL};
static uint8_t USART6_DMA_Buffer_Rx[USART6_DMA_BufferSize_Rx] = {NULL};
static __IO void (*USART6_ReceiveDataFinishCallback)(void) = NULL;
static __IO uint64_t USART6_ReceiveDataCount = NULL;
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
#endif /* (USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx) || (USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx) */

#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
#if (USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx) || (USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx)
static __IO bool USART7_RCC_Flag = false, USART7_GPIO_Flag = false, USART7_NVIC_Flag = false;

#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
static __IO bool USART7_DMA_Tx_Flag = false, USART7_SendData_Flag = false;
static __IO uint32_t USART7_SendData_P = NULL;
static uint8_t USART7_Buffer_Tx[USART7_BufferSize_Tx]         = {NULL};
static uint8_t USART7_DMA_Buffer_Tx[USART7_DMA_BufferSize_Tx] = {NULL};
static __IO void (*USART7_SendDataFinishCallback)(void) = NULL;
static __IO uint64_t USART7_SendDataCount = NULL;
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */

#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
static __IO bool USART7_DMA_Rx_Flag = false;
static __IO uint32_t USART7_ReceiveData_P = NULL;
static uint8_t USART7_Buffer_Rx[USART7_BufferSize_Rx]         = {NULL};
static uint8_t USART7_DMA_Buffer_Rx[USART7_DMA_BufferSize_Rx] = {NULL};
static __IO void (*USART7_ReceiveDataFinishCallback)(void) = NULL;
static __IO uint64_t USART7_ReceiveDataCount = NULL;
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
#endif /* (USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx) || (USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx) */

#if (USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx) || (USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx)
static __IO bool USART8_RCC_Flag = false, USART8_GPIO_Flag = false, USART8_NVIC_Flag = false;

#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
static __IO bool USART8_DMA_Tx_Flag = false, USART8_SendData_Flag = false;
static __IO uint32_t USART8_SendData_P = NULL;
static uint8_t USART8_Buffer_Tx[USART8_BufferSize_Tx]         = {NULL};
static uint8_t USART8_DMA_Buffer_Tx[USART8_DMA_BufferSize_Tx] = {NULL};
static __IO void (*USART8_SendDataFinishCallback)(void) = NULL;
static __IO uint64_t USART8_SendDataCount = NULL;
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */

#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
static __IO bool USART8_DMA_Rx_Flag = false;
static __IO uint32_t USART8_ReceiveData_P = NULL;
static uint8_t USART8_Buffer_Rx[USART8_BufferSize_Rx]         = {NULL};
static uint8_t USART8_DMA_Buffer_Rx[USART8_DMA_BufferSize_Rx] = {NULL};
static __IO void (*USART8_ReceiveDataFinishCallback)(void) = NULL;
static __IO uint64_t USART8_ReceiveDataCount = NULL;
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
#endif /* (USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx) || (USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx) */
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */

#if defined(STM32F413_423xx)
#if (USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx) || (USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx)
static __IO bool USART9_RCC_Flag = false, USART9_GPIO_Flag = false, USART9_NVIC_Flag = false;

#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
static __IO bool USART9_DMA_Tx_Flag = false, USART9_SendData_Flag = false;
static __IO uint32_t USART9_SendData_P = NULL;
static uint8_t USART9_Buffer_Tx[USART9_BufferSize_Tx]         = {NULL};
static uint8_t USART9_DMA_Buffer_Tx[USART9_DMA_BufferSize_Tx] = {NULL};
static __IO void (*USART9_SendDataFinishCallback)(void) = NULL;
static __IO uint64_t USART9_SendDataCount = NULL;
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */

#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
static __IO bool USART9_DMA_Rx_Flag = false;
static __IO uint32_t USART9_ReceiveData_P = NULL;
static uint8_t USART9_Buffer_Rx[USART9_BufferSize_Rx]         = {NULL};
static uint8_t USART9_DMA_Buffer_Rx[USART9_DMA_BufferSize_Rx] = {NULL};
static __IO void (*USART9_ReceiveDataFinishCallback)(void) = NULL;
static __IO uint64_t USART9_ReceiveDataCount = NULL;
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
#endif /* (USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx) || (USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx) */

#if (USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx) || (USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx)
static __IO bool USART10_RCC_Flag = false, USART10_GPIO_Flag = false, USART10_NVIC_Flag = false;

#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
static __IO bool USART10_DMA_Tx_Flag = false, USART10_SendData_Flag = false;
static __IO uint32_t USART10_SendData_P = NULL;
static uint8_t USART10_Buffer_Tx[USART10_BufferSize_Tx]         = {NULL};
static uint8_t USART10_DMA_Buffer_Tx[USART10_DMA_BufferSize_Tx] = {NULL};
static __IO void (*USART10_SendDataFinishCallback)(void) = NULL;
static __IO uint64_t USART10_SendDataCount = NULL;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */

#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
static __IO bool USART10_DMA_Rx_Flag = false;
static __IO uint32_t USART10_ReceiveData_P = NULL;
static uint8_t USART10_Buffer_Rx[USART10_BufferSize_Rx]         = {NULL};
static uint8_t USART10_DMA_Buffer_Rx[USART10_DMA_BufferSize_Rx] = {NULL};
static __IO void (*USART10_ReceiveDataFinishCallback)(void) = NULL;
static __IO uint64_t USART10_ReceiveDataCount = NULL;
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
#endif /* (USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx) || (USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx) */
#endif /* STM32F413_423xx */

/* Function declarations -----------------------------------------------------*/
static void USART_DMA_TX_Configure(USART_TypeDef *USARTx);
static void USART_DMA_RX_Configure(USART_TypeDef *USARTx);
static void USART_DMA_TX_Unconfigure(USART_TypeDef *USARTx);
static void USART_DMA_RX_Unconfigure(USART_TypeDef *USARTx);
static void USART_NVIC_Configure(USART_TypeDef *USARTx);
static void USART_NVIC_Unconfigure(USART_TypeDef *USARTx);
static void USART_SendDataFinishCallback(USART_TypeDef *USARTx);
static void USART_ReceiveDataFinishCallback(USART_TypeDef *USARTx);

/* Function definitions ------------------------------------------------------*/

/**
  * @brief  USART configure.
  * @param  [in] USARTx:   Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @param  [in] BaudRate: Communication baud rate.
  * @param  [in] DataBits: The number of data bits sent or received.
  * @param  [in] StopBits: The number of stop bits to send or receive.
  * @param  [in] Parity:   Parity mode.
  * @return None.
  */
void USART_Configure(USART_TypeDef *USARTx, USART_BaudRate BaudRate, USART_DataBits DataBits, USART_StopBits StopBits, USART_Parity Parity)
{
  uint16_t          USART_Mode          =  NULL;  /* USART Mode definition. */
  GPIO_InitTypeDef  GPIO_InitStructure  = {NULL}; /* GPIO Init structure definition. */
  USART_InitTypeDef USART_InitStructure = {NULL}; /* USART Init Structure definition. */
  
  if(USARTx == USART1)
  {
#if (USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx) || (USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx)
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    USART_Mode          |= USART_Mode_Tx;
    USART1_SendData_Flag = false;
    USART1_SendData_P    = NULL;
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
    
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    USART_Mode          |= USART_Mode_Rx;
    USART1_ReceiveData_P = NULL;
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
    
    if(USART1_RCC_Flag == false)
    {
      USART1_RCC_Flag = true;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  /* Enable USART1 clock. */
    }
    
    if(USART1_GPIO_Flag == false)
    {
      USART1_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART1_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART1_Tx GPIO clock. */
      RCC_AHB1PeriphClockCmd(USART1_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART1_Rx GPIO clock. */
      
      GPIO_PinAFConfig(USART1_Tx_GPIO, USART1_Tx_GPIO_PinSource, GPIO_AF_USART1); /* Connect GPIOxn to USART1_Tx. */
      GPIO_PinAFConfig(USART1_Rx_GPIO, USART1_Rx_GPIO_PinSource, GPIO_AF_USART1); /* Connect GPIOxn to USART1_Rx. */
      
      GPIO_InitStructure.GPIO_Pin = USART1_Tx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(USART1_Tx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = USART1_Rx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(USART1_Rx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
#else
    return;
#endif /* (USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx) || (USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx) */
  }
  else if(USARTx == USART2)
  {
#if (USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx) || (USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx)
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    USART_Mode          |= USART_Mode_Tx;
    USART2_SendData_Flag = false;
    USART2_SendData_P    = NULL;
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
    
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    USART_Mode          |= USART_Mode_Rx;
    USART2_ReceiveData_P = NULL;
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
    
    if(USART2_RCC_Flag == false)
    {
      USART2_RCC_Flag = true;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  /* Enable USART2 clock */
    }
    
    if(USART2_GPIO_Flag == false)
    {
      USART2_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART2_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART2_Tx GPIO clock. */
      RCC_AHB1PeriphClockCmd(USART2_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART2_Rx GPIO clock. */
      
      GPIO_PinAFConfig(USART2_Tx_GPIO, USART2_Tx_GPIO_PinSource, GPIO_AF_USART2);	/* Connect GPIOxn to USART2_Tx. */
      GPIO_PinAFConfig(USART2_Rx_GPIO, USART2_Rx_GPIO_PinSource, GPIO_AF_USART2); /* Connect GPIOxn to USART2_Rx. */
      
      GPIO_InitStructure.GPIO_Pin = USART2_Tx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(USART2_Tx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = USART2_Rx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(USART2_Rx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
#else
    return;
#endif /* (USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx) || (USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx) */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if (USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx) || (USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx)
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    USART_Mode          |= USART_Mode_Tx;
    USART3_SendData_Flag = false;
    USART3_SendData_P    = NULL;
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
    
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    USART_Mode          |= USART_Mode_Rx;
    USART3_ReceiveData_P = NULL;
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
    
    if(USART3_RCC_Flag == false)
    {
      USART3_RCC_Flag = true;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  /* Enable USART3 clock. */
    }
    
    if(USART3_GPIO_Flag == false)
    {
      USART3_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART3_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART3_Tx GPIO clock. */
      RCC_AHB1PeriphClockCmd(USART3_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART3_Rx GPIO clock. */
      
      GPIO_PinAFConfig(USART3_Tx_GPIO, USART3_Tx_GPIO_PinSource, GPIO_AF_USART3); /* Connect GPIOxn to USART3_Tx. */
      GPIO_PinAFConfig(USART3_Rx_GPIO, USART3_Rx_GPIO_PinSource, GPIO_AF_USART3); /* Connect GPIOxn to USART3_Rx. */
      
      GPIO_InitStructure.GPIO_Pin = USART3_Tx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(USART3_Tx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = USART3_Rx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(USART3_Rx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
#else
    return;
#endif /* (USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx) || (USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx) */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if (USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx) || (USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx)
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    USART_Mode          |= USART_Mode_Tx;
    USART4_SendData_Flag = false;
    USART4_SendData_P    = NULL;
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
    
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    USART_Mode          |= USART_Mode_Rx;
    USART4_ReceiveData_P = NULL;
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
    
    if(USART4_RCC_Flag == false)
    {
      USART4_RCC_Flag = true;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); /* Enable USART4 clock. */
    }
    
    if(USART4_GPIO_Flag == false)
    {
      USART4_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART4_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART4_Tx GPIO clock. */
      RCC_AHB1PeriphClockCmd(USART4_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART4_Rx GPIO clock. */
      
      GPIO_PinAFConfig(USART4_Tx_GPIO, USART4_Tx_GPIO_PinSource, GPIO_AF_UART4);  /* Connect GPIOxn to USART4_Tx. */
      GPIO_PinAFConfig(USART4_Rx_GPIO, USART4_Rx_GPIO_PinSource, GPIO_AF_UART4);  /* Connect GPIOxn to USART4_Rx. */
      
      GPIO_InitStructure.GPIO_Pin = USART4_Tx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(USART4_Tx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = USART4_Rx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(USART4_Rx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
#else
    return;
#endif /* (USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx) || (USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx) */
  }
  else if(USARTx == UART5)
  {
#if (USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx) || (USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx)
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    USART_Mode          |= USART_Mode_Tx;
    USART5_SendData_Flag = false;
    USART5_SendData_P    = NULL;
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
    
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    USART_Mode          |= USART_Mode_Rx;
    USART5_ReceiveData_P = NULL;
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
    
    if(USART5_RCC_Flag == false)
    {
      USART5_RCC_Flag = true;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); /* Enable USART5 clock. */
    }
    
    if(USART5_GPIO_Flag == false)
    {
      USART5_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART5_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART5_Tx GPIO clock. */
      RCC_AHB1PeriphClockCmd(USART5_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART5_Rx GPIO clock. */
      
      GPIO_PinAFConfig(USART5_Tx_GPIO, USART5_Tx_GPIO_PinSource, GPIO_AF_UART5);  /* Connect GPIOxn to USART5_Tx. */
      GPIO_PinAFConfig(USART5_Rx_GPIO, USART5_Rx_GPIO_PinSource, GPIO_AF_UART5);  /* Connect GPIOxn to USART5_Rx. */
      
      GPIO_InitStructure.GPIO_Pin = USART5_Tx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(USART5_Tx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = USART5_Rx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(USART5_Rx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
#else
    return;
#endif /* (USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx) || (USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx) */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if (USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx) || (USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx)
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    USART_Mode          |= USART_Mode_Tx;
    USART6_SendData_Flag = false;
    USART6_SendData_P    = NULL;
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
    
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    USART_Mode          |= USART_Mode_Rx;
    USART6_ReceiveData_P = NULL;
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
    
    if(USART6_RCC_Flag == false)
    {
      USART6_RCC_Flag = true;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);  /* Enable USART6 clock. */
    }
    
    if(USART6_GPIO_Flag == false)
    {
      USART6_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART6_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART6_Tx GPIO clock. */
      RCC_AHB1PeriphClockCmd(USART6_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART6_Rx GPIO clock. */
      
      GPIO_PinAFConfig(USART6_Tx_GPIO, USART6_Tx_GPIO_PinSource, GPIO_AF_USART6); /* Connect GPIOxn to USART6_Tx. */
      GPIO_PinAFConfig(USART6_Rx_GPIO, USART6_Rx_GPIO_PinSource, GPIO_AF_USART6); /* Connect GPIOxn to USART6_Rx. */
      
      GPIO_InitStructure.GPIO_Pin = USART6_Tx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(USART6_Tx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = USART6_Rx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(USART6_Rx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
#else
    return;
#endif /* (USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx) || (USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx) */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if (USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx) || (USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx)
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    USART_Mode          |= USART_Mode_Tx;
    USART7_SendData_Flag = false;
    USART7_SendData_P    = NULL;
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
    
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    USART_Mode          |= USART_Mode_Rx;
    USART7_ReceiveData_P = NULL;
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
    
    if(USART7_RCC_Flag == false)
    {
      USART7_RCC_Flag = true;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE); /* Enable USART7 clock. */
    }
    
    if(USART7_GPIO_Flag == false)
    {
      USART7_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART7_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART7_Tx GPIO clock. */
      RCC_AHB1PeriphClockCmd(USART7_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART7_Rx GPIO clock. */
      
      GPIO_PinAFConfig(USART7_Tx_GPIO, USART7_Tx_GPIO_PinSource, GPIO_AF_UART7);  /* Connect GPIOxn to USART7_Tx. */
      GPIO_PinAFConfig(USART7_Rx_GPIO, USART7_Rx_GPIO_PinSource, GPIO_AF_UART7);  /* Connect GPIOxn to USART7_Rx. */
      
      GPIO_InitStructure.GPIO_Pin = USART7_Tx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(USART7_Tx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = USART7_Rx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(USART7_Rx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
#else
    return;
#endif /* (USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx) || (USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx) */
  }
  else if(USARTx == UART8)
  {
#if (USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx) || (USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx)
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    USART_Mode          |= USART_Mode_Tx;
    USART8_SendData_Flag = false;
    USART8_SendData_P    = NULL;
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
    
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    USART_Mode          |= USART_Mode_Rx;
    USART8_ReceiveData_P = NULL;
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
    
    if(USART8_RCC_Flag == false)
    {
      USART8_RCC_Flag = true;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE); /* Enable USART8 clock. */
    }
    
    if(USART8_GPIO_Flag == false)
    {
      USART8_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART8_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART8_Tx GPIO clock. */
      RCC_AHB1PeriphClockCmd(USART8_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART8_Rx GPIO clock. */
      
      GPIO_PinAFConfig(USART8_Tx_GPIO, USART8_Tx_GPIO_PinSource, GPIO_AF_UART8);  /* Connect GPIOxn to USART8_Tx. */
      GPIO_PinAFConfig(USART8_Rx_GPIO, USART8_Rx_GPIO_PinSource, GPIO_AF_UART8);  /* Connect GPIOxn to USART8_Rx. */
      
      GPIO_InitStructure.GPIO_Pin = USART8_Tx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(USART8_Tx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = USART8_Rx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(USART8_Rx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
#else
    return;
#endif /* (USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx) || (USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx) */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if (USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx) || (USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx)
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    USART_Mode          |= USART_Mode_Tx;
    USART9_SendData_Flag = false;
    USART9_SendData_P    = NULL;
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
    
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    USART_Mode          |= USART_Mode_Rx;
    USART9_ReceiveData_P = NULL;
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
    
    if(USART9_RCC_Flag == false)
    {
      USART9_RCC_Flag = true;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART9, ENABLE); /* Enable USART9 clock. */
    }
    
    if(USART9_GPIO_Flag == false)
    {
      USART9_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART9_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART9_Tx GPIO clock. */
      RCC_AHB1PeriphClockCmd(USART9_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART9_Rx GPIO clock. */
      
      GPIO_PinAFConfig(USART9_Tx_GPIO, USART9_Tx_GPIO_PinSource, GPIO_AF11_UART9);  /* Connect GPIOxn to USART9_Tx. */
      GPIO_PinAFConfig(USART9_Rx_GPIO, USART9_Rx_GPIO_PinSource, GPIO_AF11_UART9);  /* Connect GPIOxn to USART9_Rx. */
      
      GPIO_InitStructure.GPIO_Pin = USART9_Tx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(USART9_Tx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = USART9_Rx_GPIO_Pin;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(USART9_Rx_GPIO, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
#else
    return;
#endif /* (USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx) || (USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx) */
  }
  else if(USARTx == UART10)
  {
#if (USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx) || (USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx)
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    USART_Mode           |= USART_Mode_Tx;
    USART10_SendData_Flag = false;
    USART10_SendData_P    = NULL;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
    
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    USART_Mode           |= USART_Mode_Rx;
    USART10_ReceiveData_P = NULL;
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
    
    if(USART10_RCC_Flag == false)
    {
      USART10_RCC_Flag = true;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART10, ENABLE);  /* Enable USART10 clock. */
    }
    
    if(USART10_GPIO_Flag == false)
    {
      USART10_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART10_Tx_RCC_AHB1Periph_GPIO, ENABLE); /* Enable USART10_Tx GPIO clock. */
      RCC_AHB1PeriphClockCmd(USART10_Rx_RCC_AHB1Periph_GPIO, ENABLE); /* Enable USART10_Rx GPIO clock. */
      
      GPIO_PinAFConfig(USART10_Tx_GPIO, USART10_Tx_GPIO_PinSource, GPIO_AF11_UART10); /* Connect GPIOxn to USART10_Tx. */
      GPIO_PinAFConfig(USART10_Rx_GPIO, USART10_Rx_GPIO_PinSource, GPIO_AF11_UART10); /* Connect GPIOxn to USART10_Rx. */
      
      GPIO_InitStructure.GPIO_Pin = USART10_Tx_GPIO_Pin;  /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(USART10_Tx_GPIO, &GPIO_InitStructure);    /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = USART10_Rx_GPIO_Pin;  /* Specifies the GPIO pins to be configured. */
      GPIO_Init(USART10_Rx_GPIO, &GPIO_InitStructure);    /* Initializes the GPIOx peripheral. */
    }
#else
    return;
#endif /* (USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx) || (USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx) */
  }
#endif /* STM32F413_423xx */
  
  USART_DeInit(USARTx);                                                           /* Deinitializes the USARTx peripheral registers to their default reset values. */
  USART_InitStructure.USART_BaudRate = BaudRate;                                  /* This member configures the USART communication baud rate. */
  USART_InitStructure.USART_WordLength = DataBits;                                /* Specifies the number of data bits transmitted or received in a frame. */
  USART_InitStructure.USART_StopBits = StopBits;                                  /* Specifies the number of stop bits transmitted. */
  USART_InitStructure.USART_Parity = Parity;                                      /* Specifies the parity mode. */
  USART_InitStructure.USART_Mode = USART_Mode;                                    /* Specifies whether the Receive or Transmit mode is enabled or disabled. */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* Specifies wether the hardware flow control mode is enabled or disabled. */
  USART_Init(USARTx, &USART_InitStructure);                                       /* Initializes the USARTx peripheral. */
  
  if(USART_Mode == USART_Mode_Tx)
  {
    USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USARTx. */
    USART_ITConfig(USARTx, USART_IT_TC, ENABLE);    /* Enabling interrupts specified USARTx. */
    USART_DMA_TX_Configure(USARTx);                 /* Configure USARTx DMA TX. */
  }
  else if(USART_Mode == USART_Mode_Rx)
  {
    USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USARTx. */
    USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);  /* Enabling interrupts specified USARTx. */
    USART_DMA_RX_Configure(USARTx);                 /* Configure USARTx DMA RX. */
  }
  else
  {
    USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USARTx. */
    USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USARTx. */
    USART_ITConfig(USARTx, USART_IT_TC, ENABLE);    /* Enabling interrupts specified USARTx. */
    USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);  /* Enabling interrupts specified USARTx. */
    USART_DMA_TX_Configure(USARTx);                 /* Configure USARTx DMA TX. */
    USART_DMA_RX_Configure(USARTx);                 /* Configure USARTx DMA RX. */
  }
  
  USART_Cmd(USARTx, ENABLE);                        /* Enables or disables the specified USARTx peripheral. */
  USART_NVIC_Configure(USARTx);                     /* Configure USARTx NVIC. */
}

/**
  * @brief  USART unconfigure.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
void USART_Unconfigure(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if (USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx) || (USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx)
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    USART1_SendData_Flag = false;
    USART1_SendData_P    = NULL;
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
    
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    USART1_ReceiveData_P = NULL;
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
    
    if(USART1_RCC_Flag == true)
    {
      USART1_RCC_Flag = false;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE); /* Disable USART1 clock. */
      
      USART_DeInit(USART1); /* Deinitializes the USART1 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(USART1); /* Unconfigure USART1 DMA TX. */
      USART_DMA_RX_Unconfigure(USART1); /* Unconfigure USART1 DMA RX. */
      
      USART_NVIC_Unconfigure(USART1);   /* Unconfigure USART1 NVIC. */
    }
#endif /* (USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx) || (USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx) */
  }
  else if(USARTx == USART2)
  {
#if (USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx) || (USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx)
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    USART2_SendData_Flag = false;
    USART2_SendData_P    = NULL;
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
    
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    USART2_ReceiveData_P = NULL;
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
    
    if(USART2_RCC_Flag == true)
    {
      USART2_RCC_Flag = false;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE); /* Disable USART2 clock. */
      
      USART_DeInit(USART2); /* Deinitializes the USART2 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(USART2); /* Unconfigure USART2 DMA TX. */
      USART_DMA_RX_Unconfigure(USART2); /* Unconfigure USART2 DMA RX. */
      
      USART_NVIC_Unconfigure(USART2);   /* Unconfigure USART2 NVIC. */
    }
#endif /* (USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx) || (USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx) */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if (USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx) || (USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx)
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    USART3_SendData_Flag = false;
    USART3_SendData_P    = NULL;
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
    
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    USART3_ReceiveData_P = NULL;
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
    
    if(USART3_RCC_Flag == true)
    {
      USART3_RCC_Flag = false;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, DISABLE); /* Disable USART3 clock. */
      
      USART_DeInit(USART3); /* Deinitializes the USART3 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(USART3); /* Unconfigure USART3 DMA TX. */
      USART_DMA_RX_Unconfigure(USART3); /* Unconfigure USART3 DMA RX. */
      
      USART_NVIC_Unconfigure(USART3);   /* Unconfigure USART3 NVIC. */
    }
#endif /* (USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx) || (USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx) */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if (USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx) || (USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx)
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    USART4_SendData_Flag = false;
    USART4_SendData_P    = NULL;
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
    
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    USART4_ReceiveData_P = NULL;
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
    
    if(USART4_RCC_Flag == true)
    {
      USART4_RCC_Flag = false;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, DISABLE);  /* Disable USART4 clock. */
      
      USART_DeInit(UART4);  /* Deinitializes the USART4 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART4);  /* Unconfigure USART4 DMA TX. */
      USART_DMA_RX_Unconfigure(UART4);  /* Unconfigure USART4 DMA RX. */
      
      USART_NVIC_Unconfigure(UART4);    /* Unconfigure USART4 NVIC. */
    }
#endif /* (USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx) || (USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx) */
  }
  else if(USARTx == UART5)
  {
#if (USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx) || (USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx)
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    USART5_SendData_Flag = false;
    USART5_SendData_P    = NULL;
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
    
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    USART5_ReceiveData_P = NULL;
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
    
    if(USART5_RCC_Flag == true)
    {
      USART5_RCC_Flag = false;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, DISABLE);  /* Disable USART5 clock. */
      
      USART_DeInit(UART5);  /* Deinitializes the USART5 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART5);  /* Unconfigure USART5 DMA TX. */
      USART_DMA_RX_Unconfigure(UART5);  /* Unconfigure USART5 DMA RX. */
      
      USART_NVIC_Unconfigure(UART5);    /* Unconfigure USART5 NVIC. */
    }
#endif /* (USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx) || (USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx) */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if (USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx) || (USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx)
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    USART6_SendData_Flag = false;
    USART6_SendData_P    = NULL;
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
    
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    USART6_ReceiveData_P = NULL;
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
    
    if(USART6_RCC_Flag == true)
    {
      USART6_RCC_Flag = false;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, DISABLE); /* Disable USART6 clock. */
      
      USART_DeInit(USART6); /* Deinitializes the USART6 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(USART6); /* Unconfigure USART6 DMA TX. */
      USART_DMA_RX_Unconfigure(USART6); /* Unconfigure USART6 DMA RX. */
      
      USART_NVIC_Unconfigure(USART6);   /* Unconfigure USART6 NVIC. */
    }
#endif /* (USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx) || (USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx) */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if (USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx) || (USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx)
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    USART7_SendData_Flag = false;
    USART7_SendData_P    = NULL;
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
    
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    USART7_ReceiveData_P = NULL;
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
    
    if(USART7_RCC_Flag == true)
    {
      USART7_RCC_Flag = false;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, DISABLE);  /* Disable USART7 clock. */
      
      USART_DeInit(UART7);  /* Deinitializes the USART7 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART7);  /* Unconfigure USART7 DMA TX. */
      USART_DMA_RX_Unconfigure(UART7);  /* Unconfigure USART7 DMA RX. */
      
      USART_NVIC_Unconfigure(UART7);    /* Unconfigure USART7 NVIC. */
    }
#endif /* (USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx) || (USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx) */
  }
  else if(USARTx == UART8)
  {
#if (USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx) || (USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx)
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    USART8_SendData_Flag = false;
    USART8_SendData_P    = NULL;
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
    
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    USART8_ReceiveData_P = NULL;
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
    
    if(USART8_RCC_Flag == true)
    {
      USART8_RCC_Flag = false;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, DISABLE);  /* Disable USART8 clock. */
      
      USART_DeInit(UART8);  /* Deinitializes the USART8 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART8);  /* Unconfigure USART8 DMA TX. */
      USART_DMA_RX_Unconfigure(UART8);  /* Unconfigure USART8 DMA RX. */
      
      USART_NVIC_Unconfigure(UART8);    /* Unconfigure USART8 NVIC. */
    }
#endif /* (USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx) || (USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx) */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if (USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx) || (USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx)
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    USART9_SendData_Flag = false;
    USART9_SendData_P    = NULL;
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
    
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    USART9_ReceiveData_P = NULL;
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
    
    if(USART9_RCC_Flag == true)
    {
      USART9_RCC_Flag = false;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART9, DISABLE);  /* Disable USART9 clock. */
      
      USART_DeInit(UART9);  /* Deinitializes the USART9 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART9);  /* Unconfigure USART9 DMA TX. */
      USART_DMA_RX_Unconfigure(UART9);  /* Unconfigure USART9 DMA RX. */
      
      USART_NVIC_Unconfigure(UART9);    /* Unconfigure USART9 NVIC. */
    }
#endif /* (USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx) || (USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx) */
  }
  else if(USARTx == UART10)
  {
#if (USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx) || (USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx)
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    USART10_SendData_Flag = false;
    USART10_SendData_P    = NULL;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
    
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    USART10_ReceiveData_P = NULL;
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
    
    if(USART10_RCC_Flag == true)
    {
      USART10_RCC_Flag = false;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART10, DISABLE); /* Disable USART10 clock. */
      
      USART_DeInit(UART10); /* Deinitializes the USART10 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART10); /* Unconfigure USART10 DMA TX. */
      USART_DMA_RX_Unconfigure(UART10); /* Unconfigure USART10 DMA RX. */
      
      USART_NVIC_Unconfigure(UART10);   /* Unconfigure USART10 NVIC. */
    }
#endif /* (USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx) || (USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx) */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  USART DMA TX configure.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
static void USART_DMA_TX_Configure(USART_TypeDef *USARTx)
{
  DMA_InitTypeDef DMA_InitStructure = {NULL}; /* DMA Init structure definition. */
  
  if((USARTx == USART2)
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
     || (USARTx == USART3)
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
     || (USARTx == UART4) || (USARTx == UART5)
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
     || (USARTx == UART7) || (USARTx == UART8)
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
    )
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  /* Enable DMA1 clock. */
  }
  else if((USARTx == USART1) || (USARTx == USART6)
#if defined(STM32F413_423xx)
          || (USARTx == UART9) || (USARTx == UART10)
#endif /* STM32F413_423xx */
         )
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  /* Enable DMA2 clock. */
  }

  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                 /* Data from memory to peripheral. */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        /* Peripheral address does not increase. */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 /* Memory address increment. */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; /* Peripheral data width of 8 bit. */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         /* Memory data width of 8 bit. */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           /* Use Normal mode. */
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   /* Medium priority. */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  /* FIFO mode disabled. */
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           /* Specifies the FIFO threshold level. */
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             /* A single transmission burst memory. */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     /* Peripherals single transmission burst. */
  
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    USART1_DMA_Tx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);     /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART1_DMA_Buffer_Tx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART1_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA2_Stream7);                         /* Deinitialize the DMA2 Stream7 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE); /* Wait for the specified DMA2 Stream7 reset to complete. */
    
    DMA_Init(DMA2_Stream7, &DMA_InitStructure); /* Initializes the DMA2 Stream7. */
    DMA_Cmd(DMA2_Stream7, DISABLE);             /* Disables the specified DMA2 Stream7. */
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    USART2_DMA_Tx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);     /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART2_DMA_Buffer_Tx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART2_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA1_Stream6);                         /* Deinitialize the DMA1 Stream6 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE); /* Wait for the specified DMA1 Stream6 reset to complete. */
    
    DMA_Init(DMA1_Stream6, &DMA_InitStructure); /* Initializes the DMA1 Stream6. */
    DMA_Cmd(DMA1_Stream6, DISABLE);             /* Disables the specified DMA1 Stream6. */
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    USART3_DMA_Tx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);     /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART3_DMA_Buffer_Tx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART3_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA1_Stream3);                         /* Deinitialize the DMA1 Stream3 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE); /* Wait for the specified DMA1 Stream3 reset to complete. */
    
    DMA_Init(DMA1_Stream3, &DMA_InitStructure); /* Initializes the DMA1 Stream3. */
    DMA_Cmd(DMA1_Stream3, DISABLE);             /* Disables the specified DMA1 Stream3. */
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    USART4_DMA_Tx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART4->DR);      /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART4_DMA_Buffer_Tx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART4_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA1_Stream4);                         /* Deinitialize the DMA1 Stream4 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE); /* Wait for the specified DMA1 Stream4 reset to complete. */
    
    DMA_Init(DMA1_Stream4, &DMA_InitStructure); /* Initializes the DMA1 Stream4. */
    DMA_Cmd(DMA1_Stream4, DISABLE);             /* Disables the specified DMA1 Stream4. */
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    USART5_DMA_Tx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART5->DR);      /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART5_DMA_Buffer_Tx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART5_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA1_Stream7);                         /* Deinitialize the DMA1 Stream7 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE); /* Wait for the specified DMA1 Stream7 reset to complete. */
    
    DMA_Init(DMA1_Stream7, &DMA_InitStructure); /* Initializes the DMA1 Stream7. */
    DMA_Cmd(DMA1_Stream7, DISABLE);             /* Disables the specified DMA1 Stream7. */
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    USART6_DMA_Tx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART6->DR);     /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART6_DMA_Buffer_Tx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART6_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA2_Stream6);                         /* Deinitialize the DMA2 Stream6 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE); /* Wait for the specified DMA2 Stream6 reset to complete. */
    
    DMA_Init(DMA2_Stream6, &DMA_InitStructure); /* Initializes the DMA2 Stream6. */
    DMA_Cmd(DMA2_Stream6, DISABLE);             /* Disables the specified DMA2 Stream6. */
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    USART7_DMA_Tx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART7->DR);      /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART7_DMA_Buffer_Tx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART7_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA1_Stream1);                         /* Deinitialize the DMA1 Stream1 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE); /* Wait for the specified DMA1 Stream1 reset to complete. */
    
    DMA_Init(DMA1_Stream1, &DMA_InitStructure); /* Initializes the DMA1 Stream1. */
    DMA_Cmd(DMA1_Stream1, DISABLE);             /* Disables the specified DMA1 Stream1. */
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    USART8_DMA_Tx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART8->DR);      /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART8_DMA_Buffer_Tx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART8_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA1_Stream0);                         /* Deinitialize the DMA1 Stream0 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE); /* Wait for the specified DMA1 Stream0 reset to complete. */
    
    DMA_Init(DMA1_Stream0, &DMA_InitStructure); /* Initializes the DMA1 Stream0. */
    DMA_Cmd(DMA1_Stream0, DISABLE);             /* Disables the specified DMA1 Stream0. */
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    USART9_DMA_Tx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_1;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART9->DR);      /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART9_DMA_Buffer_Tx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART9_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA2_Stream0);                         /* Deinitialize the DMA2 Stream0 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE); /* Wait for the specified DMA2 Stream0 reset to complete. */
    
    DMA_Init(DMA2_Stream0, &DMA_InitStructure); /* Initializes the DMA2 Stream0. */
    DMA_Cmd(DMA2_Stream0, DISABLE);             /* Disables the specified DMA2 Stream0. */
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    USART10_DMA_Tx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_6;                            /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART10->DR);       /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART10_DMA_Buffer_Tx;  /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART10_DMA_BufferSize_Tx;             /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA2_Stream7);                         /* Deinitialize the DMA2 Stream7 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE); /* Wait for the specified DMA2 Stream7 reset to complete. */
    
    DMA_Init(DMA2_Stream7, &DMA_InitStructure); /* Initializes the DMA2 Stream7. */
    DMA_Cmd(DMA2_Stream7, DISABLE);             /* Disables the specified DMA2 Stream7. */
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  USART DMA RX configure.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
static void USART_DMA_RX_Configure(USART_TypeDef *USARTx)
{
  DMA_InitTypeDef DMA_InitStructure = {NULL}; /* DMA Init structure definition. */
  
  if((USARTx == USART2)
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
     || (USARTx == USART3)
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
     || (USARTx == UART4) || (USARTx == UART5)
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
     || (USARTx == UART7) || (USARTx == UART8)
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
    )
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  /* Enable DMA1 clock. */
  }
  else if((USARTx == USART1) || (USARTx == USART6)
#if defined(STM32F413_423xx)
          || (USARTx == UART9) || (USARTx == UART10)
#endif /* STM32F413_423xx */
         )
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  /* Enable DMA2 clock. */
  }

  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                 /* Data Peripheral to Memory. */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        /* Peripheral address does not increase. */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 /* Memory address increment. */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; /* Peripheral data width of 8 bit. */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         /* Memory data width of 8 bit. */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         /* Use Circular mode. */
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   /* Medium priority. */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  /* FIFO mode disabled. */
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           /* Specifies the FIFO threshold level. */
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             /* A single transmission burst memory. */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     /* Peripherals single transmission burst. */

  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    USART1_DMA_Rx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);     /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART1_DMA_Buffer_Rx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART1_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA2_Stream2);                         /* Deinitialize the DMA2 Stream2 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA2_Stream2) != DISABLE); /* Wait for the specified DMA2 Stream2 reset to complete. */
    
    DMA_Init(DMA2_Stream2, &DMA_InitStructure); /* Initializes the DMA2 Stream2. */
    DMA_Cmd(DMA2_Stream2, ENABLE);              /* Enables the specified DMA2 Stream2. */
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    USART2_DMA_Rx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);     /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART2_DMA_Buffer_Rx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART2_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA1_Stream5);                         /* Deinitialize the DMA1 Stream5 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA1_Stream5) != DISABLE); /* Wait for the specified DMA1 Stream5 reset to complete. */
    
    DMA_Init(DMA1_Stream5, &DMA_InitStructure); /* Initializes the DMA1 Stream5. */
    DMA_Cmd(DMA1_Stream5, ENABLE);              /* Enables the specified DMA1 Stream5. */
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    USART3_DMA_Rx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);     /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART3_DMA_Buffer_Rx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART3_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA1_Stream1);                         /* Deinitialize the DMA1 Stream1 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE); /* Wait for the specified DMA1 Stream1 reset to complete. */
    
    DMA_Init(DMA1_Stream1, &DMA_InitStructure); /* Initializes the DMA1 Stream1. */
    DMA_Cmd(DMA1_Stream1, ENABLE);              /* Enables the specified DMA1 Stream1. */
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    USART4_DMA_Rx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART4->DR);      /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART4_DMA_Buffer_Rx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART4_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA1_Stream2);                         /* Deinitialize the DMA1 Stream2 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA1_Stream2) != DISABLE); /* Wait for the specified DMA1 Stream2 reset to complete. */
    
    DMA_Init(DMA1_Stream2, &DMA_InitStructure); /* Initializes the DMA1 Stream2. */
    DMA_Cmd(DMA1_Stream2, ENABLE);              /* Enables the specified DMA1 Stream2. */
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    USART5_DMA_Rx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART5->DR);      /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART5_DMA_Buffer_Rx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART5_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA1_Stream0);                         /* Deinitialize the DMA1 Stream0 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE); /* Wait for the specified DMA1 Stream0 reset to complete. */
    
    DMA_Init(DMA1_Stream0, &DMA_InitStructure); /* Initializes the DMA1 Stream0. */
    DMA_Cmd(DMA1_Stream0, ENABLE);              /* Enables the specified DMA1 Stream0. */
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    USART6_DMA_Rx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART6->DR);     /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART6_DMA_Buffer_Rx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART6_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA2_Stream1);                         /* Deinitialize the DMA2 Stream1 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE); /* Wait for the specified DMA2 Stream1 reset to complete. */
    
    DMA_Init(DMA2_Stream1, &DMA_InitStructure); /* Initializes the DMA2 Stream1. */
    DMA_Cmd(DMA2_Stream1, ENABLE);              /* Enables the specified DMA2 Stream1. */
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    USART7_DMA_Rx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART7->DR);      /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART7_DMA_Buffer_Rx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART7_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA1_Stream3);                         /* Deinitialize the DMA1 Stream3 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE); /* Wait for the specified DMA1 Stream3 reset to complete. */
    
    DMA_Init(DMA1_Stream3, &DMA_InitStructure); /* Initializes the DMA1 Stream3. */
    DMA_Cmd(DMA1_Stream3, ENABLE);              /* Enables the specified DMA1 Stream3. */
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    USART8_DMA_Rx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART8->DR);      /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART8_DMA_Buffer_Rx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART8_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA1_Stream6);                         /* Deinitialize the DMA1 Stream6 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE); /* Wait for the specified DMA1 Stream6 reset to complete. */
    
    DMA_Init(DMA1_Stream6, &DMA_InitStructure); /* Initializes the DMA1 Stream6. */
    DMA_Cmd(DMA1_Stream6, ENABLE);              /* Enables the specified DMA1 Stream6. */
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    USART9_DMA_Rx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;                          /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART9->DR);      /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART9_DMA_Buffer_Rx; /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART9_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA2_Stream7);                         /* Deinitialize the DMA2 Stream7 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE); /* Wait for the specified DMA2 Stream7 reset to complete. */
    
    DMA_Init(DMA2_Stream7, &DMA_InitStructure); /* Initializes the DMA2 Stream7. */
    DMA_Cmd(DMA2_Stream7, ENABLE);              /* Enables the specified DMA2 Stream7. */
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    USART10_DMA_Rx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                            /* Channel selection. */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART10->DR);       /* Set the DMA source: peripheral data register address. */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART10_DMA_Buffer_Rx;  /* Memory address. */
    DMA_InitStructure.DMA_BufferSize = USART10_DMA_BufferSize_Rx;             /* Specifies the DMA channel DMA buffer size. */
    
    DMA_DeInit(DMA2_Stream0);                         /* Deinitialize the DMA2 Stream0 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE); /* Wait for the specified DMA2 Stream0 reset to complete. */
    
    DMA_Init(DMA2_Stream0, &DMA_InitStructure); /* Initializes the DMA2 Stream0. */
    DMA_Cmd(DMA2_Stream0, ENABLE);              /* Enables the specified DMA2 Stream0. */
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  USART DMA TX unconfigure.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
static void USART_DMA_TX_Unconfigure(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    if(USART1_DMA_Tx_Flag == true)
    {
      USART1_DMA_Tx_Flag = false;
      
      DMA_DeInit(DMA2_Stream7);                         /* Deinitialize the DMA2 Stream7 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE); /* Wait for the specified DMA2 Stream7 reset to complete. */
    }
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    if(USART2_DMA_Tx_Flag == true)
    {
      USART2_DMA_Tx_Flag = false;
      
      DMA_DeInit(DMA1_Stream6);                         /* Deinitialize the DMA1 Stream6 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE); /* Wait for the specified DMA1 Stream6 reset to complete. */
    }
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    if(USART3_DMA_Tx_Flag == true)
    {
      USART3_DMA_Tx_Flag = false;
      
      DMA_DeInit(DMA1_Stream3);                         /* Deinitialize the DMA1 Stream3 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE); /* Wait for the specified DMA1 Stream3 reset to complete. */
    }
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    if(USART4_DMA_Tx_Flag == true)
    {
      USART4_DMA_Tx_Flag = false;
      
      DMA_DeInit(DMA1_Stream4);                         /* Deinitialize the DMA1 Stream4 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE); /* Wait for the specified DMA1 Stream4 reset to complete. */
    }
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    if(USART5_DMA_Tx_Flag == true)
    {
      USART5_DMA_Tx_Flag = false;
      
      DMA_DeInit(DMA1_Stream7);                         /* Deinitialize the DMA1 Stream7 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE); /* Wait for the specified DMA1 Stream7 reset to complete. */
    }
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    if(USART6_DMA_Tx_Flag == true)
    {
      USART6_DMA_Tx_Flag = false;
      
      DMA_DeInit(DMA2_Stream6);                         /* Deinitialize the DMA2 Stream6 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE); /* Wait for the specified DMA2 Stream6 reset to complete. */
    }
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    if(USART7_DMA_Tx_Flag == true)
    {
      USART7_DMA_Tx_Flag = false;
      
      DMA_DeInit(DMA1_Stream1);                         /* Deinitialize the DMA1 Stream1 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE); /* Wait for the specified DMA1 Stream1 reset to complete. */
    }
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    if(USART8_DMA_Tx_Flag == true)
    {
      USART8_DMA_Tx_Flag = false;
      
      DMA_DeInit(DMA1_Stream0);                         /* Deinitialize the DMA1 Stream0 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE); /* Wait for the specified DMA1 Stream0 reset to complete. */
    }
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    if(USART9_DMA_Tx_Flag == true)
    {
      USART9_DMA_Tx_Flag = false;
      
      DMA_DeInit(DMA2_Stream0);                         /* Deinitialize the DMA2 Stream0 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE); /* Wait for the specified DMA2 Stream0 reset to complete. */
    }
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    if(USART10_DMA_Tx_Flag == true)
    {
      USART10_DMA_Tx_Flag = false;
      
      DMA_DeInit(DMA2_Stream7);                         /* Deinitialize the DMA2 Stream7 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE); /* Wait for the specified DMA2 Stream7 reset to complete. */
    }
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  USART DMA RX unconfigure.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
static void USART_DMA_RX_Unconfigure(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    if(USART1_DMA_Rx_Flag == true)
    {
      USART1_DMA_Rx_Flag = false;
      
      DMA_DeInit(DMA2_Stream2);                         /* Deinitialize the DMA2 Stream2 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA2_Stream2) != DISABLE); /* Wait for the specified DMA2 Stream2 reset to complete. */
    }
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    if(USART2_DMA_Rx_Flag == true)
    {
      USART2_DMA_Rx_Flag = false;
      
      DMA_DeInit(DMA1_Stream5);                         /* Deinitialize the DMA1 Stream5 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA1_Stream5) != DISABLE); /* Wait for the specified DMA1 Stream5 reset to complete. */
    }
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    if(USART3_DMA_Rx_Flag == true)
    {
      USART3_DMA_Rx_Flag = false;
      
      DMA_DeInit(DMA1_Stream1);                         /* Deinitialize the DMA1 Stream1 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE); /* Wait for the specified DMA1 Stream1 reset to complete. */
    }
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    if(USART4_DMA_Rx_Flag == true)
    {
      USART4_DMA_Rx_Flag = false;
      
      DMA_DeInit(DMA1_Stream2);                         /* Deinitialize the DMA1 Stream2 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA1_Stream2) != DISABLE); /* Wait for the specified DMA1 Stream2 reset to complete. */
    }
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    if(USART5_DMA_Rx_Flag == true)
    {
      USART5_DMA_Rx_Flag = false;
      
      DMA_DeInit(DMA1_Stream0);                         /* Deinitialize the DMA1 Stream0 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE); /* Wait for the specified DMA1 Stream0 reset to complete. */
    }
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    if(USART6_DMA_Rx_Flag == true)
    {
      USART6_DMA_Rx_Flag = false;
      
      DMA_DeInit(DMA2_Stream1);                         /* Deinitialize the DMA2 Stream1 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE); /* Wait for the specified DMA2 Stream1 reset to complete. */
    }
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    if(USART7_DMA_Rx_Flag == true)
    {
      USART7_DMA_Rx_Flag = false;
      
      DMA_DeInit(DMA1_Stream3);                         /* Deinitialize the DMA1 Stream3 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE); /* Wait for the specified DMA1 Stream3 reset to complete. */
    }
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    if(USART8_DMA_Rx_Flag == true)
    {
      USART8_DMA_Rx_Flag = false;
      
      DMA_DeInit(DMA1_Stream6);                         /* Deinitialize the DMA1 Stream6 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE); /* Wait for the specified DMA1 Stream6 reset to complete. */
    }
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    if(USART9_DMA_Rx_Flag == true)
    {
      USART9_DMA_Rx_Flag = false;
      
      DMA_DeInit(DMA2_Stream7);                         /* Deinitialize the DMA2 Stream7 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE); /* Wait for the specified DMA2 Stream7 reset to complete. */
    }
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    if(USART10_DMA_Rx_Flag == true)
    {
      USART10_DMA_Rx_Flag = false;
      
      DMA_DeInit(DMA2_Stream0);                         /* Deinitialize the DMA2 Stream0 registers to their default reset values. */
      while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE); /* Wait for the specified DMA2 Stream0 reset to complete. */
    }
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  USART NVIC configure.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
static void USART_NVIC_Configure(USART_TypeDef *USARTx)
{
  NVIC_InitTypeDef NVIC_InitStructure = {NULL}; /* NVIC Init Structure definition */
  
  if(USARTx == USART1)
  {
#if (USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx) || (USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx)
    if(USART1_NVIC_Flag == false)
    {
      USART1_NVIC_Flag = true;
      
      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                                     /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART1_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART1_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                       /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx) || (USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx) */
  }
  else if(USARTx == USART2)
  {
#if (USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx) || (USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx)
    if(USART2_NVIC_Flag == false)
    {
      USART2_NVIC_Flag = true;
      
      NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;                                     /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART2_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART2_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                       /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx) || (USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx) */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if (USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx) || (USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx)
    if(USART3_NVIC_Flag == false)
    {
      USART3_NVIC_Flag = true;
      
      NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;                                     /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART3_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART3_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                       /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx) || (USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx) */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if (USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx) || (USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx)
    if(USART4_NVIC_Flag == false)
    {
      USART4_NVIC_Flag = true;
      
      NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;                                      /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART4_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART4_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                       /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx) || (USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx) */
  }
  else if(USARTx == UART5)
  {
#if (USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx) || (USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx)
    if(USART5_NVIC_Flag == false)
    {
      USART5_NVIC_Flag = true;
      
      NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;                                      /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART5_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART5_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                       /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx) || (USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx) */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if (USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx) || (USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx)
    if(USART6_NVIC_Flag == false)
    {
      USART6_NVIC_Flag = true;
      
      NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;                                     /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART6_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART6_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                       /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx) || (USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx) */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if (USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx) || (USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx)
    if(USART7_NVIC_Flag == false)
    {
      USART7_NVIC_Flag = true;
      
      NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;                                      /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART7_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART7_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                       /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx) || (USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx) */
  }
  else if(USARTx == UART8)
  {
#if (USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx) || (USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx)
    if(USART8_NVIC_Flag == false)
    {
      USART8_NVIC_Flag = true;
      
      NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;                                      /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART8_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART8_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                       /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx) || (USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx) */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if (USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx) || (USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx)
    if(USART9_NVIC_Flag == false)
    {
      USART9_NVIC_Flag = true;
      
      NVIC_InitStructure.NVIC_IRQChannel = UART9_IRQn;                                      /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART9_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART9_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                       /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx) || (USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx) */
  }
  else if(USARTx == UART10)
  {
#if (USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx) || (USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx)
    if(USART10_NVIC_Flag == false)
    {
      USART10_NVIC_Flag = true;
      
      NVIC_InitStructure.NVIC_IRQChannel = UART10_IRQn;                                       /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART10_IRQ_PreemptionPriority;  /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART10_IRQ_SubPriority;                /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                         /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                         /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx) || (USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx) */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  USART NVIC unconfigure.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
static void USART_NVIC_Unconfigure(USART_TypeDef *USARTx)
{
  NVIC_InitTypeDef NVIC_InitStructure = {NULL}; /* NVIC Init Structure definition */
  
  if(USARTx == USART1)
  {
#if (USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx) || (USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx)
    if(USART1_NVIC_Flag == true)
    {
      USART1_NVIC_Flag = false;
      
      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                                     /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART1_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART1_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;                                      /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx) || (USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx) */
  }
  else if(USARTx == USART2)
  {
#if (USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx) || (USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx)
    if(USART2_NVIC_Flag == true)
    {
      USART2_NVIC_Flag = false;
      
      NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;                                     /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART2_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART2_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;                                      /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx) || (USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx) */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if (USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx) || (USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx)
    if(USART3_NVIC_Flag == true)
    {
      USART3_NVIC_Flag = false;
      
      NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;                                     /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART3_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART3_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;                                      /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx) || (USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx) */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if (USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx) || (USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx)
    if(USART4_NVIC_Flag == true)
    {
      USART4_NVIC_Flag = false;
      
      NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;                                      /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART4_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART4_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;                                      /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx) || (USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx) */
  }
  else if(USARTx == UART5)
  {
#if (USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx) || (USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx)
    if(USART5_NVIC_Flag == true)
    {
      USART5_NVIC_Flag = false;
      
      NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;                                      /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART5_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART5_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;                                      /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx) || (USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx) */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if (USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx) || (USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx)
    if(USART6_NVIC_Flag == true)
    {
      USART6_NVIC_Flag = false;
      
      NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;                                     /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART6_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART6_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;                                      /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx) || (USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx) */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if (USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx) || (USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx)
    if(USART7_NVIC_Flag == true)
    {
      USART7_NVIC_Flag = false;
      
      NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;                                      /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART7_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART7_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;                                      /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx) || (USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx) */
  }
  else if(USARTx == UART8)
  {
#if (USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx) || (USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx)
    if(USART8_NVIC_Flag == true)
    {
      USART8_NVIC_Flag = false;
      
      NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;                                      /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART8_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART8_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;                                      /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx) || (USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx) */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if (USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx) || (USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx)
    if(USART9_NVIC_Flag == true)
    {
      USART9_NVIC_Flag = false;
      
      NVIC_InitStructure.NVIC_IRQChannel = UART9_IRQn;                                      /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART9_IRQ_PreemptionPriority; /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART9_IRQ_SubPriority;               /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;                                      /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                       /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx) || (USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx) */
  }
  else if(USARTx == UART10)
  {
#if (USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx) || (USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx)
    if(USART10_NVIC_Flag == true)
    {
      USART10_NVIC_Flag = false;
      
      NVIC_InitStructure.NVIC_IRQChannel = UART10_IRQn;                                       /* Specifies the IRQ channel to be enabled or disabled. */
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART10_IRQ_PreemptionPriority;  /* Specifies the pre-emption priority for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART10_IRQ_SubPriority;                /* Specifies the subpriority level for the IRQ channel specified in NVIC_IRQChannel. */
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;                                        /* Specifies whether the IRQ channel defined in NVIC_IRQChannel will be enabled or disabled. */
      NVIC_Init(&NVIC_InitStructure);                                                         /* Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct. */
    }
#endif /* (USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx) || (USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx) */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  Set the data sent by the USART.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @param  [in] Data:   The address of the data to be sent.
  * @param  [in] Length: The length of the data to be sent.
  * @return The number of data sent.
  */
uint32_t USART_SetSendData(USART_TypeDef *USARTx, const void *Data, uint32_t Length)
{
  if((Data == NULL) || (Length == NULL))
  {
    return NULL;
  }
  
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    if((USART1_RCC_Flag == false) || (USART1_GPIO_Flag == false) || (USART1_DMA_Tx_Flag == false) || (USART1_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART1_SendData_P >= USART1_BufferSize_Tx)
    {
      return NULL;
    }
    
    if(Length > (USART1_BufferSize_Tx - USART1_SendData_P))
    {
      memcpy(USART1_Buffer_Tx + USART1_SendData_P, Data, USART1_BufferSize_Tx - USART1_SendData_P);
      
      USART1_SendData_P = USART1_BufferSize_Tx;
      Length            = USART1_BufferSize_Tx - USART1_SendData_P;
    }
    else
    {
      memcpy(USART1_Buffer_Tx + USART1_SendData_P, Data, Length);
      
      USART1_SendData_P += Length;
    }
    
    if(USART1_SendData_Flag == false)
    {
      USART1_SendData_Flag = true;
      
      if(USART1_SendData_P > USART1_DMA_BufferSize_Tx)
      {
        memcpy(USART1_DMA_Buffer_Tx, USART1_Buffer_Tx, USART1_DMA_BufferSize_Tx);
        memmove(USART1_Buffer_Tx, USART1_Buffer_Tx + USART1_DMA_BufferSize_Tx, USART1_SendData_P - USART1_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);                    /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                                 /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);               /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART1_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                                  /* Enables the specified DMA2 Stream7. */
        
        USART1_SendDataCount += USART1_DMA_BufferSize_Tx;
        USART1_SendData_P    -= USART1_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART1_DMA_Buffer_Tx, USART1_Buffer_Tx, USART1_SendData_P);
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);              /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                           /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);         /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART1_SendData_P);  /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                            /* Enables the specified DMA2 Stream7. */
        
        USART1_SendDataCount += USART1_SendData_P;
        USART1_SendData_P     = NULL;
      }
    }
    
    return Length;
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    if((USART2_RCC_Flag == false) || (USART2_GPIO_Flag == false) || (USART2_DMA_Tx_Flag == false) || (USART2_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART2_SendData_P >= USART2_BufferSize_Tx)
    {
      return NULL;
    }
    
    if(Length > (USART2_BufferSize_Tx - USART2_SendData_P))
    {
      memcpy(USART2_Buffer_Tx + USART2_SendData_P, Data, USART2_BufferSize_Tx - USART2_SendData_P);
      
      USART2_SendData_P = USART2_BufferSize_Tx;
      Length            = USART2_BufferSize_Tx - USART2_SendData_P;
    }
    else
    {
      memcpy(USART2_Buffer_Tx + USART2_SendData_P, Data, Length);
      
      USART2_SendData_P += Length;
    }
    
    if(USART2_SendData_Flag == false)
    {
      USART2_SendData_Flag = true;
      
      if(USART2_SendData_P > USART2_DMA_BufferSize_Tx)
      {
        memcpy(USART2_DMA_Buffer_Tx, USART2_Buffer_Tx, USART2_DMA_BufferSize_Tx);
        memmove(USART2_Buffer_Tx, USART2_Buffer_Tx + USART2_DMA_BufferSize_Tx, USART2_SendData_P - USART2_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);                    /* Clears the DMA1 Stream6's pending flags. */
        DMA_Cmd(DMA1_Stream6, DISABLE);                                 /* Disables the specified DMA1 Stream6. */
        while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);               /* Wait for the specified DMA1 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream6, USART2_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream6. */
        DMA_Cmd(DMA1_Stream6, ENABLE);                                  /* Enables the specified DMA1 Stream6. */
        
        USART2_SendDataCount += USART2_DMA_BufferSize_Tx;
        USART2_SendData_P    -= USART2_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART2_DMA_Buffer_Tx, USART2_Buffer_Tx, USART2_SendData_P);
        
        DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);              /* Clears the DMA1 Stream6's pending flags. */
        DMA_Cmd(DMA1_Stream6, DISABLE);                           /* Disables the specified DMA1 Stream6. */
        while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);         /* Wait for the specified DMA1 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream6, USART2_SendData_P);  /* Writes the number of data units to be transferred on the DMA1 Stream6. */
        DMA_Cmd(DMA1_Stream6, ENABLE);                            /* Enables the specified DMA1 Stream6. */
        
        USART2_SendDataCount += USART2_SendData_P;
        USART2_SendData_P     = NULL;
      }
    }
    
    return Length;
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    if((USART3_RCC_Flag == false) || (USART3_GPIO_Flag == false) || (USART3_DMA_Tx_Flag == false) || (USART3_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART3_SendData_P >= USART3_BufferSize_Tx)
    {
      return NULL;
    }
    
    if(Length > (USART3_BufferSize_Tx - USART3_SendData_P))
    {
      memcpy(USART3_Buffer_Tx + USART3_SendData_P, Data, USART3_BufferSize_Tx - USART3_SendData_P);
      
      USART3_SendData_P = USART3_BufferSize_Tx;
      Length            = USART3_BufferSize_Tx - USART3_SendData_P;
    }
    else
    {
      memcpy(USART3_Buffer_Tx + USART3_SendData_P, Data, Length);
      
      USART3_SendData_P += Length;
    }
    
    if(USART3_SendData_Flag == false)
    {
      USART3_SendData_Flag = true;
      
      if(USART3_SendData_P > USART3_DMA_BufferSize_Tx)
      {
        memcpy(USART3_DMA_Buffer_Tx, USART3_Buffer_Tx, USART3_DMA_BufferSize_Tx);
        memmove(USART3_Buffer_Tx, USART3_Buffer_Tx + USART3_DMA_BufferSize_Tx, USART3_SendData_P - USART3_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);                    /* Clears the DMA1 Stream3's pending flags. */
        DMA_Cmd(DMA1_Stream3, DISABLE);                                 /* Disables the specified DMA1 Stream3. */
        while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);               /* Wait for the specified DMA1 Stream3 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream3, USART3_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream3. */
        DMA_Cmd(DMA1_Stream3, ENABLE);                                  /* Enables the specified DMA1 Stream3. */
        
        USART3_SendDataCount += USART3_DMA_BufferSize_Tx;
        USART3_SendData_P    -= USART3_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART3_DMA_Buffer_Tx, USART3_Buffer_Tx, USART3_SendData_P);
        
        DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);              /* Clears the DMA1 Stream3's pending flags. */
        DMA_Cmd(DMA1_Stream3, DISABLE);                           /* Disables the specified DMA1 Stream3. */
        while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);         /* Wait for the specified DMA1 Stream3 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream3, USART3_SendData_P);  /* Writes the number of data units to be transferred on the DMA1 Stream3. */
        DMA_Cmd(DMA1_Stream3, ENABLE);                            /* Enables the specified DMA1 Stream3. */
        
        USART3_SendDataCount += USART3_SendData_P;
        USART3_SendData_P     = NULL;
      }
    }
    
    return Length;
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    if((USART4_RCC_Flag == false) || (USART4_GPIO_Flag == false) || (USART4_DMA_Tx_Flag == false) || (USART4_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART4_SendData_P >= USART4_BufferSize_Tx)
    {
      return NULL;
    }
    
    if(Length > (USART4_BufferSize_Tx - USART4_SendData_P))
    {
      memcpy(USART4_Buffer_Tx + USART4_SendData_P, Data, USART4_BufferSize_Tx - USART4_SendData_P);
      
      USART4_SendData_P = USART4_BufferSize_Tx;
      Length            = USART4_BufferSize_Tx - USART4_SendData_P;
    }
    else
    {
      memcpy(USART4_Buffer_Tx + USART4_SendData_P, Data, Length);
      
      USART4_SendData_P += Length;
    }
    
    if(USART4_SendData_Flag == false)
    {
      USART4_SendData_Flag = true;
      
      if(USART4_SendData_P > USART4_DMA_BufferSize_Tx)
      {
        memcpy(USART4_DMA_Buffer_Tx, USART4_Buffer_Tx, USART4_DMA_BufferSize_Tx);
        memmove(USART4_Buffer_Tx, USART4_Buffer_Tx + USART4_DMA_BufferSize_Tx, USART4_SendData_P - USART4_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);                    /* Clears the DMA1 Stream4's pending flags. */
        DMA_Cmd(DMA1_Stream4, DISABLE);                                 /* Disables the specified DMA1 Stream4. */
        while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);               /* Wait for the specified DMA1 Stream4 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream4, USART4_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream4. */
        DMA_Cmd(DMA1_Stream4, ENABLE);                                  /* Enables the specified DMA1 Stream4. */
        
        USART4_SendDataCount += USART4_DMA_BufferSize_Tx;
        USART4_SendData_P    -= USART4_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART4_DMA_Buffer_Tx, USART4_Buffer_Tx, USART4_SendData_P);
        
        DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);              /* Clears the DMA1 Stream4's pending flags. */
        DMA_Cmd(DMA1_Stream4, DISABLE);                           /* Disables the specified DMA1 Stream4. */
        while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);         /* Wait for the specified DMA1 Stream4 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream4, USART4_SendData_P);  /* Writes the number of data units to be transferred on the DMA1 Stream4. */
        DMA_Cmd(DMA1_Stream4, ENABLE);                            /* Enables the specified DMA1 Stream4. */
        
        USART4_SendDataCount += USART4_SendData_P;
        USART4_SendData_P     = NULL;
      }
    }
    
    return Length;
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    if((USART5_RCC_Flag == false) || (USART5_GPIO_Flag == false) || (USART5_DMA_Tx_Flag == false) || (USART5_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART5_SendData_P >= USART5_BufferSize_Tx)
    {
      return NULL;
    }
    
    if(Length > (USART5_BufferSize_Tx - USART5_SendData_P))
    {
      memcpy(USART5_Buffer_Tx + USART5_SendData_P, Data, USART5_BufferSize_Tx - USART5_SendData_P);
      
      USART5_SendData_P = USART5_BufferSize_Tx;
      Length            = USART5_BufferSize_Tx - USART5_SendData_P;
    }
    else
    {
      memcpy(USART5_Buffer_Tx + USART5_SendData_P, Data, Length);
      
      USART5_SendData_P += Length;
    }
    
    if(USART5_SendData_Flag == false)
    {
      USART5_SendData_Flag = true;
      
      if(USART5_SendData_P > USART5_DMA_BufferSize_Tx)
      {
        memcpy(USART5_DMA_Buffer_Tx, USART5_Buffer_Tx, USART5_DMA_BufferSize_Tx);
        memmove(USART5_Buffer_Tx, USART5_Buffer_Tx + USART5_DMA_BufferSize_Tx, USART5_SendData_P - USART5_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);                    /* Clears the DMA1 Stream7's pending flags. */
        DMA_Cmd(DMA1_Stream7, DISABLE);                                 /* Disables the specified DMA1 Stream7. */
        while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);               /* Wait for the specified DMA1 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream7, USART5_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream7. */
        DMA_Cmd(DMA1_Stream7, ENABLE);                                  /* Enables the specified DMA1 Stream7. */
        
        USART5_SendDataCount += USART5_DMA_BufferSize_Tx;
        USART5_SendData_P    -= USART5_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART5_DMA_Buffer_Tx, USART5_Buffer_Tx, USART5_SendData_P);
        
        DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);              /* Clears the DMA1 Stream7's pending flags. */
        DMA_Cmd(DMA1_Stream7, DISABLE);                           /* Disables the specified DMA1 Stream7. */
        while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);         /* Wait for the specified DMA1 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream7, USART5_SendData_P);  /* Writes the number of data units to be transferred on the DMA1 Stream7. */
        DMA_Cmd(DMA1_Stream7, ENABLE);                            /* Enables the specified DMA1 Stream7. */
        
        USART5_SendDataCount += USART5_SendData_P;
        USART5_SendData_P     = NULL;
      }
    }
    
    return Length;
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    if((USART6_RCC_Flag == false) || (USART6_GPIO_Flag == false) || (USART6_DMA_Tx_Flag == false) || (USART6_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART6_SendData_P >= USART6_BufferSize_Tx)
    {
      return NULL;
    }
    
    if(Length > (USART6_BufferSize_Tx - USART6_SendData_P))
    {
      memcpy(USART6_Buffer_Tx + USART6_SendData_P, Data, USART6_BufferSize_Tx - USART6_SendData_P);
      
      USART6_SendData_P = USART6_BufferSize_Tx;
      Length            = USART6_BufferSize_Tx - USART6_SendData_P;
    }
    else
    {
      memcpy(USART6_Buffer_Tx + USART6_SendData_P, Data, Length);
      
      USART6_SendData_P += Length;
    }
    
    if(USART6_SendData_Flag == false)
    {
      USART6_SendData_Flag = true;
      
      if(USART6_SendData_P > USART6_DMA_BufferSize_Tx)
      {
        memcpy(USART6_DMA_Buffer_Tx, USART6_Buffer_Tx, USART6_DMA_BufferSize_Tx);
        memmove(USART6_Buffer_Tx, USART6_Buffer_Tx + USART6_DMA_BufferSize_Tx, USART6_SendData_P - USART6_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);                    /* Clears the DMA2 Stream6's pending flags. */
        DMA_Cmd(DMA2_Stream6, DISABLE);                                 /* Disables the specified DMA2 Stream6. */
        while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);               /* Wait for the specified DMA2 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream6, USART6_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA2 Stream6. */
        DMA_Cmd(DMA2_Stream6, ENABLE);                                  /* Enables the specified DMA2 Stream6. */
        
        USART6_SendDataCount += USART6_DMA_BufferSize_Tx;
        USART6_SendData_P    -= USART6_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART6_DMA_Buffer_Tx, USART6_Buffer_Tx, USART6_SendData_P);
        
        DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);              /* Clears the DMA2 Stream6's pending flags. */
        DMA_Cmd(DMA2_Stream6, DISABLE);                           /* Disables the specified DMA2 Stream6. */
        while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);         /* Wait for the specified DMA2 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream6, USART6_SendData_P);  /* Writes the number of data units to be transferred on the DMA2 Stream6. */
        DMA_Cmd(DMA2_Stream6, ENABLE);                            /* Enables the specified DMA2 Stream6. */
        
        USART6_SendDataCount += USART6_SendData_P;
        USART6_SendData_P     = NULL;
      }
    }
    
    return Length;
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    if((USART7_RCC_Flag == false) || (USART7_GPIO_Flag == false) || (USART7_DMA_Tx_Flag == false) || (USART7_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART7_SendData_P >= USART7_BufferSize_Tx)
    {
      return NULL;
    }
    
    if(Length > (USART7_BufferSize_Tx - USART7_SendData_P))
    {
      memcpy(USART7_Buffer_Tx + USART7_SendData_P, Data, USART7_BufferSize_Tx - USART7_SendData_P);
      
      USART7_SendData_P = USART7_BufferSize_Tx;
      Length            = USART7_BufferSize_Tx - USART7_SendData_P;
    }
    else
    {
      memcpy(USART7_Buffer_Tx + USART7_SendData_P, Data, Length);
      
      USART7_SendData_P += Length;
    }
    
    if(USART7_SendData_Flag == false)
    {
      USART7_SendData_Flag = true;
      
      if(USART7_SendData_P > USART7_DMA_BufferSize_Tx)
      {
        memcpy(USART7_DMA_Buffer_Tx, USART7_Buffer_Tx, USART7_DMA_BufferSize_Tx);
        memmove(USART7_Buffer_Tx, USART7_Buffer_Tx + USART7_DMA_BufferSize_Tx, USART7_SendData_P - USART7_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);                    /* Clears the DMA1 Stream1's pending flags. */
        DMA_Cmd(DMA1_Stream1, DISABLE);                                 /* Disables the specified DMA1 Stream1. */
        while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);               /* Wait for the specified DMA1 Stream1 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream1, USART7_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream1. */
        DMA_Cmd(DMA1_Stream1, ENABLE);                                  /* Enables the specified DMA1 Stream1. */
        
        USART7_SendDataCount += USART7_DMA_BufferSize_Tx;
        USART7_SendData_P    -= USART7_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART7_DMA_Buffer_Tx, USART7_Buffer_Tx, USART7_SendData_P);
        
        DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);              /* Clears the DMA1 Stream1's pending flags. */
        DMA_Cmd(DMA1_Stream1, DISABLE);                           /* Disables the specified DMA1 Stream1. */
        while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);         /* Wait for the specified DMA1 Stream1 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream1, USART7_SendData_P);  /* Writes the number of data units to be transferred on the DMA1 Stream1. */
        DMA_Cmd(DMA1_Stream1, ENABLE);                            /* Enables the specified DMA1 Stream1. */
        
        USART7_SendDataCount += USART7_SendData_P;
        USART7_SendData_P     = NULL;
      }
    }
    
    return Length;
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    if((USART8_RCC_Flag == false) || (USART8_GPIO_Flag == false) || (USART8_DMA_Tx_Flag == false) || (USART8_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART8_SendData_P >= USART8_BufferSize_Tx)
    {
      return NULL;
    }
    
    if(Length > (USART8_BufferSize_Tx - USART8_SendData_P))
    {
      memcpy(USART8_Buffer_Tx + USART8_SendData_P, Data, USART8_BufferSize_Tx - USART8_SendData_P);
      
      USART8_SendData_P = USART8_BufferSize_Tx;
      Length            = USART8_BufferSize_Tx - USART8_SendData_P;
    }
    else
    {
      memcpy(USART8_Buffer_Tx + USART8_SendData_P, Data, Length);
      
      USART8_SendData_P += Length;
    }
    
    if(USART8_SendData_Flag == false)
    {
      USART8_SendData_Flag = true;
      
      if(USART8_SendData_P > USART8_DMA_BufferSize_Tx)
      {
        memcpy(USART8_DMA_Buffer_Tx, USART8_Buffer_Tx, USART8_DMA_BufferSize_Tx);
        memmove(USART8_Buffer_Tx, USART8_Buffer_Tx + USART8_DMA_BufferSize_Tx, USART8_SendData_P - USART8_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);                    /* Clears the DMA1 Stream0's pending flags. */
        DMA_Cmd(DMA1_Stream0, DISABLE);                                 /* Disables the specified DMA1 Stream0. */
        while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);               /* Wait for the specified DMA1 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream0, USART8_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream0. */
        DMA_Cmd(DMA1_Stream0, ENABLE);                                  /* Enables the specified DMA1 Stream0. */
        
        USART8_SendDataCount += USART8_DMA_BufferSize_Tx;
        USART8_SendData_P    -= USART8_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART8_DMA_Buffer_Tx, USART8_Buffer_Tx, USART8_SendData_P);
        
        DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);              /* Clears the DMA1 Stream0's pending flags. */
        DMA_Cmd(DMA1_Stream0, DISABLE);                           /* Disables the specified DMA1 Stream0. */
        while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);         /* Wait for the specified DMA1 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream0, USART8_SendData_P);  /* Writes the number of data units to be transferred on the DMA1 Stream0. */
        DMA_Cmd(DMA1_Stream0, ENABLE);                            /* Enables the specified DMA1 Stream0. */
        
        USART8_SendDataCount += USART8_SendData_P;
        USART8_SendData_P     = NULL;
      }
    }
    
    return Length;
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    if((USART9_RCC_Flag == false) || (USART9_GPIO_Flag == false) || (USART9_DMA_Tx_Flag == false) || (USART9_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART9_SendData_P >= USART9_BufferSize_Tx)
    {
      return NULL;
    }
    
    if(Length > (USART9_BufferSize_Tx - USART9_SendData_P))
    {
      memcpy(USART9_Buffer_Tx + USART9_SendData_P, Data, USART9_BufferSize_Tx - USART9_SendData_P);
      
      USART9_SendData_P = USART9_BufferSize_Tx;
      Length            = USART9_BufferSize_Tx - USART9_SendData_P;
    }
    else
    {
      memcpy(USART9_Buffer_Tx + USART9_SendData_P, Data, Length);
      
      USART9_SendData_P += Length;
    }
    
    if(USART9_SendData_Flag == false)
    {
      USART9_SendData_Flag = true;
      
      if(USART9_SendData_P > USART9_DMA_BufferSize_Tx)
      {
        memcpy(USART9_DMA_Buffer_Tx, USART9_Buffer_Tx, USART9_DMA_BufferSize_Tx);
        memmove(USART9_Buffer_Tx, USART9_Buffer_Tx + USART9_DMA_BufferSize_Tx, USART9_SendData_P - USART9_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);                    /* Clears the DMA2 Stream0's pending flags. */
        DMA_Cmd(DMA2_Stream0, DISABLE);                                 /* Disables the specified DMA2 Stream0. */
        while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);               /* Wait for the specified DMA2 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream0, USART9_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA2 Stream0. */
        DMA_Cmd(DMA2_Stream0, ENABLE);                                  /* Enables the specified DMA2 Stream0. */
        
        USART9_SendDataCount += USART9_DMA_BufferSize_Tx;
        USART9_SendData_P    -= USART9_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART9_DMA_Buffer_Tx, USART9_Buffer_Tx, USART9_SendData_P);
        
        DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);              /* Clears the DMA2 Stream0's pending flags. */
        DMA_Cmd(DMA2_Stream0, DISABLE);                           /* Disables the specified DMA2 Stream0. */
        while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);         /* Wait for the specified DMA2 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream0, USART9_SendData_P);  /* Writes the number of data units to be transferred on the DMA2 Stream0. */
        DMA_Cmd(DMA2_Stream0, ENABLE);                            /* Enables the specified DMA2 Stream0. */
        
        USART9_SendDataCount += USART9_SendData_P;
        USART9_SendData_P     = NULL;
      }
    }
    
    return Length;
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    if((USART10_RCC_Flag == false) || (USART10_GPIO_Flag == false) || (USART10_DMA_Tx_Flag == false) || (USART10_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART10_SendData_P >= USART10_BufferSize_Tx)
    {
      return NULL;
    }
    
    if(Length > (USART10_BufferSize_Tx - USART10_SendData_P))
    {
      memcpy(USART10_Buffer_Tx + USART10_SendData_P, Data, USART10_BufferSize_Tx - USART10_SendData_P);
      
      USART10_SendData_P = USART10_BufferSize_Tx;
      Length             = USART10_BufferSize_Tx - USART10_SendData_P;
    }
    else
    {
      memcpy(USART10_Buffer_Tx + USART10_SendData_P, Data, Length);
      
      USART10_SendData_P += Length;
    }
    
    if(USART10_SendData_Flag == false)
    {
      USART10_SendData_Flag = true;
      
      if(USART10_SendData_P > USART10_DMA_BufferSize_Tx)
      {
        memcpy(USART10_DMA_Buffer_Tx, USART10_Buffer_Tx, USART10_DMA_BufferSize_Tx);
        memmove(USART10_Buffer_Tx, USART10_Buffer_Tx + USART10_DMA_BufferSize_Tx, USART10_SendData_P - USART10_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);                      /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                                   /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);                 /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART10_DMA_BufferSize_Tx);  /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                                    /* Enables the specified DMA2 Stream7. */
        
        USART10_SendDataCount += USART10_DMA_BufferSize_Tx;
        USART10_SendData_P    -= USART10_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART10_DMA_Buffer_Tx, USART10_Buffer_Tx, USART10_SendData_P);
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);              /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                           /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);         /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART10_SendData_P); /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                            /* Enables the specified DMA2 Stream7. */
        
        USART10_SendDataCount += USART10_SendData_P;
        USART10_SendData_P     = NULL;
      }
    }
    
    return Length;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
  
  return NULL;
}

/**
  * @brief  Get the data that the USART receives.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @param  [in] Data:   To store the address of the receive data.
  * @param  [in] Length: To read the length of the received data.
  * @return The number of data obtained.
  */
uint32_t USART_GetReceiveData(USART_TypeDef *USARTx, void *Data, uint32_t Length)
{
  uint32_t number = NULL;
  
  if((Data == NULL) || (Length == NULL))
  {
    return NULL;
  }
  
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    if((USART1_RCC_Flag == false) || (USART1_GPIO_Flag == false) || (USART1_DMA_Rx_Flag == false) || (USART1_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART1_ReceiveData_P == NULL)
    {
      return NULL;
    }
    
    if(USART1_ReceiveData_P > Length)
    {
      memcpy(Data, USART1_Buffer_Rx, Length);
      memmove(USART1_Buffer_Rx, USART1_Buffer_Rx + Length, USART1_ReceiveData_P - Length);
      
      number                = Length;
      USART1_ReceiveData_P -= Length;
    }
    else
    {
      memcpy(Data, USART1_Buffer_Rx, USART1_ReceiveData_P);
      
      number               = USART1_ReceiveData_P;
      USART1_ReceiveData_P = NULL;
    }
    
    return number;
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    if((USART2_RCC_Flag == false) || (USART2_GPIO_Flag == false) || (USART2_DMA_Rx_Flag == false) || (USART2_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART2_ReceiveData_P == NULL)
    {
      return NULL;
    }
    
    if(USART2_ReceiveData_P > Length)
    {
      memcpy(Data, USART2_Buffer_Rx, Length);
      memmove(USART2_Buffer_Rx, USART2_Buffer_Rx + Length, USART2_ReceiveData_P - Length);
      
      number                = Length;
      USART2_ReceiveData_P -= Length;
    }
    else
    {
      memcpy(Data, USART2_Buffer_Rx, USART2_ReceiveData_P);
      
      number               = USART2_ReceiveData_P;
      USART2_ReceiveData_P = NULL;
    }
    
    return number;
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    if((USART3_RCC_Flag == false) || (USART3_GPIO_Flag == false) || (USART3_DMA_Rx_Flag == false) || (USART3_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART3_ReceiveData_P == NULL)
    {
      return NULL;
    }
    
    if(USART3_ReceiveData_P > Length)
    {
      memcpy(Data, USART3_Buffer_Rx, Length);
      memmove(USART3_Buffer_Rx, USART3_Buffer_Rx + Length, USART3_ReceiveData_P - Length);
      
      number                = Length;
      USART3_ReceiveData_P -= Length;
    }
    else
    {
      memcpy(Data, USART3_Buffer_Rx, USART3_ReceiveData_P);
      
      number               = USART3_ReceiveData_P;
      USART3_ReceiveData_P = NULL;
    }
    
    return number;
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    if((USART4_RCC_Flag == false) || (USART4_GPIO_Flag == false) || (USART4_DMA_Rx_Flag == false) || (USART4_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART4_ReceiveData_P == NULL)
    {
      return NULL;
    }
    
    if(USART4_ReceiveData_P > Length)
    {
      memcpy(Data, USART4_Buffer_Rx, Length);
      memmove(USART4_Buffer_Rx, USART4_Buffer_Rx + Length, USART4_ReceiveData_P - Length);
      
      number                = Length;
      USART4_ReceiveData_P -= Length;
    }
    else
    {
      memcpy(Data, USART4_Buffer_Rx, USART4_ReceiveData_P);
      
      number               = USART4_ReceiveData_P;
      USART4_ReceiveData_P = NULL;
    }
    
    return number;
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    if((USART5_RCC_Flag == false) || (USART5_GPIO_Flag == false) || (USART5_DMA_Rx_Flag == false) || (USART5_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART5_ReceiveData_P == NULL)
    {
      return NULL;
    }
    
    if(USART5_ReceiveData_P > Length)
    {
      memcpy(Data, USART5_Buffer_Rx, Length);
      memmove(USART5_Buffer_Rx, USART5_Buffer_Rx + Length, USART5_ReceiveData_P - Length);
      
      number                = Length;
      USART5_ReceiveData_P -= Length;
    }
    else
    {
      memcpy(Data, USART5_Buffer_Rx, USART5_ReceiveData_P);
      
      number               = USART5_ReceiveData_P;
      USART5_ReceiveData_P = NULL;
    }
    
    return number;
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    if((USART6_RCC_Flag == false) || (USART6_GPIO_Flag == false) || (USART6_DMA_Rx_Flag == false) || (USART6_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART6_ReceiveData_P == NULL)
    {
      return NULL;
    }
    
    if(USART6_ReceiveData_P > Length)
    {
      memcpy(Data, USART6_Buffer_Rx, Length);
      memmove(USART6_Buffer_Rx, USART6_Buffer_Rx + Length, USART6_ReceiveData_P - Length);
      
      number                = Length;
      USART6_ReceiveData_P -= Length;
    }
    else
    {
      memcpy(Data, USART6_Buffer_Rx, USART6_ReceiveData_P);
      
      number               = USART6_ReceiveData_P;
      USART6_ReceiveData_P = NULL;
    }
    
    return number;
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    if((USART7_RCC_Flag == false) || (USART7_GPIO_Flag == false) || (USART7_DMA_Rx_Flag == false) || (USART7_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART7_ReceiveData_P == NULL)
    {
      return NULL;
    }
    
    if(USART7_ReceiveData_P > Length)
    {
      memcpy(Data, USART7_Buffer_Rx, Length);
      memmove(USART7_Buffer_Rx, USART7_Buffer_Rx + Length, USART7_ReceiveData_P - Length);
      
      number                = Length;
      USART7_ReceiveData_P -= Length;
    }
    else
    {
      memcpy(Data, USART7_Buffer_Rx, USART7_ReceiveData_P);
      
      number               = USART7_ReceiveData_P;
      USART7_ReceiveData_P = NULL;
    }
    
    return number;
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    if((USART8_RCC_Flag == false) || (USART8_GPIO_Flag == false) || (USART8_DMA_Rx_Flag == false) || (USART8_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART8_ReceiveData_P == NULL)
    {
      return NULL;
    }
    
    if(USART8_ReceiveData_P > Length)
    {
      memcpy(Data, USART8_Buffer_Rx, Length);
      memmove(USART8_Buffer_Rx, USART8_Buffer_Rx + Length, USART8_ReceiveData_P - Length);
      
      number                = Length;
      USART8_ReceiveData_P -= Length;
    }
    else
    {
      memcpy(Data, USART8_Buffer_Rx, USART8_ReceiveData_P);
      
      number               = USART8_ReceiveData_P;
      USART8_ReceiveData_P = NULL;
    }
    
    return number;
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    if((USART9_RCC_Flag == false) || (USART9_GPIO_Flag == false) || (USART9_DMA_Rx_Flag == false) || (USART9_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART9_ReceiveData_P == NULL)
    {
      return NULL;
    }
    
    if(USART9_ReceiveData_P > Length)
    {
      memcpy(Data, USART9_Buffer_Rx, Length);
      memmove(USART9_Buffer_Rx, USART9_Buffer_Rx + Length, USART9_ReceiveData_P - Length);
      
      number                = Length;
      USART9_ReceiveData_P -= Length;
    }
    else
    {
      memcpy(Data, USART9_Buffer_Rx, USART9_ReceiveData_P);
      
      number               = USART9_ReceiveData_P;
      USART9_ReceiveData_P = NULL;
    }
    
    return number;
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    if((USART10_RCC_Flag == false) || (USART10_GPIO_Flag == false) || (USART10_DMA_Rx_Flag == false) || (USART10_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(USART10_ReceiveData_P == NULL)
    {
      return NULL;
    }
    
    if(USART10_ReceiveData_P > Length)
    {
      memcpy(Data, USART10_Buffer_Rx, Length);
      memmove(USART10_Buffer_Rx, USART10_Buffer_Rx + Length, USART10_ReceiveData_P - Length);
      
      number                 = Length;
      USART10_ReceiveData_P -= Length;
    }
    else
    {
      memcpy(Data, USART10_Buffer_Rx, USART10_ReceiveData_P);
      
      number                = USART10_ReceiveData_P;
      USART10_ReceiveData_P = NULL;
    }
    
    return number;
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
  
  return NULL;
}

/**
  * @brief  USART1 IRQ handler.
  * @param  None.
  * @return None.
  */
void USART1_IRQHandler(void)
{
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
  if(USART_GetITStatus(USART1, USART_IT_TC) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ClearITPendingBit(USART1, USART_IT_TC);     /* Clears the USARTx's interrupt pending bits. */
    
    if(USART1_SendData_P == NULL)
    {
      USART1_SendData_Flag = false;
      
      USART_SendDataFinishCallback(USART1);
    }
    else
    {
      if(USART1_SendData_P > USART1_DMA_BufferSize_Tx)
      {
        memcpy(USART1_DMA_Buffer_Tx, USART1_Buffer_Tx, USART1_DMA_BufferSize_Tx);
        memmove(USART1_Buffer_Tx, USART1_Buffer_Tx + USART1_DMA_BufferSize_Tx, USART1_SendData_P - USART1_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);                    /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                                 /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);               /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART1_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                                  /* Enables the specified DMA2 Stream7. */
        
        USART1_SendDataCount += USART1_DMA_BufferSize_Tx;
        USART1_SendData_P    -= USART1_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART1_DMA_Buffer_Tx, USART1_Buffer_Tx, USART1_SendData_P);
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);              /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                           /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);         /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART1_SendData_P);  /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                            /* Enables the specified DMA2 Stream7. */
        
        USART1_SendDataCount += USART1_SendData_P;
        USART1_SendData_P     = NULL;
      }
    }
  }
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
  if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(USART1);                          /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART1_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream2);  /* Returns the number of remaining data units in the current DMA2 Stream2 transfer. */
    
    USART1_ReceiveDataCount += number;
    
    if(number > (USART1_BufferSize_Rx - USART1_ReceiveData_P))
    {
      if(number < USART1_BufferSize_Rx)
      {
        memmove(USART1_Buffer_Rx, USART1_Buffer_Rx + (number - (USART1_BufferSize_Rx - USART1_ReceiveData_P)), USART1_BufferSize_Rx - number);
        memcpy(USART1_Buffer_Rx + (USART1_BufferSize_Rx - number), USART1_DMA_Buffer_Rx, number);
      }
      else
      {
        memcpy(USART1_Buffer_Rx, USART1_DMA_Buffer_Rx + (number - USART1_BufferSize_Rx), USART1_BufferSize_Rx);
      }
      
      USART1_ReceiveData_P = USART1_BufferSize_Rx;
    }
    else
    {
      memcpy(USART1_Buffer_Rx + USART1_ReceiveData_P, USART1_DMA_Buffer_Rx, number);
      
      USART1_ReceiveData_P += number;
    }
    
    DMA_Cmd(DMA2_Stream2, DISABLE);                                 /* Disables the specified DMA2 Stream2. */
    while(DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);               /* Wait for the specified DMA2 Stream2 disable to finish. */
    DMA_SetCurrDataCounter(DMA2_Stream2, USART1_DMA_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA2 Stream2. */
    DMA_Cmd(DMA2_Stream2, ENABLE);                                  /* Enables the specified DMA2 Stream2. */
    
    USART_ReceiveDataFinishCallback(USART1);
  }
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
}

/**
  * @brief  USART2 IRQ handler.
  * @param  None.
  * @return None.
  */
void USART2_IRQHandler(void)
{
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
  if(USART_GetITStatus(USART2, USART_IT_TC) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ClearITPendingBit(USART2, USART_IT_TC);     /* Clears the USARTx's interrupt pending bits. */
    
    if(USART2_SendData_P == NULL)
    {
      USART2_SendData_Flag = false;
      
      USART_SendDataFinishCallback(USART2);
    }
    else
    {
      if(USART2_SendData_P > USART2_DMA_BufferSize_Tx)
      {
        memcpy(USART2_DMA_Buffer_Tx, USART2_Buffer_Tx, USART2_DMA_BufferSize_Tx);
        memmove(USART2_Buffer_Tx, USART2_Buffer_Tx + USART2_DMA_BufferSize_Tx, USART2_SendData_P - USART2_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);                    /* Clears the DMA1 Stream6's pending flags. */
        DMA_Cmd(DMA1_Stream6, DISABLE);                                 /* Disables the specified DMA1 Stream6. */
        while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);               /* Wait for the specified DMA1 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream6, USART2_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream6. */
        DMA_Cmd(DMA1_Stream6, ENABLE);                                  /* Enables the specified DMA1 Stream6. */
        
        USART2_SendDataCount += USART2_DMA_BufferSize_Tx;
        USART2_SendData_P    -= USART2_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART2_DMA_Buffer_Tx, USART2_Buffer_Tx, USART2_SendData_P);
        
        DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);              /* Clears the DMA1 Stream6's pending flags. */
        DMA_Cmd(DMA1_Stream6, DISABLE);                           /* Disables the specified DMA1 Stream6. */
        while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);         /* Wait for the specified DMA1 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream6, USART2_SendData_P);  /* Writes the number of data units to be transferred on the DMA1 Stream6. */
        DMA_Cmd(DMA1_Stream6, ENABLE);                            /* Enables the specified DMA1 Stream6. */
        
        USART2_SendDataCount += USART2_SendData_P;
        USART2_SendData_P     = NULL;
      }
    }
  }
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
  if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(USART2);                          /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART2_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream5);  /* Returns the number of remaining data units in the current DMA1 Stream5 transfer. */
    
    USART2_ReceiveDataCount += number;
    
    if(number > (USART2_BufferSize_Rx - USART2_ReceiveData_P))
    {
      if(number < USART2_BufferSize_Rx)
      {
        memmove(USART2_Buffer_Rx, USART2_Buffer_Rx + (number - (USART2_BufferSize_Rx - USART2_ReceiveData_P)), USART2_BufferSize_Rx - number);
        memcpy(USART2_Buffer_Rx + (USART2_BufferSize_Rx - number), USART2_DMA_Buffer_Rx, number);
      }
      else
      {
        memcpy(USART2_Buffer_Rx, USART2_DMA_Buffer_Rx + (number - USART2_BufferSize_Rx), USART2_BufferSize_Rx);
      }
      
      USART2_ReceiveData_P = USART2_BufferSize_Rx;
    }
    else
    {
      memcpy(USART2_Buffer_Rx + USART2_ReceiveData_P, USART2_DMA_Buffer_Rx, number);
      
      USART2_ReceiveData_P += number;
    }
    
    DMA_Cmd(DMA1_Stream5, DISABLE);                                 /* Disables the specified DMA1 Stream5. */
    while(DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);               /* Wait for the specified DMA1 Stream5 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream5, USART2_DMA_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA1 Stream5. */
    DMA_Cmd(DMA1_Stream5, ENABLE);                                  /* Enables the specified DMA1 Stream5. */
    
    USART_ReceiveDataFinishCallback(USART2);
  }
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
}

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
/**
  * @brief  USART3 IRQ handler.
  * @param  None.
  * @return None.
  */
void USART3_IRQHandler(void)
{
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
  if(USART_GetITStatus(USART3, USART_IT_TC) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ClearITPendingBit(USART3, USART_IT_TC);     /* Clears the USARTx's interrupt pending bits. */
    
    if(USART3_SendData_P == NULL)
    {
      USART3_SendData_Flag = false;
      
      USART_SendDataFinishCallback(USART3);
    }
    else
    {
      if(USART3_SendData_P > USART3_DMA_BufferSize_Tx)
      {
        memcpy(USART3_DMA_Buffer_Tx, USART3_Buffer_Tx, USART3_DMA_BufferSize_Tx);
        memmove(USART3_Buffer_Tx, USART3_Buffer_Tx + USART3_DMA_BufferSize_Tx, USART3_SendData_P - USART3_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);                    /* Clears the DMA1 Stream3's pending flags. */
        DMA_Cmd(DMA1_Stream3, DISABLE);                                 /* Disables the specified DMA1 Stream3. */
        while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);               /* Wait for the specified DMA1 Stream3 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream3, USART3_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream3. */
        DMA_Cmd(DMA1_Stream3, ENABLE);                                  /* Enables the specified DMA1 Stream3. */
        
        USART3_SendDataCount += USART3_DMA_BufferSize_Tx;
        USART3_SendData_P    -= USART3_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART3_DMA_Buffer_Tx, USART3_Buffer_Tx, USART3_SendData_P);
        
        DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);              /* Clears the DMA1 Stream3's pending flags. */
        DMA_Cmd(DMA1_Stream3, DISABLE);                           /* Disables the specified DMA1 Stream3. */
        while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);         /* Wait for the specified DMA1 Stream3 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream3, USART3_SendData_P);  /* Writes the number of data units to be transferred on the DMA1 Stream3. */
        DMA_Cmd(DMA1_Stream3, ENABLE);                            /* Enables the specified DMA1 Stream3. */
        
        USART3_SendDataCount += USART3_SendData_P;
        USART3_SendData_P     = NULL;
      }
    }
  }
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
  if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(USART3);                          /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART3_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream1);  /* Returns the number of remaining data units in the current DMA1 Stream1 transfer. */
    
    USART3_ReceiveDataCount += number;
    
    if(number > (USART3_BufferSize_Rx - USART3_ReceiveData_P))
    {
      if(number < USART3_BufferSize_Rx)
      {
        memmove(USART3_Buffer_Rx, USART3_Buffer_Rx + (number - (USART3_BufferSize_Rx - USART3_ReceiveData_P)), USART3_BufferSize_Rx - number);
        memcpy(USART3_Buffer_Rx + (USART3_BufferSize_Rx - number), USART3_DMA_Buffer_Rx, number);
      }
      else
      {
        memcpy(USART3_Buffer_Rx, USART3_DMA_Buffer_Rx + (number - USART3_BufferSize_Rx), USART3_BufferSize_Rx);
      }
      
      USART3_ReceiveData_P = USART3_BufferSize_Rx;
    }
    else
    {
      memcpy(USART3_Buffer_Rx + USART3_ReceiveData_P, USART3_DMA_Buffer_Rx, number);
      
      USART3_ReceiveData_P += number;
    }
    
    DMA_Cmd(DMA1_Stream1, DISABLE);                                 /* Disables the specified DMA1 Stream1. */
    while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);               /* Wait for the specified DMA1 Stream1 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream1, USART3_DMA_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA1 Stream1. */
    DMA_Cmd(DMA1_Stream1, ENABLE);                                  /* Enables the specified DMA1 Stream1. */
    
    USART_ReceiveDataFinishCallback(USART3);
  }
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
}
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
/**
  * @brief  UART4 IRQ handler.
  * @param  None.
  * @return None.
  */
void UART4_IRQHandler(void)
{
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
  if(USART_GetITStatus(UART4, USART_IT_TC) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ClearITPendingBit(UART4, USART_IT_TC);      /* Clears the USARTx's interrupt pending bits. */
    
    if(USART4_SendData_P == NULL)
    {
      USART4_SendData_Flag = false;
      
      USART_SendDataFinishCallback(UART4);
    }
    else
    {
      if(USART4_SendData_P > USART4_DMA_BufferSize_Tx)
      {
        memcpy(USART4_DMA_Buffer_Tx, USART4_Buffer_Tx, USART4_DMA_BufferSize_Tx);
        memmove(USART4_Buffer_Tx, USART4_Buffer_Tx + USART4_DMA_BufferSize_Tx, USART4_SendData_P - USART4_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);                    /* Clears the DMA1 Stream4's pending flags. */
        DMA_Cmd(DMA1_Stream4, DISABLE);                                 /* Disables the specified DMA1 Stream4. */
        while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);               /* Wait for the specified DMA1 Stream4 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream4, USART4_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream4. */
        DMA_Cmd(DMA1_Stream4, ENABLE);                                  /* Enables the specified DMA1 Stream4. */
        
        USART4_SendDataCount += USART4_DMA_BufferSize_Tx;
        USART4_SendData_P    -= USART4_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART4_DMA_Buffer_Tx, USART4_Buffer_Tx, USART4_SendData_P);
        
        DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);              /* Clears the DMA1 Stream4's pending flags. */
        DMA_Cmd(DMA1_Stream4, DISABLE);                           /* Disables the specified DMA1 Stream4. */
        while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);         /* Wait for the specified DMA1 Stream4 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream4, USART4_SendData_P);  /* Writes the number of data units to be transferred on the DMA1 Stream4. */
        DMA_Cmd(DMA1_Stream4, ENABLE);                            /* Enables the specified DMA1 Stream4. */
        
        USART4_SendDataCount += USART4_SendData_P;
        USART4_SendData_P     = NULL;
      }
    }
  }
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
  if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(UART4);                           /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART4_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream2);  /* Returns the number of remaining data units in the current DMA1 Stream2 transfer. */
    
    USART4_ReceiveDataCount += number;
    
    if(number > (USART4_BufferSize_Rx - USART4_ReceiveData_P))
    {
      if(number < USART4_BufferSize_Rx)
      {
        memmove(USART4_Buffer_Rx, USART4_Buffer_Rx + (number - (USART4_BufferSize_Rx - USART4_ReceiveData_P)), USART4_BufferSize_Rx - number);
        memcpy(USART4_Buffer_Rx + (USART4_BufferSize_Rx - number), USART4_DMA_Buffer_Rx, number);
      }
      else
      {
        memcpy(USART4_Buffer_Rx, USART4_DMA_Buffer_Rx + (number - USART4_BufferSize_Rx), USART4_BufferSize_Rx);
      }
      
      USART4_ReceiveData_P = USART4_BufferSize_Rx;
    }
    else
    {
      memcpy(USART4_Buffer_Rx + USART4_ReceiveData_P, USART4_DMA_Buffer_Rx, number);
      
      USART4_ReceiveData_P += number;
    }
    
    DMA_Cmd(DMA1_Stream2, DISABLE);                                 /* Disables the specified DMA1 Stream2. */
    while(DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);               /* Wait for the specified DMA1 Stream2 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream2, USART4_DMA_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA1 Stream2. */
    DMA_Cmd(DMA1_Stream2, ENABLE);                                  /* Enables the specified DMA1 Stream2. */
    
    USART_ReceiveDataFinishCallback(UART4);
  }
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
}

/**
  * @brief  UART5 IRQ handler.
  * @param  None.
  * @return None.
  */
void UART5_IRQHandler(void)
{
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
  if(USART_GetITStatus(UART5, USART_IT_TC) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ClearITPendingBit(UART5, USART_IT_TC);      /* Clears the USARTx's interrupt pending bits. */
    
    if(USART5_SendData_P == NULL)
    {
      USART5_SendData_Flag = false;
      
      USART_SendDataFinishCallback(UART5);
    }
    else
    {
      if(USART5_SendData_P > USART5_DMA_BufferSize_Tx)
      {
        memcpy(USART5_DMA_Buffer_Tx, USART5_Buffer_Tx, USART5_DMA_BufferSize_Tx);
        memmove(USART5_Buffer_Tx, USART5_Buffer_Tx + USART5_DMA_BufferSize_Tx, USART5_SendData_P - USART5_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);                    /* Clears the DMA1 Stream7's pending flags. */
        DMA_Cmd(DMA1_Stream7, DISABLE);                                 /* Disables the specified DMA1 Stream7. */
        while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);               /* Wait for the specified DMA1 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream7, USART5_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream7. */
        DMA_Cmd(DMA1_Stream7, ENABLE);                                  /* Enables the specified DMA1 Stream7. */
        
        USART5_SendDataCount += USART5_DMA_BufferSize_Tx;
        USART5_SendData_P    -= USART5_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART5_DMA_Buffer_Tx, USART5_Buffer_Tx, USART5_SendData_P);
        
        DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);              /* Clears the DMA1 Stream7's pending flags. */
        DMA_Cmd(DMA1_Stream7, DISABLE);                           /* Disables the specified DMA1 Stream7. */
        while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);         /* Wait for the specified DMA1 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream7, USART5_SendData_P);  /* Writes the number of data units to be transferred on the DMA1 Stream7. */
        DMA_Cmd(DMA1_Stream7, ENABLE);                            /* Enables the specified DMA1 Stream7. */
        
        USART5_SendDataCount += USART5_SendData_P;
        USART5_SendData_P     = NULL;
      }
    }
  }
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
  if(USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(UART5);                           /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART5_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream0);  /* Returns the number of remaining data units in the current DMA1 Stream0 transfer. */
    
    USART5_ReceiveDataCount += number;
    
    if(number > (USART5_BufferSize_Rx - USART5_ReceiveData_P))
    {
      if(number < USART5_BufferSize_Rx)
      {
        memmove(USART5_Buffer_Rx, USART5_Buffer_Rx + (number - (USART5_BufferSize_Rx - USART5_ReceiveData_P)), USART5_BufferSize_Rx - number);
        memcpy(USART5_Buffer_Rx + (USART5_BufferSize_Rx - number), USART5_DMA_Buffer_Rx, number);
      }
      else
      {
        memcpy(USART5_Buffer_Rx, USART5_DMA_Buffer_Rx + (number - USART5_BufferSize_Rx), USART5_BufferSize_Rx);
      }
      
      USART5_ReceiveData_P = USART5_BufferSize_Rx;
    }
    else
    {
      memcpy(USART5_Buffer_Rx + USART5_ReceiveData_P, USART5_DMA_Buffer_Rx, number);
      
      USART5_ReceiveData_P += number;
    }
    
    DMA_Cmd(DMA1_Stream0, DISABLE);                                 /* Disables the specified DMA1 Stream0. */
    while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);               /* Wait for the specified DMA1 Stream0 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream0, USART5_DMA_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA1 Stream0. */
    DMA_Cmd(DMA1_Stream0, ENABLE);                                  /* Enables the specified DMA1 Stream0. */
    
    USART_ReceiveDataFinishCallback(UART5);
  }
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
}
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */

/**
  * @brief  USART6 IRQ handler.
  * @param  None.
  * @return None.
  */
void USART6_IRQHandler(void)
{
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
  if(USART_GetITStatus(USART6, USART_IT_TC) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ClearITPendingBit(USART6, USART_IT_TC);     /* Clears the USARTx's interrupt pending bits. */
    
    if(USART6_SendData_P == NULL)
    {
      USART6_SendData_Flag = false;
      
      USART_SendDataFinishCallback(USART6);
    }
    else
    {
      if(USART6_SendData_P > USART6_DMA_BufferSize_Tx)
      {
        memcpy(USART6_DMA_Buffer_Tx, USART6_Buffer_Tx, USART6_DMA_BufferSize_Tx);
        memmove(USART6_Buffer_Tx, USART6_Buffer_Tx + USART6_DMA_BufferSize_Tx, USART6_SendData_P - USART6_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);                    /* Clears the DMA2 Stream6's pending flags. */
        DMA_Cmd(DMA2_Stream6, DISABLE);                                 /* Disables the specified DMA2 Stream6. */
        while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);               /* Wait for the specified DMA2 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream6, USART6_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA2 Stream6. */
        DMA_Cmd(DMA2_Stream6, ENABLE);                                  /* Enables the specified DMA2 Stream6. */
        
        USART6_SendDataCount += USART6_DMA_BufferSize_Tx;
        USART6_SendData_P    -= USART6_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART6_DMA_Buffer_Tx, USART6_Buffer_Tx, USART6_SendData_P);
        
        DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);              /* Clears the DMA2 Stream6's pending flags. */
        DMA_Cmd(DMA2_Stream6, DISABLE);                           /* Disables the specified DMA2 Stream6. */
        while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);         /* Wait for the specified DMA2 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream6, USART6_SendData_P);  /* Writes the number of data units to be transferred on the DMA2 Stream6. */
        DMA_Cmd(DMA2_Stream6, ENABLE);                            /* Enables the specified DMA2 Stream6. */
        
        USART6_SendDataCount += USART6_SendData_P;
        USART6_SendData_P     = NULL;
      }
    }
  }
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
  if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(USART6);                          /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART6_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream1);  /* Returns the number of remaining data units in the current DMA2 Stream1 transfer. */
    
    USART6_ReceiveDataCount += number;
    
    if(number > (USART6_BufferSize_Rx - USART6_ReceiveData_P))
    {
      if(number < USART6_BufferSize_Rx)
      {
        memmove(USART6_Buffer_Rx, USART6_Buffer_Rx + (number - (USART6_BufferSize_Rx - USART6_ReceiveData_P)), USART6_BufferSize_Rx - number);
        memcpy(USART6_Buffer_Rx + (USART6_BufferSize_Rx - number), USART6_DMA_Buffer_Rx, number);
      }
      else
      {
        memcpy(USART6_Buffer_Rx, USART6_DMA_Buffer_Rx + (number - USART6_BufferSize_Rx), USART6_BufferSize_Rx);
      }
      
      USART6_ReceiveData_P = USART6_BufferSize_Rx;
    }
    else
    {
      memcpy(USART6_Buffer_Rx + USART6_ReceiveData_P, USART6_DMA_Buffer_Rx, number);
      
      USART6_ReceiveData_P += number;
    }
    
    DMA_Cmd(DMA2_Stream1, DISABLE);                                 /* Disables the specified DMA2 Stream1. */
    while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);               /* Wait for the specified DMA2 Stream1 disable to finish. */
    DMA_SetCurrDataCounter(DMA2_Stream1, USART6_DMA_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA2 Stream1. */
    DMA_Cmd(DMA2_Stream1, ENABLE);                                  /* Enables the specified DMA2 Stream1. */
    
    USART_ReceiveDataFinishCallback(USART6);
  }
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
}

#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
/**
  * @brief  UART7 IRQ handler.
  * @param  None.
  * @return None.
  */
void UART7_IRQHandler(void)
{
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
  if(USART_GetITStatus(UART7, USART_IT_TC) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ClearITPendingBit(UART7, USART_IT_TC);      /* Clears the USARTx's interrupt pending bits. */
    
    if(USART7_SendData_P == NULL)
    {
      USART7_SendData_Flag = false;
      
      USART_SendDataFinishCallback(UART7);
    }
    else
    {
      if(USART7_SendData_P > USART7_DMA_BufferSize_Tx)
      {
        memcpy(USART7_DMA_Buffer_Tx, USART7_Buffer_Tx, USART7_DMA_BufferSize_Tx);
        memmove(USART7_Buffer_Tx, USART7_Buffer_Tx + USART7_DMA_BufferSize_Tx, USART7_SendData_P - USART7_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);                    /* Clears the DMA1 Stream1's pending flags. */
        DMA_Cmd(DMA1_Stream1, DISABLE);                                 /* Disables the specified DMA1 Stream1. */
        while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);               /* Wait for the specified DMA1 Stream1 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream1, USART7_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream1. */
        DMA_Cmd(DMA1_Stream1, ENABLE);                                  /* Enables the specified DMA1 Stream1. */
        
        USART7_SendDataCount += USART7_DMA_BufferSize_Tx;
        USART7_SendData_P    -= USART7_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART7_DMA_Buffer_Tx, USART7_Buffer_Tx, USART7_SendData_P);
        
        DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);              /* Clears the DMA1 Stream1's pending flags. */
        DMA_Cmd(DMA1_Stream1, DISABLE);                           /* Disables the specified DMA1 Stream1. */
        while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);         /* Wait for the specified DMA1 Stream1 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream1, USART7_SendData_P);  /* Writes the number of data units to be transferred on the DMA1 Stream1. */
        DMA_Cmd(DMA1_Stream1, ENABLE);                            /* Enables the specified DMA1 Stream1. */
        
        USART7_SendDataCount += USART7_SendData_P;
        USART7_SendData_P     = NULL;
      }
    }
  }
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
  if(USART_GetITStatus(UART7, USART_IT_IDLE) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(UART7);                           /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART7_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream3);  /* Returns the number of remaining data units in the current DMA1 Stream3 transfer. */
    
    USART7_ReceiveDataCount += number;
    
    if(number > (USART7_BufferSize_Rx - USART7_ReceiveData_P))
    {
      if(number < USART7_BufferSize_Rx)
      {
        memmove(USART7_Buffer_Rx, USART7_Buffer_Rx + (number - (USART7_BufferSize_Rx - USART7_ReceiveData_P)), USART7_BufferSize_Rx - number);
        memcpy(USART7_Buffer_Rx + (USART7_BufferSize_Rx - number), USART7_DMA_Buffer_Rx, number);
      }
      else
      {
        memcpy(USART7_Buffer_Rx, USART7_DMA_Buffer_Rx + (number - USART7_BufferSize_Rx), USART7_BufferSize_Rx);
      }
      
      USART7_ReceiveData_P = USART7_BufferSize_Rx;
    }
    else
    {
      memcpy(USART7_Buffer_Rx + USART7_ReceiveData_P, USART7_DMA_Buffer_Rx, number);
      
      USART7_ReceiveData_P += number;
    }
    
    DMA_Cmd(DMA1_Stream3, DISABLE);                                 /* Disables the specified DMA1 Stream3. */
    while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);               /* Wait for the specified DMA1 Stream3 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream3, USART7_DMA_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA1 Stream3. */
    DMA_Cmd(DMA1_Stream3, ENABLE);                                  /* Enables the specified DMA1 Stream3. */
    
    USART_ReceiveDataFinishCallback(UART7);
  }
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
}

/**
  * @brief  UART8 IRQ handler.
  * @param  None.
  * @return None.
  */
void UART8_IRQHandler(void)
{
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
  if(USART_GetITStatus(UART8, USART_IT_TC) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ClearITPendingBit(UART8, USART_IT_TC);      /* Clears the USARTx's interrupt pending bits. */
    
    if(USART8_SendData_P == NULL)
    {
      USART8_SendData_Flag = false;
      
      USART_SendDataFinishCallback(UART8);
    }
    else
    {
      if(USART8_SendData_P > USART8_DMA_BufferSize_Tx)
      {
        memcpy(USART8_DMA_Buffer_Tx, USART8_Buffer_Tx, USART8_DMA_BufferSize_Tx);
        memmove(USART8_Buffer_Tx, USART8_Buffer_Tx + USART8_DMA_BufferSize_Tx, USART8_SendData_P - USART8_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);                    /* Clears the DMA1 Stream0's pending flags. */
        DMA_Cmd(DMA1_Stream0, DISABLE);                                 /* Disables the specified DMA1 Stream0. */
        while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);               /* Wait for the specified DMA1 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream0, USART8_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream0. */
        DMA_Cmd(DMA1_Stream0, ENABLE);                                  /* Enables the specified DMA1 Stream0. */
        
        USART8_SendDataCount += USART8_DMA_BufferSize_Tx;
        USART8_SendData_P    -= USART8_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART8_DMA_Buffer_Tx, USART8_Buffer_Tx, USART8_SendData_P);
        
        DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);              /* Clears the DMA1 Stream0's pending flags. */
        DMA_Cmd(DMA1_Stream0, DISABLE);                           /* Disables the specified DMA1 Stream0. */
        while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);         /* Wait for the specified DMA1 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream0, USART8_SendData_P);  /* Writes the number of data units to be transferred on the DMA1 Stream0. */
        DMA_Cmd(DMA1_Stream0, ENABLE);                            /* Enables the specified DMA1 Stream0. */
        
        USART8_SendDataCount += USART8_SendData_P;
        USART8_SendData_P     = NULL;
      }
    }
  }
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
  if(USART_GetITStatus(UART8, USART_IT_IDLE) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(UART8);                           /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART8_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream6);  /* Returns the number of remaining data units in the current DMA1 Stream6 transfer. */
    
    USART8_ReceiveDataCount += number;
    
    if(number > (USART8_BufferSize_Rx - USART8_ReceiveData_P))
    {
      if(number < USART8_BufferSize_Rx)
      {
        memmove(USART8_Buffer_Rx, USART8_Buffer_Rx + (number - (USART8_BufferSize_Rx - USART8_ReceiveData_P)), USART8_BufferSize_Rx - number);
        memcpy(USART8_Buffer_Rx + (USART8_BufferSize_Rx - number), USART8_DMA_Buffer_Rx, number);
      }
      else
      {
        memcpy(USART8_Buffer_Rx, USART8_DMA_Buffer_Rx + (number - USART8_BufferSize_Rx), USART8_BufferSize_Rx);
      }
      
      USART8_ReceiveData_P = USART8_BufferSize_Rx;
    }
    else
    {
      memcpy(USART8_Buffer_Rx + USART8_ReceiveData_P, USART8_DMA_Buffer_Rx, number);
      
      USART8_ReceiveData_P += number;
    }
    
    DMA_Cmd(DMA1_Stream6, DISABLE);                                 /* Disables the specified DMA1 Stream6. */
    while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);               /* Wait for the specified DMA1 Stream6 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream6, USART8_DMA_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA1 Stream6. */
    DMA_Cmd(DMA1_Stream6, ENABLE);                                  /* Enables the specified DMA1 Stream6. */
    
    USART_ReceiveDataFinishCallback(UART8);
  }
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
}
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */

#if defined(STM32F413_423xx)
/**
  * @brief  UART9 IRQ handler.
  * @param  None.
  * @return None.
  */
void UART9_IRQHandler(void)
{
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
  if(USART_GetITStatus(UART9, USART_IT_TC) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ClearITPendingBit(UART9, USART_IT_TC);      /* Clears the USARTx's interrupt pending bits. */
    
    if(USART9_SendData_P == NULL)
    {
      USART9_SendData_Flag = false;
      
      USART_SendDataFinishCallback(UART9);
    }
    else
    {
      if(USART9_SendData_P > USART9_DMA_BufferSize_Tx)
      {
        memcpy(USART9_DMA_Buffer_Tx, USART9_Buffer_Tx, USART9_DMA_BufferSize_Tx);
        memmove(USART9_Buffer_Tx, USART9_Buffer_Tx + USART9_DMA_BufferSize_Tx, USART9_SendData_P - USART9_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);                    /* Clears the DMA2 Stream0's pending flags. */
        DMA_Cmd(DMA2_Stream0, DISABLE);                                 /* Disables the specified DMA2 Stream0. */
        while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);               /* Wait for the specified DMA2 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream0, USART9_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA2 Stream0. */
        DMA_Cmd(DMA2_Stream0, ENABLE);                                  /* Enables the specified DMA2 Stream0. */
        
        USART9_SendDataCount += USART9_DMA_BufferSize_Tx;
        USART9_SendData_P    -= USART9_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART9_DMA_Buffer_Tx, USART9_Buffer_Tx, USART9_SendData_P);
        
        DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);              /* Clears the DMA2 Stream0's pending flags. */
        DMA_Cmd(DMA2_Stream0, DISABLE);                           /* Disables the specified DMA2 Stream0. */
        while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);         /* Wait for the specified DMA2 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream0, USART9_SendData_P);  /* Writes the number of data units to be transferred on the DMA2 Stream0. */
        DMA_Cmd(DMA2_Stream0, ENABLE);                            /* Enables the specified DMA2 Stream0. */
        
        USART9_SendDataCount += USART9_SendData_P;
        USART9_SendData_P     = NULL;
      }
    }
  }
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
  if(USART_GetITStatus(UART9, USART_IT_IDLE) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(UART9);                           /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART9_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream7);  /* Returns the number of remaining data units in the current DMA2 Stream7 transfer. */
    
    USART9_ReceiveDataCount += number;
    
    if(number > (USART9_BufferSize_Rx - USART9_ReceiveData_P))
    {
      if(number < USART9_BufferSize_Rx)
      {
        memmove(USART9_Buffer_Rx, USART9_Buffer_Rx + (number - (USART9_BufferSize_Rx - USART9_ReceiveData_P)), USART9_BufferSize_Rx - number);
        memcpy(USART9_Buffer_Rx + (USART9_BufferSize_Rx - number), USART9_DMA_Buffer_Rx, number);
      }
      else
      {
        memcpy(USART9_Buffer_Rx, USART9_DMA_Buffer_Rx + (number - USART9_BufferSize_Rx), USART9_BufferSize_Rx);
      }
      
      USART9_ReceiveData_P = USART9_BufferSize_Rx;
    }
    else
    {
      memcpy(USART9_Buffer_Rx + USART9_ReceiveData_P, USART9_DMA_Buffer_Rx, number);
      
      USART9_ReceiveData_P += number;
    }
    
    DMA_Cmd(DMA2_Stream7, DISABLE);                                 /* Disables the specified DMA2 Stream7. */
    while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);               /* Wait for the specified DMA2 Stream7 disable to finish. */
    DMA_SetCurrDataCounter(DMA2_Stream7, USART9_DMA_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA2 Stream7. */
    DMA_Cmd(DMA2_Stream7, ENABLE);                                  /* Enables the specified DMA2 Stream7. */
    
    USART_ReceiveDataFinishCallback(UART9);
  }
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
}

/**
  * @brief  UART10 IRQ handler.
  * @param  None.
  * @return None.
  */
void UART10_IRQHandler(void)
{
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
  if(USART_GetITStatus(UART10, USART_IT_TC) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ClearITPendingBit(UART10, USART_IT_TC);     /* Clears the USARTx's interrupt pending bits. */
    
    if(USART10_SendData_P == NULL)
    {
      USART10_SendData_Flag = false;
      
      USART_SendDataFinishCallback(UART10);
    }
    else
    {
      if(USART10_SendData_P > USART10_DMA_BufferSize_Tx)
      {
        memcpy(USART10_DMA_Buffer_Tx, USART10_Buffer_Tx, USART10_DMA_BufferSize_Tx);
        memmove(USART10_Buffer_Tx, USART10_Buffer_Tx + USART10_DMA_BufferSize_Tx, USART10_SendData_P - USART10_DMA_BufferSize_Tx);
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);                      /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                                   /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);                 /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART10_DMA_BufferSize_Tx);  /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                                    /* Enables the specified DMA2 Stream7. */
        
        USART10_SendDataCount += USART10_DMA_BufferSize_Tx;
        USART10_SendData_P    -= USART10_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART10_DMA_Buffer_Tx, USART10_Buffer_Tx, USART10_SendData_P);
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);              /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                           /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);         /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART10_SendData_P); /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                            /* Enables the specified DMA2 Stream7. */
        
        USART10_SendDataCount += USART10_SendData_P;
        USART10_SendData_P     = NULL;
      }
    }
  }
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
  if(USART_GetITStatus(UART10, USART_IT_IDLE) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(UART10);                          /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART10_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream0); /* Returns the number of remaining data units in the current DMA2 Stream0 transfer. */
    
    USART10_ReceiveDataCount += number;
    
    if(number > (USART10_BufferSize_Rx - USART10_ReceiveData_P))
    {
      if(number < USART10_BufferSize_Rx)
      {
        memmove(USART10_Buffer_Rx, USART10_Buffer_Rx + (number - (USART10_BufferSize_Rx - USART10_ReceiveData_P)), USART10_BufferSize_Rx - number);
        memcpy(USART10_Buffer_Rx + (USART10_BufferSize_Rx - number), USART10_DMA_Buffer_Rx, number);
      }
      else
      {
        memcpy(USART10_Buffer_Rx, USART10_DMA_Buffer_Rx + (number - USART10_BufferSize_Rx), USART10_BufferSize_Rx);
      }
      
      USART10_ReceiveData_P = USART10_BufferSize_Rx;
    }
    else
    {
      memcpy(USART10_Buffer_Rx + USART10_ReceiveData_P, USART10_DMA_Buffer_Rx, number);
      
      USART10_ReceiveData_P += number;
    }
    
    DMA_Cmd(DMA2_Stream0, DISABLE);                                   /* Disables the specified DMA2 Stream0. */
    while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);                 /* Writes the number of data units to be transferred on the DMA2 Stream0. */
    DMA_SetCurrDataCounter(DMA2_Stream0, USART10_DMA_BufferSize_Rx);  /* Writes the number of data units to be transferred on the DMA2 Stream0. */
    DMA_Cmd(DMA2_Stream0, ENABLE);                                    /* Enables the specified DMA2 Stream0. */
    
    USART_ReceiveDataFinishCallback(UART10);
  }
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
}
#endif /* STM32F413_423xx */

/**
  * @brief  USART send data finish callback.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
static void USART_SendDataFinishCallback(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    if(USART1_SendDataFinishCallback != NULL)
    {
      USART1_SendDataFinishCallback();
    }
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    if(USART2_SendDataFinishCallback != NULL)
    {
      USART2_SendDataFinishCallback();
    }
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    if(USART3_SendDataFinishCallback != NULL)
    {
      USART3_SendDataFinishCallback();
    }
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    if(USART4_SendDataFinishCallback != NULL)
    {
      USART4_SendDataFinishCallback();
    }
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    if(USART5_SendDataFinishCallback != NULL)
    {
      USART5_SendDataFinishCallback();
    }
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    if(USART6_SendDataFinishCallback != NULL)
    {
      USART6_SendDataFinishCallback();
    }
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    if(USART7_SendDataFinishCallback != NULL)
    {
      USART7_SendDataFinishCallback();
    }
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    if(USART8_SendDataFinishCallback != NULL)
    {
      USART8_SendDataFinishCallback();
    }
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    if(USART9_SendDataFinishCallback != NULL)
    {
      USART9_SendDataFinishCallback();
    }
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    if(USART10_SendDataFinishCallback != NULL)
    {
      USART10_SendDataFinishCallback();
    }
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  USART receive data finish callback.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
static void USART_ReceiveDataFinishCallback(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    if(USART1_ReceiveDataFinishCallback != NULL)
    {
      USART1_ReceiveDataFinishCallback();
    }
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    if(USART2_ReceiveDataFinishCallback != NULL)
    {
      USART2_ReceiveDataFinishCallback();
    }
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    if(USART3_ReceiveDataFinishCallback != NULL)
    {
      USART3_ReceiveDataFinishCallback();
    }
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    if(USART4_ReceiveDataFinishCallback != NULL)
    {
      USART4_ReceiveDataFinishCallback();
    }
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    if(USART5_ReceiveDataFinishCallback != NULL)
    {
      USART5_ReceiveDataFinishCallback();
    }
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    if(USART6_ReceiveDataFinishCallback != NULL)
    {
      USART6_ReceiveDataFinishCallback();
    }
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    if(USART7_ReceiveDataFinishCallback != NULL)
    {
      USART7_ReceiveDataFinishCallback();
    }
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    if(USART8_ReceiveDataFinishCallback != NULL)
    {
      USART8_ReceiveDataFinishCallback();
    }
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    if(USART9_ReceiveDataFinishCallback != NULL)
    {
      USART9_ReceiveDataFinishCallback();
    }
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    if(USART10_ReceiveDataFinishCallback != NULL)
    {
      USART10_ReceiveDataFinishCallback();
    }
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  Set USART send data finish callback.
  * @param  [in] USARTx:   Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @param  [in] Function: Function pointer.
  * @return None.
  */
void USART_SetSendDataFinishCallback(USART_TypeDef *USARTx, void (*Function)(void))
{
  if(Function == NULL)
  {
    return;
  }
  
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    USART1_SendDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    USART2_SendDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    USART3_SendDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    USART4_SendDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    USART5_SendDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    USART6_SendDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    USART7_SendDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    USART8_SendDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    USART9_SendDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    USART10_SendDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  Set USART receive data finish callback.
  * @param  [in] USARTx:   Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @param  [in] Function: Function pointer.
  * @return None.
  */
void USART_SetReceiveDataFinishCallback(USART_TypeDef *USARTx, void (*Function)(void))
{
  if(Function == NULL)
  {
    return;
  }
  
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    USART1_ReceiveDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    USART2_ReceiveDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    USART3_ReceiveDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    USART4_ReceiveDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    USART5_ReceiveDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    USART6_ReceiveDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    USART7_ReceiveDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    USART8_ReceiveDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    USART9_ReceiveDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    USART10_ReceiveDataFinishCallback = (__IO void (*)(void))Function;
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  Clear USART send data finish callback.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
void USART_ClearSendDataFinishCallback(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    USART1_SendDataFinishCallback = NULL;
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    USART2_SendDataFinishCallback = NULL;
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    USART3_SendDataFinishCallback = NULL;
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    USART4_SendDataFinishCallback = NULL;
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    USART5_SendDataFinishCallback = NULL;
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    USART6_SendDataFinishCallback = NULL;
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    USART7_SendDataFinishCallback = NULL;
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    USART8_SendDataFinishCallback = NULL;
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    USART9_SendDataFinishCallback = NULL;
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    USART10_SendDataFinishCallback = NULL;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  Clear USART receive data finish callback.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
void USART_ClearReceiveDataFinishCallback(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    USART1_ReceiveDataFinishCallback = NULL;
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    USART2_ReceiveDataFinishCallback = NULL;
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    USART3_ReceiveDataFinishCallback = NULL;
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    USART4_ReceiveDataFinishCallback = NULL;
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    USART5_ReceiveDataFinishCallback = NULL;
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    USART6_ReceiveDataFinishCallback = NULL;
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    USART7_ReceiveDataFinishCallback = NULL;
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    USART8_ReceiveDataFinishCallback = NULL;
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    USART9_ReceiveDataFinishCallback = NULL;
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    USART10_ReceiveDataFinishCallback = NULL;
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  Get the USART send data count.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return Send data count.
  */
uint64_t USART_GetSendDataCount(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    return USART1_SendDataCount;
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    return USART2_SendDataCount;
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    return USART3_SendDataCount;
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    return USART4_SendDataCount;
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    return USART5_SendDataCount;
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    return USART6_SendDataCount;
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    return USART7_SendDataCount;
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    return USART8_SendDataCount;
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    return USART9_SendDataCount;
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    return USART10_SendDataCount;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
  
  return NULL;
}

/**
  * @brief  Get the USART receive data count.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return Receive data count.
  */
uint64_t USART_GetReceiveDataCount(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    return USART1_ReceiveDataCount;
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    return USART2_ReceiveDataCount;
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    return USART3_ReceiveDataCount;
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    return USART4_ReceiveDataCount;
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    return USART5_ReceiveDataCount;
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    return USART6_ReceiveDataCount;
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    return USART7_ReceiveDataCount;
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    return USART8_ReceiveDataCount;
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    return USART9_ReceiveDataCount;
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    return USART10_ReceiveDataCount;
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
  
  return NULL;
}

/**
  * @brief  Clear the USART send data count.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
void USART_ClearSendDataCount(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    USART1_SendDataCount = NULL;
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    USART2_SendDataCount = NULL;
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    USART3_SendDataCount = NULL;
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    USART4_SendDataCount = NULL;
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    USART5_SendDataCount = NULL;
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    USART6_SendDataCount = NULL;
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    USART7_SendDataCount = NULL;
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    USART8_SendDataCount = NULL;
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    USART9_SendDataCount = NULL;
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    USART10_SendDataCount = NULL;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  Clear the USART receive data count.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
void USART_ClearReceiveDataCount(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    USART1_ReceiveDataCount = NULL;
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    USART2_ReceiveDataCount = NULL;
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    USART3_ReceiveDataCount = NULL;
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    USART4_ReceiveDataCount = NULL;
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    USART5_ReceiveDataCount = NULL;
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    USART6_ReceiveDataCount = NULL;
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    USART7_ReceiveDataCount = NULL;
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    USART8_ReceiveDataCount = NULL;
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    USART9_ReceiveDataCount = NULL;
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    USART10_ReceiveDataCount = NULL;
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  Get the size of the USART send buffer used.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return Used the size of the send buffer.
  */
uint32_t USART_GetUsedSendBufferSize(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    return USART1_SendData_P;
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    return USART2_SendData_P;
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    return USART3_SendData_P;
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    return USART4_SendData_P;
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    return USART5_SendData_P;
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    return USART6_SendData_P;
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    return USART7_SendData_P;
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    return USART8_SendData_P;
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    return USART9_SendData_P;
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    return USART10_SendData_P;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
  
  return NULL;
}

/**
  * @brief  Get the size of the USART receive buffer used.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return Used the size of the receive buffer.
  */
uint32_t USART_GetUsedReceiveBufferSize(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    return USART1_ReceiveData_P;
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    return USART2_ReceiveData_P;
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    return USART3_ReceiveData_P;
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    return USART4_ReceiveData_P;
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    return USART5_ReceiveData_P;
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    return USART6_ReceiveData_P;
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    return USART7_ReceiveData_P;
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    return USART8_ReceiveData_P;
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    return USART9_ReceiveData_P;
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    return USART10_ReceiveData_P;
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
  
  return NULL;
}

/**
  * @brief  Get the size of the USART send buffer unused.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return Unused the size of the send buffer.
  */
uint32_t USART_GetUnusedSendBufferSize(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    return USART1_BufferSize_Tx - USART1_SendData_P;
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    return USART2_BufferSize_Tx - USART2_SendData_P;
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    return USART3_BufferSize_Tx - USART3_SendData_P;
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    return USART4_BufferSize_Tx - USART4_SendData_P;
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    return USART5_BufferSize_Tx - USART5_SendData_P;
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    return USART6_BufferSize_Tx - USART6_SendData_P;
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    return USART7_BufferSize_Tx - USART7_SendData_P;
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    return USART8_BufferSize_Tx - USART8_SendData_P;
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    return USART9_BufferSize_Tx - USART9_SendData_P;
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    return USART10_BufferSize_Tx - USART10_SendData_P;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
  
  return NULL;
}

/**
  * @brief  Get the size of the USART receive buffer unused.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return Unused the size of the receive buffer.
  */
uint32_t USART_GetUnusedReceiveBufferSize(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    return USART1_BufferSize_Rx - USART1_ReceiveData_P;
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    return USART2_BufferSize_Rx - USART2_ReceiveData_P;
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    return USART3_BufferSize_Rx - USART3_ReceiveData_P;
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    return USART4_BufferSize_Rx - USART4_ReceiveData_P;
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    return USART5_BufferSize_Rx - USART5_ReceiveData_P;
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    return USART6_BufferSize_Rx - USART6_ReceiveData_P;
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    return USART7_BufferSize_Rx - USART7_ReceiveData_P;
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    return USART8_BufferSize_Rx - USART8_ReceiveData_P;
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    return USART9_BufferSize_Rx - USART9_ReceiveData_P;
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    return USART10_BufferSize_Rx - USART10_ReceiveData_P;
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
  
  return NULL;
}

/**
  * @brief  Get the utilization rate of the USART send buffer.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return Send buffer utilization.
  */
float USART_GetSendBufferUnilizationRatio(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    return (float)USART1_SendData_P / (float)USART1_BufferSize_Tx;
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    return (float)USART2_SendData_P / (float)USART2_BufferSize_Tx;
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    return (float)USART3_SendData_P / (float)USART3_BufferSize_Tx;
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    return (float)USART4_SendData_P / (float)USART4_BufferSize_Tx;
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    return (float)USART5_SendData_P / (float)USART5_BufferSize_Tx;
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    return (float)USART6_SendData_P / (float)USART6_BufferSize_Tx;
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    return (float)USART7_SendData_P / (float)USART7_BufferSize_Tx;
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    return (float)USART8_SendData_P / (float)USART8_BufferSize_Tx;
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    return (float)USART9_SendData_P / (float)USART9_BufferSize_Tx;
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    return (float)USART10_SendData_P / (float)USART10_BufferSize_Tx;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
  
  return NULL;
}

/**
  * @brief  Get the utilization rate of the USART receive buffer.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return Receive buffer utilization.
  */
float USART_GetReceiveBufferUnilizationRatio(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    return (float)USART1_ReceiveData_P / (float)USART1_BufferSize_Rx;
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    return (float)USART2_ReceiveData_P / (float)USART2_BufferSize_Rx;
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    return (float)USART3_ReceiveData_P / (float)USART3_BufferSize_Rx;
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    return (float)USART4_ReceiveData_P / (float)USART4_BufferSize_Rx;
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    return (float)USART5_ReceiveData_P / (float)USART5_BufferSize_Rx;
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    return (float)USART6_ReceiveData_P / (float)USART6_BufferSize_Rx;
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    return (float)USART7_ReceiveData_P / (float)USART7_BufferSize_Rx;
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    return (float)USART8_ReceiveData_P / (float)USART8_BufferSize_Rx;
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    return (float)USART9_ReceiveData_P / (float)USART9_BufferSize_Rx;
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    return (float)USART10_ReceiveData_P / (float)USART10_BufferSize_Rx;
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
  
  return NULL;
}

/**
  * @brief  Clear the USART send buffer.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
void USART_ClearSendBuffer(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    USART1_SendData_P = NULL;
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    USART2_SendData_P = NULL;
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    USART3_SendData_P = NULL;
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    USART4_SendData_P = NULL;
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    USART5_SendData_P = NULL;
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    USART6_SendData_P = NULL;
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    USART7_SendData_P = NULL;
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    USART8_SendData_P = NULL;
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    USART9_SendData_P = NULL;
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    USART10_SendData_P = NULL;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  Clear the USART receive buffer.
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @return None.
  */
void USART_ClearReceiveBuffer(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    USART1_ReceiveData_P = NULL;
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    USART2_ReceiveData_P = NULL;
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    USART3_ReceiveData_P = NULL;
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    USART4_ReceiveData_P = NULL;
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    USART5_ReceiveData_P = NULL;
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    USART6_ReceiveData_P = NULL;
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    USART7_ReceiveData_P = NULL;
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    USART8_ReceiveData_P = NULL;
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    USART9_ReceiveData_P = NULL;
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    USART10_ReceiveData_P = NULL;
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
}

/**
  * @brief  Is the USART send buffer empty?
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @retval true:        The send buffer is empty.
  * @retval false:       The send buffer is not empty.
  */
bool USART_IsSendBufferEmpty(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    if(USART1_SendData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    if(USART2_SendData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    if(USART3_SendData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    if(USART4_SendData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    if(USART5_SendData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    if(USART6_SendData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    if(USART7_SendData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    if(USART8_SendData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    if(USART9_SendData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    if(USART10_SendData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
  
  return false;
}

/**
  * @brief  Is the USART receive buffer empty?
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @retval true:        The receive buffer is empty.
  * @retval false:       The receive buffer is not empty.
  */
bool USART_IsReceiveBufferEmpty(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    if(USART1_ReceiveData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    if(USART2_ReceiveData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    if(USART3_ReceiveData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    if(USART4_ReceiveData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    if(USART5_ReceiveData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    if(USART6_ReceiveData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    if(USART7_ReceiveData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    if(USART8_ReceiveData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    if(USART9_ReceiveData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    if(USART10_ReceiveData_P == NULL)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
  
  return false;
}

/**
  * @brief  Is the USART send buffer full?
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @retval true:        The send buffer is full.
  * @retval false:       The send buffer is not full.
  */
bool USART_IsSendBufferFull(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    if(USART1_SendData_P >= USART1_BufferSize_Tx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    if(USART2_SendData_P >= USART2_BufferSize_Tx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    if(USART3_SendData_P >= USART3_BufferSize_Tx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    if(USART4_SendData_P >= USART4_BufferSize_Tx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    if(USART5_SendData_P >= USART5_BufferSize_Tx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    if(USART6_SendData_P >= USART6_BufferSize_Tx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    if(USART7_SendData_P >= USART7_BufferSize_Tx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    if(USART8_SendData_P >= USART8_BufferSize_Tx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    if(USART9_SendData_P >= USART9_BufferSize_Tx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    if(USART10_SendData_P >= USART10_BufferSize_Tx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
  
  return false;
}

/**
  * @brief  Is the USART receive buffer full?
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @retval true:        The receive buffer is full.
  * @retval false:       The receive buffer is not full.
  */
bool USART_IsReceiveBufferFull(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    if(USART1_ReceiveData_P >= USART1_BufferSize_Rx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
    if(USART2_ReceiveData_P >= USART2_BufferSize_Rx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
    if(USART3_ReceiveData_P >= USART3_BufferSize_Rx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
    if(USART4_ReceiveData_P >= USART4_BufferSize_Rx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
    if(USART5_ReceiveData_P >= USART5_BufferSize_Rx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
    if(USART6_ReceiveData_P >= USART6_BufferSize_Rx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
    if(USART7_ReceiveData_P >= USART7_BufferSize_Rx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
    if(USART8_ReceiveData_P >= USART8_BufferSize_Rx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
    if(USART9_ReceiveData_P >= USART9_BufferSize_Rx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    if(USART10_ReceiveData_P >= USART10_BufferSize_Rx)
    {
      return true;
    }
    else
    {
      return false;
    }
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
  
  return false;
}

/**
  * @brief  USART is sending data?
  * @param  [in] USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
  * @retval true:        In sending data.
  * @retval false:       No sending data.
  */
bool USART_IsSendingData(USART_TypeDef *USARTx)
{
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    return USART1_SendData_Flag;
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    return USART2_SendData_Flag;
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
    return USART3_SendData_Flag;
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
    return USART4_SendData_Flag;
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    return USART5_SendData_Flag;
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
    return USART6_SendData_Flag;
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
    return USART7_SendData_Flag;
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    return USART8_SendData_Flag;
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
    return USART9_SendData_Flag;
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    return USART10_SendData_Flag;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
  
  return false;
}
