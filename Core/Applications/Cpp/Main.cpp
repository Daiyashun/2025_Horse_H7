//
// Created by 27713 on 25-2-20.
//

#include "Main.h"
#include "main.h"
#include "cmsis_os.h"
#include "UART_DMA.h"

extern "C"
void MX_FREERTOS_Init(void);

void Main()
{

    /* Init scheduler */
    osKernelInitialize();

    /* Call init function for freertos objects (in freertos.c) */
    MX_FREERTOS_Init();

    /* Start scheduler */
    osKernelStart();

    while(1)
    {

    }
}
