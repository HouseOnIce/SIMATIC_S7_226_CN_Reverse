/*!
 * @file        usbd_composite_custom_hid_if.c
 *
 * @brief       usb device Custom HID class handler
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
#include "usbd_composite_custom_hid_if.h"
#include "usbd_composite.h"
#include "usb_device_user.h"
#include "board.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

static USBD_STA_T USBD_FS_CUSTOM_HID_ItfInit(void);
static USBD_STA_T USBD_FS_CUSTOM_HID_ItfDeInit(void);
static USBD_STA_T USBD_FS_CUSTOM_HID_ItfSendReport(uint8_t *buffer, uint8_t length);
static USBD_STA_T USBD_FS_CUSTOM_HID_ItfReceive(uint8_t *buffer, uint8_t *length);

/**
 * @brief   CUSTOM HID report descriptor
 */
uint8_t USBD_CUSTOM_HIDReportDesc[USBD_CUSTOM_HID_REPORT_DESC_SIZE] =
{
    0x05, 0x01,         /* Usage Page (Generic Desktop Ctrls)   */
    0x09, 0x06,         /* Usage (Keyboard)                     */
    0xA1, 0x01,         /* Collection (Application)             */
    0x05, 0x07,         /* Usage Page (Keyboard)                */
    0x19, 0xE0,         /*   Usage Mininum (Keyboard LeftControl) */
    0x29, 0xE7,         /*   Usage Maxinum (Keyboard Right GUI) */
    0x15, 0x00,         /*   Logical Mininum (0) */
    0x25, 0x01,         /*   Logical Maxinum (1) */
    0x75, 0x01,         /*   Report Size (1) */
    0x95, 0x08,         /*   Report Count (8) */
    0x81, 0x02,         /*   Input (Data,Var,Abs) */
    0x95, 0x01,         /*   Report Count (1) */
    0x75, 0x08,         /*   Report Size (8) */
    0x81, 0x03,         /*   Input (Cnst,Var,Abs) */
    0x95, 0x05,         /*   Report Count (5) */
    0x75, 0x01,         /*   Report Size (1) */
    0x05, 0x08,         /*   Usage Page (LEDs) */
    0x19, 0x01,         /*   Usage Mininum (Num Lock) */
    0x29, 0x05,         /*   Usage Maxinum (Kana) */
    0x91, 0x02,         /*   Output (Data,Var,Abs) */
    0x95, 0x01,         /*   Report Count (1) */
    0x75, 0x03,         /*   Report Size (3) */
    0x91, 0x03,         /*   Output (Cnst,Var,Abs) */
    0x95, 0x06,         /*   Report Count (6) */
    0x75, 0x08,         /*   Report Size (8) */
    0x15, 0x00,         /*   Logical Mininum (0) */
    0x25, 0x65,         /*   Logical Maxinum (101) */
    0x05, 0x07,         /*   Usage Page (Keyboard) */
    0x19, 0x00,         /*   Usage Mininum (Reserved (no event indicated)) */
    0x29, 0x65,         /*   Usage Maxinum (Keyboard Application) */
    0x81, 0x00,         /*   Input (Data,Ary,Abs) */
    0xC0                /* End Collection */
};

/* USB CUSTOM HID interface handler */
USBD_CUSTOM_HID_INTERFACE_T USBD_CUSTOM_HID_INTERFACE_FS =
{
    "Custom HID Keyboard Interface FS",
    USBD_CUSTOM_HIDReportDesc,
    USBD_FS_CUSTOM_HID_ItfInit,
    USBD_FS_CUSTOM_HID_ItfDeInit,
    USBD_FS_CUSTOM_HID_ItfSendReport,
    USBD_FS_CUSTOM_HID_ItfReceive
};

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief       USB device initializes CUSTOM HID media handler
 *
 * @param       None
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_FS_CUSTOM_HID_ItfInit(void)
{
    USBD_STA_T usbStatus = USBD_OK;

    return usbStatus;
}

/*!
 * @brief       USB device deinitializes CUSTOM HID media handler
 *
 * @param       None
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_FS_CUSTOM_HID_ItfDeInit(void)
{
    USBD_STA_T usbStatus = USBD_OK;

    return usbStatus;
}

/*!
 * @brief       USB device CUSTOM HID interface send handler
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_FS_CUSTOM_HID_ItfSendReport(uint8_t *buffer, uint8_t length)
{
    USBD_STA_T usbStatus = USBD_OK;
    
    usbStatus = USBD_Composite_CUSTOM_HID_TxReport(&gUsbDeviceFS, buffer, length);

    return usbStatus;
}

/*!
 * @brief       USB device CUSTOM HID interface receive handler
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_FS_CUSTOM_HID_ItfReceive(uint8_t *buffer, uint8_t *length)
{
    USBD_STA_T usbStatus = USBD_OK;

    if(buffer[0] & 0x01)
    {
        APM_EVAL_LEDOn(LED2);
    }
    else
    {
        APM_EVAL_LEDOff(LED2);
    }
    if(buffer[0] & 0x02)
    {
        APM_EVAL_LEDOn(LED3);
    }
    else
    {
        APM_EVAL_LEDOff(LED3);
    }

    USBD_Composite_CUSTOM_HID_RxPacket(&gUsbDeviceFS);
    
    return usbStatus;
}

