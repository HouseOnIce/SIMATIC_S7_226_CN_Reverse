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

/** @addtogroup USBD_Composite_CDC
  @{
  */

/** @defgroup USBD_Composite_CDC_Functions Functions
  @{
  */
static USBD_STA_T USBD_Composite_ClassInitHandler(USBD_INFO_T* usbInfo, uint8_t cfgIndex);
static USBD_STA_T USBD_Composite_ClassDeInitHandler(USBD_INFO_T* usbInfo, uint8_t cfgIndex);
static USBD_STA_T USBD_Composite_SOFHandler(USBD_INFO_T* usbInfo);
static USBD_STA_T USBD_Composite_SetupHandler(USBD_INFO_T* usbInfo, USBD_REQ_SETUP_T* req);
static USBD_STA_T USBD_Composite_RxEP0Handler(USBD_INFO_T* usbInfo);
static USBD_STA_T USBD_Composite_DataInHandler(USBD_INFO_T* usbInfo, uint8_t epNum);
static USBD_STA_T USBD_Composite_DataOutHandler(USBD_INFO_T* usbInfo, uint8_t epNum);

/**@} end of group USBD_Composite_CDC_Functions */

/** @defgroup USBD_Composite_CDC_Structures Structures
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

/**@} end of group USBD_Composite_CDC_Structures*/

