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

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

uint32_t voltage = 0;

/* Timer tick */
uint16_t cntTick = 0;

CAN_TxMessage_T   Can1TxMessage;
CAN_TxMessage_T   Can2TxMessage;

CAN_RxMessage_T   Can1RxMessage;
CAN_RxMessage_T   Can2RxMessage;

/* Menu title string*/
const char *menuTitleStr[] =
{
    " CAN Dual Example  ",
};

/* Menu item string*/
const char *menuItemStr[] =
{
    "Press KEY1 for CAN1 transmit",
    "and CAN2 receive test.",
    "Press KEY2 to CAN2 transmit",
    "and CAN1 receive test.",
    "       Test OK          ",
    "       Test Error       ",
    "       Testing          ",
};

/* Menu bottom column string*/
const char *menuBottomColStr[] =
{
    "   GEEHY SEMICONDUCTOR   ",
};

/* Private function prototypes ********************************************/

void APM_EVAL_Init(void);
void CAN_Init(void);
void Menu_DisplayInit(void);
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
    printf("This is an example of CAN Dual detection \r\n");

    printf("Press KEY1 or KEY2 to start test.\r\n");

    while (1)
    {
        if (APM_EVAL_PBGetState(BUTTON_KEY1) == RESET)
        {
            APM_EVAL_DelayMs(10);
            if (APM_EVAL_PBGetState(BUTTON_KEY1) == RESET)
            {
                printf("CAN1 Transmit 0x55 !\r\n");
                LCD_DisplayString(10,LCD_LINE_3,menuItemStr[6],LCD_TEXT_FC,LCD_TEXT_BC,16,0);
                Can1TxMessage.data[0]  = 0x55;
                CAN_TxMessage(CAN1, &Can1TxMessage);
            }
            while (APM_EVAL_PBGetState(BUTTON_KEY1) == RESET);
        }
        else if (APM_EVAL_PBGetState(BUTTON_KEY2) == RESET)
        {
            APM_EVAL_DelayMs(10);
            if (APM_EVAL_PBGetState(BUTTON_KEY2) == RESET)
            {
                printf("CAN2 Transmit 0xAA !\r\n");
                LCD_DisplayString(10,LCD_LINE_7,menuItemStr[6],LCD_TEXT_FC,LCD_TEXT_BC,16,0);
                Can2TxMessage.data[0]  = 0xAA;
                CAN_TxMessage(CAN2, &Can2TxMessage);
            }
            while (APM_EVAL_PBGetState(BUTTON_KEY2) == RESET);
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

    APM_EVAL_LEDOff(LED1);
    APM_EVAL_LEDOff(LED2);
    APM_EVAL_LEDOff(LED3);

    APM_EVAL_PBInit(BUTTON_KEY1, BUTTON_MODE_GPIO);
    APM_EVAL_PBInit(BUTTON_KEY2, BUTTON_MODE_GPIO);

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

    CAN_Init();

    APM_EVAL_DelayMs(100);

    Menu_DisplayInit();
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
    LCD_DisplayString(10,LCD_LINE_1,menuItemStr[0],LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    LCD_DisplayString(10,LCD_LINE_2,menuItemStr[1],LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    LCD_DisplayString(10,LCD_LINE_5,menuItemStr[2],LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    LCD_DisplayString(10,LCD_LINE_6,menuItemStr[3],LCD_TEXT_FC,LCD_TEXT_BC,16,0);

    /* Menu bottom*/
    LCD_Clear(0, LCD_LINE_BOTTOM, LCD_WIDTH, LCD_HEIGHT, LCD_FCOLOR);
    LCD_DisplayString(60, LCD_LINE_BOTTOM, menuBottomColStr[0], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 16, 0);
}

/*!
 * @brief       CAN Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void CAN_Init(void)
{
    GPIO_Config_T gpioConfig;
    CAN_Config_T       canConfig;
    CAN_FilterConfig_T filterConfig;

    NVIC_EnableIRQRequest(USBD1_LP_CAN1_RX0_IRQn, 1, 0);
    NVIC_EnableIRQRequest(USBD2_LP_CAN2_RX0_IRQn, 1, 0);

    /* GPIOB, GPIOD and AFIO clocks enable */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOB | RCM_APB2_PERIPH_GPIOD | RCM_APB2_PERIPH_AFIO);
    /* CAN1 and CAN2 Periph clocks enable */
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_CAN1 | RCM_APB1_PERIPH_CAN2);

    /* CAN GPIO Init */
    /* CAN1 Tx remap 1 */
    GPIO_ConfigPinRemap(GPIO_REMAP1_CAN1);
    gpioConfig.pin   = GPIO_PIN_9;
    gpioConfig.mode  = GPIO_MODE_AF_PP;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOB, &gpioConfig);

    /* CAN1 Rx remap 1 */
    gpioConfig.pin = GPIO_PIN_8;
    gpioConfig.mode = GPIO_MODE_IN_PU;
    GPIO_Config(GPIOB, &gpioConfig);

    /* CAN2 Tx */
    gpioConfig.pin   = GPIO_PIN_13;
    gpioConfig.mode  = GPIO_MODE_AF_PP;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOB, &gpioConfig);

    /* CAN2 Rx */
    gpioConfig.pin  = GPIO_PIN_12;
    gpioConfig.mode = GPIO_MODE_IN_PU;
    GPIO_Config(GPIOB, &gpioConfig);

    CAN_Reset(CAN1);
    CAN_Reset(CAN2);

    CAN_ConfigStructInit(&canConfig);

    /* CAN1 and CAN2 cell init */
    canConfig.autoBusOffManage = DISABLE;
    canConfig.autoWakeUpMode   = DISABLE;
    canConfig.nonAutoRetran    = DISABLE;
    canConfig.rxFIFOLockMode   = DISABLE;
    canConfig.txFIFOPriority   = ENABLE;
    canConfig.mode             = CAN_MODE_NORMAL;
    canConfig.syncJumpWidth    = CAN_SJW_1;
    canConfig.timeSegment1     = CAN_TIME_SEGMENT1_3;
    canConfig.timeSegment2     = CAN_TIME_SEGMENT2_2;
    canConfig.prescaler = 6;

    CAN_Config(CAN1, &canConfig);
    CAN_Config(CAN2, &canConfig);

    /* CAN1 and CAN2 filter init */
    filterConfig.filterNumber = 1;
    filterConfig.filterMode = CAN_FILTER_MODE_IDMASK;
    filterConfig.filterScale = CAN_FILTER_SCALE_32BIT;
    filterConfig.filterIdHigh = 0x6420;
    filterConfig.filterIdLow  = 0x0000;
    filterConfig.filterMaskIdHigh = 0x0000;
    filterConfig.filterMaskIdLow  = 0x0000;
    filterConfig.filterFIFO = CAN_FILTER_FIFO_0;
    filterConfig.filterActivation =  ENABLE;
    CAN_ConfigFilter(CAN1, &filterConfig);
    CAN_ConfigFilter(CAN2, &filterConfig);

    /* CAN Transmit Init */
    Can1TxMessage.stdID = 0x321;
    Can1TxMessage.extID = 0x01;
    Can1TxMessage.remoteTxReq   = CAN_RTXR_DATA;
    Can1TxMessage.typeID   = CAN_TYPEID_STD;
    Can1TxMessage.dataLengthCode   = 1;

    Can2TxMessage.stdID = 0x321;
    Can2TxMessage.extID = 0x01;
    Can2TxMessage.remoteTxReq   = CAN_RTXR_DATA;
    Can2TxMessage.typeID   = CAN_TYPEID_STD;
    Can2TxMessage.dataLengthCode   = 1;

    CAN_EnableInterrupt(CAN1, CAN_INT_F0MP);
    CAN_EnableInterrupt(CAN2, CAN_INT_F0MP);
}

