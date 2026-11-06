/*!
 * @file        main.c
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

/* Includes */
#include "main.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup RTC_Second
  @{
  */

/** @defgroup RTC_Second_Functions Functions
  @{
*/

void RTC_Init(void);

/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 */
int main(void)
{
    SystemClockConfig();

    APM_MINI_LEDInit(LED2);
    RTC_Init();

    while (1)
    {

    }
}

/*!
 * @brief       RTC init
 *
 * @param       None
 *
 * @retval      None
 *
 */
void RTC_Init(void)
{
    RCM_EnableAPB1PeriphClock((RCM_APB1_PERIPH_T)RCM_APB1_PERIPH_PMU);
    PMU_EnableBackupAccess();

    RCM_EnableLSI();
    while (RCM_ReadStatusFlag(RCM_FLAG_LSIRDY) == RESET);
    RCM_ConfigRTCCLK(RCM_RTCCLK_LSI);
    RCM_EnableRTCCLK();

    RTC_WaitForSynchro();
    RTC_WaitForLastTask();

    RTC_EnableInterrupt(RTC_INT_SEC);
    RTC_WaitForLastTask();

    RTC_ConfigPrescaler(39999);
    RTC_WaitForLastTask();

    NVIC_EnableIRQRequest(RTC_IRQn, 0, 0);
}

/**@} end of group RTC_Second_Functions */
/**@} end of group RTC_Second */
/**@} end of group Examples */
