/****************************************************************
 * Copyright (C) 2016, XinLi, all right reserved.
 * File name:    USART.c
 * Date:         2016.03.22
 * Description:  USART Driver
*****************************************************************/

/****************************************************************
 *                        Header include
*****************************************************************/
#include "USART.h"
#include <string.h>
#include <stdlib.h>

/****************************************************************
 *                       Global variables
*****************************************************************/
static uint16_t USART1_BufferSize_Tx = NULL;
static uint16_t USART1_BufferSize_Rx = NULL;
static uint8_t* USART1_Buffer_Tx = NULL;
static uint8_t* USART1_Buffer_Rx = NULL;

static uint16_t USART2_BufferSize_Tx = NULL;
static uint16_t USART2_BufferSize_Rx = NULL;
static uint8_t* USART2_Buffer_Tx = NULL;
static uint8_t* USART2_Buffer_Rx = NULL;

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
static uint16_t USART3_BufferSize_Tx = NULL;
static uint16_t USART3_BufferSize_Rx = NULL;
static uint8_t* USART3_Buffer_Tx = NULL;
static uint8_t* USART3_Buffer_Rx = NULL;
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
static uint16_t USART4_BufferSize_Tx = NULL;
static uint16_t USART4_BufferSize_Rx = NULL;
static uint8_t* USART4_Buffer_Tx = NULL;
static uint8_t* USART4_Buffer_Rx = NULL;

static uint16_t USART5_BufferSize_Tx = NULL;
static uint16_t USART5_BufferSize_Rx= NULL;
static uint8_t* USART5_Buffer_Tx = NULL;
static uint8_t* USART5_Buffer_Rx = NULL;
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */

static uint16_t USART6_BufferSize_Tx = NULL;
static uint16_t USART6_BufferSize_Rx = NULL;
static uint8_t* USART6_Buffer_Tx = NULL;
static uint8_t* USART6_Buffer_Rx = NULL;

#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
static uint16_t USART7_BufferSize_Tx = NULL;
static uint16_t USART7_BufferSize_Rx = NULL;
static uint8_t* USART7_Buffer_Tx = NULL;
static uint8_t* USART7_Buffer_Rx = NULL;

static uint16_t USART8_BufferSize_Tx = NULL;
static uint16_t USART8_BufferSize_Rx = NULL;
static uint8_t* USART8_Buffer_Tx = NULL;
static uint8_t* USART8_Buffer_Rx = NULL;
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */

#if defined(STM32F413_423xx)
static uint16_t USART9_BufferSize_Tx = NULL;
static uint16_t USART9_BufferSize_Rx = NULL;
static uint8_t* USART9_Buffer_Tx = NULL;
static uint8_t* USART9_Buffer_Rx = NULL;

static uint16_t USART10_BufferSize_Tx = NULL;
static uint16_t USART10_BufferSize_Rx = NULL;
static uint8_t* USART10_Buffer_Tx = NULL;
static uint8_t* USART10_Buffer_Rx = NULL;
#endif /* STM32F413_423xx */

static uint16_t gpioFlag  = NULL; /* Used to record the GPIO initialization flag. */
static uint16_t usartFlag = NULL; /* Used to record the USART initialization flag. */
static uint16_t dmaTxFlag = NULL; /* Used to record the DMA TX initialization flag. */
static uint16_t dmaRxFlag = NULL; /* Used to record the DMA RX initialization flag. */
static uint16_t firstFlag = NULL; /* Used to record the USART first send data flag. */

/****************************************************************
 *                     Function declaration
*****************************************************************/
static void USART_DMA_TX_Configuration(USART_TypeDef* USARTx);
static void USART_DMA_RX_Configuration(USART_TypeDef* USARTx);
static void USART_DMA_TX_Unconfigure(USART_TypeDef* USARTx);
static void USART_DMA_RX_Unconfigure(USART_TypeDef* USARTx);

/****************************************************************
 *                     Function definition
*****************************************************************/

