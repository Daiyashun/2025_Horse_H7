

#include "Leg_Control.h"
#include "FDCAN_receive_dm.h"
#include "main.h"
#include "stm32h7xx_hal.h"
#include "vofa_setting.h"

Step_Character step;
Single_Foot_Measure leg[4];
Jump_Character Jump_Front;

extern motor_t motor[12];

// void Step_Character::Init()
// {
//     for (int i = 0; i < 4; i++)
//     {
//         leg[i].sfm_Height = step.basic.sbm_Height;
//         leg[i].sfm_Length = step.basic.sbm_Length;
//         leg[i].sfm_Basic_Height = step.basic.sbm_Basic_Height;
//     }
//     HAL_Delay(1000);
// }

void Jump_Character::Jump()
{
    if ((Jump_Tim_Cnt > 0) || Jumping_Flag)
    {
        switch (Jump_Tim_Cnt)
        {
            case Tim_Cnt_Ready_For_Jump:
                Jump_Front_Ready();
                Jump_Angle_Trans();
                Jump_Tim_Cnt++;
                break;

            case Tim_Cnt_Start_Jump:
                Jump_Front();
                Jump_Angle_Trans();
                Jump_Tim_Cnt++;
                break;

            case Tim_Cnt_Jumping:
                Jump_Front_Over();
                Jump_Angle_Trans();
                Jump_Tim_Cnt++;
                break;

            case Tim_Cnt_Jump_Over:
                Jump_Front_Over();
                Jump_Angle_Trans();
                Jump_Tim_Cnt = 0;
                Jumping_Flag = false;
                break;

            default:
                Jump_Tim_Cnt++;
                break;
        }

    }
}

