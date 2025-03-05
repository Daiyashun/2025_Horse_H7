//
// Created by 27713 on 25-3-5.
//

#ifndef LEG_CONTEOL_H
#define LEG_CONTEOL_H

#endif //LEG_CONTEOL_H

//梯形步态最底层的类，用来存放梯形所必须的参数
class Single_Foot_Measure
{
public:
    float Height;   //每踏一步的高度，在采用梯形足端轨迹的情况下可理解为梯形的高
    float Length;   //每踏一步的长度，在采用梯形足端轨迹的情况下可理解为梯形的下底边长度
    float Basic_Height;    //从电机轴心到地面的高度，可近似认为是机器人躯干到地面的高度，即身高
    float x;        //单条腿局部坐标系的x值
    float y;        //单条腿局部坐标系的y值
    float z;        //单条腿局部坐标系的z值
};

//生成梯形步态所必需的计数值的类
class Time_Counter_Measure
{
public:
    int cnt;

};
