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
#define DATA_BUF_SIZE         (255)

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

uint32_t voltage = 0;

/* Timer tick */
uint16_t cntTick = 0;

/* Menu title string*/
const char *menuTitleStr[] = {
    " Potentiometer Example  ",
};

/* Menu item string*/
const char *menuItemStr[] = {
    "Press KEY1 to start test",
    "       Test OK          ",
    "       Test Error       ",
    "       Testing          ",
};

/* Menu bottom column string*/
const char *menuBottomColStr[] = {
    "   GEEHY SEMICONDUCTOR   ",
};

/* Private function prototypes ********************************************/

void APM_EVAL_Init(void);
void Potentiometer_Test(void);
void ADC1_Init(void);
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
    printf("This is an example of potentiometer detection \r\n");

    printf("Press KEY1 to start test.\r\n");

    while (1)
    {
        if(APM_EVAL_PBGetState(BUTTON_KEY1) == 0)
        {
            Potentiometer_Test();

            while(APM_EVAL_PBGetState(BUTTON_KEY1) == 0);
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

    ADC1_Init();

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
void Potentiometer_Test(void)
{
    APM_EVAL_LEDOff(LED2);
    APM_EVAL_LEDOff(LED3);
    LCD_Clear(0,LCD_LINE_2,LCD_WIDTH,134,LCD_BCOLOR);

    printf("\r\nTest start \r\n");

    while(1)
    {
        LCD_DisplayIntNum(90,LCD_LINE_3,voltage,4,LCD_TEXT_FC,LCD_TEXT_BC,24);
        LCD_DisplayString(140,LCD_LINE_3,"mV",LCD_TEXT_FC,LCD_TEXT_BC,24,0);
        APM_EVAL_DelayMs(50);
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
    LCD_Clear(0,0,LCD_WIDTH,40,LCD_FCOLOR);
    LCD_DisplayString(10,LCD_LINE_TITLE,menuTitleStr[0],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,16,0);

    /* Menu item*/
    LCD_DisplayString(10,LCD_LINE_2,menuItemStr[0],LCD_TEXT_FC,LCD_TEXT_BC,16,0);

    /* Menu bottom*/
    LCD_Clear(0,LCD_LINE_BOTTOM,LCD_WIDTH,LCD_HEIGHT,LCD_FCOLOR);
    LCD_DisplayString(60,LCD_LINE_BOTTOM,menuBottomColStr[0],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,16,0);
}

/*!
 * @brief       ADC1 Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void ADC1_Init(void)
{
    GPIO_Config_T gpioConfig;
    ADC_Config_T  adcConfig;

    /* RCM Enable*/
    /* ADCCLK = PCLK2/4 */
    RCM_ConfigADCCLK(RCM_PCLK2_DIV_4);

    /* Enable ADC1 peripheral clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC1);

    /* GPIO Configuration */
    gpioConfig.pin = GPIO_PIN_0;
    gpioConfig.mode = GPIO_MODE_ANALOG;
    GPIO_Config(GPIOC, &gpioConfig);

    /* ADC Configuration */
    ADC_Reset(ADC1);
    ADC_ConfigStructInit(&adcConfig);

    adcConfig.mode = ADC_MODE_INDEPENDENT;
    adcConfig.scanConvMode = DISABLE;
    adcConfig.continuosConvMode = ENABLE;
    adcConfig.externalTrigConv = ADC_EXT_TRIG_CONV_None;
    adcConfig.dataAlign = ADC_DATA_ALIGN_RIGHT;
    adcConfig.nbrOfChannel = 1;
    ADC_Config(ADC1, &adcConfig);

    /* ADC1 regular channel10 configuration */
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_10, 1, ADC_SAMPLETIME_13CYCLES5);

    /* Enable Interrupt*/
    ADC_EnableInterrupt(ADC1, ADC_INT_EOC);

    NVIC_EnableIRQRequest(ADC1_2_IRQn, 2, 2);

    /* Enable ADC1 */
    ADC_Enable(ADC1);

    /* Enable ADC1 reset calibration register */
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_ReadResetCalibrationStatus(ADC1));

    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while(ADC_ReadCalibrationStartFlag(ADC1));

    /* Start ADC1 Software Conversion */
    ADC_EnableSoftwareStartConv(ADC1);
}

/*!
 * @brief       ADC1 Isr
 *
 * @param       None
 *
 * @retval      None
 *
 * @note        This function need to put into ADC1_IRQHandler in apm32f0xx_int.c
 */
void ADC1_Isr(void)
{
    uint32_t adcData = 0;

    if (ADC_ReadIntFlag(ADC1, ADC_INT_EOC) == SET)
    {
        ADC_ClearIntFlag(ADC1, ADC_INT_EOC);
        /* Read ADC Conversion value*/
        adcData = ADC_ReadConversionValue(ADC1);
        /*  voltage(mV) =  adcData * (3300mV / 4095) */
        voltage = (adcData * 3300) / 4095;
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

