/****************************************************************
 * Copyright (C) 2016, XinLi, all right reserved.
 * File name:    USART.c
 * Date:         2016.03.22
 * Description:  USART driver.
*****************************************************************/

/****************************************************************
 *                        Header include
*****************************************************************/
#include "USART.h"
#include <string.h>
#include <stdbool.h>

/****************************************************************
 *                       Global variables
*****************************************************************/
#if (USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx) || (USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx)
static bool USART1_RCC_Flag = false, USART1_GPIO_Flag = false, USART1_NVIC_Flag = false;

#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
static bool USART1_DMA_Tx_Flag = false, USART1_SendData_Flag = false;
static uint32_t USART1_SendData_P = NULL;
static uint8_t USART1_Buffer_Tx[USART1_BufferSize_Tx]         = {NULL};
static uint8_t USART1_DMA_Buffer_Tx[USART1_DMA_BufferSize_Tx] = {NULL};
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */

#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
static bool USART1_DMA_Rx_Flag = false;
static uint32_t USART1_ReceiveData_P = NULL;
static uint8_t USART1_Buffer_Rx[USART1_BufferSize_Rx]         = {NULL};
static uint8_t USART1_DMA_Buffer_Rx[USART1_DMA_BufferSize_Rx] = {NULL};
#endif /* USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx */
#endif /* (USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx) || (USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx) */

#if (USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx) || (USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx)
static bool USART2_RCC_Flag = false, USART2_GPIO_Flag = false, USART2_NVIC_Flag = false;

#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
static bool USART2_DMA_Tx_Flag = false, USART2_SendData_Flag = false;
static uint32_t USART2_SendData_P = NULL;
static uint8_t USART2_Buffer_Tx[USART2_BufferSize_Tx]         = {NULL};
static uint8_t USART2_DMA_Buffer_Tx[USART2_DMA_BufferSize_Tx] = {NULL};
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */

#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
static bool USART2_DMA_Rx_Flag = false;
static uint32_t USART2_ReceiveData_P = NULL;
static uint8_t USART2_Buffer_Rx[USART2_BufferSize_Rx]         = {NULL};
static uint8_t USART2_DMA_Buffer_Rx[USART2_DMA_BufferSize_Rx] = {NULL};
#endif /* USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx */
#endif /* (USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx) || (USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx) */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
#if (USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx) || (USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx)
static bool USART3_RCC_Flag = false, USART3_GPIO_Flag = false, USART3_NVIC_Flag = false;

#if USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx
static bool USART3_DMA_Tx_Flag = false, USART3_SendData_Flag = false;
static uint32_t USART3_SendData_P = NULL;
static uint8_t USART3_Buffer_Tx[USART3_BufferSize_Tx]         = {NULL};
static uint8_t USART3_DMA_Buffer_Tx[USART3_DMA_BufferSize_Tx] = {NULL};
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */

#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
static bool USART3_DMA_Rx_Flag = false;
static uint32_t USART3_ReceiveData_P = NULL;
static uint8_t USART3_Buffer_Rx[USART3_BufferSize_Rx]         = {NULL};
static uint8_t USART3_DMA_Buffer_Rx[USART3_DMA_BufferSize_Rx] = {NULL};
#endif /* USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx */
#endif /* (USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx) || (USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx) */
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
#if (USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx) || (USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx)
static bool USART4_RCC_Flag = false, USART4_GPIO_Flag = false, USART4_NVIC_Flag = false;

#if USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx
static bool USART4_DMA_Tx_Flag = false, USART4_SendData_Flag = false;
static uint32_t USART4_SendData_P = NULL;
static uint8_t USART4_Buffer_Tx[USART4_BufferSize_Tx]         = {NULL};
static uint8_t USART4_DMA_Buffer_Tx[USART4_DMA_BufferSize_Tx] = {NULL};
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */

#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
static bool USART4_DMA_Rx_Flag = false;
static uint32_t USART4_ReceiveData_P = NULL;
static uint8_t USART4_Buffer_Rx[USART4_BufferSize_Rx]         = {NULL};
static uint8_t USART4_DMA_Buffer_Rx[USART4_DMA_BufferSize_Rx] = {NULL};
#endif /* USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx */
#endif /* (USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx) || (USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx) */

#if (USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx) || (USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx)
static bool USART5_RCC_Flag = false, USART5_GPIO_Flag = false, USART5_NVIC_Flag = false;

#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
static bool USART5_DMA_Tx_Flag = false, USART5_SendData_Flag = false;
static uint32_t USART5_SendData_P = NULL;
static uint8_t USART5_Buffer_Tx[USART5_BufferSize_Tx]         = {NULL};
static uint8_t USART5_DMA_Buffer_Tx[USART5_DMA_BufferSize_Tx] = {NULL};
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */

#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
static bool USART5_DMA_Rx_Flag = false;
static uint32_t USART5_ReceiveData_P = NULL;
static uint8_t USART5_Buffer_Rx[USART5_BufferSize_Rx]         = {NULL};
static uint8_t USART5_DMA_Buffer_Rx[USART5_DMA_BufferSize_Rx] = {NULL};
#endif /* USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx */
#endif /* (USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx) || (USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx) */
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */

#if (USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx) || (USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx)
static bool USART6_RCC_Flag = false, USART6_GPIO_Flag = false, USART6_NVIC_Flag = false;

#if USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx
static bool USART6_DMA_Tx_Flag = false, USART6_SendData_Flag = false;
static uint32_t USART6_SendData_P = NULL;
static uint8_t USART6_Buffer_Tx[USART6_BufferSize_Tx]         = {NULL};
static uint8_t USART6_DMA_Buffer_Tx[USART6_DMA_BufferSize_Tx] = {NULL};
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */

#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
static bool USART6_DMA_Rx_Flag = false;
static uint32_t USART6_ReceiveData_P = NULL;
static uint8_t USART6_Buffer_Rx[USART6_BufferSize_Rx]         = {NULL};
static uint8_t USART6_DMA_Buffer_Rx[USART6_DMA_BufferSize_Rx] = {NULL};
#endif /* USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx */
#endif /* (USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx) || (USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx) */

#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
#if (USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx) || (USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx)
static bool USART7_RCC_Flag = false, USART7_GPIO_Flag = false, USART7_NVIC_Flag = false;

#if USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx
static bool USART7_DMA_Tx_Flag = false, USART7_SendData_Flag = false;
static uint32_t USART7_SendData_P = NULL;
static uint8_t USART7_Buffer_Tx[USART7_BufferSize_Tx]         = {NULL};
static uint8_t USART7_DMA_Buffer_Tx[USART7_DMA_BufferSize_Tx] = {NULL};
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */

#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
static bool USART7_DMA_Rx_Flag = false;
static uint32_t USART7_ReceiveData_P = NULL;
static uint8_t USART7_Buffer_Rx[USART7_BufferSize_Rx]         = {NULL};
static uint8_t USART7_DMA_Buffer_Rx[USART7_DMA_BufferSize_Rx] = {NULL};
#endif /* USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx */
#endif /* (USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx) || (USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx) */

#if (USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx) || (USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx)
static bool USART8_RCC_Flag = false, USART8_GPIO_Flag = false, USART8_NVIC_Flag = false;

#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
static bool USART8_DMA_Tx_Flag = false, USART8_SendData_Flag = false;
static uint32_t USART8_SendData_P = NULL;
static uint8_t USART8_Buffer_Tx[USART8_BufferSize_Tx]         = {NULL};
static uint8_t USART8_DMA_Buffer_Tx[USART8_DMA_BufferSize_Tx] = {NULL};
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */

#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
static bool USART8_DMA_Rx_Flag = false;
static uint32_t USART8_ReceiveData_P = NULL;
static uint8_t USART8_Buffer_Rx[USART8_BufferSize_Rx]         = {NULL};
static uint8_t USART8_DMA_Buffer_Rx[USART8_DMA_BufferSize_Rx] = {NULL};
#endif /* USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx */
#endif /* (USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx) || (USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx) */
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */

#if defined(STM32F413_423xx)
#if (USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx) || (USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx)
static bool USART9_RCC_Flag = false, USART9_GPIO_Flag = false, USART9_NVIC_Flag = false;

#if USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx
static bool USART9_DMA_Tx_Flag = false, USART9_SendData_Flag = false;
static uint32_t USART9_SendData_P = NULL;
static uint8_t USART9_Buffer_Tx[USART9_BufferSize_Tx]         = {NULL};
static uint8_t USART9_DMA_Buffer_Tx[USART9_DMA_BufferSize_Tx] = {NULL};
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */

#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
static bool USART9_DMA_Rx_Flag = false;
static uint32_t USART9_ReceiveData_P = NULL;
static uint8_t USART9_Buffer_Rx[USART9_BufferSize_Rx]         = {NULL};
static uint8_t USART9_DMA_Buffer_Rx[USART9_DMA_BufferSize_Rx] = {NULL};
#endif /* USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx */
#endif /* (USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx) || (USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx) */

#if (USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx) || (USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx)
static bool USART10_RCC_Flag = false, USART10_GPIO_Flag = false, USART10_NVIC_Flag = false;

#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
static bool USART10_DMA_Tx_Flag = false, USART10_SendData_Flag = false;
static uint32_t USART10_SendData_P = NULL;
static uint8_t USART10_Buffer_Tx[USART10_BufferSize_Tx]         = {NULL};
static uint8_t USART10_DMA_Buffer_Tx[USART10_DMA_BufferSize_Tx] = {NULL};
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */

#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
static bool USART10_DMA_Rx_Flag = false;
static uint32_t USART10_ReceiveData_P = NULL;
static uint8_t USART10_Buffer_Rx[USART10_BufferSize_Rx]         = {NULL};
static uint8_t USART10_DMA_Buffer_Rx[USART10_DMA_BufferSize_Rx] = {NULL};
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
#endif /* (USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx) || (USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx) */
#endif /* STM32F413_423xx */

/****************************************************************
 *                     Function declaration
*****************************************************************/
static void USART_DMA_TX_Configuration(USART_TypeDef* USARTx);
static void USART_DMA_RX_Configuration(USART_TypeDef* USARTx);
static void USART_DMA_TX_Unconfigure(USART_TypeDef* USARTx);
static void USART_DMA_RX_Unconfigure(USART_TypeDef* USARTx);
static void USART_NVIC_Configuration(USART_TypeDef* USARTx);
static void USART_NVIC_Unconfigure(USART_TypeDef* USARTx);

