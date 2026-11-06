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
#include "stdio.h"
/** @addtogroup Examples
  @{
*/

/** @addtogroup USART_Interrupt
  @{
*/

/** @defgroup USART_Interrupt_Macros Macros
  @{
*/

#define DEBUG_USART USART2
/**@} end of group USART_Interrupt_Macros */

/** @defgroup USART_Interrupt_Variables Variables
  @{
*/

uint8_t txBuf[] = "Hello USART2 \r\n";
uint8_t count = 0;
/**@} end of group USART_Interrupt_Variables */

/** @defgroup USART_Interrupt_Functions Functions
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

    USART_Config_T USART_ConfigStruct;

    APM_MINI_LEDInit(LED2);

    RCM_EnableAPB2PeriphClock((RCM_APB2_PERIPH_T)(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_USART1));
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_USART2);

    USART_ConfigStruct.baudRate = 115200;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode = USART_MODE_TX_RX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;
    APM_MINI_COMInit(COM1, &USART_ConfigStruct);
    APM_MINI_COMInit(COM2, &USART_ConfigStruct);

    USART_EnableInterrupt(USART1, USART_INT_TXBE);
    USART_EnableInterrupt(USART2, USART_INT_RXBNE);

    NVIC_EnableIRQRequest(USART1_IRQn, 2, 0);
    NVIC_EnableIRQRequest(USART2_IRQn, 1, 0);

    while (1)
    {
    }
}

/*!
 * @brief       USART1_Interrupt
 *
 * @param       None
 *
 * @retval      None
 *
 */
void USART1_Isr(void)
{
    USART_TxData(USART1, txBuf[count]);
    count++;
    if (count == sizeof(txBuf))
    {
        count = 0;
    }

}

/*!
 * @brief       USART2_Interrupt
 *
 * @param       None
 *
 * @retval      None
 *
 */
void USART2_Isr(void)
{
    uint8_t dat;
    dat = (uint8_t)USART_RxData(USART2);
    printf("%c", dat);
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

/**@} end of group USART_Interrupt_Functions */
/**@} end of group USART_Interrupt */
/**@} end of group Examples */
