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

/** @addtogroup TMR_TimeBase
  @{
  */

/** @defgroup TMR_TimeBase_Variables Variables
  @{
  */

volatile uint32_t tick = 0;

/**@} end of group TMR_TimeBase_Variables */

/** @defgroup TMR_TimeBase_Functions Functions
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

    TMR_BaseConfig_T TMR_BaseConfigStruct;

    APM_MINI_LEDInit(LED2);

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR5);

    TMR_BaseConfigStruct.clockDivision = TMR_CLOCK_DIV_1;
    TMR_BaseConfigStruct.countMode = TMR_COUNTER_MODE_UP;
    TMR_BaseConfigStruct.division = 119;
    TMR_BaseConfigStruct.period = 999;
    TMR_BaseConfigStruct.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR5, &TMR_BaseConfigStruct);

    TMR_EnableInterrupt(TMR5, TMR_INT_UPDATE);
    NVIC_EnableIRQRequest(TMR5_IRQn, 0, 0);

    TMR_Enable(TMR5);

    while (1)
    {
        APM_MINI_LEDToggle(LED2);

        Delay();
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
    tick = 0;

    while (tick < 1000);
}

/**@} end of group TMR_TimeBase_Functions */
/**@} end of group TMR_TimeBase */
/**@} end of group Examples */
