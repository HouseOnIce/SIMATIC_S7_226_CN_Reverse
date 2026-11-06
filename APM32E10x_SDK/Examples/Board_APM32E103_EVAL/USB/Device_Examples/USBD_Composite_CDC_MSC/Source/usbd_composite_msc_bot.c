/*!
 * @file        usbd_composite_msc_bot.c
 *
 * @brief       usb device msc bot handler
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
#include "usbd_composite_msc_bot.h"
#include "usbd_composite.h"
#include "usbd_dataXfer.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief     Init BOT of USB device MSC
 *
 * @param     usbInfo : usb handler information
 *
 * @retval    USB device operation status
 */
USBD_STA_T USBD_MSC_BOT_Init(USBD_INFO_T* usbInfo)
{
    USBD_STA_T  usbStatus = USBD_OK;

    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }

    usbDevComposite->mscInfo.usbDevSCSI.senseHead = 0;
    usbDevComposite->mscInfo.usbDevSCSI.senseEnd = 0;
    usbDevComposite->mscInfo.usbDevSCSI.mediumState = USBD_SCSI_MEDIUM_UNLOCK;

    /* Init USB device memory managment */
    ((USBD_MSC_MEMORY_T*)usbDevComposite->mscInfo.pUserData)->MemoryInit(0);

    usbDevComposite->mscInfo.usbDevBOT.state = USBD_BOT_IDLE;
    usbDevComposite->mscInfo.usbDevBOT.status = USBD_BOT_NORMAL;

    USBD_EP_FlushCallback(usbInfo, usbDevComposite->mscInfo.epInAddr);
    USBD_EP_FlushCallback(usbInfo, usbDevComposite->mscInfo.epOutAddr);

    USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->mscInfo.epOutAddr, \
                            (uint8_t*)&usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW,
                            USBD_MSC_BOT_CBW_LEN);
    return usbStatus;
}

/*!
 * @brief     De-init BOT of USB device MSC
 *
 * @param     usbInfo : usb handler information
 *
 * @retval    USB device operation status
 */
USBD_STA_T USBD_MSC_BOT_DeInit(USBD_INFO_T* usbInfo)
{
    USBD_STA_T  usbStatus = USBD_OK;

    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }

    usbDevComposite->mscInfo.usbDevBOT.state = USBD_BOT_IDLE;

    return usbStatus;
}

/*!
 * @brief     Reset BOT of USB device MSC
 *
 * @param     usbInfo : usb handler information
 *
 * @retval    USB device operation status
 */
USBD_STA_T USBD_MSC_BOT_Reset(USBD_INFO_T* usbInfo)
{
    USBD_STA_T  usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }

    usbDevComposite->mscInfo.usbDevBOT.state = USBD_BOT_IDLE;
    usbDevComposite->mscInfo.usbDevBOT.status = USBD_BOT_RECOVERY;

    USBD_EP_ClearStallCallback(usbInfo, usbDevComposite->mscInfo.epInAddr);
    USBD_EP_ClearStallCallback(usbInfo, usbDevComposite->mscInfo.epOutAddr);

    USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->mscInfo.epOutAddr, \
                            (uint8_t*)&usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW, \
                            USBD_MSC_BOT_CBW_LEN);

    return usbStatus;
}

/*!
 * @brief     Abort BOT of USB device MSC
 *
 * @param     usbInfo : usb handler information
 *
 * @retval    USB device operation status
 */
USBD_STA_T USBD_MSC_BOT_Abort(USBD_INFO_T* usbInfo)
{
    USBD_STA_T  usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }

    if ((usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.bmFlags.CBW_Flag == 0) && \
            (usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.dDataXferLen != 0) && \
            (usbDevComposite->mscInfo.usbDevBOT.status == USBD_BOT_NORMAL))
    {
        USBD_EP_StallCallback(usbInfo, usbDevComposite->mscInfo.epOutAddr);
    }

    USBD_EP_StallCallback(usbInfo, usbDevComposite->mscInfo.epInAddr);
    
    if (usbDevComposite->mscInfo.usbDevBOT.status == USBD_BOT_ERR)
    {
         USBD_EP_StallCallback(usbInfo, usbDevComposite->mscInfo.epInAddr);
         USBD_EP_StallCallback(usbInfo, usbDevComposite->mscInfo.epOutAddr);
    }

    return usbStatus;
}

/*!
 * @brief     Send CSW packet
 *
 * @param     usbInfo : usb handler information
 *
 * @param     status : CSW status
 *
 * @retval    USB device operation status
 */
USBD_STA_T USBD_MSC_BOT_SendCSW(USBD_INFO_T* usbInfo, USBD_BOT_CSW_STA_T status)
{
    USBD_STA_T  usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }

    usbDevComposite->mscInfo.usbDevBOT.cmdPack.CSW.DATA_FIELD.dSignature = USBD_MSC_BOT_CSW_SIGNATURE;
    usbDevComposite->mscInfo.usbDevBOT.cmdPack.CSW.DATA_FIELD.bStatus = status;

    usbDevComposite->mscInfo.usbDevBOT.state = USBD_BOT_IDLE;

    USBD_EP_TransferCallback(usbInfo, usbDevComposite->mscInfo.epInAddr, \
                             (uint8_t*)&usbDevComposite->mscInfo.usbDevBOT.cmdPack.CSW, \
                             USBD_MSC_BOT_CSW_LEN);

    USBD_EP_ReceiveCallback(usbInfo, usbDevComposite->mscInfo.epOutAddr, \
                            (uint8_t*)&usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW, \
                            USBD_MSC_BOT_CBW_LEN);

    return usbStatus;
}

/*!
 * @brief     Send BOT data
 *
 * @param     usbInfo : usb handler information
 *
 * @param     buffer : data buffer
 *
 * @param     length : data length
 *
 * @retval    USB device operation status
 */