/** @defgroup USBD_Composite_CDC_Functions Functions
  @{
  */

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
USBD_STA_T USBD_Composite_Init(USBD_INFO_T* usbInfo, void* itf1)
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

        usbDevComposite->ephash[USBD_CDC1_DATA_OUT_EP_ADDR & 0xF] = CDC1;
        usbDevComposite->ephash[USBD_CDC1_CMD_EP_ADDR & 0xF] = CDC1;
        usbDevComposite->ephash[USBD_CDC2_DATA_OUT_EP_ADDR & 0xF] = CDC2;
        usbDevComposite->interfacehash[USBD_CDC1_CTRL_INTERFACE_NUM] = CDC1;
        usbDevComposite->interfacehash[USBD_CDC1_DATA_INTERFACE_NUM] = CDC1;
        usbDevComposite->interfacehash[USBD_CDC2_CTRL_INTERFACE_NUM] = CDC2;
        usbDevComposite->interfacehash[USBD_CDC2_DATA_INTERFACE_NUM] = CDC2;
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
        for(uint8_t i = 0; i < USBD_CDC_NUM; i++)
        {
            if(((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo[i].pUserData)->ItfDeInit != NULL)
            {
                ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo[i].pUserData)->ItfDeInit();
            }
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

    usbDevComposite->cdcInfo[CDC1].epCmdAddr = USBD_CDC1_CMD_EP_ADDR;
    usbDevComposite->cdcInfo[CDC1].epInAddr = USBD_CDC1_DATA_IN_EP_ADDR;
    usbDevComposite->cdcInfo[CDC1].epOutAddr = USBD_CDC1_DATA_OUT_EP_ADDR;

    usbDevComposite->cdcInfo[CDC2].epCmdAddr = USBD_CDC2_CMD_EP_ADDR;
    usbDevComposite->cdcInfo[CDC2].epInAddr = USBD_CDC2_DATA_IN_EP_ADDR;
    usbDevComposite->cdcInfo[CDC2].epOutAddr = USBD_CDC2_DATA_OUT_EP_ADDR;

    /* Open Command endpoint */
    USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo[CDC1].epCmdAddr, EP_TYPE_INTERRUPT, USBD_CDC_CMD_MP_SIZE);
    usbInfo->devEpIn[usbDevComposite->cdcInfo[CDC1].epCmdAddr & 0x0F].useStatus = ENABLE;

    USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo[CDC2].epCmdAddr, EP_TYPE_INTERRUPT, USBD_CDC_CMD_MP_SIZE);
    usbInfo->devEpIn[usbDevComposite->cdcInfo[CDC2].epCmdAddr & 0x0F].useStatus = ENABLE;

    /* Open Data endpoint */
    switch (usbInfo->devSpeed)
    {
        case USBD_SPEED_FS:
            /* CDC1 */
            USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo[CDC1].epOutAddr, EP_TYPE_BULK, USBD_CDC_FS_MP_SIZE);
            usbInfo->devEpOut[usbDevComposite->cdcInfo[CDC1].epOutAddr & 0x0F].useStatus = ENABLE;

            USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo[CDC1].epInAddr, EP_TYPE_BULK, USBD_CDC_FS_MP_SIZE);
            usbInfo->devEpIn[usbDevComposite->cdcInfo[CDC1].epInAddr & 0x0F].useStatus = ENABLE;
    
            usbInfo->devEpIn[usbDevComposite->cdcInfo[CDC1].epCmdAddr & 0x0F].interval = USBD_CDC_FS_INTERVAL;
            /* CDC2 */
            USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo[CDC2].epOutAddr, EP_TYPE_BULK, USBD_CDC_FS_MP_SIZE);
            usbInfo->devEpOut[usbDevComposite->cdcInfo[CDC2].epOutAddr & 0x0F].useStatus = ENABLE;

            USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo[CDC2].epInAddr, EP_TYPE_BULK, USBD_CDC_FS_MP_SIZE);
            usbInfo->devEpIn[usbDevComposite->cdcInfo[CDC2].epInAddr & 0x0F].useStatus = ENABLE;
        
            usbInfo->devEpIn[usbDevComposite->cdcInfo[CDC2].epCmdAddr & 0x0F].interval = USBD_CDC_FS_INTERVAL;
    
            break;

        default:
            /* CDC1 */
            USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo[CDC1].epOutAddr, EP_TYPE_BULK, USBD_CDC_HS_MP_SIZE);
            usbInfo->devEpOut[usbDevComposite->cdcInfo[CDC1].epOutAddr & 0x0F].useStatus = ENABLE;

            USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo[CDC1].epInAddr, EP_TYPE_BULK, USBD_CDC_HS_MP_SIZE);
            usbInfo->devEpIn[usbDevComposite->cdcInfo[CDC1].epInAddr & 0x0F].useStatus = ENABLE;
    
            usbInfo->devEpIn[usbDevComposite->cdcInfo[CDC1].epCmdAddr & 0x0F].interval = USBD_CDC_HS_INTERVAL;
            /* CDC2 */
            USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo[CDC2].epOutAddr, EP_TYPE_BULK, USBD_CDC_HS_MP_SIZE);
            usbInfo->devEpOut[usbDevComposite->cdcInfo[CDC2].epOutAddr & 0x0F].useStatus = ENABLE;

            USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo[CDC2].epInAddr, EP_TYPE_BULK, USBD_CDC_HS_MP_SIZE);
            usbInfo->devEpIn[usbDevComposite->cdcInfo[CDC2].epInAddr & 0x0F].useStatus = ENABLE;
        
            usbInfo->devEpIn[usbDevComposite->cdcInfo[CDC2].epCmdAddr & 0x0F].interval = USBD_CDC_HS_INTERVAL;
            break;
    }

    /* Interface Init */
    /* CDC1 */
    usbDevComposite->cdcInfo[CDC1].cdcTx.buffer = NULL;
    usbDevComposite->cdcInfo[CDC1].cdcRx.buffer = NULL;

    usbDevComposite->cdcInfo[CDC1].cdcTx.state = USBD_CDC_XFER_IDLE;
    usbDevComposite->cdcInfo[CDC1].cdcRx.state = USBD_CDC_XFER_IDLE;
    
    /* CDC2 */
    usbDevComposite->cdcInfo[CDC2].cdcTx.buffer = NULL;
    usbDevComposite->cdcInfo[CDC2].cdcRx.buffer = NULL;

    usbDevComposite->cdcInfo[CDC2].cdcTx.state = USBD_CDC_XFER_IDLE;
    usbDevComposite->cdcInfo[CDC2].cdcRx.state = USBD_CDC_XFER_IDLE;


    if (usbDevComposite->cdcInfo[CDC1].pUserData != NULL)
    {
        ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo[CDC1].pUserData)->ItfInit(CDC1);
    }

    if (usbDevComposite->cdcInfo[CDC2].pUserData != NULL)
    {
        ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo[CDC2].pUserData)->ItfInit(CDC2);
    }

    if(usbDevComposite->cdcInfo[CDC1].cdcRx.buffer == NULL || usbDevComposite->cdcInfo[CDC2].cdcRx.buffer == NULL)
    {
        USBD_USR_LOG("cdcRx buffer is NULL");
        return USBD_FAIL;
    }

    switch (usbInfo->devSpeed)
    {
        case USBD_SPEED_FS:
            USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->cdcInfo[CDC1].epOutAddr, \
                                    usbDevComposite->cdcInfo[CDC1].cdcRx.buffer, \
                                    USBD_CDC_FS_MP_SIZE);
            USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->cdcInfo[CDC2].epOutAddr, \
                                    usbDevComposite->cdcInfo[CDC2].cdcRx.buffer, \
                                    USBD_CDC_FS_MP_SIZE);
            break;

        default:
            USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->cdcInfo[CDC1].epOutAddr, \
                                    usbDevComposite->cdcInfo[CDC1].cdcRx.buffer, \
                                    USBD_CDC_HS_MP_SIZE);
            USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->cdcInfo[CDC2].epOutAddr, \
                                    usbDevComposite->cdcInfo[CDC2].cdcRx.buffer, \
                                    USBD_CDC_HS_MP_SIZE);
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

    for(uint8_t i = 0; i < USBD_CDC_NUM; i++)
    {
        /* Close CDC EP */
        USBD_EP_CloseCallback(usbInfo, usbDevComposite->cdcInfo[i].epOutAddr);
        usbInfo->devEpOut[usbDevComposite->cdcInfo[i].epOutAddr & 0x0F].useStatus = DISABLE;

        USBD_EP_CloseCallback(usbInfo, usbDevComposite->cdcInfo[i].epInAddr);
        usbInfo->devEpIn[usbDevComposite->cdcInfo[i].epInAddr & 0x0F].useStatus = DISABLE;
    }
    
    USBD_EP_CloseCallback(usbInfo, usbDevComposite->cdcInfo[CDC1].epCmdAddr);
    usbInfo->devEpIn[usbDevComposite->cdcInfo[CDC1].epCmdAddr & 0x0F].useStatus = DISABLE;
    usbInfo->devEpIn[usbDevComposite->cdcInfo[CDC1].epCmdAddr & 0x0F].interval = 0;

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
                        ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo[usbDevComposite->interfacehash[wIndex & 0xFF]].pUserData)->ItfCtrl(request, \
                                                                                             (uint8_t *)usbDevComposite->cdcInfo[usbDevComposite->interfacehash[wIndex & 0xFF]].data,
                                                                                             wLength, usbDevComposite->interfacehash[wIndex & 0xFF]);
                        
                        length = USBD_CDC_DATA_MP_SIZE < wLength ? USBD_CDC_DATA_MP_SIZE : wLength;
                        USBD_CtrlSendData(usbInfo, (uint8_t *)usbDevComposite->cdcInfo[usbDevComposite->interfacehash[wIndex & 0xFF]].data, length);
                    }
                    else
                    {
                        usbDevComposite->cdcInfo[usbDevComposite->interfacehash[wIndex & 0xFF]].cdcCmd.opcode = request;
                        usbDevComposite->cdcInfo[usbDevComposite->interfacehash[wIndex & 0xFF]].cdcCmd.length = wLength < USBD_EP0_PACKET_MAX_SIZE ? \
                                                   wLength : USBD_EP0_PACKET_MAX_SIZE;
                        
                        USBD_Composite_CDC_CtrlReceiveData(usbInfo, (uint8_t *)usbDevComposite->cdcInfo[usbDevComposite->interfacehash[wIndex & 0xFF]].data, \
                                                           usbDevComposite->cdcInfo[usbDevComposite->interfacehash[wIndex & 0xFF]].cdcCmd.length);
                    }
                }
                else
                {
                    ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo[usbDevComposite->interfacehash[wIndex & 0xFF]].pUserData)->ItfCtrl(request, \
                                                                                         (uint8_t *)req, \
                                                                                         0, usbDevComposite->interfacehash[wIndex & 0xFF]);
                }
            break;

        case USBD_REQ_TYPE_VENDOR:
            USBD_REQ_CtrlError(usbInfo, req);
            usbStatus = USBD_FAIL;
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

    for(uint8_t i = 0; i < USBD_CDC_NUM; i++)
    {
        /* CDC */
        if((usbDevComposite->cdcInfo[i].pUserData != NULL) && (usbDevComposite->cdcInfo[i].cdcCmd.opcode != 0xFF))
        {
            ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo[i].pUserData)->ItfCtrl(usbDevComposite->cdcInfo[i].cdcCmd.opcode, \
                                                                                (uint8_t *)usbDevComposite->cdcInfo[i].data, \
                                                                                (uint16_t)usbDevComposite->cdcInfo[i].cdcCmd.length, i);
        
            usbDevComposite->cdcInfo[i].cdcCmd.opcode = 0xFF;
        }
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
            usbDevComposite->cdcInfo[usbDevComposite->ephash[epNum & 0x0F]].cdcTx.state = USBD_CDC_XFER_IDLE;
                
            if(((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo[usbDevComposite->ephash[epNum & 0x0F]].pUserData)->ItfSendEnd != NULL)
            {
                ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo[usbDevComposite->ephash[epNum & 0x0F]].pUserData)->ItfSendEnd(epNum, \
                                                          usbDevComposite->cdcInfo[usbDevComposite->ephash[epNum & 0x0F]].cdcTx.buffer, \
                                                          &usbDevComposite->cdcInfo[usbDevComposite->ephash[epNum & 0x0F]].cdcTx.length);
            }
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

    usbDevComposite->cdcInfo[usbDevComposite->ephash[epNum & 0x0F]].cdcRx.length = USBD_EP_ReadRxDataLenCallback(usbInfo, epNum);
    
    ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo[usbDevComposite->ephash[epNum & 0x0F]].pUserData)->ItfReceive(\
                                                            usbDevComposite->cdcInfo[usbDevComposite->ephash[epNum & 0x0F]].cdcRx.buffer,\
                                                            &usbDevComposite->cdcInfo[usbDevComposite->ephash[epNum & 0x0F]].cdcRx.length,\
                                                            usbDevComposite->ephash[epNum]);

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
        for(uint8_t i = 0; i < USBD_CDC_NUM; i++)
        {
            usbDevComposite->cdcInfo[i].pUserData = itf;
        }
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
USBD_STA_T USBD_Composite_CDC_ConfigTxBuffer(USBD_INFO_T* usbInfo, uint8_t *buffer, uint32_t length, uint8_t cdcnum)
{
    USBD_STA_T  usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];
    
    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }
    
    usbDevComposite->cdcInfo[cdcnum].cdcTx.buffer = buffer;
    usbDevComposite->cdcInfo[cdcnum].cdcTx.length = length;
    
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
USBD_STA_T USBD_Composite_CDC_ConfigRxBuffer(USBD_INFO_T* usbInfo, uint8_t *buffer, uint8_t cdcnum)
{
    USBD_STA_T  usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];
    
    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }
    
    usbDevComposite->cdcInfo[cdcnum].cdcRx.buffer = buffer;
    
    return usbStatus;
}

