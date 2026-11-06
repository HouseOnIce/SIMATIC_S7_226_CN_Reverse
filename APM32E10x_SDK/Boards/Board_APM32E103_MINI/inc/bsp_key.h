/*!
 * @file        bsp_key.h
 *
 * @brief       Header for bsp_key.c module
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
#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "Board.h"
#include "main.h"

/* Exported typedef *******************************************************/

/* Exported macros ********************************************************/

#define KEY1                    GPIO_ReadInputBit(KEY1_BUTTON_GPIO_PORT,KEY1_BUTTON_PIN)
#define KEY2                    GPIO_ReadInputBit(KEY2_BUTTON_GPIO_PORT,KEY2_BUTTON_PIN)

/**
 * @brief    Key value
 */
typedef enum
{
    KEY_ALL_REALEASED,          /*!< All key is realeased */
    KEY1_PRESSED,               /*!< KEY1 is pressed */
    KEY2_PRESSED,               /*!< KEY2 is pressed */
} KEY_VALUE_T;

/**
 * @brief    Key mode
 */
typedef enum
{
    KEY_MODE_SINGLE,                /*!< Single detect mode */
    KEY_MODE_CONTINUOUS,            /*!< Continuous detect mode */
} KEY_MODE_T;

/* Exported function prototypes *******************************************/

uint8_t Key_Scan(uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif
