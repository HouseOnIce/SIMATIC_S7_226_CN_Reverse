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

/** @addtogroup TMR_OCToggle
  @{
  */

/** @defgroup TMR_OCToggle_Variables Variables
  @{
  */

uint16_t CC1_Val = (72000000 / 5000);
uint16_t CC2_Val = (72000000 / 10000);
uint16_t CC3_Val = (72000000 / 50000);
uint16_t CC4_Val = (72000000 / 100000);

/**@} end of group TMR_OCToggle_Variables */

/** @defgroup TMR_OCToggle_Functions Functions
  @{
  */

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

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    APM_MINI_LEDInit(LED2);

    /* Config TMR2 GPIO for output */
    GPIO_ConfigStruct.pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);

    /* config TMR2 */
    TMR_TimeBaseStruct.clockDivision = TMR_CLOCK_DIV_1;
    TMR_TimeBaseStruct.countMode = TMR_COUNTER_MODE_UP;
    TMR_TimeBaseStruct.division = 0;
    TMR_TimeBaseStruct.period = 0xFFFF;
    TMR_ConfigTimeBase(TMR2, &TMR_TimeBaseStruct);

    /* Configure channel1 */
    TMR_ConfigOCStructInit(&OCcongigStruct);

    OCcongigStruct.mode = TMR_OC_MODE_TOGGLE;
    OCcongigStruct.outputState = TMR_OC_STATE_ENABLE;
    OCcongigStruct.polarity = TMR_OC_POLARITY_LOW;

    /* Configure channel1 */
    OCcongigStruct.pulse = CC1_Val;
    TMR_ConfigOC1(TMR2, &OCcongigStruct);
    TMR_ConfigOC1Preload(TMR2, TMR_OC_PRELOAD_DISABLE);

    /* Configure channel2 */
    OCcongigStruct.pulse = CC2_Val;
    TMR_ConfigOC2(TMR2, &OCcongigStruct);
    TMR_ConfigOC2Preload(TMR2, TMR_OC_PRELOAD_DISABLE);

    /* Configure channel3 */
    OCcongigStruct.pulse = CC3_Val;
    TMR_ConfigOC3(TMR2, &OCcongigStruct);
    TMR_ConfigOC3Preload(TMR2, TMR_OC_PRELOAD_DISABLE);

    /* Configure channel4 */
    OCcongigStruct.pulse = CC4_Val;
    TMR_ConfigOC4(TMR2, &OCcongigStruct);
    TMR_ConfigOC4Preload(TMR2, TMR_OC_PRELOAD_DISABLE);

    APM_MINI_LEDOn(LED2);

    NVIC_EnableIRQRequest(TMR2_IRQn, 0, 0);

    /* start generate four different signals */
    TMR_Enable(TMR2);

    TMR_EnableInterrupt(TMR2, TMR_INT_CC1 | TMR_INT_CC2 | TMR_INT_CC3 | TMR_INT_CC4);
    while (1)
    {
    }
}
/*!
 * @brief       TMR2 Handler
 *
 * @param       None
 *
 * @retval      None
 */
void TMR2_IRQHandler(void)
{
    uint32_t capValue = 0;

    /* TMR2_CH1 toggling with frequency = 2.5KHz */
    if (TMR_ReadIntFlag(TMR2, TMR_INT_CC1))
    {
        /* Clear the TMR2 CC1 pending bit */
        TMR_ClearIntFlag(TMR2, TMR_INT_CC1);
        capValue = TMR_ReadCaputer1(TMR2);
        TMR_ConfigCompare1(TMR2, capValue + CC1_Val);
    }

    /* TMR2_CH2 toggling with frequency = 5KHz */
    if (TMR_ReadIntFlag(TMR2, TMR_INT_CC2))
    {
        /* Clear the TMR2 CC2 pending bit */
        TMR_ClearIntFlag(TMR2, TMR_INT_CC2);
        capValue = TMR_ReadCaputer2(TMR2);
        TMR_ConfigCompare2(TMR2, capValue + CC2_Val);
    }

    /* TMR2_CH3 toggling with frequency = 25KHz */
    if (TMR_ReadIntFlag(TMR2, TMR_INT_CC3))
    {
        /* Clear the TMR2 CC3 pending bit */
        TMR_ClearIntFlag(TMR2, TMR_INT_CC3);
        capValue = TMR_ReadCaputer3(TMR2);
        TMR_ConfigCompare3(TMR2, capValue + CC3_Val);
    }

    /* TMR2_CH4 toggling with frequency = 50KHz */
    if (TMR_ReadIntFlag(TMR2, TMR_INT_CC4))
    {
        /* Clear the TMR2 CC4 pending bit */
        TMR_ClearIntFlag(TMR2, TMR_INT_CC4);
        capValue = TMR_ReadCaputer4(TMR2);
        TMR_ConfigCompare4(TMR2, capValue + CC4_Val);
    }
}

/**@} end of group TMR_OCToggle_Functions */
/**@} end of group TMR_OCToggle */
/**@} end of group Examples */