/*!
 * @brief       USB device CDC transmit packet handler
 *
 * @param       usbInfo: usb device information
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_Composite_CDC_TxPacket(USBD_INFO_T* usbInfo, uint8_t cdcnum)
{
    USBD_STA_T usbStatus = USBD_BUSY;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];
    
    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }
    
    if(usbDevComposite->cdcInfo[cdcnum].cdcTx.state == USBD_CDC_XFER_IDLE)
    {
        usbDevComposite->cdcInfo[cdcnum].cdcTx.state = USBD_CDC_XFER_BUSY;
        
        usbInfo->devEpIn[usbDevComposite->cdcInfo[cdcnum].epInAddr & 0x0F].length = usbDevComposite->cdcInfo[cdcnum].cdcTx.length;
        
        USBD_EP_TransferCallback(usbInfo, usbDevComposite->cdcInfo[cdcnum].epInAddr, usbDevComposite->cdcInfo[cdcnum].cdcTx.buffer, usbDevComposite->cdcInfo[cdcnum].cdcTx.length);
        
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
USBD_STA_T USBD_Composite_CDC_RxPacket(USBD_INFO_T* usbInfo, uint8_t cdcnum)
{
    USBD_STA_T usbStatus = USBD_BUSY;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];
    
    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }
    
    if(usbInfo->devSpeed == USBD_SPEED_HS)
    {
        USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->cdcInfo[cdcnum].epOutAddr, \
                                usbDevComposite->cdcInfo[cdcnum].cdcRx.buffer, \
                                USBD_CDC_HS_MP_SIZE);
    }
    else
    {
        USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->cdcInfo[cdcnum].epOutAddr, \
                                usbDevComposite->cdcInfo[cdcnum].cdcRx.buffer, \
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

/**@} end of group USBD_Composite_CDC_Functions */
/**@} end of group USBD_Composite_CDC */
/**@} end of group Examples */