/****************************************************************
 * Function:    USART_Configuration
 * Description: USART Configuration.
 * Input:       USARTx
 *              BaudRate
 *              USART_IT
 *              BufferSize_Tx
 *              BufferSize_Rx
 * Output:
 * Return:
*****************************************************************/
void USART_Configuration(USART_TypeDef* USARTx, uint32_t BaudRate, uint16_t USART_IT, uint16_t BufferSize_Tx, uint16_t BufferSize_Rx)
{
  uint16_t USART_Mode = NULL;
  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  
  if( (BufferSize_Tx != NULL) && (BufferSize_Rx == NULL) )
  {
    USART_Mode = USART_Mode_Tx;
  }
  else if( (BufferSize_Tx == NULL) && (BufferSize_Rx != NULL) )
  {
    USART_Mode = USART_Mode_Rx;
  }
  else if( (BufferSize_Tx != NULL) && (BufferSize_Rx != NULL) )
  {
    USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  }
  else
  {
    USART_Unconfigure(USARTx);  /* Unconfigure USARTx */
    
    return;
  }
  
  if(USARTx == USART1)
  {
    firstFlag &= 0xFFFE;
    
    if(USART1_Buffer_Tx != NULL)
    {
      free(USART1_Buffer_Tx); /* Release memory */
      USART1_Buffer_Tx = NULL;
      USART1_BufferSize_Tx = NULL;
    }
    
    if(USART1_Buffer_Rx != NULL)
    {
      free(USART1_Buffer_Rx); /* Release memory */
      USART1_Buffer_Rx = NULL;
      USART1_BufferSize_Rx = NULL;
    }
    
    USART1_BufferSize_Tx = BufferSize_Tx;
    USART1_BufferSize_Rx = BufferSize_Rx;
    
    if(USART1_BufferSize_Tx != NULL)
    {
      USART1_Buffer_Tx = malloc(USART1_BufferSize_Tx);      /* Apply for memory */
      memset(USART1_Buffer_Tx, NULL, USART1_BufferSize_Tx); /* Empty memory */
    }
    
    if(USART1_BufferSize_Rx != NULL)
    {
      USART1_Buffer_Rx = malloc(USART1_BufferSize_Rx);      /* Apply for memory */
      memset(USART1_Buffer_Rx, NULL, USART1_BufferSize_Rx); /* Empty memory */
    }
    
    if( (usartFlag & 0x0001) != 0x0001 )
    {
      usartFlag |= 0x0001;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  /* Enable USART1 clock */
    }
    
    if( (gpioFlag & 0x0001) != 0x0001 )
    {
      gpioFlag |= 0x0001;
      
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART1_Tx, ENABLE);  /* Enable USART1_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART1_Rx, ENABLE);  /* Enable USART1_Rx GPIO clock */
      
      GPIO_PinAFConfig(GPIO_USART1_Tx, GPIO_PinSource_USART1_Tx, GPIO_AF_USART1); /* Connect GPIOxn to USART1_Tx */
      GPIO_PinAFConfig(GPIO_USART1_Rx, GPIO_PinSource_USART1_Rx, GPIO_AF_USART1); /* Connect GPIOxn to USART1_Rx */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART1_Tx;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(GPIO_USART1_Tx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART1_Rx;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(GPIO_USART1_Rx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
  }
  else if(USARTx == USART2)
  {
    firstFlag &= 0xFFFD;
    
    if(USART2_Buffer_Tx != NULL)
    {
      free(USART2_Buffer_Tx); /* Release memory */
      USART2_Buffer_Tx = NULL;
      USART2_BufferSize_Tx = NULL;
    }
    
    if(USART2_Buffer_Rx != NULL)
    {
      free(USART2_Buffer_Rx); /* Release memory */
      USART2_Buffer_Rx = NULL;
      USART2_BufferSize_Rx = NULL;
    }
    
    USART2_BufferSize_Tx = BufferSize_Tx;
    USART2_BufferSize_Rx = BufferSize_Rx;
    
    if(USART2_BufferSize_Tx != NULL)
    {
      USART2_Buffer_Tx = malloc(USART2_BufferSize_Tx);      /* Apply for memory */
      memset(USART2_Buffer_Tx, NULL, USART2_BufferSize_Tx); /* Empty memory */
    }
    
    if(USART2_BufferSize_Rx != NULL)
    {
      USART2_Buffer_Rx = malloc(USART2_BufferSize_Rx);      /* Apply for memory */
      memset(USART2_Buffer_Rx, NULL, USART2_BufferSize_Rx); /* Empty memory */
    }
    
    if( (usartFlag & 0x0002) != 0x0002 )
    {
      usartFlag |= 0x0002;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  /* Enable USART2 clock */
    }
    
    if( (gpioFlag & 0x0002) != 0x0002 )
    {
      gpioFlag |= 0x0002;
      
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART2_Tx, ENABLE);  /* Enable USART2_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART2_Rx, ENABLE);  /* Enable USART2_Rx GPIO clock */
      
      GPIO_PinAFConfig(GPIO_USART2_Tx, GPIO_PinSource_USART2_Tx, GPIO_AF_USART2);	/* Connect GPIOxn to USART2_Tx */
      GPIO_PinAFConfig(GPIO_USART2_Rx, GPIO_PinSource_USART2_Rx, GPIO_AF_USART2);	/* Connect GPIOxn to USART2_Rx */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART2_Tx;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(GPIO_USART2_Tx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART2_Rx;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(GPIO_USART2_Rx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
    firstFlag &= 0xFFFB;
    
    if(USART3_Buffer_Tx != NULL)
    {
      free(USART3_Buffer_Tx); /* Release memory */
      USART3_Buffer_Tx = NULL;
      USART3_BufferSize_Tx = NULL;
    }
    
    if(USART3_Buffer_Rx != NULL)
    {
      free(USART3_Buffer_Rx); /* Release memory */
      USART3_Buffer_Rx = NULL;
      USART3_BufferSize_Rx = NULL;
    }
    
    USART3_BufferSize_Tx = BufferSize_Tx;
    USART3_BufferSize_Rx = BufferSize_Rx;
    
    if(USART3_BufferSize_Tx != NULL)
    {
      USART3_Buffer_Tx = malloc(USART3_BufferSize_Tx);      /* Apply for memory */
      memset(USART3_Buffer_Tx, NULL, USART3_BufferSize_Tx); /* Empty memory */
    }
    
    if(USART3_BufferSize_Rx != NULL)
    {
      USART3_Buffer_Rx = malloc(USART3_BufferSize_Rx);      /* Apply for memory */
      memset(USART3_Buffer_Rx, NULL, USART3_BufferSize_Rx); /* Empty memory */
    }
    
    if( (usartFlag & 0x0004) != 0x0004 )
    {
      usartFlag |= 0x0004;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  /* Enable USART3 clock */
    }
    
    if( (gpioFlag & 0x0004) != 0x0004 )
    {
      gpioFlag |= 0x0004;
      
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART3_Tx, ENABLE);  /* Enable USART3_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART3_Rx, ENABLE);  /* Enable USART3_Rx GPIO clock */
      
      GPIO_PinAFConfig(GPIO_USART3_Tx, GPIO_PinSource_USART3_Tx, GPIO_AF_USART3); /* Connect GPIOxn to USART3_Tx */
      GPIO_PinAFConfig(GPIO_USART3_Rx, GPIO_PinSource_USART3_Rx, GPIO_AF_USART3); /* Connect GPIOxn to USART3_Rx */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART3_Tx;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(GPIO_USART3_Tx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART3_Rx;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(GPIO_USART3_Rx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
    firstFlag &= 0xFFF7;
    
    if(USART4_Buffer_Tx != NULL)
    {
      free(USART4_Buffer_Tx); /* Release memory */
      USART4_Buffer_Tx = NULL;
      USART4_BufferSize_Tx = NULL;
    }
    
    if(USART4_Buffer_Rx != NULL)
    {
      free(USART4_Buffer_Rx); /* Release memory */
      USART4_Buffer_Rx = NULL;
      USART4_BufferSize_Rx = NULL;
    }
    
    USART4_BufferSize_Tx = BufferSize_Tx;
    USART4_BufferSize_Rx = BufferSize_Rx;
    
    if(USART4_BufferSize_Tx != NULL)
    {
      USART4_Buffer_Tx = malloc(USART4_BufferSize_Tx);      /* Apply for memory */
      memset(USART4_Buffer_Tx, NULL, USART4_BufferSize_Tx); /* Empty memory */
    }
    
    if(USART4_BufferSize_Rx != NULL)
    {
      USART4_Buffer_Rx = malloc(USART4_BufferSize_Rx);      /* Apply for memory */
      memset(USART4_Buffer_Rx, NULL, USART4_BufferSize_Rx); /* Empty memory */
    }
    
    if( (usartFlag & 0x0008) != 0x0008 )
    {
      usartFlag |= 0x0008;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); /* Enable USART4 clock */
    }
    
    if( (gpioFlag & 0x0008) != 0x0008 )
    {
      gpioFlag |= 0x0008;
      
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART4_Tx, ENABLE);  /* Enable USART4_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART4_Rx, ENABLE);  /* Enable USART4_Rx GPIO clock */
      
      GPIO_PinAFConfig(GPIO_USART4_Tx, GPIO_PinSource_USART4_Tx, GPIO_AF_UART4);  /* Connect GPIOxn to USART4_Tx */
      GPIO_PinAFConfig(GPIO_USART4_Rx, GPIO_PinSource_USART4_Rx, GPIO_AF_UART4);  /* Connect GPIOxn to USART4_Rx */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART4_Tx;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(GPIO_USART4_Tx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART4_Rx;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(GPIO_USART4_Rx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
  }
  else if(USARTx == UART5)
  {
    firstFlag &= 0xFFEF;
    
    if(USART5_Buffer_Tx != NULL)
    {
      free(USART5_Buffer_Tx); /* Release memory */
      USART5_Buffer_Tx = NULL;
      USART5_BufferSize_Tx = NULL;
    }
    
    if(USART5_Buffer_Rx != NULL)
    {
      free(USART5_Buffer_Rx); /* Release memory */
      USART5_Buffer_Rx = NULL;
      USART5_BufferSize_Rx = NULL;
    }
    
    USART5_BufferSize_Tx = BufferSize_Tx;
    USART5_BufferSize_Rx = BufferSize_Rx;
    
    if(USART5_BufferSize_Tx != NULL)
    {
      USART5_Buffer_Tx = malloc(USART5_BufferSize_Tx);      /* Apply for memory */
      memset(USART5_Buffer_Tx, NULL, USART5_BufferSize_Tx); /* Empty memory */
    }
    
    if(USART5_BufferSize_Rx != NULL)
    {
      USART5_Buffer_Rx = malloc(USART5_BufferSize_Rx);      /* Apply for memory */
      memset(USART5_Buffer_Rx, NULL, USART5_BufferSize_Rx); /* Empty memory */
    }
    
    if( (usartFlag & 0x0010) != 0x0010 )
    {
      usartFlag |= 0x0010;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); /* Enable USART5 clock */
    }
    
    if( (gpioFlag & 0x0010) != 0x0010 )
    {
      gpioFlag |= 0x0010;
      
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART5_Tx, ENABLE);  /* Enable USART5_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART5_Rx, ENABLE);  /* Enable USART5_Rx GPIO clock */
      
      GPIO_PinAFConfig(GPIO_USART5_Tx, GPIO_PinSource_USART5_Tx, GPIO_AF_UART5);  /* Connect GPIOxn to USART5_Tx */
      GPIO_PinAFConfig(GPIO_USART5_Rx, GPIO_PinSource_USART5_Rx, GPIO_AF_UART5);  /* Connect GPIOxn to USART5_Rx */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART5_Tx;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(GPIO_USART5_Tx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART5_Rx;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(GPIO_USART5_Rx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
    firstFlag &= 0xFFDF;
    
    if(USART6_Buffer_Tx != NULL)
    {
      free(USART6_Buffer_Tx); /* Release memory */
      USART6_Buffer_Tx = NULL;
      USART6_BufferSize_Tx = NULL;
    }
    
    if(USART6_Buffer_Rx != NULL)
    {
      free(USART6_Buffer_Rx); /* Release memory */
      USART6_Buffer_Rx = NULL;
      USART6_BufferSize_Rx = NULL;
    }
    
    USART6_BufferSize_Tx = BufferSize_Tx;
    USART6_BufferSize_Rx = BufferSize_Rx;
    
    if(USART6_BufferSize_Tx != NULL)
    {
      USART6_Buffer_Tx = malloc(USART6_BufferSize_Tx);      /* Apply for memory */
      memset(USART6_Buffer_Tx, NULL, USART6_BufferSize_Tx); /* Empty memory */
    }
    
    if(USART6_BufferSize_Rx != NULL)
    {
      USART6_Buffer_Rx = malloc(USART6_BufferSize_Rx);      /* Apply for memory */
      memset(USART6_Buffer_Rx, NULL, USART6_BufferSize_Rx); /* Empty memory */
    }
    
    if( (usartFlag & 0x0020) != 0x0020 )
    {
      usartFlag |= 0x0020;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);  /* Enable USART6 clock */
    }
    
    if( (gpioFlag & 0x0020) != 0x0020 )
    {
      gpioFlag |= 0x0020;
      
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART6_Tx, ENABLE);  /* Enable USART6_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART6_Rx, ENABLE);  /* Enable USART6_Rx GPIO clock */
      
      GPIO_PinAFConfig(GPIO_USART6_Tx, GPIO_PinSource_USART6_Tx, GPIO_AF_USART6);  /* Connect GPIOxn to USART6_Tx */
      GPIO_PinAFConfig(GPIO_USART6_Rx, GPIO_PinSource_USART6_Rx, GPIO_AF_USART6);  /* Connect GPIOxn to USART6_Rx */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART6_Tx;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(GPIO_USART6_Tx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART6_Rx;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(GPIO_USART6_Rx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
    firstFlag &= 0xFFBF;
    
    if(USART7_Buffer_Tx != NULL)
    {
      free(USART7_Buffer_Tx); /* Release memory */
      USART7_Buffer_Tx = NULL;
      USART7_BufferSize_Tx = NULL;
    }
    
    if(USART7_Buffer_Rx != NULL)
    {
      free(USART7_Buffer_Rx); /* Release memory */
      USART7_Buffer_Rx = NULL;
      USART7_BufferSize_Rx = NULL;
    }
    
    USART7_BufferSize_Tx = BufferSize_Tx;
    USART7_BufferSize_Rx = BufferSize_Rx;
    
    if(USART7_BufferSize_Tx != NULL)
    {
      USART7_Buffer_Tx = malloc(USART7_BufferSize_Tx);      /* Apply for memory */
      memset(USART7_Buffer_Tx, NULL, USART7_BufferSize_Tx); /* Empty memory */
    }
    
    if(USART7_BufferSize_Rx != NULL)
    {
      USART7_Buffer_Rx = malloc(USART7_BufferSize_Rx);      /* Apply for memory */
      memset(USART7_Buffer_Rx, NULL, USART7_BufferSize_Rx); /* Empty memory */
    }
    
    if( (usartFlag & 0x0040) != 0x0040 )
    {
      usartFlag |= 0x0040;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE); /* Enable USART7 clock */
    }
    
    if( (gpioFlag & 0x0040) != 0x0040 )
    {
      gpioFlag |= 0x0040;
      
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART7_Tx, ENABLE);  /* Enable USART7_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART7_Rx, ENABLE);  /* Enable USART7_Rx GPIO clock */
      
      GPIO_PinAFConfig(GPIO_USART7_Tx, GPIO_PinSource_USART7_Tx, GPIO_AF_UART7);  /* Connect GPIOxn to USART7_Tx */
      GPIO_PinAFConfig(GPIO_USART7_Rx, GPIO_PinSource_USART7_Rx, GPIO_AF_UART7);  /* Connect GPIOxn to USART7_Rx */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART7_Tx;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(GPIO_USART7_Tx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART7_Rx;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(GPIO_USART7_Rx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
  }
  else if(USARTx == UART8)
  {
    firstFlag &= 0xFF7F;
    
    if(USART8_Buffer_Tx != NULL)
    {
      free(USART8_Buffer_Tx); /* Release memory */
      USART8_Buffer_Tx = NULL;
      USART8_BufferSize_Tx = NULL;
    }
    
    if(USART8_Buffer_Rx != NULL)
    {
      free(USART8_Buffer_Rx); /* Release memory */
      USART8_Buffer_Rx = NULL;
      USART8_BufferSize_Rx = NULL;
    }
    
    USART8_BufferSize_Tx = BufferSize_Tx;
    USART8_BufferSize_Rx = BufferSize_Rx;
    
    if(USART8_BufferSize_Tx != NULL)
    {
      USART8_Buffer_Tx = malloc(USART8_BufferSize_Tx);      /* Apply for memory */
      memset(USART8_Buffer_Tx, NULL, USART8_BufferSize_Tx); /* Empty memory */
    }
    
    if(USART8_BufferSize_Rx != NULL)
    {
      USART8_Buffer_Rx = malloc(USART8_BufferSize_Rx);      /* Apply for memory */
      memset(USART8_Buffer_Rx, NULL, USART8_BufferSize_Rx); /* Empty memory */
    }
    
    if( (usartFlag & 0x0080) != 0x0080 )
    {
      usartFlag |= 0x0080;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE); /* Enable USART8 clock */
    }
    
    if( (gpioFlag & 0x0080) != 0x0080 )
    {
      gpioFlag |= 0x0080;
      
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART8_Tx, ENABLE);  /* Enable USART8_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART8_Rx, ENABLE);  /* Enable USART8_Rx GPIO clock */
      
      GPIO_PinAFConfig(GPIO_USART8_Tx, GPIO_PinSource_USART8_Tx, GPIO_AF_UART8);  /* Connect GPIOxn to USART8_Tx */
      GPIO_PinAFConfig(GPIO_USART8_Rx, GPIO_PinSource_USART8_Rx, GPIO_AF_UART8);  /* Connect GPIOxn to USART8_Rx */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART8_Tx;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(GPIO_USART8_Tx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART8_Rx;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(GPIO_USART8_Rx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
    firstFlag &= 0xFEFF;
    
    if(USART9_Buffer_Tx != NULL)
    {
      free(USART9_Buffer_Tx); /* Release memory */
      USART9_Buffer_Tx = NULL;
      USART9_BufferSize_Tx = NULL;
    }
    
    if(USART9_Buffer_Rx != NULL)
    {
      free(USART9_Buffer_Rx); /* Release memory */
      USART9_Buffer_Rx = NULL;
      USART9_BufferSize_Rx = NULL;
    }
    
    USART9_BufferSize_Tx = BufferSize_Tx;
    USART9_BufferSize_Rx = BufferSize_Rx;
    
    if(USART9_BufferSize_Tx != NULL)
    {
      USART9_Buffer_Tx = malloc(USART9_BufferSize_Tx);      /* Apply for memory */
      memset(USART9_Buffer_Tx, NULL, USART9_BufferSize_Tx); /* Empty memory */
    }
    
    if(USART9_BufferSize_Rx != NULL)
    {
      USART9_Buffer_Rx = malloc(USART9_BufferSize_Rx);      /* Apply for memory */
      memset(USART9_Buffer_Rx, NULL, USART9_BufferSize_Rx); /* Empty memory */
    }
    
    if( (usartFlag & 0x0100) != 0x0100 )
    {
      usartFlag |= 0x0100;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART9, ENABLE); /* Enable USART9 clock */
    }
    
    if( (gpioFlag & 0x0100) != 0x0100 )
    {
      gpioFlag |= 0x0100;
      
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART9_Tx, ENABLE);  /* Enable USART9_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART9_Rx, ENABLE);  /* Enable USART9_Rx GPIO clock */
      
      GPIO_PinAFConfig(GPIO_USART9_Tx, GPIO_PinSource_USART9_Tx, GPIO_AF11_UART9);  /* Connect GPIOxn to USART9_Tx */
      GPIO_PinAFConfig(GPIO_USART9_Rx, GPIO_PinSource_USART9_Rx, GPIO_AF11_UART9);  /* Connect GPIOxn to USART9_Rx */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART9_Tx;   /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(GPIO_USART9_Tx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART9_Rx;   /* Specifies the GPIO pins to be configured. */
      GPIO_Init(GPIO_USART9_Rx, &GPIO_InitStructure);     /* Initializes the GPIOx peripheral. */
    }
  }
  else if(USARTx == UART10)
  {
    firstFlag &= 0xFDFF;
    
    if(USART10_Buffer_Tx != NULL)
    {
      free(USART10_Buffer_Tx);  /* Release memory */
      USART10_Buffer_Tx = NULL;
      USART10_BufferSize_Tx = NULL;
    }
    
    if(USART10_Buffer_Rx != NULL)
    {
      free(USART10_Buffer_Rx);  /* Release memory */
      USART10_Buffer_Rx = NULL;
      USART10_BufferSize_Rx = NULL;
    }
    
    USART10_BufferSize_Tx = BufferSize_Tx;
    USART10_BufferSize_Rx = BufferSize_Rx;
    
    if(USART10_BufferSize_Tx != NULL)
    {
      USART10_Buffer_Tx = malloc(USART10_BufferSize_Tx);      /* Apply for memory */
      memset(USART10_Buffer_Tx, NULL, USART10_BufferSize_Tx); /* Empty memory */
    }
    
    if(USART10_BufferSize_Rx != NULL)
    {
      USART10_Buffer_Rx = malloc(USART10_BufferSize_Rx);      /* Apply for memory */
      memset(USART10_Buffer_Rx, NULL, USART10_BufferSize_Rx); /* Empty memory */
    }
    
    if( (usartFlag & 0x0200) != 0x0200 )
    {
      usartFlag |= 0x0200;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART10, ENABLE);  /* Enable USART10 clock */
    }
    
    if( (gpioFlag & 0x0200) != 0x0200 )
    {
      gpioFlag |= 0x0200;
      
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART10_Tx, ENABLE);  /* Enable USART10_Tx GPIO clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART10_Rx, ENABLE);  /* Enable USART10_Rx GPIO clock */
      
      GPIO_PinAFConfig(GPIO_USART10_Tx, GPIO_PinSource_USART10_Tx, GPIO_AF11_UART10); /* Connect GPIOxn to USART10_Tx */
      GPIO_PinAFConfig(GPIO_USART10_Rx, GPIO_PinSource_USART10_Rx, GPIO_AF11_UART10); /* Connect GPIOxn to USART10_Rx */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART10_Tx;  /* Specifies the GPIO pins to be configured. */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /* Specifies the operating mode for the selected pins. */
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* Specifies the speed for the selected pins. */
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* Specifies the operating output type for the selected pins. */
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* Specifies the operating Pull-up/Pull down for the selected pins. */
      GPIO_Init(GPIO_USART10_Tx, &GPIO_InitStructure);    /* Initializes the GPIOx peripheral. */
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART10_Rx;  /* Specifies the GPIO pins to be configured. */
      GPIO_Init(GPIO_USART10_Rx, &GPIO_InitStructure);    /* Initializes the GPIOx peripheral. */
    }
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
  
  if( (USART_Mode & USART_Mode_Tx) == USART_Mode_Tx )
  {
    USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);   /* Enabling the DMA request specified USARTx */
  }
  if( (USART_Mode & USART_Mode_Rx) == USART_Mode_Rx )
  {
    USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);   /* Enabling the DMA request specified USARTx */
  }
  
  if( (USART_IT & USART_IT_PE) == USART_IT_PE )
  {
    USART_ITConfig(USARTx, USART_IT_PE, ENABLE);     /* Enabling interrupts specified USARTx */
  }
  if( (USART_IT & USART_IT_TXE) == USART_IT_TXE )
  {
    USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);    /* Enabling interrupts specified USARTx */
  }
  if( (USART_IT & USART_IT_TC) == USART_IT_TC )
  {
    USART_ITConfig(USARTx, USART_IT_TC, ENABLE);     /* Enabling interrupts specified USARTx */
  }
  if( (USART_IT & USART_IT_RXNE) == USART_IT_RXNE )
  {
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);   /* Enabling interrupts specified USARTx */
  }
  if( (USART_IT & USART_IT_ORE_RX) == USART_IT_ORE_RX )
  {
    USART_ITConfig(USARTx, USART_IT_ORE_RX, ENABLE); /* Enabling interrupts specified USARTx */
  }
  if( (USART_IT & USART_IT_IDLE) == USART_IT_IDLE )
  {
    USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);   /* Enabling interrupts specified USARTx */
  }
  if( (USART_IT & USART_IT_LBD) == USART_IT_LBD )
  {
    USART_ITConfig(USARTx, USART_IT_LBD, ENABLE);    /* Enabling interrupts specified USARTx */
  }
  if( (USART_IT & USART_IT_CTS) == USART_IT_CTS )
  {
    USART_ITConfig(USARTx, USART_IT_CTS, ENABLE);    /* Enabling interrupts specified USARTx */
  }
  if( (USART_IT & USART_IT_ERR) == USART_IT_ERR )
  {
    USART_ITConfig(USARTx, USART_IT_ERR, ENABLE);    /* Enabling interrupts specified USARTx */
  }
  if( (USART_IT & USART_IT_ORE_ER) == USART_IT_ORE_ER )
  {
    USART_ITConfig(USARTx, USART_IT_ORE_ER, ENABLE); /* Enabling interrupts specified USARTx */
  }
  if( (USART_IT & USART_IT_NE) == USART_IT_NE )
  {
    USART_ITConfig(USARTx, USART_IT_NE, ENABLE);     /* Enabling interrupts specified USARTx */
  }
  if( (USART_IT & USART_IT_FE) == USART_IT_FE )
  {
    USART_ITConfig(USARTx, USART_IT_FE, ENABLE);     /* Enabling interrupts specified USARTx */
  }
  
  USART_Cmd(USARTx, ENABLE);                         /* Enables or disables the specified USARTx peripheral. */
  
  if( USART_Mode == USART_Mode_Tx )
  {
    USART_DMA_TX_Configuration(USARTx); /* Configuration USARTx DMA TX */
    USART_DMA_RX_Unconfigure(USARTx);   /* Unconfigure USARTx DMA RX */
  }
  else if( USART_Mode == USART_Mode_Rx )
  {
    USART_DMA_RX_Configuration(USARTx); /* Configuration USARTx DMA RX */
    USART_DMA_TX_Unconfigure(USARTx);   /* Unconfigure USARTx DMA TX */
  }
  else
  {
    USART_DMA_TX_Configuration(USARTx); /* Configuration USARTx DMA TX */
    USART_DMA_RX_Configuration(USARTx); /* Configuration USARTx DMA RX */
  }
}

/****************************************************************
 * Function:    USART_Unconfigure
 * Description: USART Unconfigure.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
void USART_Unconfigure(USART_TypeDef* USARTx)
{
  if(USARTx == USART1)
  {
    firstFlag &= 0xFFFE;
    
    if(USART1_Buffer_Tx != NULL)
    {
      free(USART1_Buffer_Tx); /* Release memory */
      USART1_Buffer_Tx = NULL;
      USART1_BufferSize_Tx = NULL;
    }
    
    if(USART1_Buffer_Rx != NULL)
    {
      free(USART1_Buffer_Rx); /* Release memory */
      USART1_Buffer_Rx = NULL;
      USART1_BufferSize_Rx = NULL;
    }
    
    if( (usartFlag & 0x0001) == 0x0001 )
    {
      usartFlag &= 0xFFFE;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE); /* Disable USART1 clock */
      
      USART_DeInit(USART1); /* Deinitializes the USART1 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(USART1); /* Unconfigure USART1 DMA TX */
      USART_DMA_RX_Unconfigure(USART1); /* Unconfigure USART1 DMA RX */
    }
  }
  else if(USARTx == USART2)
  {
    firstFlag &= 0xFFFD;
    
    if(USART2_Buffer_Tx != NULL)
    {
      free(USART2_Buffer_Tx); /* Release memory */
      USART2_Buffer_Tx = NULL;
      USART2_BufferSize_Tx = NULL;
    }
    
    if(USART2_Buffer_Rx != NULL)
    {
      free(USART2_Buffer_Rx); /* Release memory */
      USART2_Buffer_Rx = NULL;
      USART2_BufferSize_Rx = NULL;
    }
    
    if( (usartFlag & 0x0002) == 0x0002 )
    {
      usartFlag &= 0xFFFD;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE); /* Disable USART2 clock */
      
      USART_DeInit(USART2); /* Deinitializes the USART2 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(USART2); /* Unconfigure USART2 DMA TX */
      USART_DMA_RX_Unconfigure(USART2); /* Unconfigure USART2 DMA RX */
    }
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
    firstFlag &= 0xFFFB;
    
    if(USART3_Buffer_Tx != NULL)
    {
      free(USART3_Buffer_Tx); /* Release memory */
      USART3_Buffer_Tx = NULL;
      USART3_BufferSize_Tx = NULL;
    }
    
    if(USART3_Buffer_Rx != NULL)
    {
      free(USART3_Buffer_Rx); /* Release memory */
      USART3_Buffer_Rx = NULL;
      USART3_BufferSize_Rx = NULL;
    }
    
    if( (usartFlag & 0x0004) == 0x0004 )
    {
      usartFlag &= 0xFFFB;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, DISABLE); /* Disable USART3 clock */
      
      USART_DeInit(USART3); /* Deinitializes the USART3 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(USART3); /* Unconfigure USART3 DMA TX */
      USART_DMA_RX_Unconfigure(USART3); /* Unconfigure USART3 DMA RX */
    }
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
    firstFlag &= 0xFFF7;
    
    if(USART4_Buffer_Tx != NULL)
    {
      free(USART4_Buffer_Tx); /* Release memory */
      USART4_Buffer_Tx = NULL;
      USART4_BufferSize_Tx = NULL;
    }
    
    if(USART4_Buffer_Rx != NULL)
    {
      free(USART4_Buffer_Rx); /* Release memory */
      USART4_Buffer_Rx = NULL;
      USART4_BufferSize_Rx = NULL;
    }
    
    if( (usartFlag & 0x0008) == 0x0008 )
    {
      usartFlag &= 0xFFF7;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, DISABLE);  /* Disable USART4 clock */
      
      USART_DeInit(UART4); /* Deinitializes the USART4 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART4); /* Unconfigure USART4 DMA TX */
      USART_DMA_RX_Unconfigure(UART4); /* Unconfigure USART4 DMA RX */
    }
  }
  else if(USARTx == UART5)
  {
    firstFlag &= 0xFFEF;
    
    if(USART5_Buffer_Tx != NULL)
    {
      free(USART5_Buffer_Tx); /* Release memory */
      USART5_Buffer_Tx = NULL;
      USART5_BufferSize_Tx = NULL;
    }
    
    if(USART5_Buffer_Rx != NULL)
    {
      free(USART5_Buffer_Rx); /* Release memory */
      USART5_Buffer_Rx = NULL;
      USART5_BufferSize_Rx = NULL;
    }
    
    if( (usartFlag & 0x0010) == 0x0010 )
    {
      usartFlag &= 0xFFEF;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, DISABLE);  /* Disable USART5 clock */
      
      USART_DeInit(UART5); /* Deinitializes the USART5 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART5); /* Unconfigure USART5 DMA TX */
      USART_DMA_RX_Unconfigure(UART5); /* Unconfigure USART5 DMA RX */
    }
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
    firstFlag &= 0xFFDF;
    
    if(USART6_Buffer_Tx != NULL)
    {
      free(USART6_Buffer_Tx); /* Release memory */
      USART6_Buffer_Tx = NULL;
      USART6_BufferSize_Tx = NULL;
    }
    
    if(USART6_Buffer_Rx != NULL)
    {
      free(USART6_Buffer_Rx); /* Release memory */
      USART6_Buffer_Rx = NULL;
      USART6_BufferSize_Rx = NULL;
    }
    
    if( (usartFlag & 0x0020) == 0x0020 )
    {
      usartFlag &= 0xFFDF;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, DISABLE); /* Disable USART6 clock */
      
      USART_DeInit(USART6); /* Deinitializes the USART6 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(USART6); /* Unconfigure USART6 DMA TX */
      USART_DMA_RX_Unconfigure(USART6); /* Unconfigure USART6 DMA RX */
    }
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
    firstFlag &= 0xFFBF;
    
    if(USART7_Buffer_Tx != NULL)
    {
      free(USART7_Buffer_Tx); /* Release memory */
      USART7_Buffer_Tx = NULL;
      USART7_BufferSize_Tx = NULL;
    }
    
    if(USART7_Buffer_Rx != NULL)
    {
      free(USART7_Buffer_Rx); /* Release memory */
      USART7_Buffer_Rx = NULL;
      USART7_BufferSize_Rx = NULL;
    }
    
    if( (usartFlag & 0x0040) == 0x0040 )
    {
      usartFlag &= 0xFFBF;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, DISABLE);  /* Disable USART7 clock */
      
      USART_DeInit(UART7); /* Deinitializes the USART7 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART7); /* Unconfigure USART7 DMA TX */
      USART_DMA_RX_Unconfigure(UART7); /* Unconfigure USART7 DMA RX */
    }
  }
  else if(USARTx == UART8)
  {
    firstFlag &= 0xFF7F;
    
    if(USART8_Buffer_Tx != NULL)
    {
      free(USART8_Buffer_Tx); /* Release memory */
      USART8_Buffer_Tx = NULL;
      USART8_BufferSize_Tx = NULL;
    }
    
    if(USART8_Buffer_Rx != NULL)
    {
      free(USART8_Buffer_Rx); /* Release memory */
      USART8_Buffer_Rx = NULL;
      USART8_BufferSize_Rx = NULL;
    }
    
    if( (usartFlag & 0x0080) == 0x0080 )
    {
      usartFlag &= 0xFF7F;
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, DISABLE);  /* Disable USART8 clock */
      
      USART_DeInit(UART8); /* Deinitializes the USART8 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART8); /* Unconfigure USART8 DMA TX */
      USART_DMA_RX_Unconfigure(UART8); /* Unconfigure USART8 DMA RX */
    }
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
    firstFlag &= 0xFEFF;
    
    if(USART9_Buffer_Tx != NULL)
    {
      free(USART9_Buffer_Tx); /* Release memory */
      USART9_Buffer_Tx = NULL;
      USART9_BufferSize_Tx = NULL;
    }
    
    if(USART9_Buffer_Rx != NULL)
    {
      free(USART9_Buffer_Rx); /* Release memory */
      USART9_Buffer_Rx = NULL;
      USART9_BufferSize_Rx = NULL;
    }
    
    if( (usartFlag & 0x0100) == 0x0100 )
    {
      usartFlag &= 0xFEFF;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART9, DISABLE);  /* Disable USART9 clock */
      
      USART_DeInit(UART9); /* Deinitializes the USART9 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART9); /* Unconfigure USART9 DMA TX */
      USART_DMA_RX_Unconfigure(UART9); /* Unconfigure USART9 DMA RX */
    }
  }
  else if(USARTx == UART10)
  {
    firstFlag &= 0xFDFF;
    
    if(USART10_Buffer_Tx != NULL)
    {
      free(USART10_Buffer_Tx);  /* Release memory */
      USART10_Buffer_Tx = NULL;
      USART10_BufferSize_Tx = NULL;
    }
    
    if(USART10_Buffer_Rx != NULL)
    {
      free(USART10_Buffer_Rx);  /* Release memory */
      USART10_Buffer_Rx = NULL;
      USART10_BufferSize_Rx = NULL;
    }
    
    if( (usartFlag & 0x0200) == 0x0200 )
    {
      usartFlag &= 0xFDFF;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART10, DISABLE); /* Disable USART10 clock */
      
      USART_DeInit(UART10); /* Deinitializes the USART10 peripheral registers to their default reset values. */
      
      USART_DMA_TX_Unconfigure(UART10); /* Unconfigure USART10 DMA TX */
      USART_DMA_RX_Unconfigure(UART10); /* Unconfigure USART10 DMA RX */
    }
  }