/****************************************************************
 *                     Function definition
*****************************************************************/

/****************************************************************
 * Function:    USART_Configuration
 * Description: USART configuration.
 * Input:       USARTx
 *              BaudRate
 * Output:
 * Return:
*****************************************************************/
void USART_Configuration(USART_TypeDef* USARTx, uint32_t BaudRate)
{
  uint16_t USART_Mode = NULL;             /* USART Mode definition */
  
  GPIO_InitTypeDef GPIO_InitStructure;    /* GPIO Init structure definition */
  USART_InitTypeDef USART_InitStructure;  /* USART Init Structure definition */
  
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
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  /* Enable USART1 clock */
    }
    
    if(USART1_GPIO_Flag == false)
    {
      USART1_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART1_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART1_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(USART1_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART1_Rx GPIO clock */
      
      GPIO_PinAFConfig(USART1_Tx_GPIO, USART1_Tx_GPIO_PinSource, GPIO_AF_USART1); /* Connect GPIOxn to USART1_Tx */
      GPIO_PinAFConfig(USART1_Rx_GPIO, USART1_Rx_GPIO_PinSource, GPIO_AF_USART1); /* Connect GPIOxn to USART1_Rx */
      
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
      
      RCC_AHB1PeriphClockCmd(USART2_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART2_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(USART2_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART2_Rx GPIO clock */
      
      GPIO_PinAFConfig(USART2_Tx_GPIO, USART2_Tx_GPIO_PinSource, GPIO_AF_USART2);	/* Connect GPIOxn to USART2_Tx */
      GPIO_PinAFConfig(USART2_Rx_GPIO, USART2_Rx_GPIO_PinSource, GPIO_AF_USART2);	/* Connect GPIOxn to USART2_Rx */
      
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
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  /* Enable USART3 clock */
    }
    
    if(USART3_GPIO_Flag == false)
    {
      USART3_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART3_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART3_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(USART3_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART3_Rx GPIO clock */
      
      GPIO_PinAFConfig(USART3_Tx_GPIO, USART3_Tx_GPIO_PinSource, GPIO_AF_USART3); /* Connect GPIOxn to USART3_Tx */
      GPIO_PinAFConfig(USART3_Rx_GPIO, USART3_Rx_GPIO_PinSource, GPIO_AF_USART3); /* Connect GPIOxn to USART3_Rx */
      
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
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); /* Enable USART4 clock */
    }
    
    if(USART4_GPIO_Flag == false)
    {
      USART4_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART4_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART4_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(USART4_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART4_Rx GPIO clock */
      
      GPIO_PinAFConfig(USART4_Tx_GPIO, USART4_Tx_GPIO_PinSource, GPIO_AF_UART4);  /* Connect GPIOxn to USART4_Tx */
      GPIO_PinAFConfig(USART4_Rx_GPIO, USART4_Rx_GPIO_PinSource, GPIO_AF_UART4);  /* Connect GPIOxn to USART4_Rx */
      
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
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); /* Enable USART5 clock */
    }
    
    if(USART5_GPIO_Flag == false)
    {
      USART5_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART5_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART5_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(USART5_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART5_Rx GPIO clock */
      
      GPIO_PinAFConfig(USART5_Tx_GPIO, USART5_Tx_GPIO_PinSource, GPIO_AF_UART5);  /* Connect GPIOxn to USART5_Tx */
      GPIO_PinAFConfig(USART5_Rx_GPIO, USART5_Rx_GPIO_PinSource, GPIO_AF_UART5);  /* Connect GPIOxn to USART5_Rx */
      
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
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);  /* Enable USART6 clock */
    }
    
    if(USART6_GPIO_Flag == false)
    {
      USART6_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART6_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART6_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(USART6_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART6_Rx GPIO clock */
      
      GPIO_PinAFConfig(USART6_Tx_GPIO, USART6_Tx_GPIO_PinSource, GPIO_AF_USART6);  /* Connect GPIOxn to USART6_Tx */
      GPIO_PinAFConfig(USART6_Rx_GPIO, USART6_Rx_GPIO_PinSource, GPIO_AF_USART6);  /* Connect GPIOxn to USART6_Rx */
      
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
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE); /* Enable USART7 clock */
    }
    
    if(USART7_GPIO_Flag == false)
    {
      USART7_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART7_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART7_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(USART7_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART7_Rx GPIO clock */
      
      GPIO_PinAFConfig(USART7_Tx_GPIO, USART7_Tx_GPIO_PinSource, GPIO_AF_UART7);  /* Connect GPIOxn to USART7_Tx */
      GPIO_PinAFConfig(USART7_Rx_GPIO, USART7_Rx_GPIO_PinSource, GPIO_AF_UART7);  /* Connect GPIOxn to USART7_Rx */
      
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
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE); /* Enable USART8 clock */
    }
    
    if(USART8_GPIO_Flag == false)
    {
      USART8_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART8_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART8_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(USART8_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART8_Rx GPIO clock */
      
      GPIO_PinAFConfig(USART8_Tx_GPIO, USART8_Tx_GPIO_PinSource, GPIO_AF_UART8);  /* Connect GPIOxn to USART8_Tx */
      GPIO_PinAFConfig(USART8_Rx_GPIO, USART8_Rx_GPIO_PinSource, GPIO_AF_UART8);  /* Connect GPIOxn to USART8_Rx */
      
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
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART9, ENABLE); /* Enable USART9 clock */
    }
    
    if(USART9_GPIO_Flag == false)
    {
      USART9_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART9_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART9_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(USART9_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART9_Rx GPIO clock */
      
      GPIO_PinAFConfig(USART9_Tx_GPIO, USART9_Tx_GPIO_PinSource, GPIO_AF11_UART9);  /* Connect GPIOxn to USART9_Tx */
      GPIO_PinAFConfig(USART9_Rx_GPIO, USART9_Rx_GPIO_PinSource, GPIO_AF11_UART9);  /* Connect GPIOxn to USART9_Rx */
      
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
    USART_Mode          |= USART_Mode_Tx;
    USART10_SendData_Flag = false;
    USART10_SendData_P    = NULL;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
    
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
    USART_Mode          |= USART_Mode_Rx;
    USART10_ReceiveData_P = NULL;
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
    
    if(USART10_RCC_Flag == false)
    {
      USART10_RCC_Flag = true;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART10, ENABLE);  /* Enable USART10 clock */
    }
    
    if(USART10_GPIO_Flag == false)
    {
      USART10_GPIO_Flag = true;
      
      RCC_AHB1PeriphClockCmd(USART10_Tx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART10_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(USART10_Rx_RCC_AHB1Periph_GPIO, ENABLE);  /* Enable USART10_Rx GPIO clock */
      
      GPIO_PinAFConfig(USART10_Tx_GPIO, USART10_Tx_GPIO_PinSource, GPIO_AF11_UART10); /* Connect GPIOxn to USART10_Tx */
      GPIO_PinAFConfig(USART10_Rx_GPIO, USART10_Rx_GPIO_PinSource, GPIO_AF11_UART10); /* Connect GPIOxn to USART10_Rx */
      
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
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     /* Specifies the number of data bits transmitted or received in a frame. */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;                          /* Specifies the number of stop bits transmitted. */
  USART_InitStructure.USART_Parity = USART_Parity_No;                             /* Specifies the parity mode. */
  USART_InitStructure.USART_Mode = USART_Mode;                                    /* Specifies whether the Receive or Transmit mode is enabled or disabled. */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* Specifies wether the hardware flow control mode is enabled or disabled. */
  USART_Init(USARTx, &USART_InitStructure);                                       /* Initializes the USARTx peripheral. */
  
  if(USART_Mode == USART_Mode_Tx)
  {
    USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USARTx */
    USART_ITConfig(USARTx, USART_IT_TC, ENABLE);    /* Enabling interrupts specified USARTx */
    USART_DMA_TX_Configuration(USARTx);             /* Configuration USARTx DMA TX */
  }
  else if(USART_Mode == USART_Mode_Rx)
  {
    USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USARTx */
    USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);  /* Enabling interrupts specified USARTx */
    USART_DMA_RX_Configuration(USARTx);             /* Configuration USARTx DMA RX */
  }
  else
  {
    USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USARTx */
    USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USARTx */
    USART_ITConfig(USARTx, USART_IT_TC, ENABLE);    /* Enabling interrupts specified USARTx */
    USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);  /* Enabling interrupts specified USARTx */
    USART_DMA_TX_Configuration(USARTx);             /* Configuration USARTx DMA TX */
    USART_DMA_RX_Configuration(USARTx);             /* Configuration USARTx DMA RX */
  }
  
  USART_Cmd(USARTx, ENABLE);                        /* Enables or disables the specified USARTx peripheral. */
  USART_NVIC_Configuration(USARTx);                 /* Configuration USARTx NVIC */
}

/****************************************************************
 * Function:    USART_Unconfigure
 * Description: USART unconfigure.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
void USART_Unconfigure(USART_TypeDef* USARTx)
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
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE); /* Disable USART1 clock */
      
      USART_DeInit(USART1); /* Deinitializes the USART1 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(USART1); /* Unconfigure USART1 DMA TX */
      USART_DMA_RX_Unconfigure(USART1); /* Unconfigure USART1 DMA RX */
      
      USART_NVIC_Unconfigure(USART1);   /* Unconfigure USART1 NVIC */
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
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE); /* Disable USART2 clock */
      
      USART_DeInit(USART2); /* Deinitializes the USART2 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(USART2); /* Unconfigure USART2 DMA TX */
      USART_DMA_RX_Unconfigure(USART2); /* Unconfigure USART2 DMA RX */
      
      USART_NVIC_Unconfigure(USART2);   /* Unconfigure USART2 NVIC */
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
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, DISABLE); /* Disable USART3 clock */
      
      USART_DeInit(USART3); /* Deinitializes the USART3 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(USART3); /* Unconfigure USART3 DMA TX */
      USART_DMA_RX_Unconfigure(USART3); /* Unconfigure USART3 DMA RX */
      
      USART_NVIC_Unconfigure(USART3);   /* Unconfigure USART3 NVIC */
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
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, DISABLE);  /* Disable USART4 clock */
      
      USART_DeInit(UART4); /* Deinitializes the USART4 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART4); /* Unconfigure USART4 DMA TX */
      USART_DMA_RX_Unconfigure(UART4); /* Unconfigure USART4 DMA RX */
      
      USART_NVIC_Unconfigure(UART4);   /* Unconfigure USART4 NVIC */
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
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, DISABLE);  /* Disable USART5 clock */
      
      USART_DeInit(UART5); /* Deinitializes the USART5 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART5); /* Unconfigure USART5 DMA TX */
      USART_DMA_RX_Unconfigure(UART5); /* Unconfigure USART5 DMA RX */
      
      USART_NVIC_Unconfigure(UART5);   /* Unconfigure USART5 NVIC */
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
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, DISABLE); /* Disable USART6 clock */
      
      USART_DeInit(USART6); /* Deinitializes the USART6 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(USART6); /* Unconfigure USART6 DMA TX */
      USART_DMA_RX_Unconfigure(USART6); /* Unconfigure USART6 DMA RX */
      
      USART_NVIC_Unconfigure(USART6);   /* Unconfigure USART6 NVIC */
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
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, DISABLE);  /* Disable USART7 clock */
      
      USART_DeInit(UART7); /* Deinitializes the USART7 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART7); /* Unconfigure USART7 DMA TX */
      USART_DMA_RX_Unconfigure(UART7); /* Unconfigure USART7 DMA RX */
      
      USART_NVIC_Unconfigure(UART7);   /* Unconfigure USART7 NVIC */
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
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, DISABLE);  /* Disable USART8 clock */
      
      USART_DeInit(UART8); /* Deinitializes the USART8 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART8); /* Unconfigure USART8 DMA TX */
      USART_DMA_RX_Unconfigure(UART8); /* Unconfigure USART8 DMA RX */
      
      USART_NVIC_Unconfigure(UART8);   /* Unconfigure USART8 NVIC */
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
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART9, DISABLE);  /* Disable USART9 clock */
      
      USART_DeInit(UART9); /* Deinitializes the USART9 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART9); /* Unconfigure USART9 DMA TX */
      USART_DMA_RX_Unconfigure(UART9); /* Unconfigure USART9 DMA RX */
      
      USART_NVIC_Unconfigure(UART9);   /* Unconfigure USART9 NVIC */
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
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART10, DISABLE); /* Disable USART10 clock */
      
      USART_DeInit(UART10); /* Deinitializes the USART10 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART10); /* Unconfigure USART10 DMA TX */
      USART_DMA_RX_Unconfigure(UART10); /* Unconfigure USART10 DMA RX */
      
      USART_NVIC_Unconfigure(UART10);   /* Unconfigure USART10 NVIC */
    }
