/*!
 * @file        usbd_composite.c
 *
 * @brief       usb device composite class handler
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
#include "usbd_composite.h"
#include "usbd_stdReq.h"
#include "usbd_dataXfer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @addtogroup Examples
  @{
  */

/** @addtogroup OTGD_Composite_CDC_WINUSB
  @{
  */

/** @defgroup OTGD_Composite_CDC_WINUSB_Functions Functions
  @{
  */
static USBD_STA_T USBD_Composite_ClassInitHandler(USBD_INFO_T* usbInfo, uint8_t cfgIndex);
static USBD_STA_T USBD_Composite_ClassDeInitHandler(USBD_INFO_T* usbInfo, uint8_t cfgIndex);
static USBD_STA_T USBD_Composite_SOFHandler(USBD_INFO_T* usbInfo);
static USBD_STA_T USBD_Composite_SetupHandler(USBD_INFO_T* usbInfo, USBD_REQ_SETUP_T* req);
static USBD_STA_T USBD_Composite_RxEP0Handler(USBD_INFO_T* usbInfo);
static USBD_STA_T USBD_Composite_DataInHandler(USBD_INFO_T* usbInfo, uint8_t epNum);
static USBD_STA_T USBD_Composite_DataOutHandler(USBD_INFO_T* usbInfo, uint8_t epNum);
/**@} end of group OTGD_Composite_CDC_WINUSB_Functions */

/** @defgroup OTGD_Composite_CDC_WINUSB_Structures Structures
  @{
  */

/* Composite class handler */
USBD_CLASS_T USBD_COMPOSITE_CLASS =
{
    /* Class handler */
    "Class Composite",
    USBD_Composite_ClassInitHandler,
    USBD_Composite_ClassDeInitHandler,
    USBD_Composite_SOFHandler,

    /* Control endpoint */
    USBD_Composite_SetupHandler,
    NULL,
    USBD_Composite_RxEP0Handler,
    /* Specific endpoint */
    USBD_Composite_DataInHandler,
    USBD_Composite_DataOutHandler,
    NULL,
    NULL,
};

/**@} end of group OTGD_Composite_CDC_WINUSB_Structures*/

/** @defgroup OTGD_Composite_CDC_WINUSB_Variables Variables
  @{
  */

/**
 * @brief   WinUSB OS feature descriptor
 */
