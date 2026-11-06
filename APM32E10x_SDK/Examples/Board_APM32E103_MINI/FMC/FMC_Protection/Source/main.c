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
#include <stdio.h>
#include <string.h>

/** @addtogroup Examples
  @{
  */

/** @addtogroup FMC_Protection
  @{
  */

/** @defgroup FMC_Protection_Macros Macros
  @{
*/

/* printf using USART1  */
#define DEBUG_USART             USART1


#define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#define FLASH_PROTECTION_PAGE (FLASH_WRP_PAGE_14_15|FLASH_WRP_PAGE_16_17)


#define BANK1_WRITE_START_ADDR  ((uint32_t)0x08008000)
#define BANK1_WRITE_END_ADDR    ((uint32_t)0x0800A000)

#define FLASH_PAGE_WRITE
/* choose whether open write protection */
//#define FLASH_WRITE_PROTECTION
#define FLASH_DISABLE_PROTECTION
/**@} end of group FMC_Protection_Macros*/

/** @defgroup FMC_Protection_Variables Variables
  @{
*/

volatile FMC_STATUS_T FlashState = FMC_STATUS_COMPLETE;

uint32_t WPR_STATE = 0x0;
/**@} end of group FMC_Protection_Variables */

/** @defgroup FMC_Protection_Functions Functions
  @{
*/
void USART_Init(void);
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

    /* led init */
    APM_MINI_LEDInit(LED2);
    APM_MINI_LEDInit(LED3);
    APM_MINI_LEDOff(LED2);
    APM_MINI_LEDOff(LED3);
    /* choose usart1 */
    USART_Init();

    FMC_Unlock();
    /* read write protection page */
    WPR_STATE = FMC_ReadOptionByteWriteProtection();

#ifdef FLASH_WRITE_PROTECTION
    /* check whether pages need write protection */
    if ((~WPR_STATE) != (uint32_t)FLASH_PROTECTION_PAGE)
    {
        FMC_EraseOptionBytes();
        /* set write protection */
        FlashState = FMC_EnableWriteProtection(FLASH_PROTECTION_PAGE | ~WPR_STATE);
        /* reload the option byte */
        NVIC_SystemReset();
    }
#elif defined FLASH_DISABLE_PROTECTION
    /* check whether pages need close write protection */
    if ((~WPR_STATE) & FLASH_PROTECTION_PAGE)
    {
        FMC_EraseOptionBytes();
        /* set write protection for pages which don't close */
        FlashState = FMC_EnableWriteProtection((~WPR_STATE) & (~FLASH_PROTECTION_PAGE));
        /* reload the option byte */
        NVIC_SystemReset();
    }
#endif

#ifdef FLASH_PAGE_WRITE
    /* erase flash page */
    FlashState = FMC_ErasePage(0x08008000);
    /* check whether it is write protection error */
    if (FlashState == FMC_STATUS_ERROR_WRP)
    {
        printf("WRITE PROTECTION ERROR!\r\n");
        APM_MINI_LEDOn(LED3);
    }
    else
    {
        /* write page */
        FlashState = FMC_ProgramWord(0x08008000, 0xAAAAAAAA);
        /* check whether writing successful */
        if (FlashState == FMC_STATUS_COMPLETE && *(uint32_t*)0x08008000 == 0xAAAAAAAA)
        {
            printf("WRITE SUCCESSFUL!\r\n");
            APM_MINI_LEDOn(LED2);
        }
    }


#endif
    while (1)
    {

    }
}
/*!
 * @brief       USART configs
 *
 * @param       None
 *
 * @retval      None
 *
 */
void USART_Init(void)
{
    USART_Config_T USART_ConfigStruct;

    USART_ConfigStruct.baudRate = 115200;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode = USART_MODE_TX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;

    APM_MINI_COMInit(COM1, &USART_ConfigStruct);
}

#if defined (__CC_ARM) || defined (__ICCARM__) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       ch:  The characters that need to be send.
*
* @param       *f:  pointer to a FILE that can recording all information
*              needed to control a stream
*
* @retval      The characters that need to be send.
*
* @note
*/
int fputc(int ch, FILE* f)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return (ch);
}

#elif defined (__GNUC__)

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       ch:  The characters that need to be send.
*
* @retval      The characters that need to be send.
*
* @note
*/
int __io_putchar(int ch)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, ch);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return ch;
}

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       file:  Meaningless in this function.
*
* @param       *ptr:  Buffer pointer for data to be sent.
*
* @param       len:  Length of data to be sent.
*
* @retval      The characters that need to be send.
*
* @note
*/
int _write(int file, char* ptr, int len)
{
    UNUSED(file);
    int i;
    for (i = 0; i < len; i++)
    {
        __io_putchar(*ptr++);
    }

    return len;
}

#else
#warning Not supported compiler type
#endif
/**@} end of group FMC_Protection_Functions */
/**@} end of group FMC_Protection */
/**@} end of group Examples */