#endif /* (USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx) || (USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx) */
  }
#endif /* STM32F413_423xx */
}

/****************************************************************
 * Function:    USART_DMA_TX_Configuration
 * Description: USART DMA TX configuration.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
static void USART_DMA_TX_Configuration(USART_TypeDef* USARTx)
{
  DMA_InitTypeDef DMA_InitStructure;  /* DMA Init structure definition */

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
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  /* Enable DMA1 clock */
  }
  else if((USARTx == USART1) || (USARTx == USART6)
#if defined(STM32F413_423xx)
          || (USARTx == UART9) || (USARTx == UART10)
#endif /* STM32F413_423xx */
         )
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  /* Enable DMA2 clock */
  }

  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                 /* Data from memory to peripheral */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        /* Peripheral address does not increase */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 /* Memory address increment */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; /* Peripheral data width of 8 bit */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         /* Memory data width of 8 bit */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           /* Use Normal mode */
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   /* Medium priority */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  /* FIFO mode disabled */
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           /* Specifies the FIFO threshold level */
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             /* A single transmission burst memory */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     /* Peripherals single transmission burst */
  
  if(USARTx == USART1)
  {
#if USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx
    USART1_DMA_Tx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);     /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART1_DMA_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART1_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);     /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART2_DMA_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART2_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);     /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART3_DMA_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART3_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART4->DR);      /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART4_DMA_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART4_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART5->DR);      /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART5_DMA_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART5_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART6->DR);     /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART6_DMA_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART6_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART7->DR);      /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART7_DMA_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART7_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART8->DR);      /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART8_DMA_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART8_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_1;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART9->DR);      /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART9_DMA_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART9_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_6;                           /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART10->DR);      /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART10_DMA_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART10_DMA_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA2_Stream7);                         /* Deinitialize the DMA2 Stream7 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE); /* Wait for the specified DMA2 Stream7 reset to complete. */
    
    DMA_Init(DMA2_Stream7, &DMA_InitStructure); /* Initializes the DMA2 Stream7. */
    DMA_Cmd(DMA2_Stream7, DISABLE);             /* Disables the specified DMA2 Stream7. */
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
}

/****************************************************************
 * Function:    USART_DMA_RX_Configuration
 * Description: USART DMA RX configuration.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
static void USART_DMA_RX_Configuration(USART_TypeDef* USARTx)
{
  DMA_InitTypeDef DMA_InitStructure;  /* DMA Init structure definition */

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
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  /* Enable DMA1 clock */
  }
  else if((USARTx == USART1) || (USARTx == USART6)
#if defined(STM32F413_423xx)
          || (USARTx == UART9) || (USARTx == UART10)
#endif /* STM32F413_423xx */
         )
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  /* Enable DMA2 clock */
  }

  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                 /* Data Peripheral to Memory */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        /* Peripheral address does not increase */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 /* Memory address increment */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; /* Peripheral data width of 8 bit */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         /* Memory data width of 8 bit */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         /* Use Circular mode */
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   /* Medium priority */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  /* FIFO mode disabled */
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           /* Specifies the FIFO threshold level */
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             /* A single transmission burst memory */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     /* Peripherals single transmission burst */

  if(USARTx == USART1)
  {
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
    USART1_DMA_Rx_Flag = true;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);     /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART1_DMA_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART1_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);     /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART2_DMA_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART2_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);     /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART3_DMA_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART3_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART4->DR);      /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART4_DMA_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART4_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART5->DR);      /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART5_DMA_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART5_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA1_Stream0);                           /* Deinitialize the DMA1 Stream0 registers to their default reset values. */
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART6->DR);     /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART6_DMA_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART6_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART7->DR);      /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART7_DMA_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART7_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART8->DR);      /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART8_DMA_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART8_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;                          /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART9->DR);      /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART9_DMA_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART9_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
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
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                           /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART10->DR);      /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART10_DMA_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART10_DMA_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA2_Stream0);                         /* Deinitialize the DMA2 Stream0 registers to their default reset values. */
    while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE); /* Wait for the specified DMA2 Stream0 reset to complete. */
    
    DMA_Init(DMA2_Stream0, &DMA_InitStructure); /* Initializes the DMA2 Stream0. */
    DMA_Cmd(DMA2_Stream0, ENABLE);              /* Enables the specified DMA2 Stream0. */
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
}

/****************************************************************
 * Function:    USART_DMA_TX_Unconfigure
 * Description: USART DMA TX unconfigure.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
static void USART_DMA_TX_Unconfigure(USART_TypeDef* USARTx)
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

/****************************************************************
 * Function:    USART_DMA_RX_Unconfigure
 * Description: USART DMA RX unconfigure.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
static void USART_DMA_RX_Unconfigure(USART_TypeDef* USARTx)
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

/****************************************************************
 * Function:    USART_NVIC_Configuration
 * Description: USART NVIC configuration.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
static void USART_NVIC_Configuration(USART_TypeDef* USARTx)
{
  NVIC_InitTypeDef NVIC_InitStructure;  /* NVIC Init Structure definition */
  
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

/****************************************************************
 * Function:    USART_NVIC_Unconfigure
 * Description: USART NVIC unconfigure.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
static void USART_NVIC_Unconfigure(USART_TypeDef* USARTx)
{
  NVIC_InitTypeDef NVIC_InitStructure;  /* NVIC Init Structure definition */
  
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

/****************************************************************
 * Function:    USART_SetSendData
 * Description: USART set send data.
 * Input:       USARTx
                Buffer
                BufferSize
 * Output:
 * Return:      Send Data Number
*****************************************************************/
uint32_t USART_SetSendData(USART_TypeDef* USARTx, void* Buffer, uint32_t BufferSize)
{
  if((Buffer == NULL) || (BufferSize == NULL))
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
    
    if(BufferSize > (USART1_BufferSize_Tx - USART1_SendData_P))
    {
      return NULL;
    }
    
    memcpy(USART1_Buffer_Tx + USART1_SendData_P, Buffer, BufferSize); /* Copy memory */
    USART1_SendData_P += BufferSize;
    
    if(USART1_SendData_Flag == false)
    {
      USART1_SendData_Flag = true;
      
      if(USART1_SendData_P > USART1_DMA_BufferSize_Tx)
      {
        memcpy(USART1_DMA_Buffer_Tx, USART1_Buffer_Tx, USART1_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART1_Buffer_Tx, USART1_Buffer_Tx + USART1_DMA_BufferSize_Tx, USART1_SendData_P - USART1_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);                    /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                                 /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);               /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART1_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                                  /* Enables the specified DMA2 Stream7. */
        
        USART1_SendData_P -= USART1_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART1_DMA_Buffer_Tx, USART1_Buffer_Tx, USART1_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);             /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                          /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);        /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART1_SendData_P); /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                           /* Enables the specified DMA2 Stream7. */
        
        USART1_SendData_P = NULL;
      }
    }
    
    return BufferSize;
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  }
  else if(USARTx == USART2)
  {
#if USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx
    if((USART2_RCC_Flag == false) || (USART2_GPIO_Flag == false) || (USART2_DMA_Tx_Flag == false) || (USART2_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(BufferSize > (USART2_BufferSize_Tx - USART2_SendData_P))
    {
      return NULL;
    }
    
    memcpy(USART2_Buffer_Tx + USART2_SendData_P, Buffer, BufferSize); /* Copy memory */
    USART2_SendData_P += BufferSize;
    
    if(USART2_SendData_Flag == false)
    {
      USART2_SendData_Flag = true;
      
      if(USART2_SendData_P > USART2_DMA_BufferSize_Tx)
      {
        memcpy(USART2_DMA_Buffer_Tx, USART2_Buffer_Tx, USART2_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART2_Buffer_Tx, USART2_Buffer_Tx + USART2_DMA_BufferSize_Tx, USART2_SendData_P - USART2_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);                    /* Clears the DMA1 Stream6's pending flags. */
        DMA_Cmd(DMA1_Stream6, DISABLE);                                 /* Disables the specified DMA1 Stream6. */
        while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);               /* Wait for the specified DMA1 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream6, USART2_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream6. */
        DMA_Cmd(DMA1_Stream6, ENABLE);                                  /* Enables the specified DMA1 Stream6. */
        
        USART2_SendData_P -= USART2_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART2_DMA_Buffer_Tx, USART2_Buffer_Tx, USART2_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);             /* Clears the DMA1 Stream6's pending flags. */
        DMA_Cmd(DMA1_Stream6, DISABLE);                          /* Disables the specified DMA1 Stream6. */
        while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);        /* Wait for the specified DMA1 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream6, USART2_SendData_P); /* Writes the number of data units to be transferred on the DMA1 Stream6. */
        DMA_Cmd(DMA1_Stream6, ENABLE);                           /* Enables the specified DMA1 Stream6. */
        
        USART2_SendData_P = NULL;
      }
    }
    
    return BufferSize;
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
    
    if(BufferSize > (USART3_BufferSize_Tx - USART3_SendData_P))
    {
      return NULL;
    }
    
    memcpy(USART3_Buffer_Tx + USART3_SendData_P, Buffer, BufferSize); /* Copy memory */
    USART3_SendData_P += BufferSize;
    
    if(USART3_SendData_Flag == false)
    {
      USART3_SendData_Flag = true;
      
      if(USART3_SendData_P > USART3_DMA_BufferSize_Tx)
      {
        memcpy(USART3_DMA_Buffer_Tx, USART3_Buffer_Tx, USART3_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART3_Buffer_Tx, USART3_Buffer_Tx + USART3_DMA_BufferSize_Tx, USART3_SendData_P - USART3_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);                    /* Clears the DMA1 Stream3's pending flags. */
        DMA_Cmd(DMA1_Stream3, DISABLE);                                 /* Disables the specified DMA1 Stream3. */
        while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);               /* Wait for the specified DMA1 Stream3 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream3, USART3_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream3. */
        DMA_Cmd(DMA1_Stream3, ENABLE);                                  /* Enables the specified DMA1 Stream3. */
        
        USART3_SendData_P -= USART3_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART3_DMA_Buffer_Tx, USART3_Buffer_Tx, USART3_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);             /* Clears the DMA1 Stream3's pending flags. */
        DMA_Cmd(DMA1_Stream3, DISABLE);                          /* Disables the specified DMA1 Stream3. */
        while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);        /* Wait for the specified DMA1 Stream3 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream3, USART3_SendData_P); /* Writes the number of data units to be transferred on the DMA1 Stream3. */
        DMA_Cmd(DMA1_Stream3, ENABLE);                           /* Enables the specified DMA1 Stream3. */
        
        USART3_SendData_P = NULL;
      }
    }
    
    return BufferSize;
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
    
    if(BufferSize > (USART4_BufferSize_Tx - USART4_SendData_P))
    {
      return NULL;
    }
    
    memcpy(USART4_Buffer_Tx + USART4_SendData_P, Buffer, BufferSize); /* Copy memory */
    USART4_SendData_P += BufferSize;
    
    if(USART4_SendData_Flag == false)
    {
      USART4_SendData_Flag = true;
      
      if(USART4_SendData_P > USART4_DMA_BufferSize_Tx)
      {
        memcpy(USART4_DMA_Buffer_Tx, USART4_Buffer_Tx, USART4_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART4_Buffer_Tx, USART4_Buffer_Tx + USART4_DMA_BufferSize_Tx, USART4_SendData_P - USART4_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);                    /* Clears the DMA1 Stream4's pending flags. */
        DMA_Cmd(DMA1_Stream4, DISABLE);                                 /* Disables the specified DMA1 Stream4. */
        while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);               /* Wait for the specified DMA1 Stream4 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream4, USART4_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream4. */
        DMA_Cmd(DMA1_Stream4, ENABLE);                                  /* Enables the specified DMA1 Stream4. */
        
        USART4_SendData_P -= USART4_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART4_DMA_Buffer_Tx, USART4_Buffer_Tx, USART4_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);             /* Clears the DMA1 Stream4's pending flags. */
        DMA_Cmd(DMA1_Stream4, DISABLE);                          /* Disables the specified DMA1 Stream4. */
        while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);        /* Wait for the specified DMA1 Stream4 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream4, USART4_SendData_P); /* Writes the number of data units to be transferred on the DMA1 Stream4. */
        DMA_Cmd(DMA1_Stream4, ENABLE);                           /* Enables the specified DMA1 Stream4. */
        
        USART4_SendData_P = NULL;
      }
    }
    
    return BufferSize;
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART5)
  {
#if USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx
    if((USART5_RCC_Flag == false) || (USART5_GPIO_Flag == false) || (USART5_DMA_Tx_Flag == false) || (USART5_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(BufferSize > (USART5_BufferSize_Tx - USART5_SendData_P))
    {
      return NULL;
    }
    
    memcpy(USART5_Buffer_Tx + USART5_SendData_P, Buffer, BufferSize); /* Copy memory */
    USART5_SendData_P += BufferSize;
    
    if(USART5_SendData_Flag == false)
    {
      USART5_SendData_Flag = true;
      
      if(USART5_SendData_P > USART5_DMA_BufferSize_Tx)
      {
        memcpy(USART5_DMA_Buffer_Tx, USART5_Buffer_Tx, USART5_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART5_Buffer_Tx, USART5_Buffer_Tx + USART5_DMA_BufferSize_Tx, USART5_SendData_P - USART5_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);                    /* Clears the DMA1 Stream7's pending flags. */
        DMA_Cmd(DMA1_Stream7, DISABLE);                                 /* Disables the specified DMA1 Stream7. */
        while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);               /* Wait for the specified DMA1 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream7, USART5_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream7. */
        DMA_Cmd(DMA1_Stream7, ENABLE);                                  /* Enables the specified DMA1 Stream7. */
        
        USART5_SendData_P -= USART5_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART5_DMA_Buffer_Tx, USART5_Buffer_Tx, USART5_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);             /* Clears the DMA1 Stream7's pending flags. */
        DMA_Cmd(DMA1_Stream7, DISABLE);                          /* Disables the specified DMA1 Stream7. */
        while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);        /* Wait for the specified DMA1 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream7, USART5_SendData_P); /* Writes the number of data units to be transferred on the DMA1 Stream7. */
        DMA_Cmd(DMA1_Stream7, ENABLE);                           /* Enables the specified DMA1 Stream7. */
        
        USART5_SendData_P = NULL;
      }
    }
    
    return BufferSize;
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
    
    if(BufferSize > (USART6_BufferSize_Tx - USART6_SendData_P))
    {
      return NULL;
    }
    
    memcpy(USART6_Buffer_Tx + USART6_SendData_P, Buffer, BufferSize); /* Copy memory */
    USART6_SendData_P += BufferSize;
    
    if(USART6_SendData_Flag == false)
    {
      USART6_SendData_Flag = true;
      
      if(USART6_SendData_P > USART6_DMA_BufferSize_Tx)
      {
        memcpy(USART6_DMA_Buffer_Tx, USART6_Buffer_Tx, USART6_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART6_Buffer_Tx, USART6_Buffer_Tx + USART6_DMA_BufferSize_Tx, USART6_SendData_P - USART6_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);                    /* Clears the DMA2 Stream6's pending flags. */
        DMA_Cmd(DMA2_Stream6, DISABLE);                                 /* Disables the specified DMA2 Stream6. */
        while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);               /* Wait for the specified DMA2 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream6, USART6_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA2 Stream6. */
        DMA_Cmd(DMA2_Stream6, ENABLE);                                  /* Enables the specified DMA2 Stream6. */
        
        USART6_SendData_P -= USART6_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART6_DMA_Buffer_Tx, USART6_Buffer_Tx, USART6_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);             /* Clears the DMA2 Stream6's pending flags. */
        DMA_Cmd(DMA2_Stream6, DISABLE);                          /* Disables the specified DMA2 Stream6. */
        while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);        /* Wait for the specified DMA2 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream6, USART6_SendData_P); /* Writes the number of data units to be transferred on the DMA2 Stream6. */
        DMA_Cmd(DMA2_Stream6, ENABLE);                           /* Enables the specified DMA2 Stream6. */
        
        USART6_SendData_P = NULL;
      }
    }
    
    return BufferSize;
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
    
    if(BufferSize > (USART7_BufferSize_Tx - USART7_SendData_P))
    {
      return NULL;
    }
    
    memcpy(USART7_Buffer_Tx + USART7_SendData_P, Buffer, BufferSize); /* Copy memory */
    USART7_SendData_P += BufferSize;
    
    if(USART7_SendData_Flag == false)
    {
      USART7_SendData_Flag = true;
      
      if(USART7_SendData_P > USART7_DMA_BufferSize_Tx)
      {
        memcpy(USART7_DMA_Buffer_Tx, USART7_Buffer_Tx, USART7_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART7_Buffer_Tx, USART7_Buffer_Tx + USART7_DMA_BufferSize_Tx, USART7_SendData_P - USART7_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);                    /* Clears the DMA1 Stream1's pending flags. */
        DMA_Cmd(DMA1_Stream1, DISABLE);                                 /* Disables the specified DMA1 Stream1. */
        while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);               /* Wait for the specified DMA1 Stream1 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream1, USART7_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream1. */
        DMA_Cmd(DMA1_Stream1, ENABLE);                                  /* Enables the specified DMA1 Stream1. */
        
        USART7_SendData_P -= USART7_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART7_DMA_Buffer_Tx, USART7_Buffer_Tx, USART7_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);             /* Clears the DMA1 Stream1's pending flags. */
        DMA_Cmd(DMA1_Stream1, DISABLE);                          /* Disables the specified DMA1 Stream1. */
        while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);        /* Wait for the specified DMA1 Stream1 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream1, USART7_SendData_P); /* Writes the number of data units to be transferred on the DMA1 Stream1. */
        DMA_Cmd(DMA1_Stream1, ENABLE);                           /* Enables the specified DMA1 Stream1. */
        
        USART7_SendData_P = NULL;
      }
    }
    
    return BufferSize;
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART8)
  {
#if USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx
    if((USART8_RCC_Flag == false) || (USART8_GPIO_Flag == false) || (USART8_DMA_Tx_Flag == false) || (USART8_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(BufferSize > (USART8_BufferSize_Tx - USART8_SendData_P))
    {
      return NULL;
    }
    
    memcpy(USART8_Buffer_Tx + USART8_SendData_P, Buffer, BufferSize); /* Copy memory */
    USART8_SendData_P += BufferSize;
    
    if(USART8_SendData_Flag == false)
    {
      USART8_SendData_Flag = true;
      
      if(USART8_SendData_P > USART8_DMA_BufferSize_Tx)
      {
        memcpy(USART8_DMA_Buffer_Tx, USART8_Buffer_Tx, USART8_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART8_Buffer_Tx, USART8_Buffer_Tx + USART8_DMA_BufferSize_Tx, USART8_SendData_P - USART8_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);                    /* Clears the DMA1 Stream0's pending flags. */
        DMA_Cmd(DMA1_Stream0, DISABLE);                                 /* Disables the specified DMA1 Stream0. */
        while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);               /* Wait for the specified DMA1 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream0, USART8_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream0. */
        DMA_Cmd(DMA1_Stream0, ENABLE);                                  /* Enables the specified DMA1 Stream0. */
        
        USART8_SendData_P -= USART8_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART8_DMA_Buffer_Tx, USART8_Buffer_Tx, USART8_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);             /* Clears the DMA1 Stream0's pending flags. */
        DMA_Cmd(DMA1_Stream0, DISABLE);                          /* Disables the specified DMA1 Stream0. */
        while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);        /* Wait for the specified DMA1 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream0, USART8_SendData_P); /* Writes the number of data units to be transferred on the DMA1 Stream0. */
        DMA_Cmd(DMA1_Stream0, ENABLE);                           /* Enables the specified DMA1 Stream0. */
        
        USART8_SendData_P = NULL;
      }
    }
    
    return BufferSize;
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
    
    if(BufferSize > (USART9_BufferSize_Tx - USART9_SendData_P))
    {
      return NULL;
    }
    
    memcpy(USART9_Buffer_Tx + USART9_SendData_P, Buffer, BufferSize); /* Copy memory */
    USART9_SendData_P += BufferSize;
    
    if(USART9_SendData_Flag == false)
    {
      USART9_SendData_Flag = true;
      
      if(USART9_SendData_P > USART9_DMA_BufferSize_Tx)
      {
        memcpy(USART9_DMA_Buffer_Tx, USART9_Buffer_Tx, USART9_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART9_Buffer_Tx, USART9_Buffer_Tx + USART9_DMA_BufferSize_Tx, USART9_SendData_P - USART9_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);                    /* Clears the DMA2 Stream0's pending flags. */
        DMA_Cmd(DMA2_Stream0, DISABLE);                                 /* Disables the specified DMA2 Stream0. */
        while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);               /* Wait for the specified DMA2 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream0, USART9_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA2 Stream0. */
        DMA_Cmd(DMA2_Stream0, ENABLE);                                  /* Enables the specified DMA2 Stream0. */
        
        USART9_SendData_P -= USART9_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART9_DMA_Buffer_Tx, USART9_Buffer_Tx, USART9_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);             /* Clears the DMA2 Stream0's pending flags. */
        DMA_Cmd(DMA2_Stream0, DISABLE);                          /* Disables the specified DMA2 Stream0. */
        while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);        /* Wait for the specified DMA2 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream0, USART9_SendData_P); /* Writes the number of data units to be transferred on the DMA2 Stream0. */
        DMA_Cmd(DMA2_Stream0, ENABLE);                           /* Enables the specified DMA2 Stream0. */
        
        USART9_SendData_P = NULL;
      }
    }
    
    return BufferSize;
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  }
  else if(USARTx == UART10)
  {
#if USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx
    if((USART10_RCC_Flag == false) || (USART10_GPIO_Flag == false) || (USART10_DMA_Tx_Flag == false) || (USART10_NVIC_Flag == false))
    {
      return NULL;
    }
    
    if(BufferSize > (USART10_BufferSize_Tx - USART10_SendData_P))
    {
      return NULL;
    }
    
    memcpy(USART10_Buffer_Tx + USART10_SendData_P, Buffer, BufferSize); /* Copy memory */
    USART10_SendData_P += BufferSize;
    
    if(USART10_SendData_Flag == false)
    {
      USART10_SendData_Flag = true;
      
      if(USART10_SendData_P > USART10_DMA_BufferSize_Tx)
      {
        memcpy(USART10_DMA_Buffer_Tx, USART10_Buffer_Tx, USART10_DMA_BufferSize_Tx);                                               /* Copy memory */
        memmove(USART10_Buffer_Tx, USART10_Buffer_Tx + USART10_DMA_BufferSize_Tx, USART10_SendData_P - USART10_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);                      /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                                   /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);                 /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART10_DMA_BufferSize_Tx);  /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                                    /* Enables the specified DMA2 Stream7. */
        
        USART10_SendData_P -= USART10_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART10_DMA_Buffer_Tx, USART10_Buffer_Tx, USART10_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);               /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                            /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);          /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART10_SendData_P);  /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                             /* Enables the specified DMA2 Stream7. */
        
        USART10_SendData_P = NULL;
      }
    }
    
    return BufferSize;
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  }
#endif /* STM32F413_423xx */
  
  return NULL;
}

