/*!
 * @file        main.h
 *
 * @brief       Main program body
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes */
#include "Board.h"
#include <string.h>
#include "apm32e10x_fmc.h"
#include "apm32e10x_spi.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup I2S_Interrupt
  @{
  */

/** @defgroup I2S_Interrupt_Variables Variables
  @{
*/

extern const uint16_t I2S3_Buffer_Tx[32];
extern uint16_t I2S2_Buffer_Rx[32];
extern volatile uint32_t txCnt;
extern volatile uint32_t rxCnt;

/**@} end of group I2S_Interrupt_Variables */
/**@} end of group I2S_Interrupt */
/**@} end of group Examples */

#endif
