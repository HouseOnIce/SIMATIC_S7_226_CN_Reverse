/*!
 * @file        bsp_delay.h
 *
 * @brief       Header for bsp_delay.c module
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
#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "main.h"

/* Exported typedef *******************************************************/

extern uint32_t cntUs;
extern uint32_t cntMs;

/* Exported macro *********************************************************/

/* Exported function prototypes *******************************************/

void APM_EVAL_DelayInit(void);

/* Delay*/
void APM_EVAL_DelayMs(__IO uint32_t nms);
void APM_EVAL_DelayUs(__IO uint32_t nus);

#ifdef __cplusplus
}
#endif

#endif