/****************************************************************
 * Function:    USART_GetReceiveData
 * Description: USART get receive data.
 * Input:       USARTx
 *              Buffer
 *              BufferSize
 * Output:
 * Return:      Receive Data Number
*****************************************************************/
uint32_t USART_GetReceiveData(USART_TypeDef* USARTx, void* Buffer, uint32_t BufferSize)
{
  uint32_t number = NULL;
  
  if((Buffer == NULL) || (BufferSize == NULL))
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
    
    if(USART1_ReceiveData_P > BufferSize)
    {
      memcpy(Buffer, USART1_Buffer_Rx, BufferSize);                                                 /* Copy memory */
      memmove(USART1_Buffer_Rx, USART1_Buffer_Rx + BufferSize, USART1_ReceiveData_P - BufferSize);  /* Copy memory */
      
      number = BufferSize;
      USART1_ReceiveData_P -= BufferSize;
    }
    else
    {
      memcpy(Buffer, USART1_Buffer_Rx, USART1_ReceiveData_P); /* Copy memory */
      
      number = USART1_ReceiveData_P;
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
    
    if(USART2_ReceiveData_P > BufferSize)
    {
      memcpy(Buffer, USART2_Buffer_Rx, BufferSize);                                                 /* Copy memory */
      memmove(USART2_Buffer_Rx, USART2_Buffer_Rx + BufferSize, USART2_ReceiveData_P - BufferSize);  /* Copy memory */
      
      number = BufferSize;
      USART2_ReceiveData_P -= BufferSize;
    }
    else
    {
      memcpy(Buffer, USART2_Buffer_Rx, USART2_ReceiveData_P); /* Copy memory */
      
      number = USART2_ReceiveData_P;
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
    
    if(USART3_ReceiveData_P > BufferSize)
    {
      memcpy(Buffer, USART3_Buffer_Rx, BufferSize);                                                 /* Copy memory */
      memmove(USART3_Buffer_Rx, USART3_Buffer_Rx + BufferSize, USART3_ReceiveData_P - BufferSize);  /* Copy memory */
      
      number = BufferSize;
      USART3_ReceiveData_P -= BufferSize;
    }
    else
    {
      memcpy(Buffer, USART3_Buffer_Rx, USART3_ReceiveData_P); /* Copy memory */
      
      number = USART3_ReceiveData_P;
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
    
    if(USART4_ReceiveData_P > BufferSize)
    {
      memcpy(Buffer, USART4_Buffer_Rx, BufferSize);                                                 /* Copy memory */
      memmove(USART4_Buffer_Rx, USART4_Buffer_Rx + BufferSize, USART4_ReceiveData_P - BufferSize);  /* Copy memory */
      
      number = BufferSize;
      USART4_ReceiveData_P -= BufferSize;
    }
    else
    {
      memcpy(Buffer, USART4_Buffer_Rx, USART4_ReceiveData_P); /* Copy memory */
      
      number = USART4_ReceiveData_P;
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
    
    if(USART5_ReceiveData_P > BufferSize)
    {
      memcpy(Buffer, USART5_Buffer_Rx, BufferSize);                                                 /* Copy memory */
      memmove(USART5_Buffer_Rx, USART5_Buffer_Rx + BufferSize, USART5_ReceiveData_P - BufferSize);  /* Copy memory */
      
      number = BufferSize;
      USART5_ReceiveData_P -= BufferSize;
    }
    else
    {
      memcpy(Buffer, USART5_Buffer_Rx, USART5_ReceiveData_P); /* Copy memory */
      
      number = USART5_ReceiveData_P;
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
    
    if(USART6_ReceiveData_P > BufferSize)
    {
      memcpy(Buffer, USART6_Buffer_Rx, BufferSize);                                                 /* Copy memory */
      memmove(USART6_Buffer_Rx, USART6_Buffer_Rx + BufferSize, USART6_ReceiveData_P - BufferSize);  /* Copy memory */
      
      number = BufferSize;
      USART6_ReceiveData_P -= BufferSize;
    }
    else
    {
      memcpy(Buffer, USART6_Buffer_Rx, USART6_ReceiveData_P); /* Copy memory */
      
      number = USART6_ReceiveData_P;
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
    
    if(USART7_ReceiveData_P > BufferSize)
    {
      memcpy(Buffer, USART7_Buffer_Rx, BufferSize);                                                 /* Copy memory */
      memmove(USART7_Buffer_Rx, USART7_Buffer_Rx + BufferSize, USART7_ReceiveData_P - BufferSize);  /* Copy memory */
      
      number = BufferSize;
      USART7_ReceiveData_P -= BufferSize;
    }
    else
    {
      memcpy(Buffer, USART7_Buffer_Rx, USART7_ReceiveData_P); /* Copy memory */
      
      number = USART7_ReceiveData_P;
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
    
    if(USART8_ReceiveData_P > BufferSize)
    {
      memcpy(Buffer, USART8_Buffer_Rx, BufferSize);                                                 /* Copy memory */
      memmove(USART8_Buffer_Rx, USART8_Buffer_Rx + BufferSize, USART8_ReceiveData_P - BufferSize);  /* Copy memory */
      
      number = BufferSize;
      USART8_ReceiveData_P -= BufferSize;
    }
    else
    {
      memcpy(Buffer, USART8_Buffer_Rx, USART8_ReceiveData_P); /* Copy memory */
      
      number = USART8_ReceiveData_P;
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
    
    if(USART9_ReceiveData_P > BufferSize)
    {
      memcpy(Buffer, USART9_Buffer_Rx, BufferSize);                                                 /* Copy memory */
      memmove(USART9_Buffer_Rx, USART9_Buffer_Rx + BufferSize, USART9_ReceiveData_P - BufferSize);  /* Copy memory */
      
      number = BufferSize;
      USART9_ReceiveData_P -= BufferSize;
    }
    else
    {
      memcpy(Buffer, USART9_Buffer_Rx, USART9_ReceiveData_P); /* Copy memory */
      
      number = USART9_ReceiveData_P;
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
    
    if(USART10_ReceiveData_P > BufferSize)
    {
      memcpy(Buffer, USART10_Buffer_Rx, BufferSize);                                                   /* Copy memory */
      memmove(USART10_Buffer_Rx, USART10_Buffer_Rx + BufferSize, USART10_ReceiveData_P - BufferSize);  /* Copy memory */
      
      number = BufferSize;
      USART10_ReceiveData_P -= BufferSize;
    }
    else
    {
      memcpy(Buffer, USART10_Buffer_Rx, USART10_ReceiveData_P); /* Copy memory */
      
      number = USART10_ReceiveData_P;
      USART10_ReceiveData_P = NULL;
    }
    
    return number;
#endif /* USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx */
  }
#endif /* STM32F413_423xx */
  
  return NULL;
}

/****************************************************************
 * Function:    USART1_IRQHandler
 * Description: USART1 IRQ handler.
 * Input:
 * Output:
 * Return:
*****************************************************************/
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
        memcpy(USART1_DMA_Buffer_Tx, USART1_Buffer_Tx, USART1_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART1_Buffer_Tx, USART1_Buffer_Tx + USART1_DMA_BufferSize_Tx, USART1_SendData_P - USART1_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);                    /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                                 /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);               /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART1_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                                  /* Enables the specified DMA2 Stream7. */
        
        USART1_SendData_P -= USART1_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART1_DMA_Buffer_Tx, USART1_Buffer_Tx, USART1_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);             /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                          /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);        /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART1_SendData_P); /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                           /* Enables the specified DMA2 Stream7. */
        
        USART1_SendData_P = NULL;
      }
    }
  }
#endif /* USART1_BufferSize_Tx && USART1_DMA_BufferSize_Tx */
  
#if USART1_BufferSize_Rx && USART1_DMA_BufferSize_Rx
  if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(USART1);  /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART1_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream2); /* Returns the number of remaining data units in the current DMA2 Stream2 transfer. */
    
    if(number > (USART1_BufferSize_Rx - USART1_ReceiveData_P))
    {
      if(number < USART1_BufferSize_Rx)
      {
        memmove(USART1_Buffer_Rx, USART1_Buffer_Rx + (number - (USART1_BufferSize_Rx - USART1_ReceiveData_P)), USART1_BufferSize_Rx - number);  /* Copy memory */
        memcpy(USART1_Buffer_Rx + (USART1_BufferSize_Rx - number), USART1_DMA_Buffer_Rx, number);                                               /* Copy memory */
      }
      else
      {
        memcpy(USART1_Buffer_Rx, USART1_DMA_Buffer_Rx + (number - USART1_BufferSize_Rx), USART1_BufferSize_Rx); /* Copy memory */
      }
      
      USART1_ReceiveData_P = USART1_BufferSize_Rx;
    }
    else
    {
      memcpy(USART1_Buffer_Rx + USART1_ReceiveData_P, USART1_DMA_Buffer_Rx, number); /* Copy memory */
      
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

/****************************************************************
 * Function:    USART2_IRQHandler
 * Description: USART2 IRQ handler.
 * Input:
 * Output:
 * Return:
*****************************************************************/
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
        memcpy(USART2_DMA_Buffer_Tx, USART2_Buffer_Tx, USART2_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART2_Buffer_Tx, USART2_Buffer_Tx + USART2_DMA_BufferSize_Tx, USART2_SendData_P - USART2_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);                    /* Clears the DMA1 Stream6's pending flags. */
        DMA_Cmd(DMA1_Stream6, DISABLE);                                 /* Disables the specified DMA1 Stream6. */
        while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);               /* Wait for the specified DMA1 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream6, USART2_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream6. */
        DMA_Cmd(DMA1_Stream6, ENABLE);                                  /* Enables the specified DMA1 Stream6. */
        
        USART2_SendData_P -= USART2_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART2_DMA_Buffer_Tx, USART2_Buffer_Tx, USART2_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);             /* Clears the DMA1 Stream6's pending flags. */
        DMA_Cmd(DMA1_Stream6, DISABLE);                          /* Disables the specified DMA1 Stream6. */
        while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);        /* Wait for the specified DMA1 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream6, USART2_SendData_P); /* Writes the number of data units to be transferred on the DMA1 Stream6. */
        DMA_Cmd(DMA1_Stream6, ENABLE);                           /* Enables the specified DMA1 Stream6. */
        
        USART2_SendData_P = NULL;
      }
    }
  }
