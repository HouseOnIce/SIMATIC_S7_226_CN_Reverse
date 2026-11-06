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

/** @addtogroup RTC_Alarm
  @{
  */

/** @defgroup RTC_Alarm_Macros Macros
  @{
*/

/* printf function configs to USART1*/
#define DEBUG_USART  USART1

/**@} end of group RTC_Alarm_Macros*/

/** @defgroup RTC_Alarm_Variables Variables
  @{
*/
uint32_t systick = 0;

/**@} end of group RTC_Alarm_Variables */

/** @defgroup RTC_Alarm_Functions Functions
  @{
*/

/* Delay */
void Delay(uint32_t count);
/* RTC Init */
void RTC_Init(void);

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

    USART_Config_T usartConfigStruct;

    /* Configures LED2 and LED3 */
    APM_MINI_LEDInit(LED2);
    APM_MINI_LEDInit(LED3);

    /* USART configuration */
    USART_ConfigStructInit(&usartConfigStruct);
    usartConfigStruct.baudRate = 115200;
    usartConfigStruct.mode = USART_MODE_TX_RX;
    usartConfigStruct.parity = USART_PARITY_NONE;
    usartConfigStruct.stopBits = USART_STOP_BIT_1;
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;
    usartConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;

    /* COM1 init*/
    APM_MINI_COMInit(COM1, &usartConfigStruct);
    /* KEY1 init*/
    APM_MINI_PBInit(BUTTON_KEY1, BUTTON_MODE_EINT);

    RTC_Init();

    if (SysTick_Config(SystemCoreClock / 1000))
    {
        while (1);
    }
    printf("This example is for RTC Alarm\r\n");
    printf("Alarm will be triggered in 5 s\r\n");
    printf("Later press KEY1 can set alarm for 5 s\r\n");
    while (1)
    {
        APM_MINI_LEDToggle(LED2);
        APM_MINI_LEDToggle(LED3);
        Delay(500);
    }
}

/*!
 * @brief     Delay
 *
 * @param     count:  delay count
 *
 * @retval    None
 */
void RTC_Init(void)
{
    RCM_EnableAPB1PeriphClock((RCM_APB1_PERIPH_T)RCM_APB1_PERIPH_PMU);
    PMU_EnableBackupAccess();

    /* LSI Enable */
    RCM_EnableLSI();
    while (RCM_ReadStatusFlag(RCM_FLAG_LSIRDY) == RESET);
    RCM_ConfigRTCCLK(RCM_RTCCLK_LSI);
    RCM_EnableRTCCLK();

    RTC_WaitForSynchro();
    RTC_WaitForLastTask();

    /* ALARM INTERRUPT Enable */
    RTC_EnableInterrupt(RTC_INT_ALR);
    RTC_WaitForLastTask();

    RTC_ConfigPrescaler(39999);
    RTC_WaitForLastTask();

    /* 5 s ALARM */
    RTC_ConfigAlarm(5);
    RTC_WaitForLastTask();

    /* COUNT SET 0 */
    RTC_ConfigCounter(0);
    RTC_WaitForLastTask();

    NVIC_EnableIRQRequest(RTC_IRQn, 0, 0);
}

/*!
 * @brief     RTC_IRQHandler
 *
 * @param     None
 *
 * @retval    None
 */
void RTC_Isr(void)
{
    uint16_t time;
    time = RTC_ReadCounter();

    printf("Time : %ds\r\n", time);
    printf("ALARM INTERRUPT!\r\n");

    RTC_ClearIntFlag(RTC_INT_ALR);
}

/*!
 * @brief     RTC_IRQHandler
 *
 * @param     None
 *
 * @retval    None
 */
void EINT1_Isr(void)
{
    if (EINT_ReadIntFlag(EINT_LINE_1) == SET)
    {
        uint16_t time;
        time = RTC_ReadCounter();
        /* set alarm for 5 s */
        RTC_ConfigAlarm(time + 5);
        RTC_WaitForLastTask();

        printf("Time : %ds\r\n", time);
        printf("5s later trigger the alarm\r\n");
        EINT_ClearIntFlag(EINT_LINE_1);
    }
}

/*!
 * @brief     Delay
 *
 * @param     count:  delay count
 *
 * @retval    None
 */
void Delay(uint32_t count)
{
    systick = count;

    while (systick);
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

/**@} end of group RTC_Alarm_Functions */
/**@} end of group RTC_Alarm */
/**@} end of group Examples */
