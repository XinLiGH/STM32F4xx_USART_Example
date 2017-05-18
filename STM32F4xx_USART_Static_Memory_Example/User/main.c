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
  USART_Configuration(USART1, 115200);
}

/****************************************************************
 * Function:    USART_ReceiveDataFinishCallback
 * Description: USART Receive Data Finish Callback.
 * Input:       USARTx
 * Output:
 * Return:
*****************************************************************/
void USART_ReceiveDataFinishCallback(USART_TypeDef* USARTx)
{
  if(USARTx == USART1)
  {
    uint8_t buff[100] = {0};
    
    uint32_t num = USART_GetReceiveData(USART1, buff, sizeof(buff));
    USART_SetSendData(USART1, buff, num);
  }
}