#endif /* USART2_BufferSize_Tx && USART2_DMA_BufferSize_Tx */
  
#if USART2_BufferSize_Rx && USART2_DMA_BufferSize_Rx
  if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(USART2);  /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART2_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream5); /* Returns the number of remaining data units in the current DMA1 Stream5 transfer. */
    
    if(number > (USART2_BufferSize_Rx - USART2_ReceiveData_P))
    {
      if(number < USART2_BufferSize_Rx)
      {
        memmove(USART2_Buffer_Rx, USART2_Buffer_Rx + (number - (USART2_BufferSize_Rx - USART2_ReceiveData_P)), USART2_BufferSize_Rx - number);  /* Copy memory */
        memcpy(USART2_Buffer_Rx + (USART2_BufferSize_Rx - number), USART2_DMA_Buffer_Rx, number);                                               /* Copy memory */
      }
      else
      {
        memcpy(USART2_Buffer_Rx, USART2_DMA_Buffer_Rx + (number - USART2_BufferSize_Rx), USART2_BufferSize_Rx); /* Copy memory */
      }
      
      USART2_ReceiveData_P = USART2_BufferSize_Rx;
    }
    else
    {
      memcpy(USART2_Buffer_Rx + USART2_ReceiveData_P, USART2_DMA_Buffer_Rx, number); /* Copy memory */
      
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
/****************************************************************
 * Function:    USART3_IRQHandler
 * Description: USART3 IRQ handler.
 * Input:
 * Output:
 * Return:
*****************************************************************/
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
        memcpy(USART3_DMA_Buffer_Tx, USART3_Buffer_Tx, USART3_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART3_Buffer_Tx, USART3_Buffer_Tx + USART3_DMA_BufferSize_Tx, USART3_SendData_P - USART3_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);                    /* Clears the DMA1 Stream3's pending flags. */
        DMA_Cmd(DMA1_Stream3, DISABLE);                                 /* Disables the specified DMA1 Stream3. */
        while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);               /* Wait for the specified DMA1 Stream3 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream3, USART3_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream3. */
        DMA_Cmd(DMA1_Stream3, ENABLE);                                  /* Enables the specified DMA1 Stream3. */
        
        USART3_SendData_P -= USART3_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART3_DMA_Buffer_Tx, USART3_Buffer_Tx, USART3_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);             /* Clears the DMA1 Stream3's pending flags. */
        DMA_Cmd(DMA1_Stream3, DISABLE);                          /* Disables the specified DMA1 Stream3. */
        while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);        /* Wait for the specified DMA1 Stream3 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream3, USART3_SendData_P); /* Writes the number of data units to be transferred on the DMA1 Stream3. */
        DMA_Cmd(DMA1_Stream3, ENABLE);                           /* Enables the specified DMA1 Stream3. */
        
        USART3_SendData_P = NULL;
      }
    }
  }
#endif /* USART3_BufferSize_Tx && USART3_DMA_BufferSize_Tx */
  
#if USART3_BufferSize_Rx && USART3_DMA_BufferSize_Rx
  if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(USART3);                          /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART3_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream1); /* Returns the number of remaining data units in the current DMA1 Stream1 transfer. */
    
    if(number > (USART3_BufferSize_Rx - USART3_ReceiveData_P))
    {
      if(number < USART3_BufferSize_Rx)
      {
        memmove(USART3_Buffer_Rx, USART3_Buffer_Rx + (number - (USART3_BufferSize_Rx - USART3_ReceiveData_P)), USART3_BufferSize_Rx - number);  /* Copy memory */
        memcpy(USART3_Buffer_Rx + (USART3_BufferSize_Rx - number), USART3_DMA_Buffer_Rx, number);                                               /* Copy memory */
      }
      else
      {
        memcpy(USART3_Buffer_Rx, USART3_DMA_Buffer_Rx + (number - USART3_BufferSize_Rx), USART3_BufferSize_Rx); /* Copy memory */
      }
      
      USART3_ReceiveData_P = USART3_BufferSize_Rx;
    }
    else
    {
      memcpy(USART3_Buffer_Rx + USART3_ReceiveData_P, USART3_DMA_Buffer_Rx, number); /* Copy memory */
      
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
/****************************************************************
 * Function:    UART4_IRQHandler
 * Description: UART4 IRQ handler.
 * Input:
 * Output:
 * Return:
*****************************************************************/
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
        memcpy(USART4_DMA_Buffer_Tx, USART4_Buffer_Tx, USART4_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART4_Buffer_Tx, USART4_Buffer_Tx + USART4_DMA_BufferSize_Tx, USART4_SendData_P - USART4_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);                    /* Clears the DMA1 Stream4's pending flags. */
        DMA_Cmd(DMA1_Stream4, DISABLE);                                 /* Disables the specified DMA1 Stream4. */
        while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);               /* Wait for the specified DMA1 Stream4 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream4, USART4_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream4. */
        DMA_Cmd(DMA1_Stream4, ENABLE);                                  /* Enables the specified DMA1 Stream4. */
        
        USART4_SendData_P -= USART4_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART4_DMA_Buffer_Tx, USART4_Buffer_Tx, USART4_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);             /* Clears the DMA1 Stream4's pending flags. */
        DMA_Cmd(DMA1_Stream4, DISABLE);                          /* Disables the specified DMA1 Stream4. */
        while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);        /* Wait for the specified DMA1 Stream4 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream4, USART4_SendData_P); /* Writes the number of data units to be transferred on the DMA1 Stream4. */
        DMA_Cmd(DMA1_Stream4, ENABLE);                           /* Enables the specified DMA1 Stream4. */
        
        USART4_SendData_P = NULL;
      }
    }
  }
