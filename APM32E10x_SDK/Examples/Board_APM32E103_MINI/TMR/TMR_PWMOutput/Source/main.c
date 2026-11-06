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

/** @addtogroup TMR_PWMOutput
  @{
  */

/** @defgroup TMR_PWMOutput_Variables Variables
  @{
  */

volatile uint32_t tick = 0;

/**@} end of group TMR_PWMOutput_Variables */

/** @defgroup TMR_PWMOutput_Functions Functions
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

    GPIO_Config_T GPIO_ConfigStruct;
    TMR_BaseConfig_T TMR_TimeBaseStruct;
    TMR_OCConfig_T OCcongigStruct;

    RCM_EnableAPB2PeriphClock((RCM_APB2_PERIPH_T)(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_TMR1 | RCM_APB2_PERIPH_GPIOB));

    GPIO_ConfigStruct.pin = GPIO_PIN_13;
    GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOB, &GPIO_ConfigStruct);

    GPIO_ConfigStruct.pin = GPIO_PIN_8;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);

    TMR_TimeBaseStruct.clockDivision = TMR_CLOCK_DIV_1;
    TMR_TimeBaseStruct.countMode = TMR_COUNTER_MODE_UP;
    TMR_TimeBaseStruct.division = 119;
    TMR_TimeBaseStruct.period = 999;
    TMR_ConfigTimeBase(TMR1, &TMR_TimeBaseStruct);

    OCcongigStruct.idleState = TMR_OC_IDLE_STATE_RESET;
    OCcongigStruct.mode = TMR_OC_MODE_PWM1;
    OCcongigStruct.nIdleState = TMR_OC_NIDLE_STATE_RESET;
    OCcongigStruct.nPolarity = TMR_OC_NPOLARITY_HIGH;
    OCcongigStruct.outputNState = TMR_OC_NSTATE_ENABLE;
    OCcongigStruct.outputState = TMR_OC_STATE_ENABLE;
    OCcongigStruct.polarity = TMR_OC_POLARITY_HIGH;
    OCcongigStruct.pulse = 300;
    TMR_ConfigOC1(TMR1, &OCcongigStruct);

    TMR_ConfigOC1Preload(TMR1, TMR_OC_PRELOAD_ENABLE);
    TMR_EnableAutoReload(TMR1);
    TMR_Enable(TMR1);
    TMR_EnablePWMOutputs(TMR1);
    while (1)
    {
    }
}

/**@} end of group TMR_PWMOutput_Functions */
/**@} end of group TMR_PWMOutput */
/**@} end of group Examples */
