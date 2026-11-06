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

/** @addtogroup OTGD_Composite_CDC_MSC
  @{
  */

/** @defgroup OTGD_Composite_CDC_MSC_Functions Functions
  @{
  */
static USBD_STA_T USBD_Composite_ClassInitHandler(USBD_INFO_T* usbInfo, uint8_t cfgIndex);
static USBD_STA_T USBD_Composite_ClassDeInitHandler(USBD_INFO_T* usbInfo, uint8_t cfgIndex);
static USBD_STA_T USBD_Composite_SOFHandler(USBD_INFO_T* usbInfo);
static USBD_STA_T USBD_Composite_SetupHandler(USBD_INFO_T* usbInfo, USBD_REQ_SETUP_T* req);
static USBD_STA_T USBD_Composite_RxEP0Handler(USBD_INFO_T* usbInfo);
static USBD_STA_T USBD_Composite_DataInHandler(USBD_INFO_T* usbInfo, uint8_t epNum);
static USBD_STA_T USBD_Composite_DataOutHandler(USBD_INFO_T* usbInfo, uint8_t epNum);
/**@} end of group OTGD_Composite_CDC_MSC_Functions */

/** @defgroup OTGD_Composite_CDC_MSC_Structures Structures
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

/**@} end of group OTGD_Composite_CDC_MSC_Structures*/

/** @defgroup OTGD_Composite_CDC_MSC_Variables Variables
  @{
  */

/**@} end of group OTGD_Composite_CDC_MSC_Variables*/