#endif /* STM32F413_423xx */
}

/****************************************************************
 * Function:    USART_DMA_TX_Configuration
 * Description: USART DMA TX Configuration.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
static void USART_DMA_TX_Configuration(USART_TypeDef* USARTx)
{
  DMA_InitTypeDef DMA_InitStructure;

  if( (USARTx == USART2)
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
  else if( (USARTx == USART1) || (USARTx == USART6)
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
    dmaTxFlag |= 0x0001;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;   /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART1_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART1_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA2_Stream7);                           /* Deinitialize the DMA2 Stream7 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA2_Stream7) != DISABLE ); /* Wait for the specified DMA2 Stream7 reset to complete. */
    
    DMA_Init(DMA2_Stream7, &DMA_InitStructure); /* Initializes the DMA2 Stream7. */
    DMA_Cmd(DMA2_Stream7, DISABLE);             /* Disables the specified DMA2 Stream7. */
  }
  else if(USARTx == USART2)
  {
    dmaTxFlag |= 0x0002;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;   /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART2_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART2_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA1_Stream6);                           /* Deinitialize the DMA1 Stream6 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA1_Stream6) != DISABLE ); /* Wait for the specified DMA1 Stream6 reset to complete. */
    
    DMA_Init(DMA1_Stream6, &DMA_InitStructure); /* Initializes the DMA1 Stream6. */
    DMA_Cmd(DMA1_Stream6, DISABLE);             /* Disables the specified DMA1 Stream6. */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
    dmaTxFlag |= 0x0004;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;   /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART3_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART3_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA1_Stream3);                           /* Deinitialize the DMA1 Stream3 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA1_Stream3) != DISABLE ); /* Wait for the specified DMA1 Stream3 reset to complete. */
    
    DMA_Init(DMA1_Stream3, &DMA_InitStructure); /* Initializes the DMA1 Stream3. */
    DMA_Cmd(DMA1_Stream3, DISABLE);             /* Disables the specified DMA1 Stream3. */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
    dmaTxFlag |= 0x0008;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR;    /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART4_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART4_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA1_Stream4);                           /* Deinitialize the DMA1 Stream4 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA1_Stream4) != DISABLE ); /* Wait for the specified DMA1 Stream4 reset to complete. */
    
    DMA_Init(DMA1_Stream4, &DMA_InitStructure); /* Initializes the DMA1 Stream4. */
    DMA_Cmd(DMA1_Stream4, DISABLE);             /* Disables the specified DMA1 Stream4. */
  }
  else if(USARTx == UART5)
  {
    dmaTxFlag |= 0x0010;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART5->DR;    /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART5_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART5_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA1_Stream7);                           /* Deinitialize the DMA1 Stream7 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA1_Stream7) != DISABLE ); /* Wait for the specified DMA1 Stream7 reset to complete. */
    
    DMA_Init(DMA1_Stream7, &DMA_InitStructure); /* Initializes the DMA1 Stream7. */
    DMA_Cmd(DMA1_Stream7, DISABLE);             /* Disables the specified DMA1 Stream7. */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
    dmaTxFlag |= 0x0020;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;   /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART6_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART6_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA2_Stream6);                           /* Deinitialize the DMA2 Stream6 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA2_Stream6) != DISABLE ); /* Wait for the specified DMA2 Stream6 reset to complete. */
    
    DMA_Init(DMA2_Stream6, &DMA_InitStructure); /* Initializes the DMA2 Stream6. */
    DMA_Cmd(DMA2_Stream6, DISABLE);             /* Disables the specified DMA2 Stream6. */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
    dmaTxFlag |= 0x0040;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART7->DR;    /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART7_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART7_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA1_Stream1);                           /* Deinitialize the DMA1 Stream1 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA1_Stream1) != DISABLE ); /* Wait for the specified DMA1 Stream1 reset to complete. */
    
    DMA_Init(DMA1_Stream1, &DMA_InitStructure); /* Initializes the DMA1 Stream1. */
    DMA_Cmd(DMA1_Stream1, DISABLE);             /* Disables the specified DMA1 Stream1. */
  }
  else if(USARTx == UART8)
  {
    dmaTxFlag |= 0x0080;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART8->DR;    /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART8_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART8_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA1_Stream0);                           /* Deinitialize the DMA1 Stream0 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA1_Stream0) != DISABLE ); /* Wait for the specified DMA1 Stream0 reset to complete. */
    
    DMA_Init(DMA1_Stream0, &DMA_InitStructure); /* Initializes the DMA1 Stream0. */
    DMA_Cmd(DMA1_Stream0, DISABLE);             /* Disables the specified DMA1 Stream0. */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
    dmaTxFlag |= 0x0100;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_1;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART9->DR;    /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART9_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART9_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA2_Stream0);                           /* Deinitialize the DMA2 Stream0 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA2_Stream0) != DISABLE ); /* Wait for the specified DMA2 Stream0 reset to complete. */
    
    DMA_Init(DMA2_Stream0, &DMA_InitStructure); /* Initializes the DMA2 Stream0. */
    DMA_Cmd(DMA2_Stream0, DISABLE);             /* Disables the specified DMA2 Stream0. */
  }
  else if(USARTx == UART10)
  {
    dmaTxFlag |= 0x0200;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_6;                       /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART10->DR;    /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART10_Buffer_Tx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART10_BufferSize_Tx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA2_Stream7);                           /* Deinitialize the DMA2 Stream7 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA2_Stream7) != DISABLE ); /* Wait for the specified DMA2 Stream7 reset to complete. */
    
    DMA_Init(DMA2_Stream7, &DMA_InitStructure); /* Initializes the DMA2 Stream7. */
    DMA_Cmd(DMA2_Stream7, DISABLE);             /* Disables the specified DMA2 Stream7. */
  }
