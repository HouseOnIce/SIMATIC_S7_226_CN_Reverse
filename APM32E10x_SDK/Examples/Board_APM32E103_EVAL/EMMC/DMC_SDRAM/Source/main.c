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

/* Includes ***************************************************************/
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bsp_lcd.h"
#include "bsp_key.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* printf using USART1  */
#define DEBUG_USART           USART1

/* printf message */
#define MESSAGE0    "*******************************"
#define MESSAGE1    "* APM32E103xE DMC SDRAM Demo "
#define MESSAGE2    "* SystemCoreClock      :"
#define MESSAGE3    "* SDRAMClock           :"
#define MESSAGE4    "*  8 bit W/R Err Count :"
#define MESSAGE5    "* 16 bit W/R Err Count :"
#define MESSAGE6    "* 32 bit W/R Err Count :"

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

uint32_t voltage = 0;

/* Timer tick */
uint16_t cntTick = 0;

/**
 * @brief    W/R Err Count structure definition
 */
typedef struct
{
    uint32_t    ByteCount;
    uint32_t    HalfWordCount;
    uint32_t    WordCount;
} Err_Count_T;

Err_Count_T Err;

/* Menu title string*/
const char* menuTitleStr[] =
{
    " DMC SDRAM Example  ",
};

/* Menu item string*/
const char* menuItemStr[] =
{
    "Press KEY1 to start test",
    "       Test OK          ",
    "       Test Error       ",
    "       Testing          ",
};

/* Menu bottom column string*/
const char* menuBottomColStr[] =
{
    "   GEEHY SEMICONDUCTOR   ",
};

/* Private function prototypes ********************************************/

void APM_EVAL_Init(void);

void APM_EVAL_SDRAM_System_Clock_Init(void);
void SDRAM_Test(void);

void Menu_DisplayInit(void);
void APM_EVAL_TMR7_Init(uint16_t period, uint16_t div);
void SystemCoreClockConfig(uint32_t pllMf, uint32_t latency);

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
int main(void)
{
    SystemClockConfig();

    APM_EVAL_Init();

    printf("Press KEY1 to start test.\r\n");

    while (1)
    {
        if (APM_EVAL_PBGetState(BUTTON_KEY1) == 0)
        {
            SDRAM_Test();

            while (APM_EVAL_PBGetState(BUTTON_KEY1) == 0);
        }
    }
}

/*!
 * @brief       Init EVAL board
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void APM_EVAL_Init(void)
{
    USART_Config_T usartConfigStruct;

    /* Init delay function */
    APM_EVAL_DelayInit();

    APM_EVAL_LEDInit(LED1);
    APM_EVAL_LEDInit(LED2);
    APM_EVAL_LEDInit(LED3);

    APM_EVAL_PBInit(BUTTON_KEY1, BUTTON_MODE_GPIO);

    APM_EVAL_SDRAM_System_Clock_Init();

    SystemCoreClockUpdate();

    usartConfigStruct.baudRate = 115200;
    usartConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    usartConfigStruct.mode = USART_MODE_TX_RX;
    usartConfigStruct.parity = USART_PARITY_NONE;
    usartConfigStruct.stopBits = USART_STOP_BIT_1;
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;
    APM_EVAL_COMInit(COM1, &usartConfigStruct);

    APM_EVAL_TMR7_Init(1000, SystemCoreClock/1000000);

    SDRAM_Init();

    APM_LCDInit();
    LCD_Clear(0, 0, LCD_WIDTH, LCD_HEIGHT, LCD_BCOLOR);

    APM_EVAL_DelayMs(100);

    Menu_DisplayInit();
}

/*!
 * @brief       Detect potentiometer
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void SDRAM_Test(void)
{
    /* printf message */
    printf("\r\n");
    printf("%s \r\n", MESSAGE0);
    printf("%s \r\n", MESSAGE1);
    printf("%s %ld MHz\r\n", MESSAGE2, RCM_ReadSYSCLKFreq()/1000000);

    printf("%s %ld MHz\r\n", MESSAGE3, RCM_ReadSYSCLKFreq()/1000000/(RCM->CFG_B.SDRAMPSC + 1));

    LCD_DisplayString(10, LCD_LINE_2, menuItemStr[3], LCD_TEXT_FC, LCD_TEXT_BC, 16, 0);

    /* SDRAM byte write/read test */
    Err.ByteCount = SDRAM_Byte_Test();
    printf("%s %ld\r\n", MESSAGE4, Err.ByteCount);

    /* SDRAM halfword write/read test */
    Err.HalfWordCount = SDRAM_HalfWord_Test();
    printf("%s %ld\r\n", MESSAGE5, Err.HalfWordCount);

    /* SDRAM word write/read test */
    Err.WordCount = SDRAM_Word_Test();
    printf("%s %ld\r\n", MESSAGE6, Err.WordCount);

    printf("%s \r\n", MESSAGE0);

    /* The SDRAM test failed */
    if ((Err.ByteCount != 0) || (Err.HalfWordCount != 0) || ((Err.WordCount != 0)))
    {
        APM_EVAL_LEDOff(LED3);
        LCD_DisplayString(10, LCD_LINE_2, menuItemStr[2], LCD_TEXT_FC, LCD_TEXT_BC, 16, 0);
    }
    else
    {
        APM_EVAL_LEDOn(LED3);
        LCD_DisplayString(10, LCD_LINE_2, menuItemStr[1], LCD_TEXT_FC, LCD_TEXT_BC, 16, 0);
    }
}

