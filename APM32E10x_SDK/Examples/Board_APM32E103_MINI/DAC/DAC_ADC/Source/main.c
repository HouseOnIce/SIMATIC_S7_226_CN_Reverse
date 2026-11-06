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
/** @addtogroup Examples
  @{
  */

/** @addtogroup DAC_ADC
  @{
  */

/** @defgroup DAC_ADC_Macros Macros
  @{
*/

/* printf function configs to USART1*/
#define DEBUG_USART  USART1

/**@} end of group DAC_ADC_Macros*/

/** @defgroup DAC_ADC_Variables Variables
  @{
*/

/* use to generate analog signal for DAC*/
uint16_t dacData = 0;

/**@} end of group DAC_ADC_Variables */

/** @defgroup DAC_ADC_Functions Functions
  @{
*/

/* Delay */
void Delay(uint32_t count);
/* DAC init */
void DAC_Init(void);
/* ADC init */
void ADC_Init(void);

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

    USART_Config_T usartConfigStruct;

    /* Configures LED2 and LED3 */
    APM_MINI_LEDInit(LED2);
    APM_MINI_LEDInit(LED3);

    /* USART configuration */
    USART_ConfigStructInit(&usartConfigStruct);
    usartConfigStruct.baudRate = 115200;
    usartConfigStruct.mode = USART_MODE_TX_RX;
    usartConfigStruct.parity = USART_PARITY_NONE;
    usartConfigStruct.stopBits = USART_STOP_BIT_1;
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;
    usartConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;

    /* COM1 init*/
    APM_MINI_COMInit(COM1, &usartConfigStruct);

    /* DAC init*/
    DAC_Init();

    /* ADC init*/
    ADC_Init();

    while (1)
    {
    }
}
/*!
 * @brief     DAC Init
 *
 * @param     None
 *
 * @retval    None
 */
void DAC_Init()
{
    GPIO_Config_T   gpioConfig;
    DAC_Config_T    dacConfig;

    /* Enable GPIOA clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    /* DAC out PA4 pin configuration */
    GPIO_ConfigStructInit(&gpioConfig);
    gpioConfig.mode    = GPIO_MODE_ANALOG;
    gpioConfig.pin     = GPIO_PIN_4;
    GPIO_Config(GPIOA, &gpioConfig);

    /* Enable DAC clock */
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_DAC);

    /* DAC channel 1 configuration */
    DAC_ConfigStructInit(&dacConfig);
    dacConfig.trigger             = DAC_TRIGGER_NONE;
    dacConfig.waveGeneration      = DAC_WAVE_GENERATION_NONE;
    dacConfig.maskAmplitudeSelect = DAC_LFSR_MASK_BIT11_1;
    dacConfig.outputBuffer        = DAC_OUTPUT_BUFFER_ENBALE;
    DAC_Config(DAC_CHANNEL_1, &dacConfig);

    /* Enable DAC channel 1 */
    DAC_Enable(DAC_CHANNEL_1);
}

/*!
 * @brief     ADC Init
 *
 * @param     None
 *
 * @retval    None
 */
void ADC_Init(void)
{
    GPIO_Config_T   gpioConfig;
    ADC_Config_T    adcConfig;

    /* Enable GPIOA clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    /* ADC channel 0 configuration */
    GPIO_ConfigStructInit(&gpioConfig);
    gpioConfig.mode    = GPIO_MODE_ANALOG;
    gpioConfig.speed     = GPIO_SPEED_50MHz;
    gpioConfig.pin     = GPIO_PIN_0;
    GPIO_Config(GPIOA, &gpioConfig);

    /* Enable ADC clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC1);

    /* ADC configuration */
    ADC_Reset(ADC1);
    ADC_ConfigStructInit(&adcConfig);
    adcConfig.continuosConvMode  = ENABLE;
    adcConfig.dataAlign          = ADC_DATA_ALIGN_RIGHT;
    adcConfig.externalTrigConv   = ADC_EXT_TRIG_CONV_None;
    adcConfig.scanConvMode       = DISABLE;
    ADC_Config(ADC1, &adcConfig);

    /* ADC channel 10 Convert configuration */
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_0, 1, ADC_SAMPLETIME_239CYCLES5);

    /* Enable complete conversion interupt */
    ADC_EnableInterrupt(ADC1, ADC_INT_EOC);

    /* NVIC configuration */
    NVIC_EnableIRQRequest(ADC1_2_IRQn, 1, 1);

    /* Enable ADC */
    ADC_Enable(ADC1);

    /* ADC start conversion */
    ADC_EnableSoftwareStartConv(ADC1);
}

/*!
 * @brief     ADC interrupt service routine
 *
 * @param     None
 *
 * @retval    None
 */
void ADC_Isr(void)
{
    uint16_t adcData = 0;
    uint16_t voltage = 0;

    /* generate analog signal*/
    DAC_ConfigChannel1Data(DAC_ALIGN_12BIT_R, dacData);

    if (dacData >= 0x0FFF)
    {
        dacData = 0;
    }

    dacData += 0x7F;

    if (ADC_ReadStatusFlag(ADC1, ADC_FLAG_EOC))
    {
        adcData = ADC_ReadConversionValue(ADC1);

        voltage = (adcData * 3300) / 4095;
        printf("\r\n voltage : %d mV\r\n", voltage);

        /* voltage greater than 2000mv */
        if (voltage > 2000)
        {
            APM_MINI_LEDOn(LED2);
            APM_MINI_LEDOff(LED3);
        }
        /* voltage less than 800mv */
        else if (voltage < 800)
        {
            APM_MINI_LEDOn(LED3);
            APM_MINI_LEDOff(LED2);
        }
        /* 800mv <= voltage <= 2000 */
        else
        {
            APM_MINI_LEDOn(LED2);
            APM_MINI_LEDOn(LED3);
        }

        Delay(0x2FFFFF);

        ADC_ClearStatusFlag(ADC1, ADC_FLAG_EOC);
    }
}

/*!
 * @brief     Delay
 *
 * @param     count:  delay count
 *
 * @retval    None
 */
void Delay(uint32_t count)
{
    volatile uint32_t delay = count;

    while (delay--);
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

/**@} end of group DAC_ADC_Functions */
/**@} end of group DAC_ADC */
/**@} end of group Examples */
