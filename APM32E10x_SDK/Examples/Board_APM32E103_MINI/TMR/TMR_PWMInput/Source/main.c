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

/** @addtogroup TMR_PWMInput
  @{
*/

/** @defgroup TMR_PWMInput_Macros Macros
  @{
*/

/* printf function configs to USART1 */
#define DEBUG_USART  USART1

/**@} end of group TMR_PWMInput_Macros */

/** @defgroup TMR_PWMInput_Variables Variables
  @{
*/

uint8_t  irqFlag = 0;
uint32_t IC2Value[2] = {0};
double Frequency = 0;
double dutycycle = 0;
uint32_t hightime = 0;
uint32_t Updatecnt = 0;
uint32_t pclk1 = 0;
uint32_t pclk2 = 0;


volatile uint32_t tick = 0;

/**@} end of group TMR_PWMInput_Variables */

/** @defgroup TMR_PWMInput_Functions Functions
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

    GPIO_Config_T gpioConfig;
    USART_Config_T usartConfig;
    TMR_ICConfig_T ICConfig;
    TMR_BaseConfig_T baseConfig;

    /* Init LED */
    APM_MINI_LEDInit(LED2);

    /* USART1 Tx:PA9 Rx:PA10 */
    usartConfig.baudRate = 115200;
    usartConfig.mode = USART_MODE_TX_RX;
    usartConfig.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    usartConfig.parity = USART_PARITY_NONE;
    usartConfig.stopBits = USART_STOP_BIT_1;
    usartConfig.wordLength = USART_WORD_LEN_8B;
    APM_MINI_COMInit(COM1, &usartConfig);

    /* Enable Periph Clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR1);
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR3);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    /* TMR3 channel2 for PA7 */
    gpioConfig.pin = GPIO_PIN_7;
    gpioConfig.mode = GPIO_MODE_IN_PD;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &gpioConfig);

    /* TMR3 PWM Input mode */
    ICConfig.channel = TMR_CHANNEL_2;
    ICConfig.polarity = TMR_IC_POLARITY_RISING;
    ICConfig.selection = TMR_IC_SELECTION_DIRECT_TI;
    ICConfig.prescaler = TMR_IC_PSC_1;
    ICConfig.filter = 0x00;
    TMR_ConfigPWM(TMR3, &ICConfig);

    /* Enable TMR5 Interrupt */
    TMR_Enable(TMR3);
    NVIC_EnableIRQRequest(TMR3_IRQn, 0, 0);
    TMR_EnableInterrupt(TMR3, TMR_INT_CC2);
    TMR_EnableInterrupt(TMR3, TMR_INT_CC1);
    TMR_EnableInterrupt(TMR3, TMR_INT_UPDATE);

    /* Config TMR1 */
    baseConfig.clockDivision = TMR_CLOCK_DIV_1;
    baseConfig.countMode = TMR_COUNTER_MODE_UP;
    baseConfig.division = 119;
    baseConfig.period = 999;
    baseConfig.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR1, &baseConfig);

    /* Enable TMR1 Interrupt */
    TMR_Enable(TMR1);
    TMR_EnableInterrupt(TMR1, TMR_INT_UPDATE);
    NVIC_EnableIRQRequest(TMR1_UP_IRQn, 0, 0);

    RCM_ReadPCLKFreq(&pclk1, &pclk2);
    while (1)
    {
        printf("Frequency: %0.1f Hz Duty cycle: %0.1f%%\r\n", Frequency, dutycycle);
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
int fputc(int ch, FILE *f)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /* wait for the data to be send */
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

    /* wait for the data to be send */
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
int _write(int file, char *ptr, int len)
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

/**@} end of group TMR_PWMInput_Functions */
/**@} end of group TMR_PWMInput */
/**@} end of group Examples */