#endif /* USART4_BufferSize_Tx && USART4_DMA_BufferSize_Tx */
  
#if USART4_BufferSize_Rx && USART4_DMA_BufferSize_Rx
  if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(UART4); /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART4_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream2); /* Returns the number of remaining data units in the current DMA1 Stream2 transfer. */
    
    if(number > (USART4_BufferSize_Rx - USART4_ReceiveData_P))
    {
      if(number < USART4_BufferSize_Rx)
      {
        memmove(USART4_Buffer_Rx, USART4_Buffer_Rx + (number - (USART4_BufferSize_Rx - USART4_ReceiveData_P)), USART4_BufferSize_Rx - number);  /* Copy memory */
        memcpy(USART4_Buffer_Rx + (USART4_BufferSize_Rx - number), USART4_DMA_Buffer_Rx, number);                                               /* Copy memory */
      }
      else
      {
        memcpy(USART4_Buffer_Rx, USART4_DMA_Buffer_Rx + (number - USART4_BufferSize_Rx), USART4_BufferSize_Rx); /* Copy memory */
      }
      
      USART4_ReceiveData_P = USART4_BufferSize_Rx;
    }
    else
    {
      memcpy(USART4_Buffer_Rx + USART4_ReceiveData_P, USART4_DMA_Buffer_Rx, number); /* Copy memory */
      
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

/****************************************************************
 * Function:    UART5_IRQHandler
 * Description: UART5 IRQ handler.
 * Input:
 * Output:
 * Return:
*****************************************************************/
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
        memcpy(USART5_DMA_Buffer_Tx, USART5_Buffer_Tx, USART5_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART5_Buffer_Tx, USART5_Buffer_Tx + USART5_DMA_BufferSize_Tx, USART5_SendData_P - USART5_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);                    /* Clears the DMA1 Stream7's pending flags. */
        DMA_Cmd(DMA1_Stream7, DISABLE);                                 /* Disables the specified DMA1 Stream7. */
        while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);               /* Wait for the specified DMA1 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream7, USART5_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream7. */
        DMA_Cmd(DMA1_Stream7, ENABLE);                                  /* Enables the specified DMA1 Stream7. */
        
        USART5_SendData_P -= USART5_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART5_DMA_Buffer_Tx, USART5_Buffer_Tx, USART5_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);             /* Clears the DMA1 Stream7's pending flags. */
        DMA_Cmd(DMA1_Stream7, DISABLE);                          /* Disables the specified DMA1 Stream7. */
        while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);        /* Wait for the specified DMA1 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream7, USART5_SendData_P); /* Writes the number of data units to be transferred on the DMA1 Stream7. */
        DMA_Cmd(DMA1_Stream7, ENABLE);                           /* Enables the specified DMA1 Stream7. */
        
        USART5_SendData_P = NULL;
      }
    }
  }
#endif /* USART5_BufferSize_Tx && USART5_DMA_BufferSize_Tx */
  
#if USART5_BufferSize_Rx && USART5_DMA_BufferSize_Rx
  if(USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(UART5); /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART5_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream0); /* Returns the number of remaining data units in the current DMA1 Stream0 transfer. */
    
    if(number > (USART5_BufferSize_Rx - USART5_ReceiveData_P))
    {
      if(number < USART5_BufferSize_Rx)
      {
        memmove(USART5_Buffer_Rx, USART5_Buffer_Rx + (number - (USART5_BufferSize_Rx - USART5_ReceiveData_P)), USART5_BufferSize_Rx - number);  /* Copy memory */
        memcpy(USART5_Buffer_Rx + (USART5_BufferSize_Rx - number), USART5_DMA_Buffer_Rx, number);                                               /* Copy memory */
      }
      else
      {
        memcpy(USART5_Buffer_Rx, USART5_DMA_Buffer_Rx + (number - USART5_BufferSize_Rx), USART5_BufferSize_Rx); /* Copy memory */
      }
      
      USART5_ReceiveData_P = USART5_BufferSize_Rx;
    }
    else
    {
      memcpy(USART5_Buffer_Rx + USART5_ReceiveData_P, USART5_DMA_Buffer_Rx, number); /* Copy memory */
      
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

/****************************************************************
 * Function:    USART6_IRQHandler
 * Description: USART6 IRQ handler.
 * Input:
 * Output:
 * Return:
*****************************************************************/
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
        memcpy(USART6_DMA_Buffer_Tx, USART6_Buffer_Tx, USART6_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART6_Buffer_Tx, USART6_Buffer_Tx + USART6_DMA_BufferSize_Tx, USART6_SendData_P - USART6_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);                    /* Clears the DMA2 Stream6's pending flags. */
        DMA_Cmd(DMA2_Stream6, DISABLE);                                 /* Disables the specified DMA2 Stream6. */
        while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);               /* Wait for the specified DMA2 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream6, USART6_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA2 Stream6. */
        DMA_Cmd(DMA2_Stream6, ENABLE);                                  /* Enables the specified DMA2 Stream6. */
        
        USART6_SendData_P -= USART6_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART6_DMA_Buffer_Tx, USART6_Buffer_Tx, USART6_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);             /* Clears the DMA2 Stream6's pending flags. */
        DMA_Cmd(DMA2_Stream6, DISABLE);                          /* Disables the specified DMA2 Stream6. */
        while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);        /* Wait for the specified DMA2 Stream6 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream6, USART6_SendData_P); /* Writes the number of data units to be transferred on the DMA2 Stream6. */
        DMA_Cmd(DMA2_Stream6, ENABLE);                           /* Enables the specified DMA2 Stream6. */
        
        USART6_SendData_P = NULL;
      }
    }
  }
#endif /* USART6_BufferSize_Tx && USART6_DMA_BufferSize_Tx */
  
#if USART6_BufferSize_Rx && USART6_DMA_BufferSize_Rx
  if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(USART6);  /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART6_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream1); /* Returns the number of remaining data units in the current DMA2 Stream1 transfer. */
    
    if(number > (USART6_BufferSize_Rx - USART6_ReceiveData_P))
    {
      if(number < USART6_BufferSize_Rx)
      {
        memmove(USART6_Buffer_Rx, USART6_Buffer_Rx + (number - (USART6_BufferSize_Rx - USART6_ReceiveData_P)), USART6_BufferSize_Rx - number);  /* Copy memory */
        memcpy(USART6_Buffer_Rx + (USART6_BufferSize_Rx - number), USART6_DMA_Buffer_Rx, number);                                               /* Copy memory */
      }
      else
      {
        memcpy(USART6_Buffer_Rx, USART6_DMA_Buffer_Rx + (number - USART6_BufferSize_Rx), USART6_BufferSize_Rx); /* Copy memory */
      }
      
      USART6_ReceiveData_P = USART6_BufferSize_Rx;
    }
    else
    {
      memcpy(USART6_Buffer_Rx + USART6_ReceiveData_P, USART6_DMA_Buffer_Rx, number); /* Copy memory */
      
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
/****************************************************************
 * Function:    UART7_IRQHandler
 * Description: UART7 IRQ handler.
 * Input:
 * Output:
 * Return:
*****************************************************************/
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
        memcpy(USART7_DMA_Buffer_Tx, USART7_Buffer_Tx, USART7_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART7_Buffer_Tx, USART7_Buffer_Tx + USART7_DMA_BufferSize_Tx, USART7_SendData_P - USART7_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);                    /* Clears the DMA1 Stream1's pending flags. */
        DMA_Cmd(DMA1_Stream1, DISABLE);                                 /* Disables the specified DMA1 Stream1. */
        while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);               /* Wait for the specified DMA1 Stream1 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream1, USART7_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream1. */
        DMA_Cmd(DMA1_Stream1, ENABLE);                                  /* Enables the specified DMA1 Stream1. */
        
        USART7_SendData_P -= USART7_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART7_DMA_Buffer_Tx, USART7_Buffer_Tx, USART7_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);             /* Clears the DMA1 Stream1's pending flags. */
        DMA_Cmd(DMA1_Stream1, DISABLE);                          /* Disables the specified DMA1 Stream1. */
        while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);        /* Wait for the specified DMA1 Stream1 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream1, USART7_SendData_P); /* Writes the number of data units to be transferred on the DMA1 Stream1. */
        DMA_Cmd(DMA1_Stream1, ENABLE);                           /* Enables the specified DMA1 Stream1. */
        
        USART7_SendData_P = NULL;
      }
    }
  }
#endif /* USART7_BufferSize_Tx && USART7_DMA_BufferSize_Tx */
  