/*!
 * @brief   CAN1 Rx interrupt
 *
 * @param   None
 *
 * @retval  None
 *
 */
void CAN1_RX0_Isr(void)
{
    CAN_RxMessage(CAN1, CAN_RX_FIFO_0, &Can1RxMessage);

    if ((Can1RxMessage.stdID == 0x321) && (Can1RxMessage.typeID == CAN_TYPEID_STD) && (Can1RxMessage.dataLengthCode == 1) && (Can1RxMessage.data[0] == 0xAA))
    {
        APM_EVAL_LEDOn(LED2);
        printf("CAN1 Receive 0xAA !\r\n\r\n");
        LCD_DisplayString(10,LCD_LINE_7,menuItemStr[4],LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    }
    else
    {
        APM_EVAL_LEDOff(LED2);
        printf("CAN1 Receive Error !\r\n\r\n");
        LCD_DisplayString(10,LCD_LINE_7,menuItemStr[5],LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    }
}

/*!
 * @brief   CAN2 Rx interrupt
 *
 * @param   None
 *
 * @retval  None
 *
 */
void CAN2_RX0_Isr(void)
{
    CAN_RxMessage(CAN2, CAN_RX_FIFO_0, &Can2RxMessage);

    if ((Can2RxMessage.stdID == 0x321) && (Can2RxMessage.typeID == CAN_TYPEID_STD) && (Can2RxMessage.dataLengthCode == 1) && (Can2RxMessage.data[0] == 0x55))
    {
        APM_EVAL_LEDOn(LED3);
        printf("CAN2 Receive 0x55 !\r\n\r\n");
        LCD_DisplayString(10,LCD_LINE_3,menuItemStr[4],LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    }
    else
    {
        APM_EVAL_LEDOff(LED3);
        printf("CAN2 Receive Error !\r\n\r\n");
        LCD_DisplayString(10,LCD_LINE_3,menuItemStr[5],LCD_TEXT_FC,LCD_TEXT_BC,16,0);
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

    /* Enable TMR7 */
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

