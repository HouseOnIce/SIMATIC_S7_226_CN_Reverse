/*!
 * @file        usbd_composite_custom_hid_if.h
 *
 * @brief       usb device Custom HID interface handler header file
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
#ifndef _USBD_COMPOSITE_CUSTOM_HID_IF_H_
#define _USBD_COMPOSITE_CUSTOM_HID_IF_H_

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "usbd_composite.h"

/* Exported typedef *******************************************************/

/* Exported macro *********************************************************/

/* Exported function prototypes *******************************************/

extern USBD_CUSTOM_HID_INTERFACE_T USBD_CUSTOM_HID_INTERFACE_FS;

#ifdef __cplusplus
}
#endif

#endif
