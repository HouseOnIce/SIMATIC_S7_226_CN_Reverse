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

/* Buffer Size */
#define DATA_BUF_SIZE         (56)

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Timer tick */
uint16_t cntTick = 0;

MENU_INFO_T MenuTable[] =
{
    /* current menu, enter, prev, next, return, function*/
    {0, 0, 3, 1, 0, (*LCD_HomePageHandle), 0},
    {1, 4, 3, 2, 0, (*LCD_HomePageHandle), 1},
    {2, 5, 1, 3, 0, (*LCD_HomePageHandle), 2},
    {3, 6, 2, 1, 0, (*LCD_HomePageHandle), 3},

    {4, 4, 4, 4, 1, (*LCD_MenuLevel1Handle), 0},
    {5, 5, 5, 5, 2, (*LCD_MenuLevel2Handle), 0},
    {6, 6, 6, 6, 3, (*LCD_MenuLevel3Handle), 0},
};

/* Menu title string*/
const char *menuTitleStr[] =
{
    "  SPI LCD Example     ",
    "  Menu 0 - 1 Title    ",
    "  Menu 0 - 2 Title    ",
    "  Menu 0 - 3 Title    ",
};

/* Menu item string*/
const char *menuItemStr[] =
{
    " 1.Menu 0 - 1           ",
    " 2.Menu 0 - 2           ",
    " 3.Menu 0 - 3           ",
};

/* Menu bottom column string*/
const char *menuBottomColStr[] =
{
    "   GEEHY SEMICONDUCTOR   ",
};

uint8_t curFuncIndex = 0;
uint8_t menuChange = 0;

/* Private function prototypes ********************************************/

void (*curFunction)(uint8_t param);
void APM_EVAL_Init(void);
void ADC1_Init(void);
void Menu_DisplayInit(void);
void LCD_MenuHandle(void);
void APM_EVAL_TMR7_Init(uint16_t period, uint16_t div);

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
    printf("This is an example of multi-level menu by SPI LCD \r\n");

    while (1)
    {
        LCD_MenuHandle();
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
    APM_EVAL_PBInit(BUTTON_KEY2, BUTTON_MODE_GPIO);
    APM_EVAL_PBInit(BUTTON_KEY3, BUTTON_MODE_GPIO);

    usartConfigStruct.baudRate = 115200;
    usartConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    usartConfigStruct.mode = USART_MODE_TX_RX;
    usartConfigStruct.parity = USART_PARITY_NONE;
    usartConfigStruct.stopBits = USART_STOP_BIT_1;
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;

    APM_EVAL_COMInit(COM1, &usartConfigStruct);

    APM_EVAL_TMR7_Init(1000, 72);
    APM_LCDInit();
    LCD_Clear(0, 0, LCD_WIDTH, LCD_HEIGHT, LCD_BCOLOR);

    APM_EVAL_DelayMs(100);

    Menu_DisplayInit();
}

/*!
 * @brief       Initialize the LCD menu level 1 UI
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void LCD_MenuLevel1Handle(uint8_t menuState)
{
	UNUSED(menuState);
    /* Clear content*/
    LCD_Clear(0, 40, LCD_WIDTH, 150, LCD_BCOLOR);
    LCD_Clear(0, 0, LCD_WIDTH, 40, LCD_FCOLOR);
    /* Menu title*/
    LCD_Clear(0, 0, LCD_WIDTH, 40, LCD_FCOLOR);
    LCD_DisplayString(10, LCD_LINE_TITLE, menuTitleStr[1], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 24, 0);

    /* Menu item*/
    LCD_DisplayString(10, LCD_LINE_1, " Menu 0 - 1 Cotent   ", LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);

    /* Menu bottom*/
    LCD_Clear(0, LCD_LINE_BOTTOM, LCD_WIDTH, LCD_HEIGHT, LCD_FCOLOR);
    LCD_DisplayString(60, LCD_LINE_BOTTOM, menuBottomColStr[0], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 16, 0);
}

/*!
 * @brief       Initialize the LCD menu level 2 UI
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void LCD_MenuLevel2Handle(uint8_t menuState)
{
	UNUSED(menuState);
    /* Clear content*/
    LCD_Clear(0, 40, LCD_WIDTH, 150, LCD_BCOLOR);
    LCD_Clear(0, 0, LCD_WIDTH, 40, LCD_FCOLOR);
    /* Menu title*/
    LCD_Clear(0, 0, LCD_WIDTH, 40, LCD_FCOLOR);
    LCD_DisplayString(10, LCD_LINE_TITLE, menuTitleStr[2], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 24, 0);

    /* Menu item*/
    LCD_DisplayString(10, LCD_LINE_1, " Menu 0 - 2 Cotent   ", LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);

    /* Menu bottom*/
    LCD_Clear(0, LCD_LINE_BOTTOM, LCD_WIDTH, LCD_HEIGHT, LCD_FCOLOR);
    LCD_DisplayString(60, LCD_LINE_BOTTOM, menuBottomColStr[0], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 16, 0);
}

/*!
 * @brief       Initialize the LCD menu level 3 UI
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void LCD_MenuLevel3Handle(uint8_t menuState)
{
	UNUSED(menuState);
    /* Clear content*/
    LCD_Clear(0, 40, LCD_WIDTH, 150, LCD_BCOLOR);
    LCD_Clear(0, 0, LCD_WIDTH, 40, LCD_FCOLOR);
    /* Menu title*/
    LCD_Clear(0, 0, LCD_WIDTH, 40, LCD_FCOLOR);
    LCD_DisplayString(10, LCD_LINE_TITLE, menuTitleStr[3], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 24, 0);

    /* Menu item*/
    LCD_DisplayString(10, LCD_LINE_1, " Menu 0 - 3 Cotent   ", LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);

    /* Menu bottom*/
    LCD_Clear(0, LCD_LINE_BOTTOM, LCD_WIDTH, LCD_HEIGHT, LCD_FCOLOR);
    LCD_DisplayString(60, LCD_LINE_BOTTOM, menuBottomColStr[0], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 16, 0);
}


