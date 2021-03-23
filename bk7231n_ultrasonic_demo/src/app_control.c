/*
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @LastEditors: zgw
 * @file name: sht21.h
 * @Description: SHT21 IIC drive src file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2020-12-16 18:51:29
 * @LastEditTime: 2021-03-18 17:02:28
 */

#include "app_control.h"
#include "tuya_gpio.h"
#include "tuya_uart.h"
#include "BkDriverUart.h"
#include "sys_timer.h"
#include "uni_time.h"
#include "app_pwm.h"
#include "HC_SR04.h"
/***********************************************************
*************************types define***********************
***********************************************************/
typedef enum
{
    LOW = 0,
    HIGH,
}default_level;

APP_CTRL_DATA_T app_ctrl_data = {0};
/***********************************************************
*************************IO control device define***********
***********************************************************/
#define RADAR_INPUT_PIN                      (6)
#define RADAR_OUTPUT_PIN                     (7)
/***********************************************************
*************************about adc init*********************
***********************************************************/


/***********************************************************
*************************about iic init*********************
***********************************************************/

/***********************************************************
*************************function***************************
***********************************************************/

STATIC VOID __ctrl_gpio_init(CONST TY_GPIO_PORT_E port, CONST BOOL_T high)
{   
    //Set IO port as output mode
    tuya_gpio_inout_set(port, FALSE);
    //Set IO port level
    tuya_gpio_write(port, high);
}


VOID app_device_init(VOID)
{
    radar_pin_init(RADAR_INPUT_PIN, RADAR_OUTPUT_PIN);
}



VOID app_ctrl_handle(VOID)
{   
    INT16_T ret = 0;
    UINT16_T distance = 0;

    radar_measure_start();
    ret = radar_get_time(&app_ctrl_data.measure_time_ms);
    if(ret != OPRT_OK) {
        PR_NOTICE("-----------measure failed----------");
    }else {
        app_ctrl_data.measure_distance_mm = app_ctrl_data.measure_time_ms*17/2; //distance = (time * Speed of sound(340M/S)) /2
        PR_NOTICE("-----------distance_mm = %d----------",app_ctrl_data.measure_distance_mm);
    }

}

VOID app_ctrl_all_off(VOID)
{   

}

 