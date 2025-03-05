//
// Created by 27713 on 25-3-5.
//

#include "Leg_Control.h"
#include "main.h"
#include "stm32h7xx_hal.h"

Step_Character step;
Single_Foot_Measure leg[4];

void Step_Character::Init()
{
    for (int i = 0; i < 4; i++)
    {
        leg[i].sfm_Height = step.basic.sbm_Height;
        leg[i].sfm_Length = step.basic.sbm_Length;
        leg[i].sfm_Basic_Height = step.basic.sbm_Basic_Height;
    }
    HAL_Delay(1000);
}




