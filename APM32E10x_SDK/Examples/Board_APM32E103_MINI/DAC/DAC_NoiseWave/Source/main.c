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

/** @addtogroup DAC_NoiseWave
  @{
  */

/** @defgroup DAC_NoiseWave_Functions Functions
  @{
  */

void RCM_Configuration(void);
void GPIO_Configuration(void);

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

    DAC_Config_T DAC_ConfigStruct;

    RCM_Configuration();
    GPIO_Configuration();

    DAC_ConfigStruct.trigger = DAC_TRIGGER_SOFT;
    DAC_ConfigStruct.waveGeneration = DAC_WAVE_GENERATION_NOISE;
    DAC_ConfigStruct.maskAmplitudeSelect = DAC_LFSR_MASK_BIT11_1;
    DAC_ConfigStruct.outputBuffer = DAC_OUTPUT_BUFFER_ENBALE;
    DAC_Config((uint32_t)DAC_CHANNEL_1, &DAC_ConfigStruct);

    DAC_Enable(DAC_CHANNEL_1);

    DAC_ConfigChannel1Data(DAC_ALIGN_12BIT_L, 0XFFF0);

    while (1)
    {
        DAC_EnableSoftwareTrigger(DAC_CHANNEL_1);
    }
}

/*!
 * @brief       Configures the different system clocks
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void RCM_Configuration(void)
{
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_DAC);
}
/*!
 * @brief       Configures the different GPIO ports
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void GPIO_Configuration(void)
{
    GPIO_Config_T GPIO_ConfigStruct;

    GPIO_ConfigStruct.pin = GPIO_PIN_4;
    GPIO_ConfigStruct.mode = GPIO_MODE_ANALOG;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);
}

/**@} end of group DAC_NoiseWave_Functions */
/**@} end of group DAC_NoiseWave */
/**@} end of group Examples */