/*!
 * @brief       Initialize the LCD home page UI
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void LCD_HomePageHandle(uint8_t menuState)
{
    switch (menuState)
    {
    case 0:
        /* Clear content*/
        LCD_Clear(0, 40, LCD_WIDTH, 150, LCD_BCOLOR);
        LCD_Clear(0, 0, LCD_WIDTH, 40, LCD_FCOLOR);
        /* Menu title*/
        LCD_Clear(0, 0, LCD_WIDTH, 40, LCD_FCOLOR);
        LCD_DisplayString(10, LCD_LINE_TITLE, menuTitleStr[0], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 24, 0);

        /* Menu item*/
        LCD_DisplayString(10, LCD_LINE_0, menuItemStr[0], LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);
        LCD_DisplayString(10, LCD_LINE_1, menuItemStr[1], LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);
        LCD_DisplayString(10, LCD_LINE_2, menuItemStr[2], LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);

        LCD_DrawLine(10, 160, 310, 160, LCD_FCOLOR);
        /* Menu button*/
        LCD_DisplayString(20, LCD_LINE_4, "RESET", LCD_TEXT_FC, LCD_TEXT_BC, 16, 0);
        LCD_DisplayString(80, LCD_LINE_4, "PREV", LCD_TEXT_FC, LCD_TEXT_BC, 16, 0);
        LCD_DisplayString(80, 190, "ENTER", LCD_TEXT_FC, LCD_TEXT_BC, 16, 0);
        LCD_DisplayString(80, 210, "RETURN", LCD_TEXT_FC, LCD_TEXT_BC, 16, 0);

        /* Menu bottom*/
        LCD_Clear(0, LCD_LINE_BOTTOM, LCD_WIDTH, LCD_HEIGHT, LCD_FCOLOR);
        LCD_DisplayString(60, LCD_LINE_BOTTOM, menuBottomColStr[0], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 16, 0);
        break;

    case 1:
        /* Menu item*/
        LCD_DisplayString(10, LCD_LINE_2, menuItemStr[2], LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);
        LCD_DisplayString(10, LCD_LINE_1, menuItemStr[1], LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);
        LCD_DisplayString(10, LCD_LINE_0, menuItemStr[0], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 24, 0);
        break;

    case 2:
        /* Menu item*/
        LCD_DisplayString(10, LCD_LINE_0, menuItemStr[0], LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);
        LCD_DisplayString(10, LCD_LINE_2, menuItemStr[2], LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);
        LCD_DisplayString(10, LCD_LINE_1, menuItemStr[1], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 24, 0);
        break;

    case 3:
        /* Menu item*/
        LCD_DisplayString(10, LCD_LINE_1, menuItemStr[1], LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);
        LCD_DisplayString(10, LCD_LINE_2, menuItemStr[2], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 24, 0);
        LCD_DisplayString(10, LCD_LINE_0, menuItemStr[0], LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);
        break;
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
    LCD_DisplayString(10, LCD_LINE_TITLE, menuTitleStr[0], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 24, 0);

    /* Menu item*/
    /* Menu item*/
    LCD_DisplayString(10, LCD_LINE_0, menuItemStr[0], LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);
    LCD_DisplayString(10, LCD_LINE_1, menuItemStr[1], LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);
    LCD_DisplayString(10, LCD_LINE_2, menuItemStr[2], LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);

    LCD_DrawLine(10, 160, 310, 160, LCD_FCOLOR);
    /* Menu button*/
    LCD_DisplayString(20, LCD_LINE_4, "RESET", LCD_TEXT_FC, LCD_TEXT_BC, 16, 0);
    LCD_DisplayString(80, LCD_LINE_4, "PREV", LCD_TEXT_FC, LCD_TEXT_BC, 16, 0);
    LCD_DisplayString(80, 190, "ENTER", LCD_TEXT_FC, LCD_TEXT_BC, 16, 0);
    LCD_DisplayString(80, 210, "RETURN", LCD_TEXT_FC, LCD_TEXT_BC, 16, 0);

    /* Menu bottom*/
    LCD_Clear(0, LCD_LINE_BOTTOM, LCD_WIDTH, LCD_HEIGHT, LCD_FCOLOR);
    LCD_DisplayString(60, LCD_LINE_BOTTOM, menuBottomColStr[0], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 16, 0);
}

/*!
 * @brief       LCD menu handle
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void LCD_MenuHandle(void)
{
    uint8_t keyType;
    static uint8_t menuTemp = 0;

    keyType = Key_Scan(KEY_MODE_CONTINUOUS);

    switch (keyType)
    {
    case MENU_KEY_PREV:
        curFuncIndex = MenuTable[curFuncIndex].prevIndex;
        break;

    case MENU_KEY_ENTER:
        curFuncIndex = MenuTable[curFuncIndex].enterIndex;
        break;

    case MENU_KEY_RETURN:
        curFuncIndex = MenuTable[curFuncIndex].retIndex;
        break;

//        case MENU_KEY_RETURN:
//            curFuncIndex = MenuTable[curFuncIndex].retIndex;
//            break;
    }

    if (menuTemp !=  MenuTable[curFuncIndex].curMenu)
    {
        menuTemp = MenuTable[curFuncIndex].curMenu;

        (*MenuTable[curFuncIndex].function)(MenuTable[curFuncIndex].stateMenu);
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

