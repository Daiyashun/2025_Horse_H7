#include "filter.h"

#include "stm32h7xx_hal.h"
/**
 * @brief cal the average value
 * @param a
 * @param len
 * @return
 */
float my_sum(float a[], uint16_t len)
{
    uint16_t k = 0;
    float sum_cnt = 0;
    for(k = 0; k < len; k++)
        sum_cnt += a[k];
    return sum_cnt;
}
void Filter_Init()
{

}
float filter_Cal(Filter_t *filter)
{

}
float One_Order_Filter(Filter_t *filter, float input)
{
    filter->in[1] = filter->in[0];
    filter->in[0] = input;
    filter->out[0] = (filter->in[0] * filter->a[0] +
                      filter->in[1] * filter->a[1] -
                      filter->out[1] * filter->b[1]) / filter->b[0];
    filter->out[1] = filter->out[0];
}
/**
 * @brief Initializes the 1 order filter
 * @param a0
 * @param a1
 * @param b0
 * @param b1
 * @param in
 * @param filter
 */
void Filter1_init(float a0, float a1, float b0, float b1, float in, Filter1_t* filter)
{
    filter->a[0] = a0;
    filter->a[1] = a1;
    filter->b[0] = b0;
    filter->b[1] = b1;
    filter->in = in;
    filter->out = in;
}
/**
 * @brief Initializes the moving average filter
 * @param input
 * @param a
 * @param len
 */
void Filter_a_init(float input, Filter_a_t* a, uint16_t len)
{
    uint16_t  k = 0;
    a->len = len;
    for(k = 0; k <= a->len; k++)
    {
        a->buf[k] = input;//initial for the first point;
    }
}
/**
 * @brief Function for 1 order filter
 * @param filter
 * @param input
 * @return
 */
float Filter_Order1(Filter1_t *filter, float input)
{
    filter->in = input;
    filter->out = (filter->a[0] * input + filter->a[0] * filter->in -
                   filter->out * filter->b[1]) / filter->b[0];
    return filter->out;

}
float moving_average_filter(Filter_a_t* a, float input)
{
    uint16_t k;
    a->out = input;
    a->buf[0] = input;
    for (k = 0; k < a->len - 1; k++)
    {
        a->buf[a->len - k - 1] = a->buf[a->len - k - 2];// move to the back
        a->out += a->buf[a->len - k];
    }


    return a->out = a->out/ (float)a->len;
}