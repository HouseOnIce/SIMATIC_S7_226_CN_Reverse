/*!
 * @file        bsp_w25q16.h
 *
 * @brief       Header for bsp_w25q16.c module
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
#ifndef __BOARD_W25Q16_H
#define __BOARD_W25Q16_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "main.h"

/* Exported typedef *******************************************************/

/* Exported macro *********************************************************/

#define W25Q16_FLASH_ID                   0xEF4015
#define W25Q16_FLASH_PAGE_SIZE            256

#define W25Q16_FLASH_WRITE_ADDR           0x00000
#define W25Q16_FLASH_READ_ADDR            W25Q16_FLASH_WRITE_ADDR

/* W25Q16 CMD */
#define W25Q16_WRITE_ENABLE               0x06
#define W25Q16_WRITE_DISABLE              0x04
#define W25Q16_READ_STATUS_REG            0x05
#define W25Q16_WRITE_STATUS_REG           0x01
#define W25Q16_READ_DATA                  0x03
#define W25Q16_FAST_READ_DATA             0x0B
#define W25Q16_FAST_RAED_DUAL             0x3B
#define W25Q16_PAGE_PROGRAM               0x02
#define W25Q16_BLOCK_ERASE                0xD8
#define W25Q16_SECTOR_ERASE               0x20
#define W25Q16_CHIP_ERASE                 0xC7
#define W25Q16_POWER_DOWN                 0xB9
#define W25Q16_RELEASE_POWER_DOWN         0xAB
#define W25Q16_DEVICE_ID                  0xAB
#define W25Q16_MANUFACT_DEVICE_ID         0x90
#define W25Q16_JEDEC_DEVICE_ID            0x9F
#define W25Q16_WIP_FLAG                   0x01
#define W25Q16_DUMMY_BYTE                 0xFF

/* FLASH CS */
#define FLASH_SPI_BUS                     SPI3
#define FLASH_SPI_BUS_CLK                 RCM_APB1_PERIPH_SPI3

#define FLASH_SPI_MOSI_PIN                GPIO_PIN_5
#define FLASH_SPI_MISO_PIN                GPIO_PIN_4
#define FLASH_SPI_SCK_PIN                 GPIO_PIN_3
#define FLASH_SPI_GPIO_CLK                RCM_APB2_PERIPH_GPIOB
#define FLASH_SPI_GPIO_PORT               GPIOB

#define FLASH_SPI_CS_PIN                  GPIO_PIN_15
#define FLASH_SPI_CS_GPIO_CLK             RCM_APB2_PERIPH_GPIOA
#define FLASH_SPI_CS_GPIO_PORT            GPIOA
                                        
//#define FLASH_SPI_MOSI_SOURCE             GPIO_PIN_SOURCE_15
//#define FLASH_SPI_MISO_SOURCE             GPIO_PIN_SOURCE_14
//#define FLASH_SPI_SCK_SOURCE              GPIO_PIN_SOURCE_13
//#define FLASH_SPI_CS_SOURCE               GPIO_PIN_SOURCE_12
//#define FLASH_SPI_GPIO_AF                 GPIO_AF_PIN1
                                        
#define FLASH_SPI_MOSI_CLR()              GPIO_ResetBit(FLASH_SPI_GPIO_PORT, FLASH_SPI_MOSI_PIN)
#define FLASH_SPI_MOSI_SET()              GPIO_SetBit(FLASH_SPI_GPIO_PORT, FLASH_SPI_MOSI_PIN)
                                        
#define FLASH_SPI_MISO_CLR()              GPIO_ResetBit(FLASH_SPI_GPIO_PORT, FLASH_SPI_MISO_PIN)
#define FLASH_SPI_MISO_SET()              GPIO_SetBit(FLASH_SPI_GPIO_PORT, FLASH_SPI_MISO_PIN)
                                        
#define FLASH_SPI_SCK_CLR()               GPIO_ResetBit(FLASH_SPI_GPIO_PORT, FLASH_SPI_SCK_PIN)
#define FLASH_SPI_SCK_SET()               GPIO_SetBit(FLASH_SPI_GPIO_PORT, FLASH_SPI_SCK_PIN)
                                        
#define FLASH_SPI_CS_CLR()                GPIO_ResetBit(FLASH_SPI_CS_GPIO_PORT, FLASH_SPI_CS_PIN)
#define FLASH_SPI_CS_SET()                GPIO_SetBit(FLASH_SPI_CS_GPIO_PORT, FLASH_SPI_CS_PIN)

/**
 * @brief  W25Q16 Flash information structure
 */
typedef struct {
    uint32_t deviceID;
    uint32_t flashID;
} W25Q16_INFO_T;

/* Exported function prototypes *******************************************/

void W25Q16_IntoPowerDown(void);
uint8_t W25Q16_SendByte(uint8_t byte);
uint8_t W25Q16_ReadByte(void);
void W25Q16_SPI_Init(void);
void W25Q16_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumToWrite);
void W25Q16_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumToWrite);
void W25Q16_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumToRead);
uint32_t W25Q16_ReadFlashID(void);
uint32_t W25Q16_ReadFlashDeviceID(void);
void W25Q16_EnableFlashWrite(void);
void W25Q16_WaitFlashWriteEnd(void);
void W25Q16_EraseSector(uint32_t SectorAddr);

#ifdef __cplusplus
}
#endif

#endif

