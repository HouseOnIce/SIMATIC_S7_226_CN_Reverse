/*!
 * @file        dmc_sdram.h
 *
 * @brief       This file contains all the functions prototypes for the APM32E103xCS SDRAM firmware library
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
#ifndef __DMC_SDRAM_H
#define __DMC_SDRAM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ***************************************************************/
#include "apm32e10x.h"
#include <string.h>
#include <stdio.h>

/* Exported typedef *******************************************************/

/* Exported macro *********************************************************/

#define  RCM_SDRAM_GPIO_PERIPH (RCM_APB2_PERIPH_AFIO  | \
                                RCM_APB2_PERIPH_GPIOB | \
                                RCM_APB2_PERIPH_GPIOC | \
                                RCM_APB2_PERIPH_GPIOD | \
                                RCM_APB2_PERIPH_GPIOE | \
                                RCM_APB2_PERIPH_GPIOF | \
                                RCM_APB2_PERIPH_GPIOG)

#define  RCM_SDRAM_PERIPH (RCM_AHB_PERIPH_SMC)

#define SDRAM_START_ADDR ((uint32_t)0x60000000)
#define SDRAM_END_ADDR   ((uint32_t)0x60200000)

/* Exported function prototypes *******************************************/

void SDRAM_Init(void);

uint32_t SDRAM_Word_Test(void);
uint32_t SDRAM_HalfWord_Test(void);
uint32_t SDRAM_Byte_Test(void);

void SDRAM_WriteWord(uint32_t address, uint32_t data);
void SDRAM_WriteHalfWord(uint32_t address, uint16_t data);
void SDRAM_WriteByte(uint32_t address, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif /* __DMC_SDRAM_H */