uint8_t USBD_WinUsbOsFeatureDesc[USBD_WINUSB_OS_FEATURE_DESC_SIZE] =
{
    /* dwLength */
    0x28, 0x00, 0x00, 0x00,
    /* bcdVersion */
    0x00, 0x01,
    /* wIndex extended compat ID descritor */
    0x04, 0x00,
    /* bCount */
    0x01,
    /* Reserved */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* WCID Function */
    /* bFirstInterfaceNumber */
    0x00,
    /* bReserved */
    0x00,
    /* CID */
    'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,
    /* Sub CID */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* Reserved */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/* L"DeviceInterfaceGUID" : wIndex = 0x0005 */
/* L"{12345678-1234-1234-1234-123456789ABC}" */
uint8_t USBD_WinUsbOsPropertyDesc[USBD_WINUSB_OS_PROPERTY_DESC_SIZE] = 
{
    /* dwTotalSize = Header + All sections */
    0x8E, 0x00, 0x00, 0x00,
    /* bcdVersion */
    0x00, 0x01,
    /* wIndex */
    0x05, 0x00,
    /* bCount */
    0x01, 0x00,

    /* dwSize - this section */
    0x84, 0x00, 0x00, 0x00,

    /* dwPropertyDataType */
    0x01, 0x00, 0x00, 0x00,

    /* wPropertyNameLength */
    0x28, 0x00,

    /* WCHAR L"DeviceInterfaceGUID" */
    'D', 0x00, 'e', 0x00,
    'v', 0x00, 'i', 0x00,
    'c', 0x00, 'e', 0x00,
    'I', 0x00, 'n', 0x00,
    't', 0x00, 'e', 0x00,
    'r', 0x00, 'f', 0x00,
    'a', 0x00, 'c', 0x00,
    'e', 0x00, 'G', 0x00,
    'U', 0x00, 'I', 0x00,
    'D', 0x00, 0x00, 0x00,

    /* dwPropertyDataLength : 78 Bytes = 0x0000004E */
    0x4E, 0x00, 0x00, 0x00,

    /* WCHAR : L"{12345678-1234-1234-1234-123456789ABC}" */
    '{', 0x00, '1', 0x00,
    '2', 0x00, '3', 0x00,
    '4', 0x00, '5', 0x00,
    '6', 0x00, '7', 0x00,
    '8', 0x00, '-', 0x00,
    '1', 0x00, '2', 0x00,
    '3', 0x00, '4', 0x00,
    '-', 0x00, '1', 0x00,
    '2', 0x00, '3', 0x00,
    '4', 0x00, '-', 0x00,
    '1', 0x00, '2', 0x00,
    '3', 0x00, '4', 0x00,
    '-', 0x00, '1', 0x00,
    '2', 0x00, '3', 0x00,
    '4', 0x00, '5', 0x00,
    '6', 0x00, '7', 0x00,
    '8', 0x00, '9', 0x00,
    'A', 0x00, 'B', 0x00,
    'C', 0x00, '}', 0x00,
    0x00, 0x00
};

/**@} end of group OTGD_Composite_CDC_WINUSB_Variables*/

/** @defgroup OTGD_Composite_CDC_WINUSB_Functions Functions
  @{
  */

/*!
 * @brief     USB device WINUSB feature descriptor
 *
 * @param     usbSpeed : usb speed
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_Composite_WinUsbFeatureDescHandler(uint8_t usbSpeed)
{
    USBD_DESC_INFO_T descInfo;

    descInfo.desc = USBD_WinUsbOsFeatureDesc;
    descInfo.size = sizeof(USBD_WinUsbOsFeatureDesc);

    return descInfo;
}

/*!
 * @brief     USB device WINUSB property descriptor
 *
 * @param     usbSpeed : usb speed
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_Composite_WinUsbPropertyDescHandler(uint8_t usbSpeed)
{
    USBD_DESC_INFO_T descInfo;

    descInfo.desc = USBD_WinUsbOsPropertyDesc;
    descInfo.size = sizeof(USBD_WinUsbOsPropertyDesc);

    return descInfo;
}

/*!
 * @brief       USB device composite init
 *
 * @param       usbInfo: usb device information
 *
 * @param       itf1: class interface 1
 *
 * @param       itf2: class interface 2
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_Composite_Init(USBD_INFO_T* usbInfo, void* itf1, void* itf2)
{
    USBD_STA_T usbStatus = USBD_OK;

    USBD_COMPOSITE_INFO_T* usbDevComposite;

    /* Link class data */
    usbInfo->devClassData[usbInfo->classID] = (USBD_COMPOSITE_INFO_T*)malloc(sizeof(USBD_COMPOSITE_INFO_T));
    usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];
    memset(usbDevComposite, 0, sizeof(USBD_COMPOSITE_INFO_T));

    USBD_USR_Debug("USBD_COMPOSITE_INFO_T size %d\r\n", sizeof(USBD_COMPOSITE_INFO_T));

    if (usbDevComposite == NULL)
    {
        USBD_USR_LOG("usbDevComposite is NULL");
        return USBD_FAIL;
    }

    if((USBD_CDC_INTERFACE_T*)itf1 == NULL)
    {
        USBD_USR_LOG("itf1 is NULL");
        return USBD_FAIL;
    }
    else
    {
        USBD_Composite_RegisterCDCItf(usbInfo, (USBD_CDC_INTERFACE_T*)itf1);
    }

    if((USBD_WINUSB_INTERFACE_T*)itf2 == NULL)
    {
        USBD_USR_LOG("itf2 is NULL");
        return USBD_FAIL;
    }
    else
    {
        USBD_Composite_RegisterWINUSBItf(usbInfo, (USBD_WINUSB_INTERFACE_T*)itf2);
    }

    return usbStatus;
}

