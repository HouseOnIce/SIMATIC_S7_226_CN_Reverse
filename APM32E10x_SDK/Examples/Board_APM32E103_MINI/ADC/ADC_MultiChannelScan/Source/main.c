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

/** @addtogroup ADC_MultiChannelScan
  @{
  */

/** @defgroup ADC_MultiChannelScan_Macros Macros
  @{
*/

/* printf function configs to USART1 */
#define DEBUG_USART  USART1

/* ADC channel number */
#define ADC_CH_SIZE         3

/* ADC1 data register address */
#define ADC_DR_ADDR         ((uint32_t)ADC1_BASE + 0x4C)

/**@} end of group ADC_MultiChannelScan_Macros*/

/** @defgroup ADC_MultiChannelScan_Variables Variables
  @{
*/

/* save adc data */
uint16_t adcData[ADC_CH_SIZE];

/**@} end of group ADC_MultiChannelScan_Variables*/

/** @defgroup ADC_MultiChannelScan_Functions Functions
  @{
  */

void ADC_Init(void);
void ADC_MultiChannelPolling(void);
void Delay(uint32_t count);

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
        ADC_MultiChannelPolling();
        Delay(1000);
    }
}

/*!
 * @brief     DMA Init
 *
 * @param     None
 *
 * @retval    None
 */
void DMA_Init(void)
{
    DMA_Config_T dmaConfig;

    /* Enable DMA Clock */
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA1);

    /* DMA config */
    dmaConfig.peripheralBaseAddr = ADC_DR_ADDR;
    dmaConfig.memoryBaseAddr = (uint32_t)&adcData;
    dmaConfig.dir = DMA_DIR_PERIPHERAL_SRC;
    dmaConfig.bufferSize = ADC_CH_SIZE;
    dmaConfig.peripheralInc = DMA_PERIPHERAL_INC_DISABLE;
    dmaConfig.memoryInc = DMA_MEMORY_INC_ENABLE;
    dmaConfig.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_HALFWORD;
    dmaConfig.memoryDataSize = DMA_MEMORY_DATA_SIZE_HALFWORD;
    dmaConfig.loopMode = DMA_MODE_CIRCULAR;
    dmaConfig.priority = DMA_PRIORITY_HIGH;
    dmaConfig.M2M = DMA_M2MEN_DISABLE;

    /* Enable DMA channel */
    DMA_Config(DMA1_Channel1, &dmaConfig);

    /* Enable DMA */
    DMA_Enable(DMA1_Channel1);
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
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    /* ADC channel 0 configuration */
    GPIO_ConfigStructInit(&gpioConfig);
    gpioConfig.mode    = GPIO_MODE_ANALOG;
    gpioConfig.pin     = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_Config(GPIOA, &gpioConfig);

    /* Enable ADC clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC1);

    /* ADC configuration */
    ADC_Reset(ADC1);

    ADC_ConfigStructInit(&adcConfig);
    adcConfig.mode                  = ADC_MODE_INDEPENDENT;
    adcConfig.scanConvMode          = ENABLE;
    adcConfig.continuosConvMode     = ENABLE;
    adcConfig.externalTrigConv      = ADC_EXT_TRIG_CONV_None;
    adcConfig.dataAlign             = ADC_DATA_ALIGN_RIGHT;
    /* channel number */
    adcConfig.nbrOfChannel          = ADC_CH_SIZE;
    ADC_Config(ADC1, &adcConfig);

    /* ADCCLK = PCLK2/4 */
    RCM_ConfigADCCLK(RCM_PCLK2_DIV_4);

    /* ADC channel Convert configuration */
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_0, 1, ADC_SAMPLETIME_13CYCLES5);
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_1, 2, ADC_SAMPLETIME_13CYCLES5);
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_2, 3, ADC_SAMPLETIME_13CYCLES5);

    /* Config DMA */
    DMA_Init();

    /* Enable ADC DMA */
    ADC_EnableDMA(ADC1);

    /* Enable ADC */
    ADC_Enable(ADC1);

    /* Enable ADC1 reset calibration register */
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while (ADC_ReadResetCalibrationStatus(ADC1));

    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while (ADC_ReadCalibrationStartFlag(ADC1));

    /* Start ADC1 Software Conversion */
    ADC_EnableSoftwareStartConv(ADC1);
}

/*!
 * @brief     ADC multi channel polling
 *
 * @param     None
 *
 * @retval    None
 */
void ADC_MultiChannelPolling(void)
{
    float voltage;
    uint8_t index;

    for (index = 0; index < ADC_CH_SIZE; index++)
    {
        voltage = (adcData[index] * 3300.0) / 4095.0;

        printf("ADC CH[%d] voltage = %.03f mV\r\n", index, voltage);
    }
    printf("\r\n");
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
    uint16_t i = 0;

    while (count--)
    {
        i = 7995;
        while (i--);
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

/**@} end of group ADC_MultiChannelScan_Functions */
/**@} end of group ADC_MultiChannelScan */
/**@} end of group Examples */