/*!
 * @brief       Initialize the Menu UI
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void Menu_DisplayInit(void)
{
    /* Menu title*/
    LCD_Clear(0, 0, LCD_WIDTH, 40, LCD_FCOLOR);
    LCD_DisplayString(10, LCD_LINE_TITLE, menuTitleStr[0], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 16, 0);

    /* Menu item*/
    LCD_DisplayString(10, LCD_LINE_2, menuItemStr[0], LCD_TEXT_FC, LCD_TEXT_BC, 16, 0);

    /* Menu bottom*/
    LCD_Clear(0, LCD_LINE_BOTTOM, LCD_WIDTH, LCD_HEIGHT, LCD_FCOLOR);
    LCD_DisplayString(60, LCD_LINE_BOTTOM, menuBottomColStr[0], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 16, 0);
}


/*!
 * @brief       SDRAM Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void APM_EVAL_SDRAM_System_Clock_Init(void)
{
    /* Sets System clock frequency */
    /* 15*8 = 120 Mhz *//**/
    SystemCoreClockConfig(15, 4);
    RCM_ConfigSDRAMCLK(RCM_DMC_DIV_4);
}

/*!
 * @brief       Sets System clock frequency
 *
 * @param       pllMf  : PLL multiplication factor
 *                       This parameter can be RCM_PLLMF_x where x can be a value from 2 to 16.
 *
 * @param       latency: the FMC Latency value.
 *
 * @retval      None
 *
 */
void SystemCoreClockConfig(uint32_t pllMf, uint32_t latency)
{
    __IO uint32_t i;

    /* Set HSIEN bit */
    RCM->CTRL_B.HSIEN = BIT_SET;
    /* Reset SCLKSW, AHBPSC, APB1PSC, APB2PSC, ADCPSC and MCOSEL bits */
    RCM->CFG &= (uint32_t)0xF8FF0000;
    /* Reset HSEEN, CSSEN and PLLEN bits */
    RCM->CTRL &= (uint32_t)0xFEF6FFFF;
    /* Reset HSEBCFG bit */
    RCM->CTRL_B.HSEBCFG = BIT_RESET;
    /* Reset PLLSRCSEL, PLLHSEPSC, PLLMULCFG and USBDIV bits */
    RCM->CFG &= (uint32_t)0xFF80FFFF;
    /* Disable all interrupts and clear pending bits */
    RCM->INT = 0x009F0000;

    if (!(RCM->CTRL_B.HSERDYFLG))
    {
        RCM->CTRL_B.HSEEN= BIT_SET;

        for (i = 0; i < HSE_STARTUP_TIMEOUT; i++)
        {
            if (RCM->CTRL_B.HSERDYFLG)
            {
                break;
            }
        }
    }

    if (RCM->CTRL_B.HSERDYFLG)
    {
        /* HSelect HSE as system clock source */
        RCM->CFG_B.SCLKSEL = 0x01;

        /* Enable Prefetch Buffer */
        FMC->CTRL1_B.PBEN = BIT_SET;
        /* Flash latency wait state */
        FMC->CTRL1_B.WS = latency;

        /* HCLK = SYSCLK */
        RCM->CFG_B.AHBPSC= 0X00;
        /* PCLK2 = HCLK */
        RCM->CFG_B.APB2PSC= 0;
        /* PCLK1 = HCLK / 2 */
        RCM->CFG_B.APB1PSC = 4;

        /* Disable PLL */
        RCM->CTRL_B.PLLEN = 0;

        /* PLL: HSE * PLLm */
        RCM->CFG_B.PLLSRCSEL = 1;
        RCM->CFG_B.PLLMULCFG = (pllMf - 2);

        /* Enable PLL */
        RCM->CTRL_B.PLLEN = 1;

        /* Wait PLL Ready */
        while (RCM->CTRL_B.PLLRDYFLG == BIT_RESET);

        /* Select PLL as system clock source */
        RCM->CFG_B.SCLKSEL = 2;

        /* Wait till PLL is used as system clock source */
        while (RCM->CFG_B.SCLKSELSTS!= 0x02);
    }
}

/*!
 * @brief       TMR7 init
 *
 * @param       None
 *
 * @retval      None
 *
 */
void APM_EVAL_TMR7_Init(uint16_t period, uint16_t div)
{
    TMR_BaseConfig_T  timeBaseConfig;

    /* Enable Clock*/
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR7);

    /* Set clockDivision = 1 */
    timeBaseConfig.clockDivision =  TMR_CLOCK_DIV_1;
    /* Up-counter */
    timeBaseConfig.countMode =  TMR_COUNTER_MODE_UP;
    /* Set divider */
    timeBaseConfig.division = div - 1 ;
    /* Set counter = 0xffff */
    timeBaseConfig.period = period - 1;
    /* Repetition counter = 0x0 */
    timeBaseConfig.repetitionCounter =  0;

    TMR_ConfigTimeBase(TMR7, &timeBaseConfig);

    /* Enable update interrupt*/
    TMR_EnableInterrupt(TMR7, TMR_INT_UPDATE);
    NVIC_EnableIRQRequest(TMR7_IRQn, 2, 2);

    /*  Enable TMR7  */
    TMR_Enable(TMR7);
}

/*!
 * @brief       TMR4 interrupt service runtine
 *
 * @param       None
 *
 * @retval      None
 *
 * @note        This function need to put into TMR7_IRQHandler() in apm32f0xx_int.c
 */
void TMR7_Isr(void)
{
    if (TMR_ReadIntFlag(TMR7, TMR_INT_UPDATE) == SET)
    {
        TMR_ClearIntFlag(TMR7, TMR_INT_UPDATE);
        cntTick++;

        if (cntTick >= 500)
        {
            cntTick = 0;
            APM_EVAL_LEDToggle(LED1);
        }
    }
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

    /* wait for the data to be send */
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

    /* wait for the data to be send */
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
