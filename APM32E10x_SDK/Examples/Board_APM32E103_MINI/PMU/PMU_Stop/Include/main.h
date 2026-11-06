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

/* Includes */
#include "Board.h"
#include "apm32e10x_pmu.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup PMU_Stop
  @{
  */

/** @defgroup PMU_Stop_Functions Functions
  @{
  */

void Eint1_Isr(void);
void Usart_Init(void);
void Eint0_Isr(void);

/**@} end of group PMU_Stop_Functions */
/**@} end of group PMU_Stop */
/**@} end of group Examples */

#endif
