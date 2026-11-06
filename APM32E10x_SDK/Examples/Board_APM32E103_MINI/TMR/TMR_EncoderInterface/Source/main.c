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

/** @addtogroup TMR_EncoderInterface
  @{
  */

/** @defgroup TMR_EncoderInterface_Macros Macros
  @{
  */

/* printf function configs to USART1*/
#define DEBUG_USART  USART2

/**@} end of group TMR_EncoderInterface_Macros*/

/** @defgroup TMR_EncoderInterface_Variables Variables
  @{
  */

uint16_t encoderNum = 0;
volatile uint32_t tick = 0;

/**@} end of group TMR_EncoderInterface_Variables */

/** @defgroup TMR_EncoderInterface_Functions Functions
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

    GPIO_Config_T    gpioConfig;
    USART_Config_T   usartConfig;
    TMR_BaseConfig_T baseConfig;
    TMR_ICConfig_T   ICConfig;

    APM_MINI_LEDInit(LED2);
    APM_MINI_LEDOff(LED2);

    /* USART2 Tx PA2 Rx PA3 */
    usartConfig.baudRate = 115200;
    usartConfig.mode     = USART_MODE_TX_RX;
    usartConfig.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    usartConfig.parity   = USART_PARITY_NONE;
    usartConfig.stopBits =  USART_STOP_BIT_1;
    usartConfig.wordLength = USART_WORD_LEN_8B;
    APM_MINI_COMInit(COM2, &usartConfig);

    /* Enable GPIOA Periph Clock */
    RCM_EnableAHBPeriphClock(RCM_APB2_PERIPH_GPIOA);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_AFIO);

    /* Config GPIO */
    gpioConfig.pin = GPIO_PIN_8 | GPIO_PIN_9;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.mode = GPIO_MODE_IN_PU;
    GPIO_Config(GPIOA, &gpioConfig);

    /* Config GPIOA  Alernate Function for TMR1  */
    GPIO_ConfigPinLock(GPIOA, GPIO_PIN_SOURCE_8);
    GPIO_ConfigPinLock(GPIOA, GPIO_PIN_SOURCE_9);
    GPIO_ConfigPinRemap(GPIO_PARTIAL_REMAP_TMR1);

    /* Enable TMR1,TMR2 Periph Clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR1);
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2);

    /* Config TMR1 */
    baseConfig.clockDivision = TMR_CLOCK_DIV_1;
    baseConfig.countMode = TMR_COUNTER_MODE_UP;
    baseConfig.division = 119;
    baseConfig.period = 0x0008;
    baseConfig.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR1, &baseConfig);

    /* Config TMR2 */
    baseConfig.clockDivision = TMR_CLOCK_DIV_1;
    baseConfig.countMode = TMR_COUNTER_MODE_UP;
    baseConfig.division = 119;
    baseConfig.period = 999;
    baseConfig.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR2, &baseConfig);

    /* Encoder */
    TMR_ConfigEncodeInterface(TMR1, TMR_ENCODER_MODE_TI1, TMR_IC_POLARITY_RISING, TMR_IC_POLARITY_RISING);

    /* Input Capture */
    ICConfig.channel = TMR_CHANNEL_1;
    ICConfig.polarity = TMR_IC_POLARITY_RISING;
    ICConfig.selection = TMR_IC_SELECTION_DIRECT_TI;
    ICConfig.prescaler = TMR_IC_PSC_1;
    ICConfig.filter = 0;
    TMR_ConfigIC(TMR1, &ICConfig);

    ICConfig.channel = TMR_CHANNEL_2;
    TMR_ConfigIC(TMR1, &ICConfig);

    /* Counter */
    TMR_ConfigCounter(TMR1, 0x0004);
    TMR_ClearStatusFlag(TMR1, TMR_FLAG_UPDATE);
    TMR_ClearStatusFlag(TMR2, TMR_FLAG_UPDATE);

    /* Enable TMR1,TMR2 Interrupt */
    TMR_EnableInterrupt(TMR1, TMR_INT_UPDATE);
    TMR_EnableInterrupt(TMR2, TMR_INT_UPDATE);
    NVIC_EnableIRQRequest(TMR1_UP_IRQn, 0, 0);
    NVIC_EnableIRQRequest(TMR2_IRQn, 0, 0);

    /* Enable TMR1,TMR2 */
    TMR_Enable(TMR1);
    TMR_Enable(TMR2);

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
 */
void Delay(void)
{
    tick = 0;
    while (tick < 500)
    {
    }
}

#if defined (__CC_ARM) || defined (__ICCARM__) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       ch:  The characters that need to be send.
*
* @param       *f:  pointer to a FILE that can recording all information
*              needed to control a stream
*
* @retval      The characters that need to be send.
*
* @note
*/
int fputc(int ch, FILE* f)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return (ch);
}

#elif defined (__GNUC__)

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       ch:  The characters that need to be send.
*
* @retval      The characters that need to be send.
*
* @note
*/
int __io_putchar(int ch)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, ch);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return ch;
}

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       file:  Meaningless in this function.
*
* @param       *ptr:  Buffer pointer for data to be sent.
*
* @param       len:  Length of data to be sent.
*
* @retval      The characters that need to be send.
*
* @note
*/
int _write(int file, char* ptr, int len)
{
	UNUSED(file);
    int i;
    for (i = 0; i < len; i++)
    {
        __io_putchar(*ptr++);
    }

    return len;
}

#else
#warning Not supported compiler type
#endif

/**@} end of group TMR_EncoderInterface_Functions */
/**@} end of group TMR_EncoderInterface */
/**@} end of group Examples */
