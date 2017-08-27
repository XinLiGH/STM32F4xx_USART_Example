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
  USART_Configuration(USART1, 115200);
}

/****************************************************************
 * Function:    USART_ReceiveDataFinishCallback
 * Description: USART receive data finish callback.
 * Input:       USARTx: Where x can be 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 to select the USART or UART peripheral.
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
