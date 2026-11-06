/*!
 * @file        main.h
 *
 * @brief       Main program body
 *
 * @version     V1.0.3
 *
 * @date        2025-06-17
 *
 * @attention
 *
 *  Copyright (C) 2022-2025 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Define to prevent recursive inclusion */
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "apm32e10x.h"
#include "apm32e10x_adc.h"
#include "apm32e10x_i2c.h"
#include "apm32e10x_eint.h"
#include "apm32e10x_rcm.h"
#include "apm32e10x_gpio.h"
#include "apm32e10x_usart.h"
#include "apm32e10x_misc.h"
#include "apm32e10x_tmr.h"
#include "apm32e10x_spi.h"
#include "apm32e10x_rtc.h"
#include "apm32e10x_pmu.h"
#include "apm32e10x_bakpr.h"

#include "Board.h"
#include "bsp_delay.h"
#include "bsp_lcd.h"
#include "bsp_key.h"

/* Exported typedef *******************************************************/

/* Exported macro *********************************************************/

/* Exported function prototypes *******************************************/
 
typedef struct
{
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} TIME_T;

void RTC_Isr(void);
void TMR7_Isr(void);
void LCD_HomePageHandle(uint8_t menuState);
void LCD_MenuLevel1Handle(uint8_t menuState);
void LCD_MenuLevel2Handle(uint8_t menuState);
void LCD_MenuLevel3Handle(uint8_t menuState);

#ifdef __cplusplus
}
#endif

#endif

