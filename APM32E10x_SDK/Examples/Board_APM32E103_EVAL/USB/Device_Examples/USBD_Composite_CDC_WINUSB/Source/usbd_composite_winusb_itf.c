/*!
 * @file        usbd_composite_winusb_itf.c
 *
 * @brief       usb device WINUSB class interface handler
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
#include "usbd_composite_winusb_itf.h"
#include "usb_device_user.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

uint8_t winusbTxBuffer[USBD_WINUSB_TX_BUF_LEN];
uint8_t winusbRxBuffer[USBD_WINUSB_RX_BUF_LEN];

/* Private function prototypes ********************************************/

static USBD_STA_T USBD_FS_WINUSB_ItfInit(void);
static USBD_STA_T USBD_FS_WINUSB_ItfDeInit(void);
static USBD_STA_T USBD_FS_WINUSB_ItfCtrl(uint8_t command, uint8_t *buffer, uint16_t length);
static USBD_STA_T USBD_FS_WINUSB_ItfSendEnd(uint8_t epNum, uint8_t *buffer, uint32_t *length);
static USBD_STA_T USBD_FS_WINUSB_ItfReceive(uint8_t *buffer, uint32_t *length);

USBD_WINUSB_ITF_INFO_T gWinusbItf;

/* USB FS CDC interface handler */
USBD_WINUSB_INTERFACE_T USBD_WINUSB_INTERFACE_FS =
{
    "WINUSB Interface FS",
    USBD_FS_WINUSB_ItfInit,
    USBD_FS_WINUSB_ItfDeInit,
    USBD_FS_WINUSB_ItfCtrl,
    USBD_FS_WINUSB_ItfSend,
    USBD_FS_WINUSB_ItfSendEnd,
    USBD_FS_WINUSB_ItfReceive,
};

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief       USB device initializes WINUSB media handler
 *
 * @param       None
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_FS_WINUSB_ItfInit(void)
{
    USBD_STA_T usbStatus = USBD_OK;
    
    USBD_Composite_WINUSB_ConfigRxBuffer(&gUsbDeviceFS, winusbRxBuffer);
    USBD_Composite_WINUSB_ConfigTxBuffer(&gUsbDeviceFS, winusbTxBuffer, 0);
    
    return usbStatus;
}

/*!
 * @brief       USB device deinitializes WINUSB media handler
 *
 * @param       None
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_FS_WINUSB_ItfDeInit(void)
{
    USBD_STA_T usbStatus = USBD_OK;

    return usbStatus;
}

/*!
 * @brief       USB device WINUSB interface control request handler
 *
 * @param       command: Command code
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_FS_WINUSB_ItfCtrl(uint8_t command, uint8_t *buffer, uint16_t length)
{
    USBD_STA_T usbStatus = USBD_OK;

    return usbStatus;
}

/*!
 * @brief       USB device WINUSB interface send handler
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_FS_WINUSB_ItfSend(uint8_t *buffer, uint16_t length)
{
    USBD_STA_T usbStatus = USBD_OK;

    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)gUsbDeviceFS.devClassData[gUsbDeviceFS.classID];
    
    if(usbDevComposite->winusbInfo.winusbTx.state != USBD_WINUSB_XFER_IDLE)
    {
        return USBD_BUSY;
    }
    
    USBD_Composite_WINUSB_ConfigTxBuffer(&gUsbDeviceFS, buffer, length);
    
    usbStatus = USBD_Composite_WINUSB_TxPacket(&gUsbDeviceFS);
    
    return usbStatus;
}

/*!
 * @brief       USB device WINUSB interface send end event handler
 *
 * @param       epNum: endpoint number
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_FS_WINUSB_ItfSendEnd(uint8_t epNum, uint8_t *buffer, uint32_t *length)
{
    USBD_STA_T usbStatus = USBD_OK;

    return usbStatus;
}

/*!
 * @brief       USB device WINUSB interface receive handler
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_FS_WINUSB_ItfReceive(uint8_t *buffer, uint32_t *length)
{
    USBD_STA_T usbStatus = USBD_OK;
    
    USBD_Composite_WINUSB_ConfigRxBuffer(&gUsbDeviceFS, &buffer[0]);
    
    USBD_Composite_WINUSB_RxPacket(&gUsbDeviceFS);
    
    gWinusbItf.state = USBD_WINUSB_ITF_REV_UPDATE;
    gWinusbItf.rxUpdateLen = *length;
    
    return usbStatus;
}
