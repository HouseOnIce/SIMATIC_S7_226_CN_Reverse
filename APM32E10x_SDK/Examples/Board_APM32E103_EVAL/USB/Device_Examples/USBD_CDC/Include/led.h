/*!
 * @file        led.h
 *
 * @brief       This file contains the headers of the LED control functions
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
#ifndef __LED
#define __LED

/* Includes ***************************************************************/
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported typedef *******************************************************/
	
typedef void (*func)(Led_TypeDef Led);

typedef struct __attribute__((packed, aligned(4)))
{
	func ledon;
	func ledoff;
	
}LED_Function;

/* Exported macro *********************************************************/

/* Exported function prototypes *******************************************/

extern LED_Function led_func;

#ifdef __cplusplus
}
#endif

#endif
