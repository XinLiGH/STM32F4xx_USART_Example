/****************************************************************
 * Copyright (C) 2016, XinLi, all right reserved.
 * File name:    main.c
 * Date:         2016.03.22
 * Description:  USART example.
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
static void USART1_ReceiveDataFinishCallback(void);

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
 * Description: System initialization.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void System_Init(void)
{
  USART_Configure(USART1, USART_BaudRate115200, USART_DataBits8, USART_StopBits1, USART_ParityNo);
  USART_SetReceiveDataFinishCallback(USART1, USART1_ReceiveDataFinishCallback);
}

/****************************************************************
 * Function:    USART1_ReceiveDataFinishCallback
 * Description: USART1 receive data finish callback.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void USART1_ReceiveDataFinishCallback(void)
{
  uint8_t buff[100] = {0};
  
  uint32_t num = USART_GetReceiveData(USART1, buff, sizeof(buff));
  USART_SetSendData(USART1, buff, num);
}