/*!
 * @brief       USB device composite de-init
 *
 * @param       usbInfo: usb device information
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_Composite_Deinit(USBD_INFO_T* usbInfo)
{
    USBD_STA_T usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    if (usbInfo->devClassData[usbInfo->classID] != NULL)
    {
        if(((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo.pUserData)->ItfDeInit != NULL)
        {
            ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo.pUserData)->ItfDeInit();
        }

        if(((USBD_WINUSB_INTERFACE_T *)usbDevComposite->winusbInfo.pUserData)->ItfDeInit != NULL)
        {
            ((USBD_WINUSB_INTERFACE_T *)usbDevComposite->winusbInfo.pUserData)->ItfDeInit();
        }

        free(usbInfo->devClassData[usbInfo->classID]);
        usbInfo->devClassData[usbInfo->classID] = 0;
    }

    return usbStatus;
}

/*!
 * @brief       USB device composite configuration handler
 *
 * @param       usbInfo: usb device information
 *
 * @param       cfgIndex: configuration index
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_Composite_ClassInitHandler(USBD_INFO_T* usbInfo, uint8_t cfgIndex)
{
    USBD_STA_T usbStatus = USBD_OK;

    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    if (usbDevComposite == NULL)
    {
        USBD_USR_LOG("usbDevComposite is NULL");
        return USBD_FAIL;
    }

    usbDevComposite->cdcInfo.epCmdAddr = USBD_CDC_CMD_EP_ADDR;
    usbDevComposite->cdcInfo.epInAddr = USBD_CDC_DATA_IN_EP_ADDR;
    usbDevComposite->cdcInfo.epOutAddr = USBD_CDC_DATA_OUT_EP_ADDR;
    
    usbDevComposite->winusbInfo.epInAddr = USBD_WINUSB_IN_EP_ADDR;
    usbDevComposite->winusbInfo.epOutAddr = USBD_WINUSB_OUT_EP_ADDR;

    /* Open Command endpoint */
    USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo.epCmdAddr, EP_TYPE_INTERRUPT, USBD_CDC_CMD_MP_SIZE);
    usbInfo->devEpIn[usbDevComposite->cdcInfo.epCmdAddr & 0x0F].useStatus = ENABLE;

    /* Open Data endpoint */
    switch (usbInfo->devSpeed)
    {
        case USBD_SPEED_FS:
            /* WINUSB */
            USBD_EP_OpenCallback(usbInfo, usbDevComposite->winusbInfo.epOutAddr, EP_TYPE_BULK, WINUSB_FS_MAX_PACKET_SIZE);
            usbInfo->devEpOut[usbDevComposite->winusbInfo.epOutAddr & 0x0F].useStatus = ENABLE;

            USBD_EP_OpenCallback(usbInfo, usbDevComposite->winusbInfo.epInAddr, EP_TYPE_BULK, WINUSB_FS_MAX_PACKET_SIZE);
            usbInfo->devEpIn[usbDevComposite->winusbInfo.epInAddr & 0x0F].useStatus = ENABLE;

            /* CDC */
            USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo.epOutAddr, EP_TYPE_BULK, USBD_CDC_FS_MP_SIZE);
            usbInfo->devEpOut[usbDevComposite->cdcInfo.epOutAddr & 0x0F].useStatus = ENABLE;

            USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo.epInAddr, EP_TYPE_BULK, USBD_CDC_FS_MP_SIZE);
            usbInfo->devEpIn[usbDevComposite->cdcInfo.epInAddr & 0x0F].useStatus = ENABLE;
        
            usbInfo->devEpIn[usbDevComposite->cdcInfo.epCmdAddr & 0x0F].interval = USBD_CDC_FS_INTERVAL;
            break;

        default:
            /* WINUSB */
            USBD_EP_OpenCallback(usbInfo, usbDevComposite->winusbInfo.epOutAddr, EP_TYPE_BULK, WINUSB_HS_MAX_PACKET_SIZE);
            usbInfo->devEpOut[usbDevComposite->winusbInfo.epOutAddr & 0x0F].useStatus = ENABLE;

            USBD_EP_OpenCallback(usbInfo, usbDevComposite->winusbInfo.epInAddr, EP_TYPE_BULK, WINUSB_HS_MAX_PACKET_SIZE);
            usbInfo->devEpIn[usbDevComposite->winusbInfo.epInAddr & 0x0F].useStatus = ENABLE;

            /* CDC */
            USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo.epOutAddr, EP_TYPE_BULK, USBD_CDC_HS_MP_SIZE);
            usbInfo->devEpOut[usbDevComposite->cdcInfo.epOutAddr & 0x0F].useStatus = ENABLE;

            USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo.epInAddr, EP_TYPE_BULK, USBD_CDC_HS_MP_SIZE);
            usbInfo->devEpIn[usbDevComposite->cdcInfo.epInAddr & 0x0F].useStatus = ENABLE;
        
            usbInfo->devEpIn[usbDevComposite->cdcInfo.epCmdAddr & 0x0F].interval = USBD_CDC_HS_INTERVAL;
            break;
    }

    /* Interface Init */
    usbDevComposite->cdcInfo.cdcTx.buffer = NULL;
    usbDevComposite->cdcInfo.cdcRx.buffer = NULL;
    
    usbDevComposite->cdcInfo.cdcTx.state = USBD_CDC_XFER_IDLE;
    usbDevComposite->cdcInfo.cdcRx.state = USBD_CDC_XFER_IDLE;

    usbDevComposite->winusbInfo.winusbTx.buffer = NULL;
    usbDevComposite->winusbInfo.winusbRx.buffer = NULL;
    
    usbDevComposite->winusbInfo.winusbTx.state = USBD_WINUSB_XFER_IDLE;
    usbDevComposite->winusbInfo.winusbRx.state = USBD_WINUSB_XFER_IDLE;

    if (usbDevComposite->cdcInfo.pUserData != NULL)
    {
        ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo.pUserData)->ItfInit();
    }

    if (usbDevComposite->winusbInfo.pUserData != NULL)
    {
        ((USBD_WINUSB_INTERFACE_T *)usbDevComposite->winusbInfo.pUserData)->ItfInit();
    }

    if(usbDevComposite->cdcInfo.cdcRx.buffer == NULL)
    {
        USBD_USR_LOG("cdcRx buffer is NULL");
        return USBD_FAIL;
    }

    if(usbDevComposite->winusbInfo.winusbRx.buffer == NULL)
    {
        USBD_USR_LOG("winusbRx buffer is NULL");
        return USBD_FAIL;
    }

    switch (usbInfo->devSpeed)
    {
        case USBD_SPEED_FS:
            USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->cdcInfo.epOutAddr, \
                                    usbDevComposite->cdcInfo.cdcRx.buffer, \
                                    USBD_CDC_FS_MP_SIZE);

            USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->winusbInfo.epOutAddr, \
                                    usbDevComposite->winusbInfo.winusbRx.buffer, \
                                    WINUSB_FS_MAX_PACKET_SIZE);
            break;

        default:
            USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->cdcInfo.epOutAddr, \
                                    usbDevComposite->cdcInfo.cdcRx.buffer, \
                                    USBD_CDC_HS_MP_SIZE);

            USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->winusbInfo.epOutAddr, \
                                    usbDevComposite->winusbInfo.winusbRx.buffer, \
                                    WINUSB_HS_MAX_PACKET_SIZE);
            break;
    }

    return usbStatus;
}