void Jump_Character::Jump_Angle_Trans()
{
    for (int i = 1; i < 13; i++)
    {
        switch (i)
        {
            case CAN_DM_M1_ID:
                motor[CAN_DM_M1_ID - 1].send.pos += MOTOR_147A_ANGLE_OFFSET;
                //motor[i - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M2_ID:
                motor[CAN_DM_M2_ID - 1].send.pos -= MOTOR_258B_ANGLE_OFFSET;
                motor[CAN_DM_M2_ID - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M3_ID:
                motor[CAN_DM_M3_ID - 1].send.pos += MOTOR_369C_ANGLE_OFFSET;
                motor[CAN_DM_M3_ID - 1].send.pos *= DIRECTION_CORRECTION;
                motor[CAN_DM_M3_ID - 1].send.pos *= GEAR_RATIO;
                break;

            case CAN_DM_M4_ID:
                motor[CAN_DM_M4_ID - 1].send.pos -= MOTOR_147A_ANGLE_OFFSET;
                //motor[i - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M5_ID:
                motor[CAN_DM_M5_ID - 1].send.pos -= MOTOR_258B_ANGLE_OFFSET;
                //motor[i - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M6_ID:
                motor[CAN_DM_M6_ID - 1].send.pos += MOTOR_369C_ANGLE_OFFSET;
                //motor[i - 1].send.pos *= DIRECTION_CORRECTION;
                motor[CAN_DM_M6_ID - 1].send.pos *= GEAR_RATIO;
                break;

            case CAN_DM_M7_ID:
                motor[CAN_DM_M7_ID - 1].send.pos += MOTOR_147A_ANGLE_OFFSET;
                motor[CAN_DM_M7_ID - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M8_ID:
                motor[CAN_DM_M8_ID - 1].send.pos -= MOTOR_258B_ANGLE_OFFSET;
                motor[CAN_DM_M8_ID - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M9_ID:
                motor[CAN_DM_M9_ID - 1].send.pos += MOTOR_369C_ANGLE_OFFSET;
                motor[CAN_DM_M9_ID - 1].send.pos *= DIRECTION_CORRECTION;
                motor[CAN_DM_M9_ID - 1].send.pos *= GEAR_RATIO;
                break;

            case CAN_DM_M10_ID:
                motor[CAN_DM_M10_ID - 1].send.pos -= MOTOR_147A_ANGLE_OFFSET;
                motor[CAN_DM_M10_ID - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M11_ID:
                motor[CAN_DM_M11_ID - 1].send.pos -= MOTOR_258B_ANGLE_OFFSET;
                //motor[i - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M12_ID:
                motor[CAN_DM_M12_ID - 1].send.pos += MOTOR_369C_ANGLE_OFFSET;
                //motor[i - 1].send.pos *= DIRECTION_CORRECTION;
                motor[CAN_DM_M12_ID - 1].send.pos *= GEAR_RATIO;
                break;

            default:break;
        }
    }
}


void Jump_Character::Jump_Init()
{
    Jump_Tim_Cnt = 0;
    Jumping_Flag = false;
}

void Jump_Character::Jump_Front_Ready()
{
    // 定义电机位置配置
    const float positions[3] = {-0.0f, 1.1f + 0.8f, -2.65f};
    const int motorGroups[3][4] = {
        {0, 3, 6, 9},  // 第一组髋关节电机
        {1, 4, 7, 10}, // 第二组大腿电机
        {2, 5, 8, 11}  // 第三组小腿电机
    };

    // 使用循环设置电机位置
    for (int group = 0; group < 3; group++) {
        for (int i = 0; i < 4; i++) {
            int motorIndex = motorGroups[group][i];
            if (group == 0)
            {
                if (i % 2 == 0)
                {
                    motor[motorIndex].send.pos = positions[group];
                }
                else
                {
                    motor[motorIndex].send.pos = -positions[group];
                }
            }
            else
            {
                motor[motorIndex].send.pos = positions[group];
            }
        }
    }
    // motor[0].send.pos = -0.0f;
    // motor[3].send.pos = 0.0f;
    // motor[6].send.pos = -0.0f;
    // motor[9].send.pos = 0.0f;
    //
    // motor[1].send.pos = 1.1f + 1.0f;
    // motor[4].send.pos = 1.1f + 1.0f;
    // motor[7].send.pos = 1.1f + 1.0f;
    // motor[10].send.pos = 1.1f + 1.0f;
    //
    // motor[2].send.pos = -2.65f;
    // motor[5].send.pos = -2.65f;
    // motor[8].send.pos = -2.65f;
    // motor[11].send.pos = -2.65f;
}

void Jump_Character::Jump_Front()
{
    // 定义电机位置配置
    const float positions[3] = {0.1f, 0.4f + 1.15f, -0.7f};
    const int motorGroups[3][4] = {
        {0, 3, 6, 9},  // 第一组髋关节电机
        {1, 4, 7, 10}, // 第二组大腿电机
        {2, 5, 8, 11}  // 第三组小腿电机
    };

    // 使用循环设置电机位置
    for (int group = 0; group < 3; group++) {
        for (int i = 0; i < 4; i++) {
            int motorIndex = motorGroups[group][i];
            if (group == 0)
            {
                if (i % 2 == 0)
                {
                    motor[motorIndex].send.pos = positions[group];
                }
                else
                {
                    motor[motorIndex].send.pos = -positions[group];
                }
            }
            else
            {
                motor[motorIndex].send.pos = positions[group];
            }
        }
    }
    // motor[0].send.pos = -0.0f;
    // motor[3].send.pos = 0.0f;
    // motor[6].send.pos = -0.0f;
    // motor[9].send.pos = 0.0f;
    //
    // motor[1].send.pos = 0.4f;
    // motor[4].send.pos = 0.4f;
    // motor[7].send.pos = 0.4f;
    // motor[10].send.pos = 0.4f;
    //
    // motor[2].send.pos = -0.95f;
    // motor[5].send.pos = -0.95f;
    // motor[8].send.pos = -0.95f;
    // motor[11].send.pos = -0.95f;
}

void Jump_Character::Jump_Front_Over()
{
    // 定义电机位置配置
    const float positions[3] = {-0.4f, 1.1f, -2.75f};
    const int motorGroups[3][4] = {
        {0, 3, 6, 9},  // 第一组髋关节电机
        {1, 4, 7, 10}, // 第二组大腿电机
        {2, 5, 8, 11}  // 第三组小腿电机
    };

    // 使用循环设置电机位置
    for (int group = 0; group < 3; group++) {
        for (int i = 0; i < 4; i++) {
            int motorIndex = motorGroups[group][i];
            if (group == 0)
            {
                if (i % 2 == 0)
                {
                    motor[motorIndex].send.pos = positions[group];
                }
                else
                {
                    motor[motorIndex].send.pos = -positions[group];
                }
            }
            else
            {
                motor[motorIndex].send.pos = positions[group];
            }
        }
    }
    // motor[0].send.pos = -0.4f;
    // motor[3].send.pos = 0.4f;
    // motor[6].send.pos = -0.4f;
    // motor[9].send.pos = 0.4f;
    //
    // motor[1].send.pos = 1.1f;
    // motor[4].send.pos = 1.1f;
    // motor[7].send.pos = 1.1f;
    // motor[10].send.pos = 1.1f;
    //
    // motor[2].send.pos = -2.75f;
    // motor[5].send.pos = -2.75f;
    // motor[8].send.pos = -2.75f;
    // motor[11].send.pos = -2.75f;
}