#endif /* STM32F413_423xx */
}

/****************************************************************
 * Function:    USART_DMA_RX_Configuration
 * Description: USART DMA RX Configuration.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
static void USART_DMA_RX_Configuration(USART_TypeDef* USARTx)
{
  DMA_InitTypeDef DMA_InitStructure;

  if( (USARTx == USART2)
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
  else if( (USARTx == USART1) || (USARTx == USART6)
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
    dmaRxFlag |= 0x0001;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;   /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART1_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART1_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA2_Stream2);                           /* Deinitialize the DMA2 Stream2 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA2_Stream2) != DISABLE ); /* Wait for the specified DMA2 Stream2 reset to complete. */
    
    DMA_Init(DMA2_Stream2, &DMA_InitStructure); /* Initializes the DMA2 Stream2. */
    DMA_Cmd(DMA2_Stream2, ENABLE);              /* Enables the specified DMA2 Stream2. */
  }
  else if(USARTx == USART2)
  {
    dmaRxFlag |= 0x0002;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;   /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART2_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART2_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA1_Stream5);                           /* Deinitialize the DMA1 Stream5 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA1_Stream5) != DISABLE ); /* Wait for the specified DMA1 Stream5 reset to complete. */
    
    DMA_Init(DMA1_Stream5, &DMA_InitStructure); /* Initializes the DMA1 Stream5. */
    DMA_Cmd(DMA1_Stream5, ENABLE);              /* Enables the specified DMA1 Stream5. */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
    dmaRxFlag |= 0x0004;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;   /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART3_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART3_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA1_Stream1);                           /* Deinitialize the DMA1 Stream1 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA1_Stream1) != DISABLE ); /* Wait for the specified DMA1 Stream1 reset to complete. */
    
    DMA_Init(DMA1_Stream1, &DMA_InitStructure); /* Initializes the DMA1 Stream1. */
    DMA_Cmd(DMA1_Stream1, ENABLE);              /* Enables the specified DMA1 Stream1. */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
    dmaRxFlag |= 0x0008;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR;    /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART4_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART4_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA1_Stream2);                           /* Deinitialize the DMA1 Stream2 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA1_Stream2) != DISABLE ); /* Wait for the specified DMA1 Stream2 reset to complete. */
    
    DMA_Init(DMA1_Stream2, &DMA_InitStructure); /* Initializes the DMA1 Stream2. */
    DMA_Cmd(DMA1_Stream2, ENABLE);              /* Enables the specified DMA1 Stream2. */
  }
  else if(USARTx == UART5)
  {
    dmaRxFlag |= 0x0010;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART5->DR;    /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART5_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART5_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA1_Stream0);                           /* Deinitialize the DMA1 Stream0 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA1_Stream0) != DISABLE ); /* Wait for the specified DMA1 Stream0 reset to complete. */
    
    DMA_Init(DMA1_Stream0, &DMA_InitStructure); /* Initializes the DMA1 Stream0. */
    DMA_Cmd(DMA1_Stream0, ENABLE);              /* Enables the specified DMA1 Stream0. */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
    dmaRxFlag |= 0x0020;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;   /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART6_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART6_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA2_Stream1);                           /* Deinitialize the DMA2 Stream1 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA2_Stream1) != DISABLE ); /* Wait for the specified DMA2 Stream1 reset to complete. */
    
    DMA_Init(DMA2_Stream1, &DMA_InitStructure); /* Initializes the DMA2 Stream1. */
    DMA_Cmd(DMA2_Stream1, ENABLE);              /* Enables the specified DMA2 Stream1. */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
    dmaRxFlag |= 0x0040;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART7->DR;    /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART7_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART7_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA1_Stream3);                           /* Deinitialize the DMA1 Stream3 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA1_Stream3) != DISABLE ); /* Wait for the specified DMA1 Stream3 reset to complete. */
    
    DMA_Init(DMA1_Stream3, &DMA_InitStructure); /* Initializes the DMA1 Stream3. */
    DMA_Cmd(DMA1_Stream3, ENABLE);              /* Enables the specified DMA1 Stream3. */
  }
  else if(USARTx == UART8)
  {
    dmaRxFlag |= 0x0080;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART8->DR;    /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART8_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART8_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA1_Stream6);                           /* Deinitialize the DMA1 Stream6 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA1_Stream6) != DISABLE ); /* Wait for the specified DMA1 Stream6 reset to complete. */
    
    DMA_Init(DMA1_Stream6, &DMA_InitStructure); /* Initializes the DMA1 Stream6. */
    DMA_Cmd(DMA1_Stream6, ENABLE);              /* Enables the specified DMA1 Stream6. */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
    dmaRxFlag |= 0x0100;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;                      /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART9->DR;    /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART9_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART9_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA2_Stream7);                           /* Deinitialize the DMA2 Stream7 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA2_Stream7) != DISABLE ); /* Wait for the specified DMA2 Stream7 reset to complete. */
    
    DMA_Init(DMA2_Stream7, &DMA_InitStructure); /* Initializes the DMA2 Stream7. */
    DMA_Cmd(DMA2_Stream7, ENABLE);              /* Enables the specified DMA2 Stream7. */
  }
  else if(USARTx == UART10)
  {
    dmaRxFlag |= 0x0200;
    
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                       /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART10->DR;    /* Set the DMA source: peripheral data register address */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART10_Buffer_Rx; /* Memory address */
    DMA_InitStructure.DMA_BufferSize = USART10_BufferSize_Rx;            /* Specifies the DMA channel DMA buffer size */
    
    DMA_DeInit(DMA2_Stream0);                           /* Deinitialize the DMA2 Stream0 registers to their default reset values. */
    while( DMA_GetCmdStatus(DMA2_Stream0) != DISABLE ); /* Wait for the specified DMA2 Stream0 reset to complete. */
    
    DMA_Init(DMA2_Stream0, &DMA_InitStructure); /* Initializes the DMA2 Stream0. */
    DMA_Cmd(DMA2_Stream0, ENABLE);              /* Enables the specified DMA2 Stream0. */
  }
