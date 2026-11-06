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
 *  Copyright (C) 2025 Geehy Semiconductor
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

/** @addtogroup ADC_InjectionRegularSampling
  @{
  */

/** @defgroup ADC_InjectionRegularSampling_Macros Macros
  @{
*/

/* printf function configs to USART1 */
#define DEBUG_USART  USART1

/**@} end of group ADC_InjectionRegularSampling_Macros*/

/** @defgroup ADC_InjectionRegularSampling_Functions Functions
  @{
  */

void ADC_Init(void);
uint16_t GetInjectionConversion(void);
uint16_t GetRegularConversion(void);

uint16_t injectedValue1, injectedValue2;
uint16_t regularValue1, regularValue2, regularValue3, regularValue4, regularValue5;

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
    USART_ConfigStruct.mode = USART_MODE_TX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;

    APM_MINI_COMInit(COM1, &USART_ConfigStruct);

    /* ADC1 initialization */
    ADC_Init();

    while (1)
    {
        /* Read the value of the injection channel*/
        injectedValue1 = GetInjectionConversion(); // PC0
        injectedValue2 = GetInjectionConversion(); // PC1

        /* Read the value of the regular channel*/
        regularValue1 = GetRegularConversion(); // PA0
        regularValue2 = GetRegularConversion(); // PA4
        regularValue3 = GetRegularConversion(); // PC3
        regularValue4 = GetRegularConversion(); // PC4
        regularValue5 = GetRegularConversion(); // PC5

         /* Print the result to the serial port. */
        printf("Injection Values: VBUS_CUR = %d, VBAT_VOL = %d\r\n", injectedValue1, injectedValue2);
        printf("Regular Values: V_VOL = %d, W_VOL = %d, U_VOL = %d, U_CUR = %d, V_CUR = %d\r\n\r\n",
                regularValue1, regularValue2, regularValue3, regularValue4, regularValue5);
        /* Delay */
        for (volatile int i = 0; i < 100000; i++);
    }
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
    GPIO_Config_T           gpioConfig;
    ADC_Config_T            adcConfig;

    /* Enable GPIOA clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_GPIOC);

    /* Configure PC0 and PC1 as ADC Injection Channels */
    GPIO_ConfigStructInit(&gpioConfig);
    gpioConfig.mode    = GPIO_MODE_ANALOG;
    gpioConfig.pin     = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_Config(GPIOC, &gpioConfig);

    /* Configure PA0 and PA4 as regular channels. */
    gpioConfig.pin     = GPIO_PIN_0 | GPIO_PIN_4;
    GPIO_Config(GPIOA, &gpioConfig);

    /* Configure PC3, PC4 and PC5 as regular channels. */
    gpioConfig.pin     = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_3;
    GPIO_Config(GPIOC, &gpioConfig);

    /* ADCCLK = PCLK2/4 */
    RCM_ConfigADCCLK(RCM_PCLK2_DIV_4);

    /* Enable ADC clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC1);

    /* ADC configuration */
    ADC_Reset(ADC1);
    ADC_ConfigStructInit(&adcConfig);
    adcConfig.mode                  = ADC_MODE_INDEPENDENT;
    adcConfig.scanConvMode          = DISABLE;
    adcConfig.continuosConvMode     = DISABLE;
    adcConfig.externalTrigConv      = ADC_EXT_TRIG_CONV_None;
    adcConfig.dataAlign             = ADC_DATA_ALIGN_RIGHT;
    /* channel number */
    adcConfig.nbrOfChannel          = 1;
    ADC_Config(ADC1, &adcConfig);

    /* Configure Injection Channel Length*/
    ADC_ConfigInjectedSequencerLength(ADC1, 2); // 2 injection channels
    ADC_ConfigExternalTrigInjectedConv(ADC1, ADC_EXT_TRIG_INJEC_CONV_NONE); // Software Trigger

    /* Configure Injection Channel */
    ADC_ConfigInjectedChannel(ADC1, ADC_CHANNEL_10, 1, ADC_SAMPLETIME_55CYCLES5);// PC0
    ADC_ConfigInjectedChannel(ADC1, ADC_CHANNEL_11, 2, ADC_SAMPLETIME_55CYCLES5);// PC1

    /* ADC channel Regular configuration */
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_4, 1, ADC_SAMPLETIME_55CYCLES5); // PA0
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_5, 2, ADC_SAMPLETIME_55CYCLES5); // PA4
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_13, 3, ADC_SAMPLETIME_55CYCLES5);// PC3
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_14, 4, ADC_SAMPLETIME_55CYCLES5);// PC4
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_15, 5, ADC_SAMPLETIME_55CYCLES5);// PC5

    /* Enable ADC */
    ADC_Enable(ADC1);

    /* Enable ADC1 reset calibration register */
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_ReadResetCalibrationStatus(ADC1));

    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while(ADC_ReadCalibrationStartFlag(ADC1));
}

/*!
 * @brief     Get the value of the ADC injection channel
 *
 * @param     None
 *
 * @retval    ADC Injection Channel Acquisition Value
 */
uint16_t GetInjectionConversion(void)
{
    /* Start injecting conversion */
    ADC_EnableSoftwareStartInjectedConv(ADC1);

    /* Waiting for the conversion to complete. */
    while (!ADC_ReadStatusFlag(ADC1, ADC_FLAG_INJEOC));

    /* Clear the flag and return the result */
    return ADC_ReadInjectedConversionValue(ADC1, ADC_INJEC_CHANNEL_1);
}

/*!
 * @brief     Get the value of the ADC regular channel
 *
 * @param     None
 *
 * @retval    ADC Injection Channel Acquisition Value
 */
uint16_t GetRegularConversion(void)
{
    /* Start injecting conversion */
    ADC_EnableSoftwareStartConv(ADC1);

    /* Waiting for the conversion to complete. */
    while (!ADC_ReadStatusFlag(ADC1, ADC_FLAG_EOC));

    /* Clear the flag and return the result */
    return ADC_ReadConversionValue(ADC1);
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

/**@} end of group ADC_InjectionRegularSampling_Functions */
/**@} end of group ADC_InjectionRegularSampling */
/**@} end of group Examples */
