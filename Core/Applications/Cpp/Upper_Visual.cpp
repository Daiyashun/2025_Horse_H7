//
// Created by 27713 on 25-3-15.
//

#include "Upper_Visual.h"

extern motor_t motor[12];

void Upper_data_receive::Vdata_transfer()
{
    for(int i = 0; i < 12; i++)
    {
        motor[i].send.pos = Visual_motor_receive[i].send.pos;
        motor[i].send.speed = Visual_motor_receive[i].send.speed;
        motor[i].send.tor = Visual_motor_receive[i].send.tor;
        motor[i].send.P = Visual_motor_receive[i].send.P;
        motor[i].send.D = Visual_motor_receive[i].send.D;
    }
}
