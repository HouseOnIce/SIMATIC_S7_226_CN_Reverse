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
#include "stdio.h"
#include "Board.h"
#include "apm32e10x_usart.h"

/** @addtogroup Examples
  @{
*/

/** @addtogroup USART_IrDA
  @{
*/

/** @defgroup USART_IrDA_Variables Variables
  @{
*/

extern volatile uint32_t tick;

/**@} end of group USART_IrDA_Variables */

/** @defgroup USART_IrDA_Functions Functions
  @{
*/

void Delay(void);

/**@} end of group USART_IrDA_INT_Functions */
/**@} end of group USART_IrDA */
/**@} end of group Examples */

#endif /* __MAIN_H */

