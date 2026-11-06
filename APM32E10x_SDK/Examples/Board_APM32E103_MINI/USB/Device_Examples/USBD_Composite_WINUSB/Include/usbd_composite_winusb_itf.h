/*!
 * @file        usbd_composite_winusb_itf.h
 *
 * @brief       usb device WINUSB class interface handler header file
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
#ifndef _USBD_COMPOSITE_WINUSB_ITF_H_
#define _USBD_COMPOSITE_WINUSB_ITF_H_

/* Includes */
#include "usbd_composite.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup USBD_Composite_WINUSB
  @{
  */

/** @defgroup USBD_Composite_WINUSB_Macros Macros
  @{
*/

#define USBD_WINUSB_TX_BUF_LEN         1024
#define USBD_WINUSB_RX_BUF_LEN         1024

/**@} end of group USBD_Composite_WINUSB_Macros*/

/** @defgroup USBD_Composite_WINUSB_Enumerates Enumerates
  @{
  */

/**
 * @brief   USB device WINUSB ITF status
 */
typedef enum
{
    USBD_WINUSB_ITF_IDLE,
    USBD_WINUSB_ITF_REV_UPDATE,
} USBD_WINUSB_ITF_STA_T;

/**@} end of group USBD_Composite_WINUSB_Enumerates*/

/** @defgroup USBD_Composite_WINUSB_Structures Structures
  @{
  */

/**
 * @brief   USB device WINUSB ITF information
 */
typedef struct
{
    uint32_t    rxUpdateLen;
    uint8_t     state;
} USBD_WINUSB_ITF_INFO_T;

/**@} end of group USBD_Composite_WINUSB_Structures*/

/** @defgroup USBD_Composite_WINUSB_Variables Variables
  @{
  */

extern USBD_WINUSB_INTERFACE_T USBD_WINUSB_INTERFACE_FS;
extern USBD_WINUSB_ITF_INFO_T gWinusbItf[USBD_WINUSB_NUM];
extern uint8_t winusbTxBuffer[USBD_WINUSB_TX_BUF_LEN];
extern uint8_t winusbRxBuffer[USBD_WINUSB_NUM][USBD_WINUSB_RX_BUF_LEN];

/**@} end of group USBD_Composite_WINUSB_Variables*/

/** @defgroup USBD_Composite_WINUSB_Functions Functions
  @{
  */

USBD_STA_T USBD_FS_WINUSB_ItfSend(uint8_t *buffer, uint16_t length, uint8_t winusbnum);

/**@} end of group USBD_Composite_WINUSB_Functions */
/**@} end of group USBD_Composite_WINUSB */
/**@} end of group Examples */

#endif
