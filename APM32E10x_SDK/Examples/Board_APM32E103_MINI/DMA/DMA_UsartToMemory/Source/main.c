/*!
 * @file        main.c
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

/* Includes */
#include "main.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup DMA_UsartToMemory
  @{
  */

/** @defgroup DMA_UsartToMemory_Macros Macros
  @{
  */

/* USART1 data register address */
#define USART1_DR_ADDRESS        (USART1_BASE+0x04)

/* USART2 data register address */
#define USART2_DR_ADDRESS        (USART2_BASE+0x04)

/* Set Buff Size */
#define BufSize                  0xFF

/**@} end of group DMA_UsartToMemory_Macros */

/** @defgroup DMA_UsartToMemory_Enumerations Enumerations
  @{
  */

typedef enum {FALSE, TRUE} BOOL;

/**@} end of group DMA_UsartToMemory_Enumerations */



/** @defgroup DMA_UsartToMemory_Variables Variables
  @{
  */

/* USART2 TX Buf */
uint8_t DMA_USART2_TxBuf[BufSize];
/* USART1 RX Buf */
uint8_t DMA_USART1_RxBuf[BufSize];

uint32_t count = 0xfffff;
/**@} end of group DMA_UsartToMemory_Variables */

/** @defgroup DMA_UsartToMemory_Functions Functions
  @{
  */

void USART1_RX_DMA_Init(void);
void USART2_TX_DMA_Init(void);
BOOL BufferCompare(uint8_t* buf1, uint8_t* buf2, uint8_t size);

/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 */
int main(void)
{
    SystemClockConfig();

    USART_Config_T USART_ConfigStruct;

    /* USART config */
    USART_ConfigStruct.baudRate = 115200;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode = USART_MODE_TX_RX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;

    APM_MINI_COMInit(COM1, &USART_ConfigStruct);
    APM_MINI_COMInit(COM2, &USART_ConfigStruct);

    APM_MINI_LEDInit(LED2);
    APM_MINI_LEDInit(LED3);
    APM_MINI_LEDOff(LED2);
    APM_MINI_LEDOff(LED3);

    /* USART1 RX and USART2 TX DMA Init */
    USART1_RX_DMA_Init();
    USART2_TX_DMA_Init();

    /* Init TxBuf */
    for (int i = 0; i < BufSize; i++)
    {
        DMA_USART2_TxBuf[i] = i;
    }

    /* Enable USART1 RX and USART2 TX DMA */
    USART_EnableDMA(USART1, USART_DMA_RX);
    USART_EnableDMA(USART2, USART_DMA_TX);

    /* Wait until USART2 TX DMA1 Channel Transfer Complete */
    while (DMA_ReadStatusFlag(DMA1_FLAG_TC7) == RESET);

    /* Wait until USART1 RX DMA1 Channel Transfer Complete */
    while (DMA_ReadStatusFlag(DMA1_FLAG_TC5) == RESET)
    {
        count--;
        if (count == 0)
        {
            break;
        }
    }

    /* Data is ok then turn off LED2 */
    if (BufferCompare(DMA_USART2_TxBuf, DMA_USART1_RxBuf, BufSize) == TRUE)
    {
        APM_MINI_LEDOn(LED2);
    }
    else
    {
        APM_MINI_LEDOn(LED3);
    }

    while (1)
    {
    }
}

/*!
 * @brief     USART1 RX DMA Init
 *
 * @param     None
 *
 * @retval    None
 */
void USART1_RX_DMA_Init(void)
{
    DMA_Config_T dmaConfig;

    /* Enable DMA Clock */
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA1);

    /* DMA config */
    dmaConfig.peripheralBaseAddr = USART1_DR_ADDRESS;
    dmaConfig.memoryBaseAddr = (uint32_t)DMA_USART1_RxBuf;
    dmaConfig.dir = DMA_DIR_PERIPHERAL_SRC;
    dmaConfig.bufferSize = BufSize;
    dmaConfig.peripheralInc = DMA_PERIPHERAL_INC_DISABLE;
    dmaConfig.memoryInc = DMA_MEMORY_INC_ENABLE;
    dmaConfig.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_BYTE;
    dmaConfig.memoryDataSize = DMA_MEMORY_DATA_SIZE_BYTE;
    dmaConfig.loopMode = DMA_MODE_NORMAL;
    dmaConfig.priority = DMA_PRIORITY_MEDIUM;
    dmaConfig.M2M = DMA_M2MEN_DISABLE;

    /* Enable DMA channel */
    DMA_Config(DMA1_Channel5, &dmaConfig);

    /* Enable DMA */
    DMA_Enable(DMA1_Channel5);
}

/*!
 * @brief     USART2 TX DMA Init
 *
 * @param     None
 *
 * @retval    None
 */
void USART2_TX_DMA_Init(void)
{
    DMA_Config_T dmaConfig;

    /* Enable DMA Clock */
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA1);

    /* DMA config */
    dmaConfig.peripheralBaseAddr = USART2_DR_ADDRESS;
    dmaConfig.memoryBaseAddr = (uint32_t)DMA_USART2_TxBuf;
    dmaConfig.dir = DMA_DIR_PERIPHERAL_DST;
    dmaConfig.bufferSize = BufSize;
    dmaConfig.peripheralInc = DMA_PERIPHERAL_INC_DISABLE;
    dmaConfig.memoryInc = DMA_MEMORY_INC_ENABLE;
    dmaConfig.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_BYTE;
    dmaConfig.memoryDataSize = DMA_MEMORY_DATA_SIZE_BYTE;
    dmaConfig.loopMode = DMA_MODE_NORMAL;
    dmaConfig.priority = DMA_PRIORITY_HIGH;
    dmaConfig.M2M = DMA_M2MEN_DISABLE;

    /* Enable DMA channel */
    DMA_Config(DMA1_Channel7, &dmaConfig);

    /* Enable DMA */
    DMA_Enable(DMA1_Channel7);
}

/*!
 * @brief       Compares two buffers
 *
 * @param       buf1:    First buffer to be compared
 *
 * @param       buf1:    Second buffer to be compared
 *
 * @param       size:    Buffer size
 *
 * @retval      Return TRUE if buf1 = buf2. If not then return FALSE
 *
 * @note
 */
BOOL BufferCompare(uint8_t* buf1, uint8_t* buf2, uint8_t size)
{
    uint8_t i;

    for (i = 0; i < size; i++)
    {
        if (buf1[i] != buf2[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

/**@} end of group DMA_UsartToMemory_Functions */
/**@} end of group DMA_UsartToMemory */
/**@} end of group Examples */
