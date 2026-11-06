/*!
 * @file        bsp_usart.h
 *
 * @brief       Header for bsp_usart.c module
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
#ifndef __BSP_USART_H
#define __BSP_USART_H

/* Includes */
#include "Board.h"
#include "apm32e10x.h"
#include "apm32e10x_gpio.h"
#include "apm32e10x_usart.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup IAP_BootLoader
  @{
  */

/** @addtogroup BSP_USART
  @{
  */

/** @defgroup BSP_USART_Macros Macros
  @{
  */

#define DEBUG_USART                         USART2
#define DEBUG_USART_CLK                     RCM_APB1_PERIPH_USART2
#define DEBUG_USART_TX_CLK                  RCM_APB2_PERIPH_GPIOA
#define DEBUG_USART_TX_PORT                 GPIOA
#define DEUBG_USART_TX_SOURCE               GPIO_PIN_SOURCE_2
#define DEBUG_USART_TX_PIN                  GPIO_PIN_2
#define DEBUG_USART_RX_CLK                  RCM_APB2_PERIPH_GPIOA
#define DEBUG_USART_RX_PORT                 GPIOA
#define DEUBG_USART_RX_SOURCE               GPIO_PIN_SOURCE_3
#define DEBUG_USART_RX_PIN                  GPIO_PIN_3
#define DEBUG_USART_BAUDRATE                115200

/**@} end of group BSP_USART_Macros */

/** @defgroup BSP_USART_Functions Functions
  @{
  */

/* function declaration */
void USART_Init(uint32_t baudRate);

/**@} end of group BSP_USART_Functions */
/**@} end of group BSP_USART */
/**@} end of group IAP_BootLoader */
/**@} end of group Examples */

#endif
