# STM32F4xx_USART_Example

使用二级缓存方法，来实现STM32F4的USART驱动。

## 开发环境

* 适用芯片：STM32F4全部芯片
* 固件库：STM32F4xx_DSP_StdPeriph_Lib_V1.8.0
* IDE：MDK517

## API

* void USART_Configure(USART_TypeDef* USARTx, USART_BaudRate BaudRate, USART_DataBits DataBits, USART_StopBits StopBits, USART_Parity Parity)
* void USART_Unconfigure(USART_TypeDef* USARTx)
* uint32_t USART_SetSendData(USART_TypeDef* USARTx, void* Data, uint32_t Length)
* uint32_t USART_GetReceiveData(USART_TypeDef* USARTx, void* Data, uint32_t Length)
* void USART_SetSendDataFinishCallback(USART_TypeDef* USARTx, void (*Function)(void))
* void USART_SetReceiveDataFinishCallback(USART_TypeDef* USARTx, void (*Function)(void))
* void USART_ClearSendDataFinishCallback(USART_TypeDef* USARTx)
* void USART_ClearReceiveDataFinishCallback(USART_TypeDef* USARTx)

## 注意

配置比特率时需要注意，比特率必须小于等于，总线时钟频率除以16。如果想要修改缓存大小和硬件接口，请到头文件中修改。DMA缓存大小为0~65535，USART缓存不能小于DMA缓存。用不到的端口，可以将缓存改为0。
