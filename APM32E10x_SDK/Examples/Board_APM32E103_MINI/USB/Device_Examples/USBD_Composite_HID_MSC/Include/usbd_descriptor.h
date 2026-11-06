/*!
 * @file        usbd_descriptor.h
 *
 * @brief       usb device descriptor
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
#ifndef _USBD_DESCRIPTOR_H_
#define _USBD_DESCRIPTOR_H_

/* Includes */
#include "usbd_core.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup OTGD_Composite_HID_MSC
  @{
  */

/** @defgroup OTGD_Composite_HID_MSC_Macros Macros
  @{
*/

#define USBD_DEVICE_DESCRIPTOR_SIZE             18
#define USBD_CONFIG_DESCRIPTOR_SIZE             80
#define USBD_SERIAL_STRING_SIZE                 26
#define USBD_LANGID_STRING_SIZE                 4
#define USBD_DEVICE_QUALIFIER_DESCRIPTOR_SIZE   10
#define USBD_BOS_DESCRIPTOR_SIZE                12

#define USBD_DEVICE_CAPABILITY_TYPE             0x10
#define USBD_20_EXTENSION_TYPE                  0x02

#define USBD_MSC_ITF_CLASS_ID                   0x08
#define USBD_MSC_ITF_PROTOCOL                   0x50 /* BOT */
#define USBD_MSC_ITF_SUB_CLASS                  0x06 /* SCSI */

#define USBD_HID_ITF_CLASS_ID                   0x03
#define USBD_HID_SUB_CLASS_BOOT                 0x01
#define USBD_HID_SUB_CLASS_NBOOT                0x00
#define USBD_HID_ITF_PORTOCOL_NONE              0x00
#define USBD_HID_ITF_PORTOCOL_KEYBOARD          0x01
#define USBD_HID_ITF_PORTOCOL_MOUSE             0x02

/**@} end of group OTGD_Composite_HID_MSC_Macros*/

/** @defgroup OTGD_Composite_HID_MSC_Variables Variables
  @{
  */

extern USBD_DESC_T USBD_DESC_FS;

/**@} end of group OTGD_Composite_HID_MSC_Variables*/

/** @defgroup OTGD_Composite_HID_MSC_Functions Functions
  @{
  */

/**@} end of group OTGD_Composite_HID_MSC_Functions */
/**@} end of group OTGD_Composite_HID_MSC */
/**@} end of group Examples */

#endif
