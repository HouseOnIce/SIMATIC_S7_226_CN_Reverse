/*!
 * @file        usbd_composite_cdc_vcp.h
 *
 * @brief       usb device CDC class VCP handler header file
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
#ifndef _USBD_COMPOSITE_CDC_VCP_H_
#define _USBD_COMPOSITE_CDC_VCP_H_

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "usbd_composite.h"

/* Exported typedef *******************************************************/

/* Exported macro *********************************************************/

#define USBD_CDC_TX_BUF_LEN         1024
#define USBD_CDC_RX_BUF_LEN         1024

/**
 * @brief   USB device CDC VCP status
 */
typedef enum
{
    USBD_CDC_VCP_IDLE,
    USBD_CDC_VCP_REV_UPDATE,
} USBD_CDC_VCP_STA_T;

/**
 * @brief   USB device CDC VCP information
 */
typedef struct
{
    uint32_t    rxUpdateLen;
    uint8_t     state;
} USBD_CDC_VCP_INFO_T;

/* Exported function prototypes *******************************************/

extern USBD_CDC_INTERFACE_T USBD_CDC_INTERFACE_FS;
extern USBD_CDC_VCP_INFO_T gCDCVCP;
extern uint8_t cdcTxBuffer[USBD_CDC_TX_BUF_LEN];
extern uint8_t cdcRxBuffer[USBD_CDC_RX_BUF_LEN];

USBD_STA_T USBD_FS_CDC_ItfSend(uint8_t* buffer, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif /* _USBD_COMPOSITE_CDC_VCP_H_ */
