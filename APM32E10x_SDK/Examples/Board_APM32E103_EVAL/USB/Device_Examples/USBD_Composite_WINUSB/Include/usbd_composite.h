/*!
 * @file        usbd_composite.h
 *
 * @brief       usb device composite class handler header file
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

/* Define to prevent recursive inclusion */
#ifndef _USBD_COMPOSITE_H_
#define _USBD_COMPOSITE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ***************************************************************/
#include "usbd_core.h"

/* Exported typedef *******************************************************/

/* Exported macro *********************************************************/

#define USBD_WINUSB_OS_FEATURE_DESC_SIZE        0x40
#define USBD_WINUSB_OS_PROPERTY_DESC_SIZE       0x8E

#ifndef WINUSB_HS_MAX_PACKET_SIZE
#define WINUSB_HS_MAX_PACKET_SIZE               512U  /* Endpoint IN & OUT Packet size */
#endif /* WINUSB_HS_MAX_PACKET_SIZE */

#ifndef WINUSB_FS_MAX_PACKET_SIZE
#define WINUSB_FS_MAX_PACKET_SIZE               64U  /* Endpoint IN & OUT Packet size */
#endif /* WINUSB_FS_MAX_PACKET_SIZE */

#ifndef USBD_WINUSB1_INTERFACE_NUM
#define USBD_WINUSB1_INTERFACE_NUM                0
#endif /* USBD_WINUSB1_INTERFACE_NUM */

#ifndef USBD_WINUSB1_INTERFACE_CNT
#define USBD_WINUSB1_INTERFACE_CNT                1
#endif /* USBD_WINUSB1_INTERFACE_CNT */

#ifndef USBD_WINUSB1_IN_EP_ADDR
#define USBD_WINUSB1_IN_EP_ADDR                  0x81U
#endif /* USBD_WINUSB1_IN_EP_ADDR */

#ifndef USBD_WINUSB1_OUT_EP_ADDR
#define USBD_WINUSB1_OUT_EP_ADDR                 0x01U
#endif /* USBD_WINUSB1_OUT_EP_ADDR */

#ifndef USBD_WINUSB2_INTERFACE_NUM
#define USBD_WINUSB2_INTERFACE_NUM                1
#endif /* USBD_WINUSB1_INTERFACE_NUM */

#ifndef USBD_WINUSB2_INTERFACE_CNT
#define USBD_WINUSB2_INTERFACE_CNT                1
#endif /* USBD_WINUSB1_INTERFACE_CNT */

#ifndef USBD_WINUSB2_IN_EP_ADDR
#define USBD_WINUSB2_IN_EP_ADDR                  0x82U
#endif /* USBD_WINUSB1_IN_EP_ADDR */

#ifndef USBD_WINUSB2_OUT_EP_ADDR
#define USBD_WINUSB2_OUT_EP_ADDR                 0x02U
#endif /* USBD_WINUSB1_OUT_EP_ADDR */

#ifndef USBD_WINUSB_HS_INTERVAL
#define USBD_WINUSB_HS_INTERVAL                 0x10U
#endif /* USBD_WINUSB_HS_INTERVAL */

#ifndef USBD_WINUSB_FS_INTERVAL
#define USBD_WINUSB_FS_INTERVAL                 0x10U
#endif /* USBD_WINUSB_FS_INTERVAL */

/**
 * @brief   USB device WINUSB xfer status
 */
typedef enum
{
    USBD_WINUSB_XFER_IDLE,
    USBD_WINUSB_XFER_BUSY,
} USBD_WINUSB_XFER_STA_T;

/**
 * @brief   USB device WINUSB data handler
 */
typedef struct
{
    __IO uint8_t state;
    uint8_t *buffer;
    uint32_t length;
} USBD_WINUSB_DATA_XFER_T;

/**
 * @brief   USB device WINUSB interface handler
 */
typedef struct
{
    const char*  itfName;
    USBD_STA_T (*ItfInit)(uint8_t winusbnum);
    USBD_STA_T (*ItfDeInit)(void);
    USBD_STA_T (*ItfCtrl)(uint8_t command, uint8_t *buffer, uint16_t length, uint8_t winusbnum);
    USBD_STA_T (*ItfSend)(uint8_t *buffer, uint16_t length, uint8_t winusbnum);
    USBD_STA_T (*ItfSendEnd)(uint8_t epNum, uint8_t *buffer, uint32_t *length, uint8_t winusbnum);
    USBD_STA_T (*ItfReceive)(uint8_t *buffer, uint32_t *length, uint8_t winusbnum);
} USBD_WINUSB_INTERFACE_T;

/**
 * @brief    WINUSB information management
 */
typedef struct
{
    uint8_t                     itf;
    uint8_t                     epInAddr;
    uint8_t                     epOutAddr;
    USBD_WINUSB_DATA_XFER_T     winusbTx;
    USBD_WINUSB_DATA_XFER_T     winusbRx;
    uint32_t                    data[WINUSB_HS_MAX_PACKET_SIZE / 4];
    void                        *pUserData;
} USBD_WINUSB_INFO_T;

/**
 * @brief    Composite information management
 */
typedef struct
{
    USBD_WINUSB_INFO_T      winusbInfo[USBD_WINUSB_NUM];
    uint8_t ephash[8];
    uint8_t interfacehash[128];
} USBD_COMPOSITE_INFO_T;

extern USBD_CLASS_T USBD_COMPOSITE_CLASS;

/* Exported function prototypes *******************************************/

USBD_STA_T USBD_Composite_Init(USBD_INFO_T* usbInfo, void* itf1);
USBD_STA_T USBD_Composite_Deinit(USBD_INFO_T* usbInfo);

uint8_t USBD_Composite_RegisterWINUSBItf(USBD_INFO_T* usbInfo, USBD_WINUSB_INTERFACE_T* itf);
USBD_STA_T USBD_Composite_WINUSB_ConfigTxBuffer(USBD_INFO_T* usbInfo, uint8_t *buffer, uint32_t length, uint8_t winusbnum);
USBD_STA_T USBD_Composite_WINUSB_ConfigRxBuffer(USBD_INFO_T* usbInfo, uint8_t *buffer, uint8_t winusbnum);
USBD_STA_T USBD_Composite_WINUSB_TxPacket(USBD_INFO_T* usbInfo, uint8_t winusbnum);
USBD_STA_T USBD_Composite_WINUSB_RxPacket(USBD_INFO_T* usbInfo, uint8_t winusbnum);
uint8_t USBD_Composite_WINUSB_ReadInterval(USBD_INFO_T* usbInfo);

#ifdef __cplusplus
}
#endif

#endif  /* _USBD_COMPOSITE_H_ */
