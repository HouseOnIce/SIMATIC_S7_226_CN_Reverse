/*!
 * @file        apm3210e_eval_fsmc_sram.h
 *
 * @brief       This file contains all the functions prototypes for the apm3210e_eval_fsmc_sram firmware driver
 *
 * @version     V1.0.3
 *
 * @date        2025-06-17
 *
 * @attention
 *
 *  Copyright (C) 2022-2025 Geehy Semiconductor
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
#ifndef __FSMC_SRAM_H
#define __FSMC_SRAM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "apm32e10x.h"


/** @addtogroup Examples
  @{
  */

/** @addtogroup DMC_SDRAM
  @{
  */

/** @defgroup SMC_SRAM_Functions SRAM_Functions
  @{
  */


void SRAM_Init(void);
void SRAM_Write(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
void SRAM_Read(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);

/**@} end of group SMC_SRAM_Functions */
/**@} end of group DMC_SDRAM */
/**@} end of group Examples */

#ifdef __cplusplus
}
#endif

#endif
