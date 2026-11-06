/*!
 * @file        main.h
 *
 * @brief       Header for main.c module
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

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/    
#include "Board.h"
#include <stdio.h>
#include <string.h>

#include "apm32e10x.h"
#include "apm32e10x_can.h"
#include "apm32e10x_misc.h"
#include "apm32e10x_gpio.h"
#include "apm32e10x_usart.h"
#include "apm32e10x_misc.h"
#include "bsp_delay.h"
#include "bsp_sdio.h"
#include "bsp_key.h"
#include "bsp_lcd.h"
#include "Board.h"
#include "ff.h"

/* Exported typedef *******************************************************/

/* Exported macro *********************************************************/

#define APM_LEDInit APM_EVAL_LEDInit
#define APM_PBInit  APM_EVAL_PBInit
#define APM_LEDOn   APM_EVAL_LEDOn
#define APM_LEDOff  APM_EVAL_LEDOff

/* Exported function prototypes *******************************************/

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
