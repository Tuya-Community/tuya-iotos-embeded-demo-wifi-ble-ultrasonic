/*
 * @file name: 
 * @Descripttion: 
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-03-18 10:38:20
 * @LastEditors: zgw
 * @LastEditTime: 2021-03-18 16:57:38
 */

#include "HC_SR04.h"
#include "tuya_gpio.h"
#include "FreeRTOS.h"
#include "BkDriverGpio.h"

typedef struct 
{
    UCHAR_T input_pin;
    UCHAR_T output_pin;
}HC_SR04_pin_t;
STATIC HC_SR04_pin_t HC_SR04_pin = {0};

STATIC UINT8_T echo_flag = 0;
STATIC UINT8_T measure_ret = 1;
STATIC TickType_t tick_count = 0;
STATIC TickType_t tick_count_old = 0;
STATIC TickType_t measure_time_tick =0;

VOID radar_echo_irq_cb(void *arg)
{   
    BkGpioFinalize(HC_SR04_pin.output_pin);
    tick_count = xTaskGetTickCountFromISR();
    echo_flag++;
    if(echo_flag >= 2) {
        if(tick_count > tick_count_old) {
            measure_time_tick = tick_count - tick_count_old;
            measure_ret = 0;
        }else {
            measure_ret = 1;
        }
        echo_flag = 0;
        tick_count = 0;
        tick_count_old = 0;
        return ;
    }
    tick_count_old = tick_count;
    BkGpioEnableIRQ(HC_SR04_pin.output_pin, IRQ_TRIGGER_FALLING_EDGE, radar_echo_irq_cb, NULL);
}

VOID radar_pin_init(UINT8_T input_pin, UINT8_T output_pin)
{
    HC_SR04_pin.input_pin = input_pin;
    HC_SR04_pin.output_pin = output_pin;

    tuya_gpio_inout_set(HC_SR04_pin.input_pin, FALSE);
    tuya_gpio_write(HC_SR04_pin.input_pin, FALSE);

    tuya_gpio_inout_set(HC_SR04_pin.output_pin, TRUE);

}

VOID trigger_delay(USHORT_T usDelayTime)
{
    while(usDelayTime)
    {
        usDelayTime --;
    }
}

VOID radar_measure_start(VOID)
{
    BkGpioEnableIRQ(HC_SR04_pin.output_pin, IRQ_TRIGGER_RISING_EDGE, radar_echo_irq_cb, NULL);

    tuya_gpio_write(HC_SR04_pin.input_pin, TRUE);
    trigger_delay(10);
    tuya_gpio_write(HC_SR04_pin.input_pin, FALSE);
}

INT16_T radar_get_time(UINT16_T* time_ms)
{   
    UCHAR_T time_out_count = 0;
    INT16_T ret = 0;

    while (measure_ret)
    {   
        //time out 100ms
        vTaskDelay(20);
        time_out_count++;
        if(time_out_count >= 10) {
            ret = OPRT_COM_ERROR;
            return ret;
        }
    }
    
    *time_ms = measure_time_tick;

    measure_ret = 1;
    measure_time_tick = 0;

    return ret;
}