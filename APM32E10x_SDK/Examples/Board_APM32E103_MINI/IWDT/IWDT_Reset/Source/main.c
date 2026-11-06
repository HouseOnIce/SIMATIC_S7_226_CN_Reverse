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

/** @addtogroup IWDT_Reset
  @{
  */

/** @defgroup IWDT_Reset_Variables Variables
  @{
*/

volatile uint32_t TimingDelay = 0;

/**@} end of group IWDT_Reset_Variables */

/** @defgroup IWDT_Reset_Functions Functions
  @{
*/

void Delay(void);

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
    APM_MINI_LEDInit(LED3);

    APM_MINI_PBInit(BUTTON_KEY1, BUTTON_MODE_EINT);

    SysTick_Config(SystemCoreClock / 1000);

    if (RCM_ReadStatusFlag(RCM_FLAG_IWDTRST) == SET)
    {
        APM_MINI_LEDOn(LED3);
        RCM_ClearStatusFlag();
    }
    else
    {
        APM_MINI_LEDOff(LED3);
    }

    IWDT_EnableWriteAccess();
    IWDT_ConfigDivider(IWDT_DIVIDER_32);
    IWDT_ConfigReload(500);
    IWDT_Refresh();
    IWDT_Enable();

    while (1)
    {
        APM_MINI_LEDToggle(LED2);

        Delay();

        IWDT_Refresh();
    }
}

/*!
 * @brief       Delay
 *
 * @param       None
 *
 * @retval      None
 *
 */
void Delay(void)
{
    TimingDelay = 0;
    while (TimingDelay < 200);
}

/**@} end of group IWDT_Reset_Functions */
/**@} end of group IWDT_Reset */
/**@} end of group Examples */
