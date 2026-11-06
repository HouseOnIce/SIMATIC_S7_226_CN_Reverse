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
#include <stdio.h>
/** @addtogroup Examples
  @{
  */

/** @addtogroup PMU_Stop
  @{
  */

/** @defgroup PMU_Stop_Macros Macros
  @{
  */

#define DEBUG_USART USART1

/**@} end of group PMU_Stop_Macros */

/** @defgroup PMU_Stop_Functions Functions
  @{
  */
void Delay(uint32_t count);
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

    RCM_EnableAPB1PeriphClock((RCM_APB1_PERIPH_T)(RCM_APB1_PERIPH_PMU | RCM_APB1_PERIPH_BAKR));

    APM_MINI_LEDInit(LED2);
    APM_MINI_LEDInit(LED3);

    Usart_Init();
    /* KEY1 \ KEY2 Set */
    APM_MINI_PBInit(BUTTON_KEY1, BUTTON_MODE_EINT);
    APM_MINI_PBInit(BUTTON_KEY2, BUTTON_MODE_EINT);

    /* NVIC Priority Set */
    NVIC_ConfigPriorityGroup(NVIC_PRIORITY_GROUP_1);
    NVIC_EnableIRQRequest(EINT0_IRQn, 0, 1);
    NVIC_EnableIRQRequest(EINT1_IRQn, 1, 1);

    APM_MINI_LEDOn(LED2);
    APM_MINI_LEDOff(LED3);

    /* Enable PMU Periph Clock */
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_PMU);
    PMU_Reset();

    printf("Press KEY1 to Enter STOP Mode\r\n");
    while (1)
    {
        Delay(0x7FFFFF);

        APM_MINI_LEDToggle(LED2);
    }
}
/*!
 * @brief       Usart Init
 *
 * @param       None
 *
 * @retval      None
 *
 */
void Usart_Init(void)
{
    USART_Config_T usartConfigStruct;

    usartConfigStruct.baudRate = 115200;
    usartConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    usartConfigStruct.mode = USART_MODE_TX;
    usartConfigStruct.parity = USART_PARITY_NONE;
    usartConfigStruct.stopBits = USART_STOP_BIT_1;
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;
    APM_MINI_COMInit(COM1, &usartConfigStruct);
}

/*!
 * @brief       External lines 1 request
 *
 * @param       None
 *
 * @retval      None
 *
 */
void Eint1_Isr(void)
{
    if (EINT_ReadIntFlag(KEY1_BUTTON_EINT_LINE) != RESET)
    {
        APM_MINI_LEDOn(LED3);
        APM_MINI_LEDOff(LED2);

        printf("\r\nSystem have entered STOP mode\r\n");
        printf("now, LED2 remains in the previous state\r\n");
        printf("please press KEY2 or reset System to exit STOP mode\r\n");
        /* Enter STOP Mode in WFI*/
        PMU_EnterSTOPMode(PMU_REGULATOR_ON, PMU_STOP_ENTRY_WFI);
        SystemInit();
        EINT_ClearIntFlag(KEY1_BUTTON_EINT_LINE);
    }
}

void Eint0_Isr(void)
{
    if (EINT_ReadIntFlag(KEY2_BUTTON_EINT_LINE) != RESET)
    {
        SystemInit();
        APM_MINI_LEDOff(LED3);
        /* Wait for system init */
        Delay(0xfffff);
        printf("Wake up from STOP Mode\r\n");
        EINT_ClearIntFlag(KEY2_BUTTON_EINT_LINE);
    }
}

void Delay(uint32_t count)
{
    volatile uint32_t delay = count;
    while (delay--);
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

/**@} end of group PMU_Stop_Functions */
/**@} end of group PMU_Stop */
/**@} end of group Examples */