/*!
 * @brief       USB device composite reset handler
 *
 * @param       usbInfo: usb device information
 *
 * @param       cfgIndex: configuration index
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_Composite_ClassDeInitHandler(USBD_INFO_T* usbInfo, uint8_t cfgIndex)
{
    USBD_STA_T usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    /* Close CDC EP */
    USBD_EP_CloseCallback(usbInfo, usbDevComposite->cdcInfo.epOutAddr);
    usbInfo->devEpOut[usbDevComposite->cdcInfo.epOutAddr & 0x0F].useStatus = DISABLE;

    USBD_EP_CloseCallback(usbInfo, usbDevComposite->cdcInfo.epInAddr);
    usbInfo->devEpIn[usbDevComposite->cdcInfo.epInAddr & 0x0F].useStatus = DISABLE;

    USBD_EP_CloseCallback(usbInfo, usbDevComposite->cdcInfo.epCmdAddr);
    usbInfo->devEpIn[usbDevComposite->cdcInfo.epCmdAddr & 0x0F].useStatus = DISABLE;
    usbInfo->devEpIn[usbDevComposite->cdcInfo.epCmdAddr & 0x0F].interval = 0;

    /* Close WINUSB EP */
    USBD_EP_CloseCallback(usbInfo, usbDevComposite->winusbInfo.epOutAddr);
    usbInfo->devEpOut[usbDevComposite->winusbInfo.epOutAddr & 0x0F].useStatus = DISABLE;

    USBD_EP_CloseCallback(usbInfo, usbDevComposite->winusbInfo.epInAddr);
    usbInfo->devEpIn[usbDevComposite->winusbInfo.epInAddr & 0x0F].useStatus = DISABLE;

    return usbStatus;
}

/*!
 * @brief       USB device composite SOF handler
 *
 * @param       usbInfo: usb device information
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_Composite_SOFHandler(USBD_INFO_T* usbInfo)
{
    USBD_STA_T  usbStatus = USBD_BUSY;

    return usbStatus;
}

/*!
 * @brief     USB CDC device receive CTRL status
 *
 * @param     usbInfo : usb handler information
 *
 * @param     buffer : data buffer
 *
 * @param     length : length of data
 *
 * @retval    usb device status
 */
static USBD_STA_T USBD_Composite_CDC_CtrlReceiveData(USBD_INFO_T* usbInfo, uint8_t *buffer, uint32_t length)
{
    USBD_STA_T usbStatus = USBD_OK;

    usbInfo->devEp0State = USBD_DEV_EP0_DATA_OUT;
    usbInfo->devEpOut[USBD_EP_0].length = length;
    usbInfo->devEpOut[USBD_EP_0].remainLen = length;

    USBD_EP_ReceiveCallback(usbInfo, USBD_EP_0, buffer, length);

    return usbStatus;
}