/** @defgroup OTGD_Composite_CDC_MSC_Functions Functions
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

    if((USBD_MSC_MEMORY_T*)itf2 == NULL)
    {
        USBD_USR_LOG("itf2 is NULL");
        return USBD_FAIL;
    }
    else
    {
        USBD_Composite_RegisterMemoryItf(usbInfo, (USBD_MSC_MEMORY_T*)itf2);
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

        USBD_MSC_BOT_DeInit(usbInfo);

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
    
    usbDevComposite->mscInfo.epInAddr = USBD_MSC_IN_EP_ADDR;
    usbDevComposite->mscInfo.epOutAddr = USBD_MSC_OUT_EP_ADDR;

    /* Open Command endpoint */
    USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo.epCmdAddr, EP_TYPE_INTERRUPT, USBD_CDC_CMD_MP_SIZE);
    usbInfo->devEpIn[usbDevComposite->cdcInfo.epCmdAddr & 0x0F].useStatus = ENABLE;

    /* Open Data endpoint */
    switch (usbInfo->devSpeed)
    {
        case USBD_SPEED_FS:
            /* MSC */
            USBD_EP_OpenCallback(usbInfo, usbDevComposite->mscInfo.epOutAddr, EP_TYPE_BULK, USBD_MSC_FS_MP_SIZE);
            usbInfo->devEpOut[usbDevComposite->mscInfo.epOutAddr & 0x0F].useStatus = ENABLE;

            USBD_EP_OpenCallback(usbInfo, usbDevComposite->mscInfo.epInAddr, EP_TYPE_BULK, USBD_MSC_FS_MP_SIZE);
            usbInfo->devEpIn[usbDevComposite->mscInfo.epInAddr & 0x0F].useStatus = ENABLE;

            /* CDC */
            USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo.epOutAddr, EP_TYPE_BULK, USBD_CDC_FS_MP_SIZE);
            usbInfo->devEpOut[usbDevComposite->cdcInfo.epOutAddr & 0x0F].useStatus = ENABLE;

            USBD_EP_OpenCallback(usbInfo, usbDevComposite->cdcInfo.epInAddr, EP_TYPE_BULK, USBD_CDC_FS_MP_SIZE);
            usbInfo->devEpIn[usbDevComposite->cdcInfo.epInAddr & 0x0F].useStatus = ENABLE;
        
            usbInfo->devEpIn[usbDevComposite->cdcInfo.epCmdAddr & 0x0F].interval = USBD_CDC_FS_INTERVAL;
            break;

        default:
            /* MSC */
            USBD_EP_OpenCallback(usbInfo, usbDevComposite->mscInfo.epOutAddr, EP_TYPE_BULK, USBD_MSC_HS_MP_SIZE);
            usbInfo->devEpOut[usbDevComposite->mscInfo.epOutAddr & 0x0F].useStatus = ENABLE;

            USBD_EP_OpenCallback(usbInfo, usbDevComposite->mscInfo.epInAddr, EP_TYPE_BULK, USBD_MSC_HS_MP_SIZE);
            usbInfo->devEpIn[usbDevComposite->mscInfo.epInAddr & 0x0F].useStatus = ENABLE;

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

    if (usbDevComposite->cdcInfo.pUserData != NULL)
    {
        ((USBD_CDC_INTERFACE_T *)usbDevComposite->cdcInfo.pUserData)->ItfInit();
    }

    USBD_MSC_BOT_Init(usbInfo);

    if(usbDevComposite->cdcInfo.cdcRx.buffer == NULL)
    {
        USBD_USR_LOG("cdcRx buffer is NULL");
        return USBD_FAIL;
    }

    switch (usbInfo->devSpeed)
    {
        case USBD_SPEED_FS:
            USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->cdcInfo.epOutAddr, \
                                    usbDevComposite->cdcInfo.cdcRx.buffer, \
                                    USBD_CDC_FS_MP_SIZE);
            break;

        default:
            USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->cdcInfo.epOutAddr, \
                                    usbDevComposite->cdcInfo.cdcRx.buffer, \
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

    /* Close CDC EP */
    USBD_EP_CloseCallback(usbInfo, usbDevComposite->cdcInfo.epOutAddr);
    usbInfo->devEpOut[usbDevComposite->cdcInfo.epOutAddr & 0x0F].useStatus = DISABLE;

    USBD_EP_CloseCallback(usbInfo, usbDevComposite->cdcInfo.epInAddr);
    usbInfo->devEpIn[usbDevComposite->cdcInfo.epInAddr & 0x0F].useStatus = DISABLE;

    USBD_EP_CloseCallback(usbInfo, usbDevComposite->cdcInfo.epCmdAddr);
    usbInfo->devEpIn[usbDevComposite->cdcInfo.epCmdAddr & 0x0F].useStatus = DISABLE;
    usbInfo->devEpIn[usbDevComposite->cdcInfo.epCmdAddr & 0x0F].interval = 0;

    /* Close MSC EP */
    USBD_EP_CloseCallback(usbInfo, usbDevComposite->mscInfo.epOutAddr);
    usbInfo->devEpOut[usbDevComposite->mscInfo.epOutAddr & 0x0F].useStatus = DISABLE;

    USBD_EP_CloseCallback(usbInfo, usbDevComposite->mscInfo.epInAddr);
    usbInfo->devEpIn[usbDevComposite->mscInfo.epInAddr & 0x0F].useStatus = DISABLE;

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
    uint16_t wValue = req->DATA_FIELD.wValue[0] | req->DATA_FIELD.wValue[1] << 8;
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
                    if (usbInfo->devState == USBD_DEV_CONFIGURE)
                    {
                        if (wValue == USBD_FEATURE_SELECTOR_ENDPOINT_HALT)
                        {
                            USBD_EP_FlushCallback(usbInfo, wIndex);

                            /* BOT error */
                            USBD_MSC_BOT_ClearFeature(usbInfo, wIndex);
                        }
                    }
                    break;

                default:
                    USBD_REQ_CtrlError(usbInfo, req);
                    usbStatus = USBD_FAIL;
                    break;
            }
            break;

        case USBD_REQ_TYPE_CLASS:
            switch(wIndex & 0xFF)
            {
                case USBD_MSC_INTERFACE_NUM:
                    switch (request)
                    {
                        case USBD_CLASS_GET_MAX_LUN:
                            if ((req->DATA_FIELD.bmRequest.REQ_TYPE_B.dir == EP_DIR_IN) && \
                                    (wValue == 0) && (wLength == 1))
                            {
                                usbDevComposite->mscInfo.maxLun = ((USBD_MSC_MEMORY_T*)usbDevComposite->mscInfo.pUserData)->MemoryReadMaxLun();

                                USBD_CtrlSendData(usbInfo, (uint8_t*)&usbDevComposite->mscInfo.maxLun, 1);
                            }
                            else
                            {
                                USBD_REQ_CtrlError(usbInfo, req);
                                usbStatus = USBD_FAIL;
                            }
                            break;

                        case USBD_CLASS_BOT_RESET:
                            if ((req->DATA_FIELD.bmRequest.REQ_TYPE_B.dir == EP_DIR_OUT) && \
                                    (wValue == 0) && (wLength == 0))
                            {
                                USBD_MSC_BOT_Reset(usbInfo);
                            }
                            else
                            {
                                USBD_REQ_CtrlError(usbInfo, req);
                                usbStatus = USBD_FAIL;
                            }
                            break;

                        default:
                            USBD_REQ_CtrlError(usbInfo, req);
                            usbStatus = USBD_FAIL;
                            break;
                    }
                    break;
                
                case USBD_CDC_CTRL_INTERFACE_NUM:
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
                
                default:
                    USBD_REQ_CtrlError(usbInfo, req);
                    usbStatus = USBD_FAIL;
                    break;
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
    uint8_t reqStatus = USBD_BUSY;
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
        case (USBD_MSC_IN_EP_ADDR & 0x0F):
            switch (usbDevComposite->mscInfo.usbDevBOT.state)
            {
                case USBD_BOT_DATAIN:
                    reqStatus = USBD_SCSI_Handle(usbInfo, usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.bLUN, \
                                                 &usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.CB[0]);

                    if (reqStatus == USBD_FAIL)
                    {
                        USBD_MSC_BOT_SendCSW(usbInfo, USBD_BOT_CSW_FAIL);
                    }
                    break;

                case USBD_BOT_DATAIN_LAST:
                case USBD_BOT_DATA_SEND:
                    USBD_MSC_BOT_SendCSW(usbInfo, USBD_BOT_CSW_OK);
                    break;

                default:
                    break;
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
    uint8_t reqStatus = USBD_BUSY;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }

    switch(epNum & 0x0F)
    {
        case (USBD_MSC_OUT_EP_ADDR & 0x0F):
            /* Handler BOT state */
            switch (usbDevComposite->mscInfo.usbDevBOT.state)
            {
                case USBD_BOT_IDLE:
                    USBD_MSC_BOT_CBW_Decode(usbInfo);
                    break;

                case USBD_BOT_DATAOUT:
                    reqStatus = USBD_SCSI_Handle(usbInfo, usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.bLUN, \
                                                 &usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.CB[0]);
                    if (reqStatus == USBD_FAIL)
                    {
                        USBD_MSC_BOT_SendCSW(usbInfo, USBD_BOT_CSW_FAIL);
                    }
                    break;

                default:
                    break;
            }
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
 * @brief       USB device MSC register memory handler
 *
 * @param       usbInfo: usb device information
 *
 * @param       itf: interface handler
 *
 * @retval      USB device operation status
 */
uint8_t USBD_Composite_RegisterMemoryItf(USBD_INFO_T* usbInfo, USBD_MSC_MEMORY_T* memory)
{
    USBD_STA_T usbStatus = USBD_FAIL;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    if (memory != NULL)
    {
        usbDevComposite->mscInfo.pUserData = memory;
        usbStatus = USBD_OK;
    }

    return usbStatus;
}

/**@} end of group OTGD_Composite_CDC_MSC_Functions */
/**@} end of group OTGD_Composite_CDC_MSC */
/**@} end of group Examples */
