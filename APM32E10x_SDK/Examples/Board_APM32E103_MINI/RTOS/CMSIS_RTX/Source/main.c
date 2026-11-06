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
 *  Copyright (C) 2025 Geehy Semiconductor
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

/* Includes ***************************************************************/
#include "main.h"
#include "Board.h"

/* Private includes *******************************************************/
#include "apm32e10x_gpio.h"
#include "apm32e10x_misc.h"
#include "apm32e10x_usart.h"
#include "apm32e10x_rcm.h"
#include "cmsis_os2.h"
#include <stdio.h>

/* Private macro **********************************************************/
#define DEBUG_USART  USART1

/* Private typedef ********************************************************/
static osThreadId_t ledThreadID = NULL;
static osThreadId_t usartThreadID = NULL;

static const osThreadAttr_t ledThreadattr = {
    .name = "LedToggle",
    .priority = osPriorityNormal,
    .stack_size = 512
};

static const osThreadAttr_t usartThreadattr = {
    .name = "UsartTest",
    .priority = osPriorityNormal,
    .stack_size = 512
};

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/
/* Led toggle task */
void Led_Thread(void *argument);
/* Usart1 test task */
void Usart_TestThread(void *argument);

/* External variables *****************************************************/

/* External functions *****************************************************/

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

    USART_Config_T usartConfigStruct;

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

    /* Configure LED GPIO Pin */
    APM_MINI_LEDInit(LED2);
    APM_MINI_LEDInit(LED3);

    APM_MINI_PBInit(BUTTON_KEY2, BUTTON_MODE_GPIO);

    /* Initialize the RTOS scheduler */
    osKernelInitialize();

    /* Create the thread */
    ledThreadID = osThreadNew(Led_Thread, NULL, &ledThreadattr);
    if (ledThreadID == NULL)
    {
        printf("Create led thread failed!\r\n");
    }

    usartThreadID = osThreadNew(Usart_TestThread, NULL, &usartThreadattr);
    if (usartThreadID == NULL)
    {
        printf("Create usart thread failed!\r\n");
    }

    /* Start the RTOS scheduler */
    osKernelStart();

    /* Infinite loop */
    while (1);
}

/**
 * @brief   Led toggle task
 *
 * @param   argument  Not used
 *
 * @retval  None
 */
void Led_Thread(void *argument)
{
    UNUSED(argument);

    while (1)
    {
        /* Toggle LED2 */
        APM_MINI_LEDToggle(LED2);
        osDelay(500);
    }
}

/**
 * @brief   Usart1 test task
 *
 * @param   argument  Not used
 *
 * @retval  None
 */
void Usart_TestThread(void *argument)
{
    UNUSED(argument);

    while (1)
    {
        /* Print message */
        printf("Hello, world!\r\n");
        osDelay(1000);
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
    int i;

    UNUSED(file);

    for (i = 0; i < len; i++)
    {
        __io_putchar(*ptr++);
    }

    return len;
}

#else
#warning Not supported compiler type
#endif