#endif /* STM32F413_423xx */
}

/****************************************************************
 * Function:    USART_DMA_TX_Unconfigure
 * Description: USART DMA TX Unconfigure.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
static void USART_DMA_TX_Unconfigure(USART_TypeDef* USARTx)
{
  if(USARTx == USART1)
  {
    if( (dmaTxFlag & 0x0001) == 0x0001 )
    {
      dmaTxFlag &= 0xFFFE;
      
      DMA_DeInit(DMA2_Stream7);                           /* Deinitialize the DMA2 Stream7 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA2_Stream7) != DISABLE ); /* Wait for the specified DMA2 Stream7 reset to complete. */
    }
  }
  else if(USARTx == USART2)
  {
    if( (dmaTxFlag & 0x0002) == 0x0002 )
    {
      dmaTxFlag &= 0xFFFD;
      
      DMA_DeInit(DMA1_Stream6);                           /* Deinitialize the DMA1 Stream6 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA1_Stream6) != DISABLE ); /* Wait for the specified DMA1 Stream6 reset to complete. */
    }
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
    if( (dmaTxFlag & 0x0004) == 0x0004 )
    {
      dmaTxFlag &= 0xFFFB;
      
      DMA_DeInit(DMA1_Stream3);                           /* Deinitialize the DMA1 Stream3 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA1_Stream3) != DISABLE ); /* Wait for the specified DMA1 Stream3 reset to complete. */
    }
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
    if( (dmaTxFlag & 0x0008) == 0x0008 )
    {
      dmaTxFlag &= 0xFFF7;
      
      DMA_DeInit(DMA1_Stream4);                           /* Deinitialize the DMA1 Stream4 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA1_Stream4) != DISABLE ); /* Wait for the specified DMA1 Stream4 reset to complete. */
    }
  }
  else if(USARTx == UART5)
  {
    if( (dmaTxFlag & 0x0010) == 0x0010 )
    {
      dmaTxFlag &= 0xFFEF;
      
      DMA_DeInit(DMA1_Stream7);                           /* Deinitialize the DMA1 Stream7 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA1_Stream7) != DISABLE ); /* Wait for the specified DMA1 Stream7 reset to complete. */
    }
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
    if( (dmaTxFlag & 0x0020) == 0x0020 )
    {
      dmaTxFlag &= 0xFFDF;
      
      DMA_DeInit(DMA2_Stream6);                           /* Deinitialize the DMA2 Stream6 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA2_Stream6) != DISABLE ); /* Wait for the specified DMA2 Stream6 reset to complete. */
    }
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
    if( (dmaTxFlag & 0x0040) == 0x0040 )
    {
      dmaTxFlag &= 0xFFBF;
      
      DMA_DeInit(DMA1_Stream1);                           /* Deinitialize the DMA1 Stream1 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA1_Stream1) != DISABLE ); /* Wait for the specified DMA1 Stream1 reset to complete. */
    }
  }
  else if(USARTx == UART8)
  {
    if( (dmaTxFlag & 0x0080) == 0x0080 )
    {
      dmaTxFlag &= 0xFF7F;
      
      DMA_DeInit(DMA1_Stream0);                           /* Deinitialize the DMA1 Stream0 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA1_Stream0) != DISABLE ); /* Wait for the specified DMA1 Stream0 reset to complete. */
    }
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
    if( (dmaTxFlag & 0x0100) == 0x0100 )
    {
      dmaTxFlag &= 0xFEFF;
      
      DMA_DeInit(DMA2_Stream0);                           /* Deinitialize the DMA2 Stream0 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA2_Stream0) != DISABLE ); /* Wait for the specified DMA2 Stream0 reset to complete. */
    }
  }
  else if(USARTx == UART10)
  {
    if( (dmaTxFlag & 0x0200) == 0x0200 )
    {
      dmaTxFlag &= 0xFDFF;
      
      DMA_DeInit(DMA2_Stream7);                           /* Deinitialize the DMA2 Stream7 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA2_Stream7) != DISABLE ); /* Wait for the specified DMA2 Stream7 reset to complete. */
    }
  }
