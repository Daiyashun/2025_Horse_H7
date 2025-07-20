//
// Created by 27713 on 25-3-5.
//

#ifndef LEG_CONTEOL_H
#define LEG_CONTEOL_H

#endif //LEG_CONTEOL_H

#define L1   150.0f//120.0f   //单位 mm
#define L2   150.0f//160.0f   //单位 mm

//梯形步态最底层的类，用来存放生成梯形所必须的参数，生成梯形用，与最底层对接
class Single_Foot_Measure
{
public:
    float sfm_Height;   //每踏一步的高度，在采用梯形足端轨迹的情况下可理解为梯形的高
    float sfm_Length;   //每踏一步的长度，在采用梯形足端轨迹的情况下可理解为梯形的下底边长度
    float sfm_Basic_Height;    //从电机轴心到地面的高度，可近似认为是机器人躯干到地面的高度，即身高
    float sfm_x;        //单条腿局部坐标系的x值
    float sfm_y;        //单条腿局部坐标系的y值
    float sfm_z;        //单条腿局部坐标系的z值

    Single_Foot_Measure(){};
    Single_Foot_Measure(float Height, float Length, float Basic_Height, float x, float y, float z);
};

//生成梯形轨迹所必需的计数值的类
class Time_Counter_Measure
{
public:
    float tcm_cnt;  //最基础的计数值
    float tcm_cycle;    //完成一整个梯形轨迹所需的总计数个数，即周期
    float tcm_cycle_max;    //周期最大值
    float tcm_cycle_min;    //周期最小值

    Time_Counter_Measure(){};  //构造函数，无参数的情况下，所有参数都为0.0f`
    Time_Counter_Measure(float cnt, float cycle, float cycle_max, float cycle_min);
};

//用于存放梯形的基础参数，赋值用，与遥控器和上位机对接
class Step_Basic_Measure
{
public:
    float sbm_Length;       //基础步长
    float sbm_Length_Max;    //基础步长的最大值
    float sbm_Length_Min;    //基础步长的最小值

    float sbm_Height;        //基础步高
    float sbm_Height_Max;    //基础步高的最大值
    float sbm_Height_Min;    //基础步高的最小值

    float sbm_Basic_Height;       //基础身高
    float sbm_Basic_Height_Max;    //基础身高的最大值
    float sbm_Basic_Height_Min;    //基础身高的最小值

    Step_Basic_Measure(){};  //构造函数，无参数的情况下，所有参数都为0.0f
    ~Step_Basic_Measure(){};
    Step_Basic_Measure(float Length, float Length_Max, float Length_Min, float Height, float Height_Max, float Height_Min,float Basic_Height, float Basic_Height_Max, float Basic_Height_Min);
};

//用于存放使得狗的姿态发生改变的参数，赋值用，与遥控器和上位机对接
class Step_Bios_Measure
{
public:
    float sbm_turn;             //转向所用的左右身体所连接的腿的步长的差值
    float sbm_turn_max;         //转向所用的左右身体所连接的腿的步长的差值最大值
    float sbm_turn_min;         //转向所用的左右身体所连接的腿的步长的差值最小值

    float sbm_pitch;            //前倾值
    float sbm_pitch_max;         //前倾值的最大值
    float sbm_pitch_min;         //前倾值的最小值

    float sbm_roll;             //侧倾值(规定向左为正方向)
    float sbm_roll_max;          //侧倾值的最大值
    float sbm_roll_min;          //侧倾值的最小值

    Step_Bios_Measure(){};
    ~Step_Bios_Measure(){};
    Step_Bios_Measure(float turn, float turn_max, float turn_min, float pitch, float pitch_max, float pitch_min, float roll, float roll_max, float roll_min);
};

//
class Step_Character
{
public:
    Step_Basic_Measure basic;
    Step_Bios_Measure bios;
    Time_Counter_Measure timer;

    Step_Character(){};
    ~Step_Character(){};
    void Init();
};

class Jump_Character
{
public:
    int Jump_Tim_Cnt;
    float Jump_Motor_Pos[12];
    int Jump_Motor_P;
    bool Jumping_Flag;

#define Tim_Cnt_Ready_For_Jump 50
#define Tim_Cnt_Start_Jump 250 + 100
#define Tim_Cnt_Jumping 351 + 100
#define Tim_Cnt_Jump_Over 1000

    void Jump_Init();
    void Jump_Front_Ready();
    void Jump_Front();
    void Jump_Front_Over();
    void Jump_Angle_Trans();
    void Jump();


};

// Single_Foot_Measure::Single_Foot_Measure(float Height, float Length, float Basic_Height, float x, float y, float z)
// {
//     sfm_x = x;
//     sfm_y = y;
//     sfm_z = z;
//     sfm_Height = Height;
//     sfm_Length = Length;
//     sfm_Basic_Height = Basic_Height;
// }
//
// Time_Counter_Measure::Time_Counter_Measure(float cnt, float cycle, float cycle_max, float cycle_min)
// {
//     tcm_cnt = cnt;
//     tcm_cycle = cycle;
//     tcm_cycle_max = cycle_max;
//     tcm_cycle_min = cycle_min;
// }
//
// Step_Basic_Measure::Step_Basic_Measure(float Length, float Length_Max, float Length_Min, float Height, float Height_Max, float Height_Min, float Basic_Height, float Basic_Height_Max, float Basic_Height_Min)
// {
//     sbm_Length = Length;
//     sbm_Length_Max = Length_Max;
//     sbm_Length_Min = Length_Min;
//     sbm_Height = Height;
//     sbm_Height_Max = Height_Max;
//     sbm_Height_Min = Height_Min;
//     sbm_Basic_Height = Basic_Height;
//     sbm_Basic_Height_Max = Basic_Height_Max;
//     sbm_Basic_Height_Min = Basic_Height_Min;
// }
//
// Step_Bios_Measure::Step_Bios_Measure(float turn, float turn_max, float turn_min, float pitch, float pitch_max, float pitch_min, float roll, float roll_max, float roll_min)
// {
//     sbm_turn = turn;
//     sbm_turn_max = turn_max;
//     sbm_turn_min = turn_min;
//     sbm_pitch = pitch;
//     sbm_pitch_max = pitch_max;
//     sbm_pitch_min = pitch_min;
//     sbm_roll = roll;
//     sbm_roll_max = roll_max;
//     sbm_roll_min = roll_min;
// }