/*!
 * @brief       USB device composite SETUP handler
 *
 * @param       usbInfo: usb device information
 *
 * @param       req: setup request
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_Composite_SetupHandler(USBD_INFO_T* usbInfo, USBD_REQ_SETUP_T* req)
{
    USBD_STA_T  usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    uint8_t request;
    uint8_t reqType;
    uint16_t wLength = req->DATA_FIELD.wLength[0] | req->DATA_FIELD.wLength[1] << 8;
    uint16_t status = 0x0000;
    uint16_t length;
    uint16_t wIndex = req->DATA_FIELD.wIndex[0] | req->DATA_FIELD.wIndex[1] << 8;

    uint8_t itf = 0U;
    
    USBD_DESC_INFO_T descInfo;
    
    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }
    
    request = req->DATA_FIELD.bRequest;
    reqType = req->DATA_FIELD.bmRequest.REQ_TYPE_B.type;

    switch (reqType)
    {
        case USBD_REQ_TYPE_STANDARD:
            switch (request)
            {
                case USBD_STD_GET_STATUS:
                    if (usbInfo->devState == USBD_DEV_CONFIGURE)
                    {
                        USBD_CtrlSendData(usbInfo, (uint8_t*)&status, 2);
                    }
                    else
                    {
                        USBD_REQ_CtrlError(usbInfo, req);
                        usbStatus = USBD_FAIL;
                    }
                    break;

                case USBD_STD_GET_INTERFACE:
                    if (usbInfo->devState == USBD_DEV_CONFIGURE)
                    {
                        USBD_CtrlSendData(usbInfo, &itf, 1);
                    }
                    else
                    {
                        USBD_REQ_CtrlError(usbInfo, req);
                        usbStatus = USBD_FAIL;
                    }
                    break;

                case USBD_STD_SET_INTERFACE:
                    if (usbInfo->devState == USBD_DEV_CONFIGURE)
                    {
                        
                    }
                    else
                    {
                        USBD_REQ_CtrlError(usbInfo, req);
                        usbStatus = USBD_FAIL;
                    }
                    break;

                case USBD_STD_CLEAR_FEATURE:
                    break;

                default:
                    USBD_REQ_CtrlError(usbInfo, req);
                    usbStatus = USBD_FAIL;
                    break;
            }
            break;

        case USBD_REQ_TYPE_CLASS:
            if(wLength)
            {
                if((usbInfo->reqSetup.DATA_FIELD.bmRequest.REQ_TYPE & 0x80) != 0)
                {
                    ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo.pUserData)->ItfCtrl(request, \
                                                                                         (uint8_t *)usbDevComposite->cdcInfo.data,
                                                                                         wLength);
                    
                    length = USBD_CDC_DATA_MP_SIZE < wLength ? USBD_CDC_DATA_MP_SIZE : wLength;
                    USBD_CtrlSendData(usbInfo, (uint8_t *)usbDevComposite->cdcInfo.data, length);
                }
                else
                {
                    usbDevComposite->cdcInfo.cdcCmd.opcode = request;
                    usbDevComposite->cdcInfo.cdcCmd.length = wLength < USBD_EP0_PACKET_MAX_SIZE ? \
                                               wLength : USBD_EP0_PACKET_MAX_SIZE;
                    
                    USBD_Composite_CDC_CtrlReceiveData(usbInfo, (uint8_t *)usbDevComposite->cdcInfo.data, usbDevComposite->cdcInfo.cdcCmd.length);
                }
            }
            else
            {
                ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo.pUserData)->ItfCtrl(request, \
                                                                                     (uint8_t *)req, \
                                                                                     0);
            }
            break;

        case USBD_REQ_TYPE_VENDOR:
            switch (request)
            {
                case USBD_VEN_REQ_MS_CODE:
                    switch(wIndex)
                    {
                        case USBD_WINUSB_DESC_FEATURE:
                            descInfo = USBD_Composite_WinUsbFeatureDescHandler(usbInfo->devSpeed);

                            descInfo.size = descInfo.size < wLength ? descInfo.size : wLength;
                            break;
                        
                        case USBD_WINUSB_DESC_PROPERTY:
                            descInfo = USBD_Composite_WinUsbPropertyDescHandler(usbInfo->devSpeed);

                            descInfo.size = descInfo.size < wLength ? descInfo.size : wLength;
                            break;
                        
                        default:
                            USBD_REQ_CtrlError(usbInfo, req);
                            usbStatus = USBD_FAIL;
                            break;
                    }
                    
                    if (descInfo.desc != NULL)
                    {
                        USBD_CtrlSendData(usbInfo, descInfo.desc, descInfo.size);
                    }
                    break;
                
                default:
                    USBD_REQ_CtrlError(usbInfo, req);
                    usbStatus = USBD_FAIL;
                    break;
            }
            break;

        default:
            usbStatus = USBD_FAIL;
            USBD_REQ_CtrlError(usbInfo, req);
            break;
    }

    return usbStatus;
}

/*!
 * @brief       USB device composite EP0 receive handler
 *
 * @param       usbInfo: usb device information
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_Composite_RxEP0Handler(USBD_INFO_T* usbInfo)
{
    USBD_STA_T  usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }

    if((usbDevComposite->cdcInfo.pUserData != NULL) && (usbDevComposite->cdcInfo.cdcCmd.opcode != 0xFF))
    {
        ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo.pUserData)->ItfCtrl(usbDevComposite->cdcInfo.cdcCmd.opcode, \
                                                                             (uint8_t *)usbDevComposite->cdcInfo.data, \
                                                                             (uint16_t)usbDevComposite->cdcInfo.cdcCmd.length);
        
        usbDevComposite->cdcInfo.cdcCmd.opcode = 0xFF;
    }


    return usbStatus;
}


/*!
 * @brief       USB device composite IN data handler
 *
 * @param       usbInfo: usb device information
 *
 * @param       epNum: endpoint number
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_Composite_DataInHandler(USBD_INFO_T* usbInfo, uint8_t epNum)
{
    USBD_STA_T  usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

#if defined(USE_DAL_DRIVER)
    PCD_HandleTypeDef* usbdh = (PCD_HandleTypeDef *)usbInfo->dataPoint;
#else
    USBD_HANDLE_T* usbdh = (USBD_HANDLE_T *)usbInfo->dataPoint;
#endif /* USE_DAL_DRIVER */

    if (usbdh == NULL)
    {
        return USBD_FAIL;
    }
    
    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }

    switch(epNum & 0x0F)
    {
        case (USBD_WINUSB_IN_EP_ADDR & 0x0F):
#if defined(USE_DAL_DRIVER)
            if((usbInfo->devEpIn[epNum & 0x0F].length > 0) && \
               (usbInfo->devEpIn[epNum & 0x0F].length % usbdh->IN_ep[epNum & 0x0F].maxpacket) == 0)
#else
            if((usbInfo->devEpIn[epNum & 0x0F].length > 0) && \
               (usbInfo->devEpIn[epNum & 0x0F].length % usbdh->epIN[epNum & 0x0F].mps) == 0)
#endif /* USE_DAL_DRIVER */
            {
                usbInfo->devEpIn[epNum & 0x0F].length = 0;
                
                USBD_EP_TransferCallback(usbInfo, epNum, NULL, 0);
            }
            else
            {
                usbDevComposite->winusbInfo.winusbTx.state = USBD_WINUSB_XFER_IDLE;
                
                if(((USBD_WINUSB_INTERFACE_T *)usbDevComposite->winusbInfo.pUserData)->ItfSendEnd != NULL)
                {
                    ((USBD_WINUSB_INTERFACE_T *)usbDevComposite->winusbInfo.pUserData)->ItfSendEnd(epNum, \
                                                                                                   usbDevComposite->winusbInfo.winusbTx.buffer, \
                                                                                                   &usbDevComposite->winusbInfo.winusbTx.length);
                }
            }
            break;
        
        case (USBD_CDC_DATA_IN_EP_ADDR & 0x0F):
#if defined(USE_DAL_DRIVER)
            if((usbInfo->devEpIn[epNum & 0x0F].length > 0) && \
               (usbInfo->devEpIn[epNum & 0x0F].length % usbdh->IN_ep[epNum & 0x0F].maxpacket) == 0)
#else
            if((usbInfo->devEpIn[epNum & 0x0F].length > 0) && \
               (usbInfo->devEpIn[epNum & 0x0F].length % usbdh->epIN[epNum & 0x0F].mps) == 0)
#endif /* USE_DAL_DRIVER */
            {
                usbInfo->devEpIn[epNum & 0x0F].length = 0;
                
                USBD_EP_TransferCallback(usbInfo, epNum, NULL, 0);
            }
            else
            {
                usbDevComposite->cdcInfo.cdcTx.state = USBD_CDC_XFER_IDLE;
                
                if(((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo.pUserData)->ItfSendEnd != NULL)
                {
                    ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo.pUserData)->ItfSendEnd(epNum, \
                                                                                             usbDevComposite->cdcInfo.cdcTx.buffer, \
                                                                                             &usbDevComposite->cdcInfo.cdcTx.length);
                }
            }
            break;
    }

    return usbStatus;
}