#if USART7_BufferSize_Rx && USART7_DMA_BufferSize_Rx
  if(USART_GetITStatus(UART7, USART_IT_IDLE) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(UART7); /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART7_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream3); /* Returns the number of remaining data units in the current DMA1 Stream3 transfer. */
    
    if(number > (USART7_BufferSize_Rx - USART7_ReceiveData_P))
    {
      if(number < USART7_BufferSize_Rx)
      {
        memmove(USART7_Buffer_Rx, USART7_Buffer_Rx + (number - (USART7_BufferSize_Rx - USART7_ReceiveData_P)), USART7_BufferSize_Rx - number);  /* Copy memory */
        memcpy(USART7_Buffer_Rx + (USART7_BufferSize_Rx - number), USART7_DMA_Buffer_Rx, number);                                               /* Copy memory */
      }
      else
      {
        memcpy(USART7_Buffer_Rx, USART7_DMA_Buffer_Rx + (number - USART7_BufferSize_Rx), USART7_BufferSize_Rx); /* Copy memory */
      }
      
      USART7_ReceiveData_P = USART7_BufferSize_Rx;
    }
    else
    {
      memcpy(USART7_Buffer_Rx + USART7_ReceiveData_P, USART7_DMA_Buffer_Rx, number); /* Copy memory */
      
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

/****************************************************************
 * Function:    UART8_IRQHandler
 * Description: UART8 IRQ handler.
 * Input:
 * Output:
 * Return:
*****************************************************************/
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
        memcpy(USART8_DMA_Buffer_Tx, USART8_Buffer_Tx, USART8_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART8_Buffer_Tx, USART8_Buffer_Tx + USART8_DMA_BufferSize_Tx, USART8_SendData_P - USART8_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);                    /* Clears the DMA1 Stream0's pending flags. */
        DMA_Cmd(DMA1_Stream0, DISABLE);                                 /* Disables the specified DMA1 Stream0. */
        while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);               /* Wait for the specified DMA1 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream0, USART8_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA1 Stream0. */
        DMA_Cmd(DMA1_Stream0, ENABLE);                                  /* Enables the specified DMA1 Stream0. */
        
        USART8_SendData_P -= USART8_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART8_DMA_Buffer_Tx, USART8_Buffer_Tx, USART8_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);             /* Clears the DMA1 Stream0's pending flags. */
        DMA_Cmd(DMA1_Stream0, DISABLE);                          /* Disables the specified DMA1 Stream0. */
        while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);        /* Wait for the specified DMA1 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA1_Stream0, USART8_SendData_P); /* Writes the number of data units to be transferred on the DMA1 Stream0. */
        DMA_Cmd(DMA1_Stream0, ENABLE);                           /* Enables the specified DMA1 Stream0. */
        
        USART8_SendData_P = NULL;
      }
    }
  }
#endif /* USART8_BufferSize_Tx && USART8_DMA_BufferSize_Tx */
  
#if USART8_BufferSize_Rx && USART8_DMA_BufferSize_Rx
  if(USART_GetITStatus(UART8, USART_IT_IDLE) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(UART8); /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART8_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream6); /* Returns the number of remaining data units in the current DMA1 Stream6 transfer. */
    
    if(number > (USART8_BufferSize_Rx - USART8_ReceiveData_P))
    {
      if(number < USART8_BufferSize_Rx)
      {
        memmove(USART8_Buffer_Rx, USART8_Buffer_Rx + (number - (USART8_BufferSize_Rx - USART8_ReceiveData_P)), USART8_BufferSize_Rx - number);  /* Copy memory */
        memcpy(USART8_Buffer_Rx + (USART8_BufferSize_Rx - number), USART8_DMA_Buffer_Rx, number);                                               /* Copy memory */
      }
      else
      {
        memcpy(USART8_Buffer_Rx, USART8_DMA_Buffer_Rx + (number - USART8_BufferSize_Rx), USART8_BufferSize_Rx); /* Copy memory */
      }
      
      USART8_ReceiveData_P = USART8_BufferSize_Rx;
    }
    else
    {
      memcpy(USART8_Buffer_Rx + USART8_ReceiveData_P, USART8_DMA_Buffer_Rx, number); /* Copy memory */
      
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
/****************************************************************
 * Function:    UART9_IRQHandler
 * Description: UART9 IRQ handler.
 * Input:
 * Output:
 * Return:
*****************************************************************/
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
        memcpy(USART9_DMA_Buffer_Tx, USART9_Buffer_Tx, USART9_DMA_BufferSize_Tx);                                             /* Copy memory */
        memmove(USART9_Buffer_Tx, USART9_Buffer_Tx + USART9_DMA_BufferSize_Tx, USART9_SendData_P - USART9_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);                    /* Clears the DMA2 Stream0's pending flags. */
        DMA_Cmd(DMA2_Stream0, DISABLE);                                 /* Disables the specified DMA2 Stream0. */
        while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);               /* Wait for the specified DMA2 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream0, USART9_DMA_BufferSize_Tx); /* Writes the number of data units to be transferred on the DMA2 Stream0. */
        DMA_Cmd(DMA2_Stream0, ENABLE);                                  /* Enables the specified DMA2 Stream0. */
        
        USART9_SendData_P -= USART9_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART9_DMA_Buffer_Tx, USART9_Buffer_Tx, USART9_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);             /* Clears the DMA2 Stream0's pending flags. */
        DMA_Cmd(DMA2_Stream0, DISABLE);                          /* Disables the specified DMA2 Stream0. */
        while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);        /* Wait for the specified DMA2 Stream0 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream0, USART9_SendData_P); /* Writes the number of data units to be transferred on the DMA2 Stream0. */
        DMA_Cmd(DMA2_Stream0, ENABLE);                           /* Enables the specified DMA2 Stream0. */
        
        USART9_SendData_P = NULL;
      }
    }
  }
#endif /* USART9_BufferSize_Tx && USART9_DMA_BufferSize_Tx */
  
#if USART9_BufferSize_Rx && USART9_DMA_BufferSize_Rx
  if(USART_GetITStatus(UART9, USART_IT_IDLE) != RESET)  /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(UART9); /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART9_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream7); /* Returns the number of remaining data units in the current DMA2 Stream7 transfer. */
    
    if(number > (USART9_BufferSize_Rx - USART9_ReceiveData_P))
    {
      if(number < USART9_BufferSize_Rx)
      {
        memmove(USART9_Buffer_Rx, USART9_Buffer_Rx + (number - (USART9_BufferSize_Rx - USART9_ReceiveData_P)), USART9_BufferSize_Rx - number);  /* Copy memory */
        memcpy(USART9_Buffer_Rx + (USART9_BufferSize_Rx - number), USART9_DMA_Buffer_Rx, number);                                               /* Copy memory */
      }
      else
      {
        memcpy(USART9_Buffer_Rx, USART9_DMA_Buffer_Rx + (number - USART9_BufferSize_Rx), USART9_BufferSize_Rx); /* Copy memory */
      }
      
      USART9_ReceiveData_P = USART9_BufferSize_Rx;
    }
    else
    {
      memcpy(USART9_Buffer_Rx + USART9_ReceiveData_P, USART9_DMA_Buffer_Rx, number); /* Copy memory */
      
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

/****************************************************************
 * Function:    UART10_IRQHandler
 * Description: UART10 IRQ handler.
 * Input:
 * Output:
 * Return:
*****************************************************************/
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
        memcpy(USART10_DMA_Buffer_Tx, USART10_Buffer_Tx, USART10_DMA_BufferSize_Tx);                                               /* Copy memory */
        memmove(USART10_Buffer_Tx, USART10_Buffer_Tx + USART10_DMA_BufferSize_Tx, USART10_SendData_P - USART10_DMA_BufferSize_Tx); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);                      /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                                   /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);                 /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART10_DMA_BufferSize_Tx);  /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                                    /* Enables the specified DMA2 Stream7. */
        
        USART10_SendData_P -= USART10_DMA_BufferSize_Tx;
      }
      else
      {
        memcpy(USART10_DMA_Buffer_Tx, USART10_Buffer_Tx, USART10_SendData_P); /* Copy memory */
        
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);               /* Clears the DMA2 Stream7's pending flags. */
        DMA_Cmd(DMA2_Stream7, DISABLE);                            /* Disables the specified DMA2 Stream7. */
        while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);          /* Wait for the specified DMA2 Stream7 disable to finish. */
        DMA_SetCurrDataCounter(DMA2_Stream7, USART10_SendData_P);  /* Writes the number of data units to be transferred on the DMA2 Stream7. */
        DMA_Cmd(DMA2_Stream7, ENABLE);                             /* Enables the specified DMA2 Stream7. */
        
        USART10_SendData_P = NULL;
      }
    }
  }
#endif /* USART10_BufferSize_Tx && USART10_DMA_BufferSize_Tx */
  
#if USART10_BufferSize_Rx && USART10_DMA_BufferSize_Rx
  if(USART_GetITStatus(UART10, USART_IT_IDLE) != RESET) /* Checks whether the specified USART interrupt has occurred or not. */
  {
    USART_ReceiveData(UART10);  /* Returns the most recent received data by the USARTx peripheral. */
    
    uint32_t number = USART10_DMA_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream0); /* Returns the number of remaining data units in the current DMA2 Stream0 transfer. */
    
    if(number > (USART10_BufferSize_Rx - USART10_ReceiveData_P))
    {
      if(number < USART10_BufferSize_Rx)
      {
        memmove(USART10_Buffer_Rx, USART10_Buffer_Rx + (number - (USART10_BufferSize_Rx - USART10_ReceiveData_P)), USART10_BufferSize_Rx - number);  /* Copy memory */
        memcpy(USART10_Buffer_Rx + (USART10_BufferSize_Rx - number), USART10_DMA_Buffer_Rx, number);                                                 /* Copy memory */
      }
      else
      {
        memcpy(USART10_Buffer_Rx, USART10_DMA_Buffer_Rx + (number - USART10_BufferSize_Rx), USART10_BufferSize_Rx); /* Copy memory */
      }
      
      USART10_ReceiveData_P = USART10_BufferSize_Rx;
    }
    else
    {
      memcpy(USART10_Buffer_Rx + USART10_ReceiveData_P, USART10_DMA_Buffer_Rx, number); /* Copy memory */
      
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

/****************************************************************
 * Function:    USART_SendDataFinishCallback
 * Description: USART send data finish callback.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
__weak void USART_SendDataFinishCallback(USART_TypeDef* USARTx)
{
  
}

/****************************************************************
 * Function:    USART_ReceiveDataFinishCallback
 * Description: USART receive data finish callback.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
__weak void USART_ReceiveDataFinishCallback(USART_TypeDef* USARTx)
{
  
}
