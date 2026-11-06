/*!
 * @file        usbd_memory.h
 *
 * @brief       usb device memory management header file
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
#ifndef _USBD_MEMORY_H_
#define _USBD_MEMORY_H_

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "usbd_msc.h"

/* Exported typedef *******************************************************/

/* Exported macro *********************************************************/

/* Exported function prototypes *******************************************/

extern USBD_MSC_MEMORY_T USBD_MEMORY_INTERFACE_FS;

uint8_t USBD_FS_MemoryReadMaxLun(void);
USBD_STA_T USBD_FS_MemoryCheckWPR(uint8_t lun);
USBD_STA_T USBD_FS_MemoryCheckReady(uint8_t lun);
USBD_STA_T USBD_FS_MemoryInit(uint8_t lun);
USBD_STA_T USBD_FS_MemoryReadCapacity(uint8_t lun, uint32_t* blockNum, \
                                      uint16_t* blockSize);
USBD_STA_T USBD_FS_MemoryReadData(uint8_t lun, uint8_t* buffer, uint32_t blockAddr, \
                                  uint16_t blockLength);
USBD_STA_T USBD_FS_MemoryWriteData(uint8_t lun, uint8_t* buffer, uint32_t blockAddr, \
                                   uint16_t blockLength);

#ifdef __cplusplus
}
#endif

#endif
