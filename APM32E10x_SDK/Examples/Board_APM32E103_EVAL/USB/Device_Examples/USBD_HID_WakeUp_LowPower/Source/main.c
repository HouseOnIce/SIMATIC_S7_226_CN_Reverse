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
 *  Copyright (C) 2023-2025 Geehy Semiconductor
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
#include "board.h"
#include "bsp_delay.h"
#include "apm32e10x_misc.h"
#include "apm32e10x_usart.h"
#include "usb_device_user.h"
#include "usbd_hid.h"
#include <stdio.h>

/* Private includes *******************************************************/

/* Private macro **********************************************************/

#define DEBUG_USART    USART1

/* Private typedef ********************************************************/

/**
 * @brief User application state
 */
typedef enum
{
    USER_APP_INIT,
    USER_APP_RUN,
} USER_APP_STATE_T;

/* Private variables ******************************************************/

uint8_t WakeupEnable = 0;

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

extern USBD_HANDLE_T usbDeviceHandler;

/* External functions *****************************************************/

/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      int
 */
int main(void)
{
    SystemClockConfig();

    USART_Config_T usartConfigStruct;

    /* USART configuration */
    USART_ConfigStructInit(&usartConfigStruct);
    usartConfigStruct.baudRate      = 115200;
    usartConfigStruct.mode          = USART_MODE_TX_RX;
    usartConfigStruct.parity        = USART_PARITY_NONE;
    usartConfigStruct.stopBits      = USART_STOP_BIT_1;
    usartConfigStruct.wordLength    = USART_WORD_LEN_8B;
    usartConfigStruct.hardwareFlow  = USART_HARDWARE_FLOW_NONE;

    /* COM1 init*/
    APM_EVAL_COMInit(COM1, &usartConfigStruct);
    APM_EVAL_PBInit(BUTTON_KEY1, BUTTON_MODE_EINT);
    APM_EVAL_PBInit(BUTTON_KEY2, BUTTON_MODE_GPIO);

    APM_EVAL_DelayInit();
    APM_EVAL_LEDInit(LED2);

    /* USB clock = PLLCLK / 2.5 = 120MHz / 2.5 = 48MHz */
    RCM_ConfigUSBCLK(RCM_USB_DIV_2_5);
    /* Init USB device */
    USB_DeviceInit();

    printf("USB Device HID Application\r\n");

    while (1)
    {
        USB_DevUserApplication();
        APM_EVAL_DelayMs(100);
        APM_EVAL_LEDToggle(LED2);
    }
}

/*!
 * @brief       User application
 *
 * @param       None
 *
 * @retval      None
 */
void USB_DevUserApplication(void)
{
    static uint8_t userAppState = USER_APP_INIT;
    static uint8_t interval = 0;
    static int8_t report[4] = { 0 };

    switch (userAppState)
    {
        case USER_APP_INIT:
            interval = USBD_HID_ReadInterval(&gUsbDeviceFS);

            report[0] = 0;
            report[1] = 0;
            report[2] = 0;
            report[3] = 0;
            userAppState = USER_APP_RUN;
            break;

        case USER_APP_RUN:
            if(WakeupEnable)
            {
                WakeupEnable = 0;
                /* Wake Up USB Host */

                gUsbDeviceFS.devState = gUsbDeviceFS.preDevState;

                USBD_SetWakeupRequest(usbDeviceHandler.usbGlobal);

                APM_EVAL_DelayMs(10);

                USBD_ResetWakeupRequest(usbDeviceHandler.usbGlobal);
            }

            if (!APM_EVAL_PBGetState(BUTTON_KEY2))
            {
                APM_EVAL_DelayMs(10);
                if (!APM_EVAL_PBGetState(BUTTON_KEY2))
                {
                    report[1] = 5;
                    USBD_HID_TxReport(&gUsbDeviceFS, (uint8_t*)report, 4);
                }
            }

            APM_EVAL_DelayMs(interval);
            break;
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
    for (i = 0; i < len; i++)
    {
        __io_putchar(*ptr++);
    }

    return len;
}

#else
#warning Not supported compiler type
#endif

