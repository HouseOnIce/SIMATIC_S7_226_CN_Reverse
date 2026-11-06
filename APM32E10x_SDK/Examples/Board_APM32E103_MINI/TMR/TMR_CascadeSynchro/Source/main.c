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
#include "Board.h"
#include "apm32e10x_tmr.h"

/** @addtogroup Examples
  @{
*/

/** @addtogroup TMR_CascadeSynchro
  @{
*/

/** @defgroup TMR_CascadeSynchro_Functions Functions
  @{
*/

/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 */
int main(void)
{
    SystemClockConfig();

    GPIO_Config_T GPIO_ConfigStruct;
    TMR_BaseConfig_T TMR_TimeBaseStruct;
    TMR_OCConfig_T OCcongigStruct;

    /* Enable peripheral clock */
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2 | RCM_APB1_PERIPH_TMR3 | RCM_APB1_PERIPH_TMR4);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_GPIOB);

    /* Configure TMR2, TMR3, TMR4 channel 1 GPIO */
    GPIO_ConfigStruct.pin = GPIO_PIN_0 | GPIO_PIN_6;
    GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);

    GPIO_ConfigStruct.pin = GPIO_PIN_6;
    GPIO_Config(GPIOB, &GPIO_ConfigStruct);

    /* configure TMR2, TMR3, TMR4 */
    TMR_TimeBaseStruct.clockDivision = TMR_CLOCK_DIV_1;
    TMR_TimeBaseStruct.countMode = TMR_COUNTER_MODE_UP;
    TMR_TimeBaseStruct.division = 0;

    /* TMR2 frequency = (72M)/ (71 + 1) = 1 MHz */
    TMR_TimeBaseStruct.period = 71;
    TMR_ConfigTimeBase(TMR2, &TMR_TimeBaseStruct);

    /* TMR3 frequency = (10M)/ (99 + 1) = 10 KHz */
    TMR_TimeBaseStruct.period = 99;
    TMR_ConfigTimeBase(TMR3, &TMR_TimeBaseStruct);

    /* TMR4 frequency = (100K)/ (9 + 1) = 1 KHz */
    TMR_TimeBaseStruct.period = 9;
    TMR_ConfigTimeBase(TMR4, &TMR_TimeBaseStruct);

    TMR_ConfigOCStructInit(&OCcongigStruct);

    /* Configure TMR2, TMR3, TMR4 channel 1 */
    OCcongigStruct.mode = TMR_OC_MODE_PWM1;
    OCcongigStruct.outputState = TMR_OC_STATE_ENABLE;
    OCcongigStruct.polarity = TMR_OC_POLARITY_HIGH;
    OCcongigStruct.pulse = 36;
    TMR_ConfigOC1(TMR2, &OCcongigStruct);

    OCcongigStruct.pulse = 50;
    TMR_ConfigOC1(TMR3, &OCcongigStruct);

    OCcongigStruct.pulse = 5;
    TMR_ConfigOC1(TMR4, &OCcongigStruct);

    /* Enable TMR2 the Master Slave Mode */
    TMR_EnableMasterSlaveMode(TMR2);
    TMR_SelectOutputTrigger(TMR2, TMR_TRGO_SOURCE_UPDATE);

    /* Selects the Slave Mode and Input Trigger source */
    TMR_SelectSlaveMode(TMR3, TMR_SLAVE_MODE_GATED);
    TMR_SelectInputTrigger(TMR3, TMR_TRIGGER_SOURCE_ITR1);

    /* Enable TMR3 the Master Slave Mode */
    TMR_EnableMasterSlaveMode(TMR3);
    TMR_SelectOutputTrigger(TMR3, TMR_TRGO_SOURCE_UPDATE);

    /* Selects the Slave Mode and Input Trigger source */
    TMR_SelectSlaveMode(TMR4, TMR_SLAVE_MODE_GATED);
    TMR_SelectInputTrigger(TMR4, TMR_TRIGGER_SOURCE_ITR2);

    /* Enable the specified TMR peripheral */
    TMR_Enable(TMR2);
    TMR_Enable(TMR3);
    TMR_Enable(TMR4);

    while (1)
    {
    }
}

/**@} end of group TMR_CascadeSynchro_Functions */
/**@} end of group TMR_CascadeSynchro */
/**@} end of group Examples */
