/*
 * @file name: 
 * @Descripttion: 
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-03-18 10:38:01
 * @LastEditors: zgw
 * @LastEditTime: 2021-03-18 14:59:12
 */

#ifndef __HC_SR04_H__
#define __HC_SR04_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"



VOID radar_pin_init(UINT8_T input_pin, UINT8_T output_pin);

VOID radar_measure_start(VOID);

INT16_T radar_get_time(UINT16_T* time_ms);

#endif