#endif /* STM32F413_423xx */
}

/****************************************************************
 * Function:    USART_DMA_RX_Unconfigure
 * Description: USART DMA RX Unconfigure.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
static void USART_DMA_RX_Unconfigure(USART_TypeDef* USARTx)
{
  if(USARTx == USART1)
  {
    if( (dmaRxFlag & 0x0001) == 0x0001 )
    {
      dmaRxFlag &= 0xFFFE;
      
      DMA_DeInit(DMA2_Stream2);                           /* Deinitialize the DMA2 Stream2 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA2_Stream2) != DISABLE ); /* Wait for the specified DMA2 Stream2 reset to complete. */
    }
  }
  else if(USARTx == USART2)
  {
    if( (dmaRxFlag & 0x0002) == 0x0002 )
    {
      dmaRxFlag &= 0xFFFD;
      
      DMA_DeInit(DMA1_Stream5);                           /* Deinitialize the DMA1 Stream5 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA1_Stream5) != DISABLE ); /* Wait for the specified DMA1 Stream5 reset to complete. */
    }
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
    if( (dmaRxFlag & 0x0004) == 0x0004 )
    {
      dmaRxFlag &= 0xFFFB;
      
      DMA_DeInit(DMA1_Stream1);                           /* Deinitialize the DMA1 Stream1 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA1_Stream1) != DISABLE ); /* Wait for the specified DMA1 Stream1 reset to complete. */
    }
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
    if( (dmaRxFlag & 0x0008) == 0x0008 )
    {
      dmaRxFlag &= 0xFFF7;
      
      DMA_DeInit(DMA1_Stream2);                           /* Deinitialize the DMA1 Stream2 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA1_Stream2) != DISABLE ); /* Wait for the specified DMA1 Stream2 reset to complete. */
    }
  }
  else if(USARTx == UART5)
  {
    if( (dmaRxFlag & 0x0010) == 0x0010 )
    {
      dmaRxFlag &= 0xFFEF;
      
      DMA_DeInit(DMA1_Stream0);                           /* Deinitialize the DMA1 Stream0 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA1_Stream0) != DISABLE ); /* Wait for the specified DMA1 Stream0 reset to complete. */
    }
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
    if( (dmaRxFlag & 0x0020) == 0x0020 )
    {
      dmaRxFlag &= 0xFFDF;
      
      DMA_DeInit(DMA2_Stream1);                           /* Deinitialize the DMA2 Stream1 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA2_Stream1) != DISABLE ); /* Wait for the specified DMA2 Stream1 reset to complete. */
    }
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
    if( (dmaRxFlag & 0x0040) == 0x0040 )
    {
      dmaRxFlag &= 0xFFBF;
      
      DMA_DeInit(DMA1_Stream3);                           /* Deinitialize the DMA1 Stream3 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA1_Stream3) != DISABLE ); /* Wait for the specified DMA1 Stream3 reset to complete. */
    }
  }
  else if(USARTx == UART8)
  {
    if( (dmaRxFlag & 0x0080) == 0x0080 )
    {
      dmaRxFlag &= 0xFF7F;
      
      DMA_DeInit(DMA1_Stream6);                           /* Deinitialize the DMA1 Stream6 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA1_Stream6) != DISABLE ); /* Wait for the specified DMA1 Stream6 reset to complete. */
    }
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
    if( (dmaRxFlag & 0x0100) == 0x0100 )
    {
      dmaRxFlag &= 0xFEFF;
      
      DMA_DeInit(DMA2_Stream7);                           /* Deinitialize the DMA2 Stream7 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA2_Stream7) != DISABLE ); /* Wait for the specified DMA2 Stream7 reset to complete. */
    }
  }
  else if(USARTx == UART10)
  {
    if( (dmaRxFlag & 0x0200) == 0x0200 )
    {
      dmaRxFlag &= 0xFDFF;
      
      DMA_DeInit(DMA2_Stream0);                           /* Deinitialize the DMA2 Stream0 registers to their default reset values. */
      while( DMA_GetCmdStatus(DMA2_Stream0) != DISABLE ); /* Wait for the specified DMA2 Stream0 reset to complete. */
    }
  }
#endif /* STM32F413_423xx */
}