/*!
 * @brief       USB device composite OUT data handler
 *
 * @param       usbInfo: usb device information
 *
 * @param       epNum: endpoint number
 *
 * @retval      USB device operation status
 */
static USBD_STA_T USBD_Composite_DataOutHandler(USBD_INFO_T* usbInfo, uint8_t epNum)
{
    USBD_STA_T  usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }

    switch(epNum & 0x0F)
    {
        case (USBD_WINUSB_OUT_EP_ADDR & 0x0F):
            usbDevComposite->winusbInfo.winusbRx.length = USBD_EP_ReadRxDataLenCallback(usbInfo, epNum);
            
            ((USBD_WINUSB_INTERFACE_T *)usbDevComposite->winusbInfo.pUserData)->ItfReceive(usbDevComposite->winusbInfo.winusbRx.buffer, \
                                                                                                &usbDevComposite->winusbInfo.winusbRx.length);
            break;
        
        case (USBD_CDC_DATA_OUT_EP_ADDR & 0x0F):
            usbDevComposite->cdcInfo.cdcRx.length = USBD_EP_ReadRxDataLenCallback(usbInfo, epNum);
            
            ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo.pUserData)->ItfReceive(usbDevComposite->cdcInfo.cdcRx.buffer, \
                                                                                                &usbDevComposite->cdcInfo.cdcRx.length);
            break;
    }

    return usbStatus;
}

