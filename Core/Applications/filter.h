#ifndef __FILTER_H_
#define __FILTER_H_
#include "stm32h7xx_hal.h"
#define MAX_FILTER_ORDER 5   //the maximum filter order
#define MAX_AVERAGE_LENGTH 50
// the filter handler with a maximum order 4;
typedef struct{
    float a[MAX_FILTER_ORDER];
    float b[MAX_FILTER_ORDER];
    float in[MAX_FILTER_ORDER];
    float out[MAX_FILTER_ORDER];
    float order;
}Filter_t;
typedef struct{
    float a[2];
    float b[2];
    float in;
    float out;
}Filter1_t;
typedef struct{
    float buf[MAX_AVERAGE_LENGTH];//the buffer
    uint16_t len;
    float out;
}Filter_a_t;
float Filter_Order1(Filter1_t *filter, float input);
void Filter1_init(float a0, float a1, float b0, float b1, float in, Filter1_t* filter);
void Filter_a_init(float input, Filter_a_t* a, uint16_t len);
float moving_average_filter(Filter_a_t* a, float input);
#endif






