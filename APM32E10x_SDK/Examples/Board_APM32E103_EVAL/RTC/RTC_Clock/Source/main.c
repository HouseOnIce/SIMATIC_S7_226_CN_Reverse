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

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* printf using USART1  */
#define DEBUG_USART           USART1

/* Buffer Size */
#define DATA_BUF_SIZE         (255)

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

TIME_T TimeConfig;

/* Timer tick */
uint16_t cntTick = 0;

/* Menu title string*/
const char *menuTitleStr[] =
{
    " RTC Clock Example  ",
};

/* Menu item string*/
const char *menuItemStr[] =
{
    "Press KEY1 to start test",
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
void Time_Display(uint32_t TimeVar, TIME_T *TimeStruct);
void Time_Regulate(TIME_T TimeStruct);
void Time_Init(void);
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

    char timeStr[9];

    APM_EVAL_Init();

    printf("This is an example of RTC Clock detection \r\n");

    while (1)
    {
        Time_Display(RTC_ReadCounter(), &TimeConfig);
        sprintf(timeStr, "%02d:%02d:%02d", TimeConfig.hours, TimeConfig.minutes, TimeConfig.seconds);
        LCD_DisplayString(75, LCD_LINE_3, timeStr, LCD_TEXT_FC, LCD_TEXT_BC, 24, 0);
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

    Time_Init();

    if (BAKPR_ReadBackupRegister(BAKPR_DATA1) == 0x00)
    {
        /* Backup data register value is not correct or not yet programmed (when
        the first time the program is executed) */

        /* Returns the time 23:59:50 */
        TimeConfig.hours = 23;
        TimeConfig.minutes = 59;
        TimeConfig.seconds = 50;
        Time_Regulate(TimeConfig);
    }

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

    /* Menu bottom*/
    LCD_Clear(0, LCD_LINE_BOTTOM, LCD_WIDTH, LCD_HEIGHT, LCD_FCOLOR);
    LCD_DisplayString(60, LCD_LINE_BOTTOM, menuBottomColStr[0], LCD_TEXT_SELECT_FC, LCD_TEXT_SELECT_BC, 16, 0);
}


/*!
 * @brief       RTC init
 *
 * @param       None
 *
 * @retval      None
 *
 */
void Time_Init(void)
{
    /* Enable PMU BAKR Periph Clock*/
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_PMU | RCM_APB1_PERIPH_BAKR);

    /* Backup Access*/
    PMU_EnableBackupAccess();

    /* Enable LSI */
    RCM_EnableLSI();

    /* Wait until LSI is ready*/
    while (RCM_ReadStatusFlag(RCM_FLAG_LSIRDY) == RESET);

    /* Select LICT as RCM clock*/
    RCM_ConfigRTCCLK(RCM_RTCCLK_LSI);

    /* Enable RTC clock*/
    RCM_EnableRTCCLK();

    /* Wait Synchro*/
    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Enable the RTC Second */
    RTC_EnableInterrupt(RTC_INT_SEC);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Set RTC prescaler: set RTC period to 1sec */
    /* RTC period = RTCCLK/(PSC+1) �� 40kHz/(39999+1) */
    RTC_ConfigPrescaler(39999);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Enable the RTC Interrupt */
    NVIC_EnableIRQRequest(RTC_IRQn, 0, 0);
}

/**
  * @brief  Displays the current time.
  * @param  TimeVar: RTC counter value.
  * @retval None
  */
void Time_Display(uint32_t TimeVar, TIME_T *TimeStruct)
{
    /* Reset RTC Counter when Time is 23:59:59 */
    if (RTC_ReadCounter() == 0x0001517F)
    {
        RTC_ConfigCounter(0x0);
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
    }

    /* Compute  hours */
    TimeStruct->hours = TimeVar / 3600;
    /* Compute minutes */
    TimeStruct->minutes = (TimeVar % 3600) / 60;
    /* Compute seconds */
    TimeStruct->seconds = (TimeVar % 3600) % 60;
}

/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval Current time RTC counter value
  */
void Time_Regulate(TIME_T TimeStruct)
{
    /* Return the value to store in RTC counter register */
    RTC_ConfigCounter((TimeStruct.hours * 3600 + TimeStruct.minutes * 60 + TimeStruct.seconds));
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}

/*!
 * @brief       ADC1 Isr
 *
 * @param       None
 *
 * @retval      None
 *
 * @note        This function need to put into RTC_IRQHandler in apm32f0xx_int.c
 */
void RTC_Isr(void)
{
    if (RTC_ReadIntFlag(RTC_INT_SEC) != RESET)
    {
        BAKPR_ConfigBackupRegister(BAKPR_DATA1, (uint16_t)RTC_ReadCounter());
        BAKPR_ConfigBackupRegister(BAKPR_DATA2, (uint16_t)(RTC_ReadCounter() >> 16));

        RTC_ClearIntFlag(RTC_INT_SEC);

        APM_EVAL_LEDToggle(LED2);

        RTC_WaitForLastTask();
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

