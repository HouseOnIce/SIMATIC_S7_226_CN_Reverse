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

/* Includes */
#include "board.h"
#include "bsp_delay.h"
#include "apm32e10x_misc.h"
#include "apm32e10x_usart.h"
#include "usb_device_user.h"
#include "usbd_cdc.h"
#include "usbd_cdc_vcp.h"
#include <stdio.h>
#include <string.h>

/** @addtogroup Examples
  * @brief USBD CDC examples
  @{
  */

/** @addtogroup USBD_CDC
  @{
  */

/** @defgroup USBD_CDC_Macros Macros
  @{
*/

#define DEBUG_USART    USART1

/**@} end of group USBD_CDC_Macros*/

/** @defgroup USBD_CDC_Enumerates Enumerates
  @{
  */

/**
 * @brief User application state
 */
typedef enum
{
    USER_APP_INIT,
    USER_APP_RUN,
} USER_APP_STATE_T;

/**@} end of group USBD_CDC_Enumerates*/

/** @defgroup USBD_CDC_Functions Functions
  @{
  */

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
    APM_MINI_COMInit(COM1, &usartConfigStruct);
    APM_MINI_PBInit(BUTTON_KEY1, BUTTON_MODE_GPIO);
    APM_MINI_PBInit(BUTTON_KEY2, BUTTON_MODE_GPIO);

    /* USB clock = PLLCLK / 2.5 = 120MHz / 2.5 = 48MHz */
    RCM_ConfigUSBCLK(RCM_USB_DIV_2_5);

    /* Init USB device */
    USB_DeviceInit();

    printf("USB Device CDC Application\r\n");

    while (1)
    {
        USB_DevUserApplication();
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
    static uint16_t i = 0;
    static char str[] = "Hello      \r\n";
    
    switch (userAppState)
    {
        case USER_APP_INIT:
            interval = USBD_CDC_ReadInterval(&gUsbDeviceFS);
            userAppState = USER_APP_RUN;
            break;

        case USER_APP_RUN:
            /* Send Total Buffer */
            if(gUsbVCP.state == USBD_CDC_VCP_REV_UPDATE)
            {
                gUsbVCP.state = USBD_CDC_VCP_IDLE;
                
                if(gUsbVCP.rxTotalLen >= 100)
                {
                    USBD_FS_CDC_ItfSend((uint8_t *)cdcRxBufferTotal, (uint16_t)gUsbVCP.rxTotalLen);
                
                    USBD_FS_CDC_ItfSend((uint8_t *)"\r\n", strlen("\r\n"));
                    
                    gUsbVCP.rxTotalLen = 0;
                }
            }
            else
            {
                i++;
                sprintf(str,"Hello %d\r\n",i);
                USBD_FS_CDC_ItfSend((uint8_t *)str, strlen(str));
            }
            
            Delay_ms(interval);
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

/**@} end of group USBD_CDC_Functions */
/**@} end of group USBD_CDC */
/**@} end of group Examples */
