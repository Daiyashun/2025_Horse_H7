//
// Created by 27713 on 25-2-20.
//

#include "Callback.h"
#include "Main.h"
#include "fdcan.h"
#include "FDCAN_receive_dm.h"
#include "Upper_Visual.h"
#include "bsp_mc02/can_bsp.h"

extern motor_t motor[12];
extern Upper_data_receive Vdata_Rx;
extern Upper_data_send Vdata_Tx;

void TIM6_CALLBACK()
{
    Vdata_Rx.Vdata_send();
    mit_send_in_TIM();
}

uint8_t rx_data1[8] = {0};
void fdcan1_rx_callback(void)
{
    uint16_t id;
    fdcanx_receive(&hfdcan1, &id, rx_data1);
    for (int i = 0; i < 4; i++)
    {
        if (i == id - 1 - 16)
        {
            get_moto_measure(&motor[i].receive, rx_data1);
            switch (id)
            {
            case MASTER_M1_ID:
                motor[0].receive.pos *= DIRECTION_CORRECTION;
                motor[0].receive.speed *= DIRECTION_CORRECTION;
                motor[0].receive.toq *= DIRECTION_CORRECTION;
                motor[0].receive.pos += MOTOR_147A_ANGLE_OFFSET;
                break;

            case MASTER_M2_ID:
                motor[1].receive.pos *= DIRECTION_CORRECTION;
                motor[1].receive.speed *= DIRECTION_CORRECTION;
                motor[1].receive.toq *= DIRECTION_CORRECTION;
                motor[1].receive.pos += MOTOR_258B_ANGLE_OFFSET;
                break;

            case MASTER_M3_ID:
                motor[2].receive.pos *= DIRECTION_CORRECTION;
                motor[2].receive.speed *= DIRECTION_CORRECTION;
                motor[2].receive.toq *= DIRECTION_CORRECTION;
                motor[2].receive.pos -= MOTOR_369C_ANGLE_OFFSET;
                break;

            case MASTER_M4_ID:
                motor[3].receive.pos *= DIRECTION_CORRECTION;
                motor[3].receive.speed *= DIRECTION_CORRECTION;
                motor[3].receive.toq *= DIRECTION_CORRECTION;
                motor[3].receive.pos -= MOTOR_147A_ANGLE_OFFSET;
                break;

            default:break;
            }
            Vdata_Tx.Visual_motor_send[0][i][0] = motor[i].receive.pos;
            Vdata_Tx.Visual_motor_send[0][i][1] = motor[i].receive.speed;
            Vdata_Tx.Visual_motor_send[0][i][2] = motor[i].receive.toq;
        }
    }
}

uint8_t rx_data2[8] = {0};
void fdcan2_rx_callback(void)
{
    uint16_t id;
    fdcanx_receive(&hfdcan2, &id, rx_data2);
    for (int i = 4; i < 8; i++)
    {
        if (i == id - 1 - 16)
        {
            get_moto_measure(&motor[i].receive, rx_data2);
            switch (id)
            {
            case MASTER_M5_ID:
                // motor[4].receive.pos *= DIRECTION_CORRECTION;
                // motor[4].receive.speed *= DIRECTION_CORRECTION;
                // motor[4].receive.toq *= DIRECTION_CORRECTION;
                motor[4].receive.pos += MOTOR_258B_ANGLE_OFFSET;
                break;

            case MASTER_M6_ID:
                // motor[5].receive.pos *= DIRECTION_CORRECTION;
                // motor[5].receive.speed *= DIRECTION_CORRECTION;
                // motor[5].receive.toq *= DIRECTION_CORRECTION;
                motor[5].receive.pos -= MOTOR_369C_ANGLE_OFFSET;
                break;

            case MASTER_M7_ID:
                // motor[6].receive.pos *= DIRECTION_CORRECTION;
                // motor[6].receive.speed *= DIRECTION_CORRECTION;
                // motor[6].receive.toq *= DIRECTION_CORRECTION;
                motor[6].receive.pos += MOTOR_147A_ANGLE_OFFSET;
                break;

            case MASTER_M8_ID:
                motor[7].receive.pos *= DIRECTION_CORRECTION;
                motor[7].receive.speed *= DIRECTION_CORRECTION;
                motor[7].receive.toq *= DIRECTION_CORRECTION;
                motor[7].receive.pos += MOTOR_258B_ANGLE_OFFSET;
                break;

            default:break;
            }
            Vdata_Tx.Visual_motor_send[1][i - 4][0] = motor[i].receive.pos;
            Vdata_Tx.Visual_motor_send[1][i - 4][1] = motor[i].receive.speed;
            Vdata_Tx.Visual_motor_send[1][i - 4][2] = motor[i].receive.toq;
        }

    }

}
uint8_t rx_data3[8] = {0};

void fdcan3_rx_callback(void)
{
    uint16_t id;
    fdcanx_receive(&hfdcan3, &id, rx_data3);
    for (int i = 8; i < 12; i++)
    {
        if (i == id - 1 - 16)
        {
            get_moto_measure(&motor[i].receive, rx_data3);
            switch (id)
            {
            case MASTER_M9_ID:
                motor[8].receive.pos *= DIRECTION_CORRECTION;
                motor[8].receive.speed *= DIRECTION_CORRECTION;
                motor[8].receive.toq *= DIRECTION_CORRECTION;
                motor[8].receive.pos -= MOTOR_369C_ANGLE_OFFSET;
                break;

            case MASTER_M10_ID:
                // motor[9].receive.pos *= DIRECTION_CORRECTION;
                // motor[9].receive.speed *= DIRECTION_CORRECTION;
                // motor[9].receive.toq *= DIRECTION_CORRECTION;
                motor[9].receive.pos -= MOTOR_147A_ANGLE_OFFSET;
                break;

            case MASTER_M11_ID:
                // motor[10].receive.pos *= DIRECTION_CORRECTION;
                // motor[10].receive.speed *= DIRECTION_CORRECTION;
                // motor[10].receive.toq *= DIRECTION_CORRECTION;
                motor[10].receive.pos += MOTOR_258B_ANGLE_OFFSET;
                break;

            case MASTER_M12_ID:
                //motor[11].receive.pos *= DIRECTION_CORRECTION;
                // motor[11].receive.speed *= DIRECTION_CORRECTION;
                // motor[11].receive.toq *= DIRECTION_CORRECTION;
                motor[11].receive.pos -= MOTOR_369C_ANGLE_OFFSET;
                break;

            default:break;
            }
            Vdata_Tx.Visual_motor_send[2][i - 8][0] = motor[i].receive.pos;
            Vdata_Tx.Visual_motor_send[2][i - 8][1] = motor[i].receive.speed;
            Vdata_Tx.Visual_motor_send[2][i - 8][2] = motor[i].receive.toq;
        }
    }
}



