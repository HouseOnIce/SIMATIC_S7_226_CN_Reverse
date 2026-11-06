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

/** @addtogroup TMR_ExtTriggerSynchro
  @{
*/

/** @defgroup TMR_ExtTriggerSynchro_Functions Functions
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
    TMR_ICConfig_T ICConfig;

    /* Enable peripheral clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR1);
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2 | RCM_APB1_PERIPH_TMR3);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    /* Configure TMR1, TMR2, TMR3 channel 1 GPIO */
    GPIO_ConfigStruct.pin = GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);

    /* TMR1 Input Capture mode */
    ICConfig.channel = TMR_CHANNEL_2;
    ICConfig.polarity = TMR_IC_POLARITY_RISING;
    ICConfig.selection = TMR_IC_SELECTION_DIRECT_TI;
    ICConfig.prescaler = TMR_IC_PSC_1;
    ICConfig.filter = 0x00;
    TMR_ConfigIC(TMR1, &ICConfig);

    /* config TMR1,TMR2,TMR3 */
    TMR_TimeBaseStruct.clockDivision = TMR_CLOCK_DIV_1;
    TMR_TimeBaseStruct.countMode = TMR_COUNTER_MODE_UP;
    TMR_TimeBaseStruct.division = 0;
    TMR_TimeBaseStruct.repetitionCounter = 0;

    /* TMR1 frequency = 72MHz / 2 / (71 + 1) = 500KHz */
    TMR_TimeBaseStruct.period = 71;
    TMR_ConfigTimeBase(TMR1, &TMR_TimeBaseStruct);

    /* TMR2 frequency = 72MHz / 2 / (95 + 1) = 375KHz */
    TMR_TimeBaseStruct.period = 95;
    TMR_ConfigTimeBase(TMR2, &TMR_TimeBaseStruct);

    /* TMR3 frequency = 72MHz / 2 / (191 + 1) = 187KHz */
    TMR_TimeBaseStruct.period = 191;
    TMR_ConfigTimeBase(TMR3, &TMR_TimeBaseStruct);

    TMR_ConfigOCStructInit(&OCcongigStruct);

    /* Configure TMR1, TMR2, TMR3 channel 1 */
    OCcongigStruct.mode = TMR_OC_MODE_TOGGLE;
    OCcongigStruct.outputState = TMR_OC_STATE_ENABLE;
    OCcongigStruct.polarity = TMR_OC_POLARITY_HIGH;
    OCcongigStruct.idleState = TMR_OC_IDLE_STATE_SET;
    OCcongigStruct.outputNState = TMR_OC_NSTATE_DISABLE;
    OCcongigStruct.nPolarity = TMR_OC_NPOLARITY_HIGH;
    OCcongigStruct.nIdleState = TMR_OC_NIDLE_STATE_RESET;

    TMR_ConfigOC1(TMR1, &OCcongigStruct);
    TMR_ConfigOC1(TMR2, &OCcongigStruct);
    TMR_ConfigOC1(TMR3, &OCcongigStruct);

    /* Selects the Slave Mode and Input Trigger source */
    TMR_SelectSlaveMode(TMR1, TMR_SLAVE_MODE_GATED);
    TMR_SelectInputTrigger(TMR1, TMR_TRIGGER_SOURCE_TI2FP2);

    /* Enable TMR1 the Master Slave Mode */
    TMR_EnableMasterSlaveMode(TMR1);
    TMR_SelectOutputTrigger(TMR1, TMR_TRGO_SOURCE_ENABLE);

    /* Selects the Slave Mode and Input Trigger source */
    TMR_SelectSlaveMode(TMR2, TMR_SLAVE_MODE_GATED);
    TMR_SelectInputTrigger(TMR2, TMR_TRIGGER_SOURCE_ITR0);

    /* Enable TMR2 the Master Slave Mode */
    TMR_EnableMasterSlaveMode(TMR2);
    TMR_SelectOutputTrigger(TMR2, TMR_TRGO_SOURCE_ENABLE);

    /* Selects the Slave Mode and Input Trigger source */
    TMR_SelectSlaveMode(TMR3, TMR_SLAVE_MODE_GATED);
    TMR_SelectInputTrigger(TMR3, TMR_TRIGGER_SOURCE_ITR1);

    /* Enable the specified TMR peripheral */
    TMR_Enable(TMR1);
    TMR_Enable(TMR2);
    TMR_Enable(TMR3);

    TMR_EnablePWMOutputs(TMR1);

    while(1)
    {
    }
}

/**@} end of group TMR_ExtTriggerSynchro_Functions */
/**@} end of group TMR_ExtTriggerSynchro */
/**@} end of group Examples */
