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

#define USBD_CLASS_SET_IDLE                     0x0A
#define USBD_CLASS_GET_IDLE                     0x02

#define USBD_CLASS_SET_REPORT                   0x09
#define USBD_CLASS_GET_REPORT                   0x01

#define USBD_CLASS_SET_PROTOCOL                 0x0B
#define USBD_CLASS_GET_PROTOCOL                 0x03

/**
 * @brief   USB device CDC xfer status
 */
typedef enum
{
    USBD_CDC_XFER_IDLE,
    USBD_CDC_XFER_BUSY,
} USBD_CDC_XFER_STA_T;

/**
 * @brief   USB device CDC control status
 */
typedef enum
{
    USBD_CDC_SEND_ENCAPSULATED_COMMAND          = 0x00,
    USBD_CDC_GET_ENCAPSULATED_RESPONSE          = 0x01,
    USBD_CDC_SET_COMM_FEATURE                   = 0x02,
    USBD_CDC_GET_COMM_FEATURE                   = 0x03,
    USBD_CDC_CLEAR_COMM_FEATURE                 = 0x04,
    USBD_CDC_SET_LINE_CODING                    = 0x20,
    USBD_CDC_GET_LINE_CODING                    = 0x21,
    USBD_CDC_SET_CONTROL_LINE_STATE             = 0x22,
    USBD_CDC_SEND_BREAK                         = 0x23,
} USBD_CDC_CTRL_STA_T;

/**
 * @brief   USB device CDC line coding handler
 */
typedef struct
{
    uint32_t bitrate;
    uint8_t  format;
    uint8_t  paritytype;
    uint8_t  datatype;
} USBD_CDC_LineCodingTypeDef;

/**
 * @brief   USB device CDC data handler
 */
typedef struct
{
    __IO uint8_t state;
    uint8_t *buffer;
    uint32_t length;
} USBD_CDC_DATA_XFER_T;

/**
 * @brief   USB device CDC command handler
 */
typedef struct
{
    uint8_t opcode;
    uint8_t length;
} USBD_CDC_CMD_XFER_T;

/**
 * @brief   USB device CDC interface handler
 */
typedef struct
{
    const char*  itfName;
    USBD_STA_T (*ItfInit)(uint8_t cdcnum);
    USBD_STA_T (*ItfDeInit)(void);
    USBD_STA_T (*ItfCtrl)(uint8_t command, uint8_t *buffer, uint16_t length, uint8_t cdcnum);
    USBD_STA_T (*ItfSend)(uint8_t *buffer, uint16_t length, uint8_t cdcnum);
    USBD_STA_T (*ItfSendEnd)(uint8_t epNum, uint8_t *buffer, uint32_t *length);
    USBD_STA_T (*ItfReceive)(uint8_t *buffer, uint32_t *length, uint8_t cdcnum);
} USBD_CDC_INTERFACE_T;

/**
 * @brief    CDC information management
 */
typedef struct
{
    uint8_t                 itf;
    uint8_t                 epInAddr;
    uint8_t                 epOutAddr;
    uint8_t                 epCmdAddr;
    USBD_CDC_DATA_XFER_T    cdcTx;
    USBD_CDC_DATA_XFER_T    cdcRx;
    uint32_t                data[USBD_CDC_HS_MP_SIZE / 4];
    USBD_CDC_CMD_XFER_T     cdcCmd;
    void                    *pUserData;
} USBD_CDC_INFO_T;

/**
 * @brief    Composite information management
 */
typedef struct
{
    USBD_CDC_INFO_T cdcInfo[USBD_CDC_NUM];
    uint8_t ephash[8];
    uint8_t interfacehash[128];
} USBD_COMPOSITE_INFO_T;

extern USBD_CLASS_T USBD_COMPOSITE_CLASS;

/* Exported function prototypes *******************************************/

USBD_STA_T USBD_Composite_Init(USBD_INFO_T* usbInfo, void* itf1);
USBD_STA_T USBD_Composite_Deinit(USBD_INFO_T* usbInfo);

uint8_t USBD_Composite_RegisterCDCItf(USBD_INFO_T* usbInfo, USBD_CDC_INTERFACE_T* itf);
USBD_STA_T USBD_Composite_CDC_ConfigTxBuffer(USBD_INFO_T* usbInfo, uint8_t *buffer, uint32_t length, uint8_t cdcnum);
USBD_STA_T USBD_Composite_CDC_ConfigRxBuffer(USBD_INFO_T* usbInfo, uint8_t *buffer, uint8_t cdcnum);
USBD_STA_T USBD_Composite_CDC_TxPacket(USBD_INFO_T* usbInfo, uint8_t cdcnum);
USBD_STA_T USBD_Composite_CDC_RxPacket(USBD_INFO_T* usbInfo, uint8_t cdcnum);
uint8_t USBD_Composite_CDC_ReadInterval(USBD_INFO_T* usbInfo);

#ifdef __cplusplus
}
#endif

#endif  /* _USBD_COMPOSITE_H_ */
