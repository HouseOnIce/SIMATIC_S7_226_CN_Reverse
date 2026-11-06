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

/** @addtogroup Examples
  @{
*/

/** @addtogroup TMR_6Steps
  @{
*/

/** @defgroup TMR_6Steps_Variables Variables
  @{
*/

uint8_t PWMStep;

/**@} end of group TMR_6Steps_Variables */

/** @defgroup TMR_6Steps_Functions Functions
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

    Delay_Init();

    GPIO_Config_T GPIO_ConfigStruct;
    TMR_BaseConfig_T TMR_TimeBaseStruct;
    TMR_OCConfig_T OCcongigStruct;
    TMR_BDTConfig_T BDTConfig;


    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR1);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_GPIOB);

    /* Config TMR1 GPIO for complementary output PWM */
    /* CH1->PA8, CH2->PA9, CH3->PA10 */
    GPIO_ConfigStruct.pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);

    /* BKIN->PB12, CH1N->PB13, CH2N->PB14, CH3N->PB15 */
    GPIO_ConfigStruct.pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_Config(GPIOB, &GPIO_ConfigStruct);

    /* config TMR1 */
    TMR_TimeBaseStruct.clockDivision = TMR_CLOCK_DIV_1;
    TMR_TimeBaseStruct.countMode = TMR_COUNTER_MODE_UP;
    TMR_TimeBaseStruct.division = 119;
    TMR_TimeBaseStruct.period = 2047;
    TMR_TimeBaseStruct.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR1, &TMR_TimeBaseStruct);

    /* Configure channel */
    OCcongigStruct.mode = TMR_OC_MODE_TIMING;
    OCcongigStruct.idleState = TMR_OC_IDLE_STATE_SET;
    OCcongigStruct.outputState = TMR_OC_STATE_ENABLE;
    OCcongigStruct.polarity = TMR_OC_POLARITY_HIGH;
    OCcongigStruct.nIdleState = TMR_OC_NIDLE_STATE_SET;
    OCcongigStruct.outputNState = TMR_OC_NSTATE_ENABLE;
    OCcongigStruct.nPolarity = TMR_OC_NPOLARITY_HIGH;
    OCcongigStruct.pulse = 1023;

    TMR_ConfigOC1(TMR1, &OCcongigStruct);

    OCcongigStruct.pulse = 511;
    TMR_ConfigOC2(TMR1, &OCcongigStruct);

    OCcongigStruct.pulse = 255;
    TMR_ConfigOC3(TMR1, &OCcongigStruct);

    /* Configures the Break feature, dead time, Lock level, the IMOS */
    BDTConfig.RMOS = TMR_RMOS_STATE_ENABLE;
    BDTConfig.IMOS = TMR_IMOS_STATE_ENABLE;
    BDTConfig.lockLevel = TMR_LOCK_LEVEL_OFF;
    BDTConfig.deadTime = 0x01;
    BDTConfig.BRKState = TMR_BRK_STATE_ENABLE;
    BDTConfig.BRKPolarity = TMR_BRK_POLARITY_HIGH;
    BDTConfig.automaticOutput = TMR_AUTOMATIC_OUTPUT_ENABLE;
    TMR_ConfigBDT(TMR1, &BDTConfig);

    /* Timer1 Commutation Interrupt */
    TMR_EnableInterrupt(TMR1, TMR_INT_COM);
    NVIC_EnableIRQRequest(TMR1_TRG_COM_IRQn, 0, 0);

    /* Enable Capture Compare Preload Control */
    TMR_EnableCCPreload(TMR1);

    TMR_Enable(TMR1);
    TMR_EnablePWMOutputs(TMR1);

    while (1)
    {
        /* Generate the TMR1 Commutation event by software every 100 milliseconds */
        Delay_ms(100);
        TMR_GenerateEvent(TMR1, TMR_EVENT_COM);
    }
}

/*!
 * @brief       TMR1 Trigger and Commutation IRQ Handler
 *
 * @param       None
 *
 * @retval      None
 */
