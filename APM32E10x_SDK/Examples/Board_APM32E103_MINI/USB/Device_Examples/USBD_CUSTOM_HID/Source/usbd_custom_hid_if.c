/*!
 * @file        usbd_custom_hid_if.c
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

/* Includes */
#include "usbd_custom_hid_if.h"
#include "usb_device_user.h"

/** @addtogroup Examples
  * @brief USBD CUSTOM HID examples
  @{
  */

/** @addtogroup USBD_CUSTOM_HID
  @{
  */

/** @defgroup USBD_CUSTOM_HID_Variables Variables
  @{
  */

/**
 * @brief   CUSTOM HID report descriptor
 */
uint8_t USBD_CUSTOM_HIDReportDesc[USBD_CUSTOM_HID_REPORT_DESC_SIZE] =
{
    0x00,
    /* END_COLLECTION */
    0XC0 
};

/**@} end of group USBD_CUSTOM_HID_Variables*/

/** @defgroup USBD_CUSTOM_HID_Structures Structures
  @{
  */

/* USB CUSTOM HID interface handler */
USBD_CUSTOM_HID_INTERFACE_T USBD_CUSTOM_HID_INTERFACE_FS =
{
    "Custom HID Interface FS",
    USBD_CUSTOM_HIDReportDesc,
    USBD_FS_CUSTOM_HID_ItfInit,
    USBD_FS_CUSTOM_HID_ItfDeInit,
    USBD_FS_CUSTOM_HID_ItfSendReport,
    USBD_FS_CUSTOM_HID_ItfReceive
};

/**@} end of group USBD_CUSTOM_HID_Structures*/

/** @defgroup USBD_CUSTOM_HID_Functions Functions
  @{
  */

/*!
 * @brief       USB device initializes CUSTOM HID media handler
 *
 * @param       None
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_FS_CUSTOM_HID_ItfInit(void)
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
USBD_STA_T USBD_FS_CUSTOM_HID_ItfDeInit(void)
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
USBD_STA_T USBD_FS_CUSTOM_HID_ItfSendReport(uint8_t *buffer, uint8_t length)
{
    USBD_STA_T usbStatus = USBD_OK;
    
    usbStatus = USBD_CUSTOM_HID_TxReport(&gUsbDeviceFS, buffer, length);

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
USBD_STA_T USBD_FS_CUSTOM_HID_ItfReceive(uint8_t *buffer, uint8_t *length)
{
    USBD_STA_T usbStatus = USBD_OK;

    USBD_CUSTOM_HID_RxPacket(&gUsbDeviceFS);
    
    return usbStatus;
}

/**@} end of group USBD_CUSTOM_HID_Functions */
/**@} end of group USBD_CUSTOM_HID */
/**@} end of group Examples */