/****************************************************************
 * Function:    USART_SetSendData
 * Description: USART Set Send Data.
 * Input:       USARTx
 *              Buffer
 *              BufferSize
 * Output:
 * Return:      Send Data Number
*****************************************************************/
uint16_t USART_SetSendData(USART_TypeDef* USARTx, void* Buffer, uint16_t BufferSize)
{
  if(BufferSize == NULL)
  {
    return NULL;
  }
  
  if(USARTx == USART1)
  {
    if( (dmaTxFlag & 0x0001) != 0x0001 )
    {
      return NULL;
    }
    
    if(BufferSize > USART1_BufferSize_Tx)
    {
      return NULL;
    }
    
    if( (firstFlag & 0x0001) != 0x0001)
    {
      firstFlag |= 0x0001;
    }
    else
    {
      while( DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == RESET );
    }
    
    memcpy(USART1_Buffer_Tx, Buffer, BufferSize);       /* Copy memory */
    
    DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);        /* Clears the DMA2 Stream7's pending flags. */
    DMA_Cmd(DMA2_Stream7, DISABLE);                     /* Disables the specified DMA2 Stream7. */
    while( DMA_GetCmdStatus(DMA2_Stream7) != DISABLE ); /* Wait for the specified DMA2 Stream7 disable to finish. */
    DMA_SetCurrDataCounter(DMA2_Stream7, BufferSize);   /* Writes the number of data units to be transferred on the DMA2 Stream7. */
    DMA_Cmd(DMA2_Stream7, ENABLE);                      /* Enables the specified DMA2 Stream7. */
    
    return BufferSize;
  }
  else if(USARTx == USART2)
  {
    if( (dmaTxFlag & 0x0002) != 0x0002 )
    {
      return NULL;
    }
    
    if(BufferSize > USART2_BufferSize_Tx)
    {
      return NULL;
    }
    
    if( (firstFlag & 0x0002) != 0x0002 )
    {
      firstFlag |= 0x0002;
    }
    else
    {
      while( DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) == RESET );
    }
    
    memcpy(USART2_Buffer_Tx, Buffer, BufferSize);       /* Copy memory */
    
    DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);        /* Clears the DMA1 Stream6's pending flags. */
    DMA_Cmd(DMA1_Stream6, DISABLE);                     /* Disables the specified DMA1 Stream6. */
    while( DMA_GetCmdStatus(DMA1_Stream6) != DISABLE ); /* Wait for the specified DMA1 Stream6 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream6, BufferSize);   /* Writes the number of data units to be transferred on the DMA1 Stream6. */
    DMA_Cmd(DMA1_Stream6, ENABLE);                      /* Enables the specified DMA1 Stream6. */
    
    return BufferSize;
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
    if( (dmaTxFlag & 0x0004) != 0x0004 )
    {
      return NULL;
    }
    
    if(BufferSize > USART3_BufferSize_Tx)
    {
      return NULL;
    }
    
    if( (firstFlag & 0x0004) != 0x0004 )
    {
      firstFlag |= 0x0004;
    }
    else
    {
      while( DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3) == RESET );
    }
    
    memcpy(USART3_Buffer_Tx, Buffer, BufferSize);       /* Copy memory */
    
    DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);        /* Clears the DMA1 Stream3's pending flags. */
    DMA_Cmd(DMA1_Stream3, DISABLE);                     /* Disables the specified DMA1 Stream3. */
    while( DMA_GetCmdStatus(DMA1_Stream3) != DISABLE ); /* Wait for the specified DMA1 Stream3 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream3, BufferSize);   /* Writes the number of data units to be transferred on the DMA1 Stream3. */
    DMA_Cmd(DMA1_Stream3, ENABLE);                      /* Enables the specified DMA1 Stream3. */
    
    return BufferSize;
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
    if( (dmaTxFlag & 0x0008) != 0x0008 )
    {
      return NULL;
    }
    
    if(BufferSize > USART4_BufferSize_Tx)
    {
      return NULL;
    }
    
    if( (firstFlag & 0x0008) != 0x0008 )
    {
      firstFlag |= 0x0008;
    }
    else
    {
      while( DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4) == RESET );
    }
    
    memcpy(USART4_Buffer_Tx, Buffer, BufferSize);       /* Copy memory */
    
    DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);        /* Clears the DMA1 Stream4's pending flags. */
    DMA_Cmd(DMA1_Stream4, DISABLE);                     /* Disables the specified DMA1 Stream4. */
    while( DMA_GetCmdStatus(DMA1_Stream4) != DISABLE ); /* Wait for the specified DMA1 Stream4 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream4, BufferSize);   /* Writes the number of data units to be transferred on the DMA1 Stream4. */
    DMA_Cmd(DMA1_Stream4, ENABLE);                      /* Enables the specified DMA1 Stream4. */
    
    return BufferSize;
  }
  else if(USARTx == UART5)
  {
    if( (dmaTxFlag & 0x0010) != 0x0010 )
    {
      return NULL;
    }
    
    if(BufferSize > USART5_BufferSize_Tx)
    {
      return NULL;
    }
    
    if( (firstFlag & 0x0010) != 0x0010 )
    {
      firstFlag |= 0x0010;
    }
    else
    {
      while( DMA_GetFlagStatus(DMA1_Stream7, DMA_FLAG_TCIF7) == RESET );
    }
    
    memcpy(USART5_Buffer_Tx, Buffer, BufferSize);       /* Copy memory */
    
    DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);        /* Clears the DMA1 Stream7's pending flags. */
    DMA_Cmd(DMA1_Stream7, DISABLE);                     /* Disables the specified DMA1 Stream7. */
    while( DMA_GetCmdStatus(DMA1_Stream7) != DISABLE ); /* Wait for the specified DMA1 Stream7 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream7, BufferSize);   /* Writes the number of data units to be transferred on the DMA1 Stream7. */
    DMA_Cmd(DMA1_Stream7, ENABLE);                      /* Enables the specified DMA1 Stream7. */
    
    return BufferSize;
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
    if( (dmaTxFlag & 0x0020) != 0x0020 )
    {
      return NULL;
    }
    
    if(BufferSize > USART6_BufferSize_Tx)
    {
      return NULL;
    }
    
    if( (firstFlag & 0x0020) != 0x0020 )
    {
      firstFlag |= 0x0020;
    }
    else
    {
      while( DMA_GetFlagStatus(DMA2_Stream6, DMA_FLAG_TCIF6) == RESET );
    }
    
    memcpy(USART6_Buffer_Tx, Buffer, BufferSize);       /* Copy memory */
    
    DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);        /* Clears the DMA2 Stream6's pending flags. */
    DMA_Cmd(DMA2_Stream6, DISABLE);                     /* Disables the specified DMA2 Stream6. */
    while( DMA_GetCmdStatus(DMA2_Stream6) != DISABLE ); /* Wait for the specified DMA2 Stream6 disable to finish. */
    DMA_SetCurrDataCounter(DMA2_Stream6, BufferSize);   /* Writes the number of data units to be transferred on the DMA2 Stream6. */
    DMA_Cmd(DMA2_Stream6, ENABLE);                      /* Enables the specified DMA2 Stream6. */
    
    return BufferSize;
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
    if( (dmaTxFlag & 0x0040) != 0x0040 )
    {
      return NULL;
    }
    
    if(BufferSize > USART7_BufferSize_Tx)
    {
      return NULL;
    }
    
    if( (firstFlag & 0x0040) != 0x0040 )
    {
      firstFlag |= 0x0040;
    }
    else
    {
      while( DMA_GetFlagStatus(DMA1_Stream1, DMA_FLAG_TCIF1) == RESET );
    }
    
    memcpy(USART7_Buffer_Tx, Buffer, BufferSize);       /* Copy memory */
    
    DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);        /* Clears the DMA1 Stream1's pending flags. */
    DMA_Cmd(DMA1_Stream1, DISABLE);                     /* Disables the specified DMA1 Stream1. */
    while( DMA_GetCmdStatus(DMA1_Stream1) != DISABLE ); /* Wait for the specified DMA1 Stream1 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream1, BufferSize);   /* Writes the number of data units to be transferred on the DMA1 Stream1. */
    DMA_Cmd(DMA1_Stream1, ENABLE);                      /* Enables the specified DMA1 Stream1. */
    
    return BufferSize;
  }
  else if(USARTx == UART8)
  {
    if( (dmaTxFlag & 0x0080) != 0x0080 )
    {
      return NULL;
    }
    
    if(BufferSize > USART8_BufferSize_Tx)
    {
      return NULL;
    }
    
    if( (firstFlag & 0x0080) != 0x0080 )
    {
      firstFlag |= 0x0080;
    }
    else
    {
      while( DMA_GetFlagStatus(DMA1_Stream0, DMA_FLAG_TCIF0) == RESET );
    }
    
    memcpy(USART8_Buffer_Tx, Buffer, BufferSize);       /* Copy memory */
    
    DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);        /* Clears the DMA1 Stream0's pending flags. */
    DMA_Cmd(DMA1_Stream0, DISABLE);                     /* Disables the specified DMA1 Stream0. */
    while( DMA_GetCmdStatus(DMA1_Stream0) != DISABLE ); /* Wait for the specified DMA1 Stream0 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream0, BufferSize);   /* Writes the number of data units to be transferred on the DMA1 Stream0. */
    DMA_Cmd(DMA1_Stream0, ENABLE);                      /* Enables the specified DMA1 Stream0. */
    
    return BufferSize;
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
    if( (dmaTxFlag & 0x0100) != 0x0100 )
    {
      return NULL;
    }
    
    if(BufferSize > USART9_BufferSize_Tx)
    {
      return NULL;
    }
    
    if( (firstFlag & 0x0100) != 0x0100 )
    {
      firstFlag |= 0x0100;
    }
    else
    {
      while( DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0) == RESET );
    }
    
    memcpy(USART9_Buffer_Tx, Buffer, BufferSize);       /* Copy memory */
    
    DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);        /* Clears the DMA2 Stream0's pending flags. */
    DMA_Cmd(DMA2_Stream0, DISABLE);                     /* Disables the specified DMA2 Stream0. */
    while( DMA_GetCmdStatus(DMA2_Stream0) != DISABLE ); /* Wait for the specified DMA2 Stream0 disable to finish. */
    DMA_SetCurrDataCounter(DMA2_Stream0, BufferSize);   /* Writes the number of data units to be transferred on the DMA2 Stream0. */
    DMA_Cmd(DMA2_Stream0, ENABLE);                      /* Enables the specified DMA2 Stream0. */
    
    return BufferSize;
  }
  else if(USARTx == UART10)
  {
    if( (dmaTxFlag & 0x0200) != 0x0200 )
    {
      return NULL;
    }
    
    if(BufferSize > USART10_BufferSize_Tx)
    {
      return NULL;
    }
    
    if( (firstFlag & 0x0200) != 0x0200 )
    {
      firstFlag |= 0x0200;
    }
    else
    {
      while( DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == RESET );
    }
    
    memcpy(USART10_Buffer_Tx, Buffer, BufferSize);      /* Copy memory */
    
    DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);        /* Clears the DMA2 Stream7's pending flags. */
    DMA_Cmd(DMA2_Stream7, DISABLE);                     /* Disables the specified DMA2 Stream7. */
    while( DMA_GetCmdStatus(DMA2_Stream7) != DISABLE ); /* Wait for the specified DMA2 Stream7 disable to finish. */
    DMA_SetCurrDataCounter(DMA2_Stream7, BufferSize);   /* Writes the number of data units to be transferred on the DMA2 Stream7. */
    DMA_Cmd(DMA2_Stream7, ENABLE);                      /* Enables the specified DMA2 Stream7. */
    
    return BufferSize;
  }
#endif /* STM32F413_423xx */
  
  return NULL;
}