/*!
 * @brief       USB device CDC register interface handler
 *
 * @param       usbInfo: usb device information
 *
 * @param       itf: interface handler
 *
 * @retval      USB device operation status
 */
uint8_t USBD_Composite_RegisterCDCItf(USBD_INFO_T* usbInfo, USBD_CDC_INTERFACE_T* itf)
{
    USBD_STA_T usbStatus = USBD_FAIL;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    if (itf != NULL)
    {
        usbDevComposite->cdcInfo.pUserData = itf;
        usbStatus = USBD_OK;
    }

    return usbStatus;
}

/*!
 * @brief       USB device CDC configure TX buffer handler
 *
 * @param       usbInfo: usb device information
 *
 * @param       buffer: tx buffer
 *
 * @param       length: tx buffer length
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_Composite_CDC_ConfigTxBuffer(USBD_INFO_T* usbInfo, uint8_t *buffer, uint32_t length)
{
    USBD_STA_T  usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];
    
    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }
    
    usbDevComposite->cdcInfo.cdcTx.buffer = buffer;
    usbDevComposite->cdcInfo.cdcTx.length = length;
    
    return usbStatus;
}

/*!
 * @brief       USB device CDC configure RX buffer handler
 *
 * @param       usbInfo: usb device information
 *
 * @param       buffer: tx buffer
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_Composite_CDC_ConfigRxBuffer(USBD_INFO_T* usbInfo, uint8_t *buffer)
{
    USBD_STA_T  usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];
    
    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }
    
    usbDevComposite->cdcInfo.cdcRx.buffer = buffer;
    
    return usbStatus;
}

/*!
 * @brief       USB device CDC transmit packet handler
 *
 * @param       usbInfo: usb device information
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_Composite_CDC_TxPacket(USBD_INFO_T* usbInfo)
{
    USBD_STA_T usbStatus = USBD_BUSY;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];
    
    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }
    
    if(usbDevComposite->cdcInfo.cdcTx.state == USBD_CDC_XFER_IDLE)
    {
        usbDevComposite->cdcInfo.cdcTx.state = USBD_CDC_XFER_BUSY;
        
        usbInfo->devEpIn[usbDevComposite->cdcInfo.epInAddr & 0x0F].length = usbDevComposite->cdcInfo.cdcTx.length;
        
        USBD_EP_TransferCallback(usbInfo, usbDevComposite->cdcInfo.epInAddr, usbDevComposite->cdcInfo.cdcTx.buffer, usbDevComposite->cdcInfo.cdcTx.length);
        
        usbStatus = USBD_OK;
    }
    
    return usbStatus;
}

/*!
 * @brief       USB device CDC receive packet handler
 *
 * @param       usbInfo: usb device information
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_Composite_CDC_RxPacket(USBD_INFO_T* usbInfo)
{
    USBD_STA_T usbStatus = USBD_BUSY;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];
    
    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }
    
    if(usbInfo->devSpeed == USBD_SPEED_HS)
    {
        USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->cdcInfo.epOutAddr, \
                                usbDevComposite->cdcInfo.cdcRx.buffer, \
                                USBD_CDC_HS_MP_SIZE);
    }
    else
    {
        USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->cdcInfo.epOutAddr, \
                                usbDevComposite->cdcInfo.cdcRx.buffer, \
                                USBD_CDC_FS_MP_SIZE);
    }
    
    return usbStatus;
}

/*!
 * @brief     USB device CDC read interval
 *
 * @param     usbInfo: usb device information
 *
 * @retval    usb interval
 */
