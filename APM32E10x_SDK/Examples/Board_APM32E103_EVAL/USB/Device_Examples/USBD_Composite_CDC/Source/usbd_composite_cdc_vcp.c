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

/* Includes ***************************************************************/
#include "usbd_composite_cdc_vcp.h"
#include "usb_device_user.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

uint8_t cdcTxBuffer[USBD_CDC_TX_BUF_LEN];
uint8_t cdcRxBuffer[USBD_CDC_NUM][USBD_CDC_RX_BUF_LEN];

/* Private function prototypes ********************************************/

static USBD_STA_T USBD_FS_CDC_ItfInit(uint8_t cdcnum);
static USBD_STA_T USBD_FS_CDC_ItfDeInit(void);
static USBD_STA_T USBD_FS_CDC_ItfCtrl(uint8_t command, uint8_t *buffer, uint16_t length, uint8_t cdcnum);
static USBD_STA_T USBD_FS_CDC_ItfSendEnd(uint8_t epNum, uint8_t *buffer, uint32_t *length);
static USBD_STA_T USBD_FS_CDC_ItfReceive(uint8_t *buffer, uint32_t *length, uint8_t cdcnum);

USBD_CDC_LineCodingTypeDef LineCoding[USBD_CDC_NUM] = {{
    115200,                       /* baud rate */
    0x00,                         /* stop bits-1 */
    0x00,                         /* parity - none */
    0x08                          /* nb. of bits 8 */
}};

USBD_CDC_VCP_INFO_T gCDCVCP[USBD_CDC_NUM];

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

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief       USB device initializes CDC media handler
 *
 * @param       None
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_FS_CDC_ItfInit(uint8_t cdcnum)
{
    USBD_STA_T usbStatus = USBD_OK;

    USBD_Composite_CDC_ConfigRxBuffer(&gUsbDeviceFS, cdcRxBuffer[cdcnum], cdcnum);
    USBD_Composite_CDC_ConfigTxBuffer(&gUsbDeviceFS, cdcTxBuffer, 0, cdcnum);


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
static USBD_STA_T USBD_FS_CDC_ItfCtrl(uint8_t command, uint8_t *buffer, uint16_t length, uint8_t cdcnum)
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
            LineCoding[cdcnum].bitrate = (uint32_t) (buffer[0] | (buffer[1] << 8) |
                                            (buffer[2] << 16) | (buffer[3] << 24));
            LineCoding[cdcnum].format = buffer[4];
            LineCoding[cdcnum].paritytype = buffer[5];
            LineCoding[cdcnum].datatype = buffer[6];
            break;

        case USBD_CDC_GET_LINE_CODING:
            buffer[0] = (uint8_t) (LineCoding[cdcnum].bitrate);
            buffer[1] = (uint8_t) (LineCoding[cdcnum].bitrate >> 8);
            buffer[2] = (uint8_t) (LineCoding[cdcnum].bitrate >> 16);
            buffer[3] = (uint8_t) (LineCoding[cdcnum].bitrate >> 24);
            buffer[4] = LineCoding[cdcnum].format;
            buffer[5] = LineCoding[cdcnum].paritytype;
            buffer[6] = LineCoding[cdcnum].datatype;
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
USBD_STA_T USBD_FS_CDC_ItfSend(uint8_t *buffer, uint16_t length, uint8_t cdcnum)
{
    USBD_STA_T usbStatus = USBD_OK;

    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)gUsbDeviceFS.devClassData[gUsbDeviceFS.classID];
    
    if(usbDevComposite->cdcInfo[cdcnum].cdcTx.state != USBD_CDC_XFER_IDLE)
    {
        return USBD_BUSY;
    }
    
    USBD_Composite_CDC_ConfigTxBuffer(&gUsbDeviceFS, buffer, length, cdcnum);
    
    usbStatus = USBD_Composite_CDC_TxPacket(&gUsbDeviceFS, cdcnum);
    
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
static USBD_STA_T USBD_FS_CDC_ItfReceive(uint8_t *buffer, uint32_t *length, uint8_t cdcnum)
{
    USBD_STA_T usbStatus = USBD_OK;
    
    USBD_Composite_CDC_ConfigRxBuffer(&gUsbDeviceFS, &buffer[0], cdcnum);
    
    USBD_Composite_CDC_RxPacket(&gUsbDeviceFS, cdcnum);
    
    gCDCVCP[cdcnum].state = USBD_CDC_VCP_REV_UPDATE;
    gCDCVCP[cdcnum].rxUpdateLen = *length;
    
    return usbStatus;
}