/****************************************************************
 * Function:    USART_GetReceiveData
 * Description: USART Get Receive Data.
 * Input:       USARTx
 *              Buffer
 *              BufferSize
 * Output:
 * Return:      Receive Data Number
*****************************************************************/
uint16_t USART_GetReceiveData(USART_TypeDef* USARTx, void* Buffer, uint16_t BufferSize)
{
  uint16_t number = NULL;
  
  if(BufferSize == NULL)
  {
    return NULL;
  }
  
  if(USARTx == USART1)
  {
    if( (dmaRxFlag & 0x0001) != 0x0001 )
    {
      return NULL;
    }
    
    number = USART1_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream2); /* Returns the number of remaining data units in the current DMA2 Stream2 transfer. */
    
    if(number > BufferSize)
    {
      return NULL;
    }
    
    memcpy(Buffer, USART1_Buffer_Rx, number);                   /* Copy memory */
    memset(USART1_Buffer_Rx, NULL, USART1_BufferSize_Rx);       /* Empty memory */
    
    DMA_Cmd(DMA2_Stream2, DISABLE);                             /* Disables the specified DMA2 Stream2. */
    while( DMA_GetCmdStatus(DMA2_Stream2) != DISABLE );         /* Wait for the specified DMA2 Stream2 disable to finish. */
    DMA_SetCurrDataCounter(DMA2_Stream2, USART1_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA2 Stream2. */
    DMA_Cmd(DMA2_Stream2, ENABLE);                              /* Enables the specified DMA2 Stream2. */
  }
  else if(USARTx == USART2)
  {
    if( (dmaRxFlag & 0x0002) != 0x0002 )
    {
      return NULL;
    }
    
    number = USART2_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream5); /* Returns the number of remaining data units in the current DMA1 Stream5 transfer. */
    
    if(number > BufferSize)
    {
      return NULL;
    }
    
    memcpy(Buffer, USART2_Buffer_Rx, number);                   /* Copy memory */
    memset(USART2_Buffer_Rx, NULL, USART2_BufferSize_Rx);       /* Empty memory */
    
    DMA_Cmd(DMA1_Stream5, DISABLE);                             /* Disables the specified DMA1 Stream5. */
    while( DMA_GetCmdStatus(DMA1_Stream5) != DISABLE );         /* Wait for the specified DMA1 Stream5 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream5, USART2_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA1 Stream5. */
    DMA_Cmd(DMA1_Stream5, ENABLE);                              /* Enables the specified DMA1 Stream5. */
  }
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
  else if(USARTx == USART3)
  {
    if( (dmaRxFlag & 0x0004) != 0x0004 )
    {
      return NULL;
    }
    
    number = USART3_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream1); /* Returns the number of remaining data units in the current DMA1 Stream1 transfer. */
    
    if(number > BufferSize)
    {
      return NULL;
    }
    
    memcpy(Buffer, USART3_Buffer_Rx, number);                   /* Copy memory */
    memset(USART3_Buffer_Rx, NULL, USART3_BufferSize_Rx);       /* Empty memory */
    
    DMA_Cmd(DMA1_Stream1, DISABLE);                             /* Disables the specified DMA1 Stream1. */
    while( DMA_GetCmdStatus(DMA1_Stream1) != DISABLE );         /* Wait for the specified DMA1 Stream1 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream1, USART3_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA1 Stream1. */
    DMA_Cmd(DMA1_Stream1, ENABLE);                              /* Enables the specified DMA1 Stream1. */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F446xx) || defined(STM32F413_423xx)
  else if(USARTx == UART4)
  {
    if( (dmaRxFlag & 0x0008) != 0x0008 )
    {
      return NULL;
    }
    
    number = USART4_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream2); /* Returns the number of remaining data units in the current DMA1 Stream2 transfer. */
    
    if(number > BufferSize)
    {
      return NULL;
    }
    
    memcpy(Buffer, USART4_Buffer_Rx, number);                   /* Copy memory */
    memset(USART4_Buffer_Rx, NULL, USART4_BufferSize_Rx);       /* Empty memory */
    
    DMA_Cmd(DMA1_Stream2, DISABLE);                             /* Disables the specified DMA1 Stream2. */
    while( DMA_GetCmdStatus(DMA1_Stream2) != DISABLE );         /* Wait for the specified DMA1 Stream2 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream2, USART4_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA1 Stream2. */
    DMA_Cmd(DMA1_Stream2, ENABLE);                              /* Enables the specified DMA1 Stream2. */
  }
  else if(USARTx == UART5)
  {
    if( (dmaRxFlag & 0x0010) != 0x0010 )
    {
      return NULL;
    }
    
    number = USART5_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream0); /* Returns the number of remaining data units in the current DMA1 Stream0 transfer. */
    
    if(number > BufferSize)
    {
      return NULL;
    }
    
    memcpy(Buffer, USART5_Buffer_Rx, number);                   /* Copy memory */
    memset(USART5_Buffer_Rx, NULL, USART5_BufferSize_Rx);       /* Empty memory */
    
    DMA_Cmd(DMA1_Stream0, DISABLE);                             /* Disables the specified DMA1 Stream0. */
    while( DMA_GetCmdStatus(DMA1_Stream0) != DISABLE );         /* Wait for the specified DMA1 Stream0 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream0, USART5_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA1 Stream0. */
    DMA_Cmd(DMA1_Stream0, ENABLE);                              /* Enables the specified DMA1 Stream0. */
  }
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F446xx || STM32F413_423xx */
  else if(USARTx == USART6)
  {
    if( (dmaRxFlag & 0x0020) != 0x0020 )
    {
      return NULL;
    }
    
    number = USART6_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream1); /* Returns the number of remaining data units in the current DMA2 Stream1 transfer. */
    
    if(number > BufferSize)
    {
      return NULL;
    }
    
    memcpy(Buffer, USART6_Buffer_Rx, number);                   /* Copy memory */
    memset(USART6_Buffer_Rx, NULL, USART6_BufferSize_Rx);       /* Empty memory */
    
    DMA_Cmd(DMA2_Stream1, DISABLE);                             /* Disables the specified DMA2 Stream1. */
    while( DMA_GetCmdStatus(DMA2_Stream1) != DISABLE );         /* Wait for the specified DMA2 Stream1 disable to finish. */
    DMA_SetCurrDataCounter(DMA2_Stream1, USART6_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA2 Stream1. */
    DMA_Cmd(DMA2_Stream1, ENABLE);                              /* Enables the specified DMA2 Stream1. */
  }
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx) || defined(STM32F413_423xx)
  else if(USARTx == UART7)
  {
    if( (dmaRxFlag & 0x0040) != 0x0040 )
    {
      return NULL;
    }
    
    number = USART7_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream3); /* Returns the number of remaining data units in the current DMA1 Stream3 transfer. */
    
    if(number > BufferSize)
    {
      return NULL;
    }
    
    memcpy(Buffer, USART7_Buffer_Rx, number);                   /* Copy memory */
    memset(USART7_Buffer_Rx, NULL, USART7_BufferSize_Rx);       /* Empty memory */
    
    DMA_Cmd(DMA1_Stream3, DISABLE);                             /* Disables the specified DMA1 Stream3. */
    while( DMA_GetCmdStatus(DMA1_Stream3) != DISABLE );         /* Wait for the specified DMA1 Stream3 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream3, USART7_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA1 Stream3. */
    DMA_Cmd(DMA1_Stream3, ENABLE);                              /* Enables the specified DMA1 Stream3. */
  }
  else if(USARTx == UART8)
  {
    if( (dmaRxFlag & 0x0080) != 0x0080 )
    {
      return NULL;
    }
    
    number = USART8_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream6); /* Returns the number of remaining data units in the current DMA1 Stream6 transfer. */
    
    if(number > BufferSize)
    {
      return NULL;
    }
    
    memcpy(Buffer, USART8_Buffer_Rx, number);                   /* Copy memory */
    memset(USART8_Buffer_Rx, NULL, USART8_BufferSize_Rx);       /* Empty memory */
    
    DMA_Cmd(DMA1_Stream6, DISABLE);                             /* Disables the specified DMA1 Stream6. */
    while( DMA_GetCmdStatus(DMA1_Stream6) != DISABLE );         /* Wait for the specified DMA1 Stream6 disable to finish. */
    DMA_SetCurrDataCounter(DMA1_Stream6, USART8_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA1 Stream6. */
    DMA_Cmd(DMA1_Stream6, ENABLE);                              /* Enables the specified DMA1 Stream6. */
  }
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx || STM32F413_423xx */
#if defined(STM32F413_423xx)
  else if(USARTx == UART9)
  {
    if( (dmaRxFlag & 0x0100) != 0x0100 )
    {
      return NULL;
    }
    
    number = USART9_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream7); /* Returns the number of remaining data units in the current DMA2 Stream7 transfer. */
    
    if(number > BufferSize)
    {
      return NULL;
    }
    
    memcpy(Buffer, USART9_Buffer_Rx, number);                   /* Copy memory */
    memset(USART9_Buffer_Rx, NULL, USART9_BufferSize_Rx);       /* Empty memory */
    
    DMA_Cmd(DMA2_Stream7, DISABLE);                             /* Disables the specified DMA2 Stream7. */
    while( DMA_GetCmdStatus(DMA2_Stream7) != DISABLE );         /* Wait for the specified DMA2 Stream7 disable to finish. */
    DMA_SetCurrDataCounter(DMA2_Stream7, USART9_BufferSize_Rx); /* Writes the number of data units to be transferred on the DMA2 Stream7. */
    DMA_Cmd(DMA2_Stream7, ENABLE);                              /* Enables the specified DMA2 Stream7. */
  }
  else if(USARTx == UART10)
  {
    if( (dmaRxFlag & 0x0200) != 0x0200 )
    {
      return NULL;
    }
    
    number = USART10_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream0);  /* Returns the number of remaining data units in the current DMA2 Stream0 transfer. */
    
    if(number > BufferSize)
    {
      return NULL;
    }
    
    memcpy(Buffer, USART10_Buffer_Rx, number);                    /* Copy memory */
    memset(USART10_Buffer_Rx, NULL, USART10_BufferSize_Rx);       /* Empty memory */
    
    DMA_Cmd(DMA2_Stream0, DISABLE);                               /* Disables the specified DMA2 Stream0. */
    while( DMA_GetCmdStatus(DMA2_Stream0) != DISABLE );           /* Writes the number of data units to be transferred on the DMA2 Stream0. */
    DMA_SetCurrDataCounter(DMA2_Stream0, USART10_BufferSize_Rx);  /* Writes the number of data units to be transferred on the DMA2 Stream0. */
    DMA_Cmd(DMA2_Stream0, ENABLE);                                /* Enables the specified DMA2 Stream0. */
  }
#endif /* STM32F413_423xx */
  
  return number;
}
