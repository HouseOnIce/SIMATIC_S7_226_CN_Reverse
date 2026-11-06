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

/** @addtogroup USART_Printf
  @{
  */

/** @defgroup USART_Printf_Variables Variables
  @{
  */

volatile uint32_t tick = 0;
uint8_t txBuf[] = "Hello USART1 \r\n";

/**@} end of group USART_Printf_Variables */

/** @defgroup USART_Printf_Functions Functions
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

    uint8_t i;

    GPIO_Config_T GPIO_ConfigStruct;
    USART_Config_T USART_ConfigStruct;

    APM_MINI_LEDInit(LED2);

    RCM_EnableAPB2PeriphClock((RCM_APB2_PERIPH_T)(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_USART1));

    GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
    GPIO_ConfigStruct.pin = GPIO_PIN_9;
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);

    USART_ConfigStruct.baudRate = 115200;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode = USART_MODE_TX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;
    USART_Config(USART1, &USART_ConfigStruct);

    USART_Enable(USART1);

    SysTick_Config(SystemCoreClock / 1000);

    while (1)
    {
        Delay();

        for (i = 0; i < sizeof(txBuf); i++)
        {
            while (USART_ReadStatusFlag(USART1, USART_FLAG_TXBE) == RESET);
            USART_TxData(USART1, txBuf[i]);
        }

        APM_MINI_LEDToggle(LED2);
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

    while (tick < 500);
}

/**@} end of group USART_Printf_Functions */
/**@} end of group USART_Printf */
/**@} end of group Examples */
