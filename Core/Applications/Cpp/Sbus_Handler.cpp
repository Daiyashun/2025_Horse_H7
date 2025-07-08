#include "Sbus_Handler.h"
#include "usart.h"




void SBUS::Sbus_Uart_Receive_init(UART_HandleTypeDef* huart)
{

    /* 首先清除中断标志位 */
    __HAL_UART_CLEAR_IDLEFLAG(huart);

    /* 打开中断接收 */
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);

    /* 开始DMA接收 */
    HAL_UART_Receive_DMA(huart, Sbus_Receive_Buffer, sizeof(Sbus_Receive_Buffer));

}


void SBUS::Sbus_Uart_Receive_Handler(UART_HandleTypeDef* huart,uint8_t buffer[25])
{
    // if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) &&
    //       __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
    // {
    //     /* 清除中断标志位 */
    //     __HAL_UART_CLEAR_IDLEFLAG(huart);
    //
    //     /* 关闭DMA接收 */
    //     __HAL_DMA_DISABLE(huart->hdmarx);

        /* 用户处理部分 */
        Sbus_Data_Handler(huart,buffer);

        // /* 重新打开DMA接收 */
        // __HAL_DMA_ENABLE(huart->hdmarx);
    // }
}

void SBUS::Sbus_Data_Handler(UART_HandleTypeDef* huart,uint8_t buffer[25])
{
    if (buffer[0] == 0x0F && buffer[24] == 0x00 && Sbus_Data_Check())
    {
        Sbus_Data_Trans(buffer);
    }
}


uint8_t SBUS::Sbus_Data_Check()
{
    Sbus_Check_False_Num = 0;
    for(uint8_t i=0;i<25;i++)
    {
        if (Sbus_Receive_Buffer[i] != Sbus_Check_Buffer[i])
            Sbus_Check_False_Num++;
        Sbus_Check_Buffer[i] = Sbus_Receive_Buffer[i];
    }
    if(Sbus_Check_False_Num == 0 && Sbus_Check_False_Time < 200) Sbus_Check_False_Time ++;
    else if(Sbus_Check_False_Time > 0) Sbus_Check_False_Time = Sbus_Check_False_Time/2;
    if(Sbus_Check_False_Time >= 100) {HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);  return 0;}//OFFLINE
    else                        {HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);return 1;}//正常
}

void SBUS::Sbus_Data_Trans(uint8_t buffer[23])
{
    if (buffer[23] == 0) {
        Sbus_Channel[0] = ((int16_t) buffer[1] >> 0 | ((int16_t) buffer[3 - 1] << 8)) & 0x07FF;
        Sbus_Channel[1] = ((int16_t) buffer[2] >> 3 | ((int16_t) buffer[4 - 1] << 5)) & 0x07FF;
        Sbus_Channel[2] =
                ((int16_t) buffer[3] >> 6 | ((int16_t) buffer[4] << 2) | (int16_t) buffer[5] << 10) & 0x07FF;
        Sbus_Channel[3] = ((int16_t) buffer[5] >> 1 | ((int16_t) buffer[6] << 7)) & 0x07FF;
        Sbus_Channel[4] = ((int16_t) buffer[6] >> 4 | ((int16_t) buffer[7] << 4)) & 0x07FF;
        Sbus_Channel[5] =
                ((int16_t) buffer[7] >> 7 | ((int16_t) buffer[8] << 1) | (int16_t) buffer[9] << 9) & 0x07FF;
        Sbus_Channel[6] = ((int16_t) buffer[9] >> 2 | ((int16_t) buffer[10] << 6))  & 0x07FF;
        Sbus_Channel[7] = ((int16_t) buffer[10] >> 5 | ((int16_t) buffer[11] << 3)) & 0x07FF;
        Sbus_Channel[8] = ((int16_t) buffer[12] << 0 | ((int16_t) buffer[13] << 8)) & 0x07FF;
        Sbus_Channel[9] = ((int16_t) buffer[13] >> 3 | ((int16_t) buffer[14] << 5)) & 0x07FF;
        Sbus_Channel[10] =
                ((int16_t) buffer[14] >> 6 | ((int16_t) buffer[15] << 2) | (int16_t) buffer[16] << 10) & 0x07FF;
        Sbus_Channel[11] = ((int16_t) buffer[16] >> 1 | ((int16_t) buffer[17] << 7)) & 0x07FF;
        Sbus_Channel[12] = ((int16_t) buffer[17] >> 4 | ((int16_t) buffer[18] << 4)) & 0x07FF;
        Sbus_Channel[13] =
                ((int16_t) buffer[18] >> 7 | ((int16_t) buffer[19] << 1) | (int16_t) buffer[20] << 9) & 0x07FF;
        Sbus_Channel[14] = ((int16_t) buffer[20] >> 2 | ((int16_t) buffer[21] << 6)) & 0x07FF;
        Sbus_Channel[15] = ((int16_t) buffer[21] >> 5 | ((int16_t) buffer[22] << 3)) & 0x07FF;
    }
}