void TMR1_TRG_COM_IRQHandler(void)
{
    /* Clear the TMR1 COM pending bit */
    TMR_ClearIntFlag(TMR1, TMR_INT_COM);

    PWMStep++;

    switch (PWMStep)
    {
        case 1:
            /* configuration: Channel1 and Channel2 PWM1 Mode */
            TMR_SelectOCxMode(TMR1, TMR_CHANNEL_1, TMR_OC_MODE_PWM1);
            TMR_SelectOCxMode(TMR1, TMR_CHANNEL_2, TMR_OC_MODE_PWM1);

            /* configuration: Channel1 */
            TMR_EnableCCxChannel(TMR1, TMR_CHANNEL_1);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_1);

            /* configuration: Channel2 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_2);
            TMR_EnableCCxNChannel(TMR1, TMR_CHANNEL_2);

            /* configuration: Channel3 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_3);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_3);
            break;

        case 2:
            /* configuration: Channel2 and Channel3 PWM1 Mode */
            TMR_SelectOCxMode(TMR1, TMR_CHANNEL_2, TMR_OC_MODE_PWM1);
            TMR_SelectOCxMode(TMR1, TMR_CHANNEL_3, TMR_OC_MODE_PWM1);

            /* configuration: Channel1 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_1);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_1);

            /* configuration: Channel2 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_2);
            TMR_EnableCCxNChannel(TMR1, TMR_CHANNEL_2);

            /* configuration: Channel3 */
            TMR_EnableCCxChannel(TMR1, TMR_CHANNEL_3);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_3);
            break;

        case 3:
            /* configuration: Channel3 and Channel1 PWM1 Mode */
            TMR_SelectOCxMode(TMR1, TMR_CHANNEL_3, TMR_OC_MODE_PWM1);
            TMR_SelectOCxMode(TMR1, TMR_CHANNEL_1, TMR_OC_MODE_PWM1);

            /* configuration: Channel1 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_1);
            TMR_EnableCCxNChannel(TMR1, TMR_CHANNEL_1);

            /* configuration: Channel2 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_2);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_2);

            /* configuration: Channel3 */
            TMR_EnableCCxChannel(TMR1, TMR_CHANNEL_3);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_3);
            break;

        case 4:
            /* configuration: Channel1 and Channel2 PWM1 Mode */
            TMR_SelectOCxMode(TMR1, TMR_CHANNEL_1, TMR_OC_MODE_PWM1);
            TMR_SelectOCxMode(TMR1, TMR_CHANNEL_2, TMR_OC_MODE_PWM1);

            /* configuration: Channel1 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_1);
            TMR_EnableCCxNChannel(TMR1, TMR_CHANNEL_1);

            /* configuration: Channel2 */
            TMR_EnableCCxChannel(TMR1, TMR_CHANNEL_2);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_2);

            /* configuration: Channel3 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_3);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_3);
            break;

        case 5:
            /* configuration: Channel2 and Channel3 PWM1 Mode */
            TMR_SelectOCxMode(TMR1, TMR_CHANNEL_2, TMR_OC_MODE_PWM1);
            TMR_SelectOCxMode(TMR1, TMR_CHANNEL_3, TMR_OC_MODE_PWM1);

            /* configuration: Channel1 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_1);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_1);

            /* configuration: Channel2 */
            TMR_EnableCCxChannel(TMR1, TMR_CHANNEL_2);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_2);

            /* configuration: Channel3 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_3);
            TMR_EnableCCxNChannel(TMR1, TMR_CHANNEL_3);
            break;

        case 6:
            /* configuration: Channel3 and Channel1 PWM1 Mode */
            TMR_SelectOCxMode(TMR1, TMR_CHANNEL_3, TMR_OC_MODE_PWM1);
            TMR_SelectOCxMode(TMR1, TMR_CHANNEL_1, TMR_OC_MODE_PWM1);

            /* configuration: Channel1 */
            TMR_EnableCCxChannel(TMR1, TMR_CHANNEL_1);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_1);

            /* configuration: Channel2 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_2);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_2);

            /* configuration: Channel3 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_3);
            TMR_EnableCCxNChannel(TMR1, TMR_CHANNEL_3);

            PWMStep = 0;
            break;

        default:
            PWMStep = 0;
            break;
    }
}

/**@} end of group TMR_6Steps_Functions */
/**@} end of group TMR_6Steps */
/**@} end of group Examples */
