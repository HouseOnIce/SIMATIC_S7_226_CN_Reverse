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

/** @addtogroup TMR_TMR1DMABurst
  @{
  */

/** @defgroup TMR_TMR1DMABurst_Variables Variables
  @{
  */

uint16_t ConfigBuf[5] = {999, 0, 800, 700, 600};

/**@} end of group TMR_TMR1DMABurst_Variables*/

/** @defgroup TMR_TMR1DMABurst_Functions Functions
  @{
  */

void GPIO_Init(void);
void DMA_Init(void);

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

    TMR_BaseConfig_T TMR_TimeBaseStruct;
    TMR_OCConfig_T OCcongigStruct;

    GPIO_Init();

    DMA_Init();

    /* Enable TMR1 clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR1);

    /* TMR Base configuration */
    TMR_TimeBaseStruct.clockDivision = TMR_CLOCK_DIV_1;
    TMR_TimeBaseStruct.countMode = TMR_COUNTER_MODE_UP;
    TMR_TimeBaseStruct.division = 119;
    TMR_TimeBaseStruct.period = 9999;
    TMR_ConfigTimeBase(TMR1, &TMR_TimeBaseStruct);

    /* TMR Channel 1 Configuration in PWM mode */
    OCcongigStruct.idleState = TMR_OC_IDLE_STATE_RESET;
    OCcongigStruct.mode = TMR_OC_MODE_PWM1;
    OCcongigStruct.nIdleState = TMR_OC_NIDLE_STATE_RESET;
    OCcongigStruct.nPolarity = TMR_OC_NPOLARITY_HIGH;
    OCcongigStruct.outputNState = TMR_OC_NSTATE_ENABLE;
    OCcongigStruct.outputState = TMR_OC_STATE_ENABLE;
    OCcongigStruct.polarity = TMR_OC_POLARITY_HIGH;

    /* TMR CH1 config */
    OCcongigStruct.pulse = 3000;
    TMR_ConfigOC1(TMR1, &OCcongigStruct);

    /* TMR CH2 config */
    OCcongigStruct.pulse = 3000;
    TMR_ConfigOC2(TMR1, &OCcongigStruct);

    /* TMR CH3 config */
    OCcongigStruct.pulse = 3000;
    TMR_ConfigOC3(TMR1, &OCcongigStruct);

    /* TMR1 DMA Base register and DMA Burst Length Config */
    TMR_ConfigDMA(TMR1, TMR_DMA_BASE_AUTORLD, TMR_DMA_BURSTLENGTH_5TRANSFERS);

    /* TMR1 Update DMA Request enable */
    TMR_EnableDMASource(TMR1, TMR_DMA_SOURCE_UPDATE);

    /* TMR1 enable */
    TMR_Enable(TMR1);

    /* TMR1 PWM Outputs Enable */
    TMR_EnablePWMOutputs(TMR1);

    while (1)
    {
    }
}

/*!
 * @brief       GPIO Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void GPIO_Init(void)
{
    GPIO_Config_T GPIO_ConfigStruct;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_GPIOB);

    /* TMR1_CH1N(PB13)/TMR1_CH2N(PB14)/TMR1_CH3N(PB15) GPIO Config */
    GPIO_ConfigStruct.pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOB, &GPIO_ConfigStruct);

    /* TMR1_CH1(PA8)/TMR1_CH2(PA9)/TMR1_CH3(PA10) GPIO Config */
    GPIO_ConfigStruct.pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);
}

/*!
 * @brief     DMA Init
 *
 * @param     None
 *
 * @retval    None
 */
void DMA_Init(void)
{
    DMA_Config_T dmaConfig;

    /* Enable DMA Clock */
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA1);

    /* DMA config */
    dmaConfig.peripheralBaseAddr = (uint32_t)&TMR1->DMADDR;
    dmaConfig.memoryBaseAddr = (uint32_t)ConfigBuf;
    dmaConfig.dir = DMA_DIR_PERIPHERAL_DST;
    dmaConfig.bufferSize = 5;
    dmaConfig.peripheralInc = DMA_PERIPHERAL_INC_DISABLE;
    dmaConfig.memoryInc = DMA_MEMORY_INC_ENABLE;
    dmaConfig.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_HALFWORD;
    dmaConfig.memoryDataSize = DMA_MEMORY_DATA_SIZE_HALFWORD;
    dmaConfig.loopMode = DMA_MODE_NORMAL;
    dmaConfig.priority = DMA_PRIORITY_HIGH;
    dmaConfig.M2M = DMA_M2MEN_DISABLE;

    /* Config TMR1 UP DMA channel */
    DMA_Config(DMA1_Channel5, &dmaConfig);

    /* Enable DMA */
    DMA_Enable(DMA1_Channel5);
}

/**@} end of group TMR_TMR1DMABurst_Functions */
/**@} end of group TMR_TMR1DMABurst */
/**@} end of group Examples */
