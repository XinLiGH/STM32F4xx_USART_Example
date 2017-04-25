/****************************************************************
 * Copyright (C) 2016, XinLi, all right reserved.
 * File name:    main.c
 * Date:         2016.03.22
 * Description:  USART Example.
*****************************************************************/

/****************************************************************
 *                        Header include
*****************************************************************/
#include "main.h"
#include "USART.h"

/****************************************************************
 *                       Global variables
*****************************************************************/


/****************************************************************
 *                     Function declaration
*****************************************************************/
static void System_Init(void);
static void NVIC_Configuration(void);

/****************************************************************
 *                     Function definition
*****************************************************************/

/****************************************************************
 * Function:    main
 * Description: Program entry.
 * Input:
 * Output:
 * Return:
*****************************************************************/
int main(void)
{
  System_Init();
  
  for(;;)
  {
    
  }
}

/****************************************************************
 * Function:    System_Init
 * Description: Initialization System.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void System_Init(void)
{
  USART_Configuration(USART1, BAUD_RATE_115200, USART_IT_IDLE);
  NVIC_Configuration();
}

/****************************************************************
 * Function:    NVIC_Configuration
 * Description: Interrupt Vector Table Configuration.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
