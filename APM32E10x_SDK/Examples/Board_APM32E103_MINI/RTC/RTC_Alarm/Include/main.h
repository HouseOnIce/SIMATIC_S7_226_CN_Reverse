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
#include "apm32e10x_rtc.h"
#include "apm32e10x_rcm.h"
#include "apm32e10x_pmu.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup RTC_Alarm
  @{
  */

/** @defgroup RTC_Alarm_Macros Macros
  @{
*/

/**@} end of group RTC_Alarm_Macros */

/** @defgroup RTC_Alarm_Variables Variables
  @{
*/

extern uint32_t systick;

/**@} end of group RTC_Alarm_Variables */

/** @defgroup RTC_Alarm_Functions Functions
  @{
*/
void RTC_Isr(void);
void EINT1_Isr(void);

/**@} end of group RTC_Alarm_Functions */
/**@} end of group RTC_Alarm */
/**@} end of group Examples */

#endif