USBD_STA_T USBD_MSC_BOT_SendData(USBD_INFO_T* usbInfo, uint8_t* buffer, uint32_t length)
{
    USBD_STA_T  usbStatus = USBD_OK;

    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    uint32_t lengthTemp;

    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }

    lengthTemp = length;

    if (usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.dDataXferLen < length)
    {
        lengthTemp = usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.dDataXferLen;
    }

    usbDevComposite->mscInfo.usbDevBOT.cmdPack.CSW.DATA_FIELD.dDataResidue -= length;
    usbDevComposite->mscInfo.usbDevBOT.cmdPack.CSW.DATA_FIELD.bStatus = USBD_BOT_CSW_OK;
    usbDevComposite->mscInfo.usbDevBOT.state = USBD_BOT_DATA_SEND;

    USBD_EP_TransferCallback(usbInfo, usbDevComposite->mscInfo.epInAddr, buffer, lengthTemp);

    return usbStatus;
}

/*!
 * @brief     Decode CBW packet
 *
 * @param     usbInfo : usb handler information
 *
 * @retval    USB device operation status
 */
USBD_STA_T USBD_MSC_BOT_CBW_Decode(USBD_INFO_T* usbInfo)
{
    USBD_STA_T  usbStatus = USBD_OK;
    uint8_t reqStatus = USBD_BUSY;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];
    uint32_t lastRevDataLen;

    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }

    usbDevComposite->mscInfo.usbDevBOT.cmdPack.CSW.DATA_FIELD.dTag = \
            usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.dTag;

    usbDevComposite->mscInfo.usbDevBOT.cmdPack.CSW.DATA_FIELD.dDataResidue = \
            usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.dDataXferLen;

    lastRevDataLen = USBD_EP_ReadRxDataLenCallback(usbInfo, usbDevComposite->mscInfo.epOutAddr);

    if ((lastRevDataLen != USBD_MSC_BOT_CBW_LEN) || \
            (usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.dSignature != USBD_MSC_BOT_CBW_SIGNATURE) || \
            (usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.bLUN > 1) || \
            (usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.bCBLen < 1) || \
            (usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.bCBLen > 16))
    {
        USBD_SCSI_CodeSense(usbInfo, usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.bLUN, \
                            USBD_SCSI_SENSE_KEY_ILLEGAL_REQUEST, USBD_SCSI_ASC_INVALID_CDB, 0);

        usbDevComposite->mscInfo.usbDevBOT.status = USBD_BOT_ERR;
        USBD_MSC_BOT_Abort(usbInfo);
    }
    else
    {
        reqStatus = USBD_SCSI_Handle(usbInfo, usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.bLUN, \
                                     &usbDevComposite->mscInfo.usbDevBOT.cmdPack.CBW.DATA_FIELD.CB[0]);
        if (reqStatus == USBD_FAIL)
        {
            if (usbDevComposite->mscInfo.usbDevBOT.state == USBD_BOT_NO_DATA)
            {
                USBD_MSC_BOT_SendCSW(usbInfo, USBD_BOT_CSW_FAIL);
            }
            else
            {
                USBD_MSC_BOT_Abort(usbInfo);
            }
        }
        else if ((usbDevComposite->mscInfo.usbDevBOT.state != USBD_BOT_DATAIN) && \
                 (usbDevComposite->mscInfo.usbDevBOT.state != USBD_BOT_DATAOUT) && \
                 (usbDevComposite->mscInfo.usbDevBOT.state != USBD_BOT_DATAIN_LAST))
        {
            if (usbDevComposite->mscInfo.usbDevBOT.dataLen == 0)
            {
                USBD_MSC_BOT_SendCSW(usbInfo, USBD_BOT_CSW_OK);
            }
            else if (usbDevComposite->mscInfo.usbDevBOT.dataLen > 0)
            {
                USBD_MSC_BOT_SendData(usbInfo, usbDevComposite->mscInfo.usbDevBOT.data, usbDevComposite->mscInfo.usbDevBOT.dataLen);
            }
            else
            {
                USBD_MSC_BOT_Abort(usbInfo);
            }
        }
        else
        {
            return USBD_OK;
        }
    }

    return usbStatus;
}

/*!
 * @brief     Clear feature BOT of USB device MSC
 *
 * @param     usbInfo : usb handler information
 *
 * @param     epNum : endpoint number
 *
 * @retval    USB device operation status
 */
USBD_STA_T USBD_MSC_BOT_ClearFeature(USBD_INFO_T* usbInfo, uint8_t epNum)
{
    USBD_STA_T  usbStatus = USBD_OK;
    USBD_COMPOSITE_INFO_T* usbDevComposite = (USBD_COMPOSITE_INFO_T*)usbInfo->devClassData[usbInfo->classID];

    if (usbDevComposite == NULL)
    {
        return USBD_FAIL;
    }

    switch (usbDevComposite->mscInfo.usbDevBOT.status)
    {
        /* CBW signature error */
        case USBD_BOT_ERR:
            USBD_EP_StallCallback(usbInfo, usbDevComposite->mscInfo.epInAddr);
            USBD_EP_StallCallback(usbInfo, usbDevComposite->mscInfo.epOutAddr);
            break;

        case USBD_BOT_NORMAL:
            if ((epNum & 0x80) == 0x80)
            {
                usbDevComposite->mscInfo.usbDevBOT.cmdPack.cswStatus = USBD_BOT_CSW_FAIL;
                USBD_MSC_BOT_SendCSW(usbInfo, usbDevComposite->mscInfo.usbDevBOT.cmdPack.cswStatus);
            }
            break;

        default:
            break;
    }

    return usbStatus;
}

