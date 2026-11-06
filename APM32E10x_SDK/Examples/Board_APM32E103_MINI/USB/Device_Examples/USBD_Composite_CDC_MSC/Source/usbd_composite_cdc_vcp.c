/*!
 * @file        usbd_composite_cdc_vcp.c
 *
 * @brief       usb device CDC class Virtual Com Port handler
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
#include "usbd_composite_cdc_vcp.h"
#include "usb_device_user.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup OTGD_Composite_CDC_MSC
  @{
  */

/** @defgroup OTGD_Composite_CDC_MSC_Variables Variables
  @{
  */

uint8_t cdcTxBuffer[USBD_CDC_TX_BUF_LEN];
uint8_t cdcRxBuffer[USBD_CDC_RX_BUF_LEN];

/**@} end of group OTGD_Composite_CDC_MSC_Variables*/

/** @defgroup OTGD_Composite_CDC_MSC_Functions Functions
  @{
  */


static USBD_STA_T USBD_FS_CDC_ItfInit(void);
static USBD_STA_T USBD_FS_CDC_ItfDeInit(void);
static USBD_STA_T USBD_FS_CDC_ItfCtrl(uint8_t command, uint8_t *buffer, uint16_t length);
static USBD_STA_T USBD_FS_CDC_ItfSendEnd(uint8_t epNum, uint8_t *buffer, uint32_t *length);
static USBD_STA_T USBD_FS_CDC_ItfReceive(uint8_t *buffer, uint32_t *length);

/**@} end of group OTGD_Composite_CDC_MSC_Functions */

/** @defgroup OTGD_Composite_CDC_MSC_Structures Structures
  @{
  */

USBD_CDC_LineCodingTypeDef LineCoding = {
    115200,                       /* baud rate */
    0x00,                         /* stop bits-1 */
    0x00,                         /* parity - none */
    0x08                          /* nb. of bits 8 */
};

USBD_CDC_VCP_INFO_T gCDCVCP;

/* USB FS CDC interface handler */
USBD_CDC_INTERFACE_T USBD_CDC_INTERFACE_FS =
{
    "CDC Interface FS",
    USBD_FS_CDC_ItfInit,
    USBD_FS_CDC_ItfDeInit,
    USBD_FS_CDC_ItfCtrl,
    USBD_FS_CDC_ItfSend,
    USBD_FS_CDC_ItfSendEnd,
    USBD_FS_CDC_ItfReceive,
};

/**@} end of group OTGD_Composite_CDC_MSC_Structures*/

/** @defgroup OTGD_Composite_CDC_MSC_Functions Functions
  @{
  */

/*!
 * @brief       USB device initializes CDC media handler
 *
 * @param       None
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_FS_CDC_ItfInit(void)
{
    USBD_STA_T usbStatus = USBD_OK;

    USBD_Composite_CDC_ConfigRxBuffer(&gUsbDeviceFS, cdcRxBuffer);
    USBD_Composite_CDC_ConfigTxBuffer(&gUsbDeviceFS, cdcTxBuffer, 0);

    return usbStatus;
}

/*!
 * @brief       USB device deinitializes CDC media handler
 *
 * @param       None
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_FS_CDC_ItfDeInit(void)
{
    USBD_STA_T usbStatus = USBD_OK;

    return usbStatus;
}

/*!
 * @brief       USB device CDC interface control request handler
 *
 * @param       command: Command code
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_FS_CDC_ItfCtrl(uint8_t command, uint8_t *buffer, uint16_t length)
{
    USBD_STA_T usbStatus = USBD_OK;

    switch(command)
    {
        case USBD_CDC_SEND_ENCAPSULATED_COMMAND:

        break;
        case USBD_CDC_GET_ENCAPSULATED_RESPONSE:

        break;
        case USBD_CDC_SET_COMM_FEATURE:

        break;
        case USBD_CDC_GET_COMM_FEATURE:

        break;
        case USBD_CDC_CLEAR_COMM_FEATURE:

        break;

        /*******************************************************************************/
        /* Line Coding Structure                                                       */
        /*-----------------------------------------------------------------------------*/
        /* Offset | Field       | Size | Value  | Description                          */
        /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
        /* 4      | bCharFormat |   1  | Number | Stop bits                            */
        /*                                        0 - 1 Stop bit                       */
        /*                                        1 - 1.5 Stop bits                    */
        /*                                        2 - 2 Stop bits                      */
        /* 5      | bParityType |  1   | Number | Parity                               */
        /*                                        0 - None                             */
        /*                                        1 - Odd                              */
        /*                                        2 - Even                             */
        /*                                        3 - Mark                             */
        /*                                        4 - Space                            */
        /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
        /*******************************************************************************/

        case USBD_CDC_SET_LINE_CODING:
            LineCoding.bitrate = (uint32_t) (buffer[0] | (buffer[1] << 8) |
                                            (buffer[2] << 16) | (buffer[3] << 24));
            LineCoding.format = buffer[4];
            LineCoding.paritytype = buffer[5];
            LineCoding.datatype = buffer[6];
            break;

        case USBD_CDC_GET_LINE_CODING:
            buffer[0] = (uint8_t) (LineCoding.bitrate);
            buffer[1] = (uint8_t) (LineCoding.bitrate >> 8);
            buffer[2] = (uint8_t) (LineCoding.bitrate >> 16);
            buffer[3] = (uint8_t) (LineCoding.bitrate >> 24);
            buffer[4] = LineCoding.format;
            buffer[5] = LineCoding.paritytype;
            buffer[6] = LineCoding.datatype;
            break;

        case USBD_CDC_SET_CONTROL_LINE_STATE:

        break;
        case USBD_CDC_SEND_BREAK:

        break;
        default:

        break;
    }
    return usbStatus;
}

/*!
 * @brief       USB device CDC interface send handler
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_FS_CDC_ItfSend(uint8_t *buffer, uint16_t length)
{
    USBD_STA_T usbStatus = USBD_OK;

    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)gUsbDeviceFS.devClassData[gUsbDeviceFS.classID];
    
    if(usbDevComposite->cdcInfo.cdcTx.state != USBD_CDC_XFER_IDLE)
    {
        return USBD_BUSY;
    }
    
    USBD_Composite_CDC_ConfigTxBuffer(&gUsbDeviceFS, buffer, length);
    
    usbStatus = USBD_Composite_CDC_TxPacket(&gUsbDeviceFS);
    
    return usbStatus;
}

/*!
 * @brief       USB device CDC interface send end event handler
 *
 * @param       epNum: endpoint number
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_FS_CDC_ItfSendEnd(uint8_t epNum, uint8_t *buffer, uint32_t *length)
{
    USBD_STA_T usbStatus = USBD_OK;

    return usbStatus;
}

/*!
 * @brief       USB device CDC interface receive handler
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_FS_CDC_ItfReceive(uint8_t *buffer, uint32_t *length)
{
    USBD_STA_T usbStatus = USBD_OK;
    
    USBD_Composite_CDC_ConfigRxBuffer(&gUsbDeviceFS, &buffer[0]);
    
    USBD_Composite_CDC_RxPacket(&gUsbDeviceFS);
    
    gCDCVCP.state = USBD_CDC_VCP_REV_UPDATE;
    gCDCVCP.rxUpdateLen = *length;
    
    return usbStatus;
}

/**@} end of group OTGD_Composite_CDC_MSC_Functions */
/**@} end of group OTGD_Composite_CDC_MSC */
/**@} end of group Examples */
