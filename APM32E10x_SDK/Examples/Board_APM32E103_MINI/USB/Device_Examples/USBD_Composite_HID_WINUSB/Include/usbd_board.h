/*!
 * @file        usbd_board.h
 *
 * @brief       Header file for USB Board
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
#ifndef _USBD_BOARD_H_
#define _USBD_BOARD_H_

/* Includes */
#include "apm32e10x.h"
#include "apm32e10x_usb.h"
#include "apm32e10x_usb_device.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup USBD_Composite_HID_WINUSB
  @{
  */

/** @defgroup USBD_Composite_HID_WINUSB_Macros Macros
  @{
*/

#define USB1                                0
#define USB2                                1

/* Select USB peripheral
*   USB1:   Share FIFO with CAN1
*   USB2:   Private FIFO.Not share whith CAN1
*/
#define USB_SELECT                          USB2

#define USBD_SUP_CLASS_MAX_NUM              1
#define USBD_SUP_INTERFACE_MAX_NUM          1
#define USBD_SUP_CONFIGURATION_MAX_NUM      1
#define USBD_SUP_STR_DESC_MAX_NUM           512

/* Only support LPM USB device */
#define USBD_SUP_LPM                        0
#define USBD_SUP_SELF_PWR                   1

#define USBD_EP0_OUT_ADDR                   0x00
#define USBD_EP0_OUT_PMA_SIZE               0x28

#define USBD_EP0_IN_ADDR                    0x80
#define USBD_EP0_IN_PMA_SIZE                0x68

#define USBD_CUSTOM_HID_IN_EP_ADDR          0x81U
#define USBD_CUSTOM_HID_IN_EP_PMA_SIZE      0xC0
#define USBD_CUSTOM_HID_OUT_EP_ADDR         0x01U
#define USBD_CUSTOM_HID_OUT_EP_PMA_SIZE     0x100

#define USBD_WINUSB_IN_EP_ADDR                0x82U
#define USBD_WINUSB_IN_EP_PMA_SIZE            0x140
#define USBD_WINUSB_OUT_EP_ADDR               0x02U
#define USBD_WINUSB_OUT_EP_PMA_SIZE           0x180

#define USBD_CUSTOM_HID_INTERFACE_NUM           0
#define USBD_CUSTOM_HID_INTERFACE_CNT           1

#define USBD_CUSTOM_HID_FS_MP_SIZE              0x40
#define USBD_CUSTOM_HID_HS_MP_SIZE              0x200

#define USBD_CUSTOM_HID_IN_EP_SIZE              0x04
#define USBD_CUSTOM_HID_OUT_EP_SIZE             0x04

#define USBD_CUSTOM_HID_HS_INTERVAL             7
#define USBD_CUSTOM_HID_FS_INTERVAL             10

#define USBD_WINUSB_INTERFACE_NUM               1
#define USBD_WINUSB_INTERFACE_CNT               1
#define USBD_WINUSB_HS_INTERVAL                 0x10U
#define USBD_WINUSB_FS_INTERVAL                 0x10U
#define WINUSB_HS_MAX_PACKET_SIZE               512U
#define WINUSB_FS_MAX_PACKET_SIZE               64U

#define USBD_DEBUG_LEVEL                    1U

#if (USBD_DEBUG_LEVEL > 0U)
#define USBD_USR_LOG(...)   do { \
                            printf(__VA_ARGS__); \
                            printf("\r\n"); \
} while(0)
#else
#define USBD_USR_LOG(...) do {} while (0)
#endif

#if (USBD_DEBUG_LEVEL > 1U)
#define USBD_USR_Debug(...)   do { \
                            printf("Debug:"); \
                            printf(__VA_ARGS__); \
                            printf("\r\n"); \
} while(0)
#else
#define USBD_USR_Debug(...) do {} while (0)
#endif

/**@} end of group USBD_Composite_HID_WINUSB_Macros*/

/** @defgroup USBD_Composite_HID_WINUSB_Functions Functions
  @{
  */

/**@} end of group USBD_Composite_HID_WINUSB_Functions */
/**@} end of group USBD_Composite_HID_WINUSB */
/**@} end of group Examples */

#endif
