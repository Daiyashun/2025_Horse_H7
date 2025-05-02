//
// Created by 27713 on 25-2-20.
//

#ifndef MAIN_H
#define MAIN_H

/*---------------------------- 在以下空白处添加宏定义内容---------------------------*/
#define USE_DYF 1

#define SIMULATE_MODE 0
#define TEST_MODE 0

#define PI 3.1415926f
#define MOTOR_147A_ANGLE_OFFSET 0.29f
#define MOTOR_258B_ANGLE_OFFSET 1.15f
#define MOTOR_369C_ANGLE_OFFSET 2.73f
#define DIRECTION_CORRECTION -1.0f

#define SEND_P 35
#define SEND_P_TEST 5
#define SEND_D 1

#define DATA_LENGTH_REAL   51
#define DATA_LENGTH  DATA_LENGTH_REAL * 4
#define DATA_LENGTH_REAL_DYF 32
#define DATA_LENGTH_DYF DATA_LENGTH_REAL_DYF * 4
/*-----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------- C Scope Start---------------------------*/
/*---------------------------- 在以下空白处添加C语言内容---------------------------*/
#include "stdint.h"

void Main();
void disable_all();
/*---------------------------- C Scope End---------------------------*/
#ifdef __cplusplus
}
#endif
/*---------------------------- C++ Scope Start---------------------------*/
#endif //MAIN_H