uint8_t USBD_Composite_CDC_ReadInterval(USBD_INFO_T* usbInfo)
{
    uint8_t interval;

    if (usbInfo->devSpeed == USBD_SPEED_FS)
    {
        interval = USBD_CDC_FS_INTERVAL;
    }
    else
    {
        interval = USBD_CDC_HS_INTERVAL;
    }

    return interval;
}

/*!
 * @brief       USB device WINUSB register interface handler
 *
 * @param       usbInfo: usb device information
 *
 * @param       itf: interface handler
 *
 * @retval      USB device operation status
 */
uint8_t USBD_Composite_RegisterWINUSBItf(USBD_INFO_T* usbInfo, USBD_WINUSB_INTERFACE_T* itf)
{
    USBD_STA_T usbStatus = USBD_FAIL;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    if (itf != NULL)
    {
        usbDevComposite->winusbInfo.pUserData = itf;
        usbStatus = USBD_OK;
    }

    return usbStatus;
}

/*!
 * @brief       USB device WINUSB configure TX buffer handler
 *
 * @param       usbInfo: usb device information
 *
 * @param       buffer: tx buffer
 *
 * @param       length: tx buffer length
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_Composite_WINUSB_ConfigTxBuffer(USBD_INFO_T* usbInfo, uint8_t *buffer, uint32_t length)
{
    USBD_STA_T  usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];
    
    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }
    
    usbDevComposite->winusbInfo.winusbTx.buffer = buffer;
    usbDevComposite->winusbInfo.winusbTx.length = length;
    
    return usbStatus;
}

/*!
 * @brief       USB device WINUSB configure RX buffer handler
 *
 * @param       usbInfo: usb device information
 *
 * @param       buffer: tx buffer
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_Composite_WINUSB_ConfigRxBuffer(USBD_INFO_T* usbInfo, uint8_t *buffer)
{
    USBD_STA_T  usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];
    
    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }
    
    usbDevComposite->winusbInfo.winusbRx.buffer = buffer;
    
    return usbStatus;
}

/*!
 * @brief       USB device WINUSB transmit packet handler
 *
 * @param       usbInfo: usb device information
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_Composite_WINUSB_TxPacket(USBD_INFO_T* usbInfo)
{
    USBD_STA_T usbStatus = USBD_BUSY;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];
    
    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }
    
    if(usbDevComposite->winusbInfo.winusbTx.state == USBD_WINUSB_XFER_IDLE)
    {
        usbDevComposite->winusbInfo.winusbTx.state = USBD_WINUSB_XFER_BUSY;
        
        usbInfo->devEpIn[usbDevComposite->winusbInfo.epInAddr & 0x0F].length = usbDevComposite->winusbInfo.winusbTx.length;
        
        USBD_EP_TransferCallback(usbInfo, usbDevComposite->winusbInfo.epInAddr, usbDevComposite->winusbInfo.winusbTx.buffer, usbDevComposite->winusbInfo.winusbTx.length);
        
        usbStatus = USBD_OK;
    }
    
    return usbStatus;
}

/*!
 * @brief       USB device WINUSB receive packet handler
 *
 * @param       usbInfo: usb device information
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_Composite_WINUSB_RxPacket(USBD_INFO_T* usbInfo)
{
    USBD_STA_T usbStatus = USBD_BUSY;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];
    
    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }
    
    if(usbInfo->devSpeed == USBD_SPEED_HS)
    {
        USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->winusbInfo.epOutAddr, \
                                usbDevComposite->winusbInfo.winusbRx.buffer, \
                                WINUSB_HS_MAX_PACKET_SIZE);
    }
    else
    {
        USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->winusbInfo.epOutAddr, \
                                usbDevComposite->winusbInfo.winusbRx.buffer, \
                                WINUSB_FS_MAX_PACKET_SIZE);
    }
    
    return usbStatus;
}

/*!
 * @brief     USB device WINUSB read interval
 *
 * @param     usbInfo: usb device information
 *
 * @retval    usb interval
 */
uint8_t USBD_Composite_WINUSB_ReadInterval(USBD_INFO_T* usbInfo)
{
    uint8_t interval;

    if (usbInfo->devSpeed == USBD_SPEED_FS)
    {
        interval = USBD_WINUSB_FS_INTERVAL;
    }
    else
    {
        interval = USBD_WINUSB_HS_INTERVAL;
    }

    return interval;
}

/**@} end of group OTGD_Composite_CDC_WINUSB_Functions */
/**@} end of group OTGD_Composite_CDC_WINUSB */
/**@} end of group Examples */
