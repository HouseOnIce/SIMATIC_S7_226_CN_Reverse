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

/** @addtogroup ADC_DualRegulSimulMode
  @{
  */

/** @defgroup ADC_DualRegulSimulMode_Macros Macros
  @{
*/

/* printf function configs to USART1 */
#define DEBUG_USART  USART1

/* ADC1 data register address */
#define ADC_DR_ADDR         ((uint32_t)ADC1_BASE + 0x4C)

/**@} end of group ADC_DualRegulSimulMode_Macros*/

/** @defgroup ADC_DualRegulSimulMode_Variables Variables
  @{
*/

/* DMA value from ADC Dual Regular simultaneous mode */
__IO uint32_t DMA_DualConvertedValue[1] = {0};

/**@} end of group ADC_DualRegulSimulMode_Variables*/

/** @defgroup ADC_DualRegulSimulMode_Functions Functions
  @{
  */

void DMA_Init(void);
void ADC_Init(void);
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

    uint16_t temp0 = 0, temp1 = 0;

    /* ADC1 convert to volatage */
    float ADC1_CH0_ConvertedValue = 0;

    /* ADC2 convert to volatage */
    float ADC2_CH1_ConvertedValue = 0;

    USART_Config_T USART_ConfigStruct;

    /* USART config */
    USART_ConfigStruct.baudRate = 115200;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode = USART_MODE_TX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;

    APM_MINI_COMInit(COM1, &USART_ConfigStruct);

    /* DMA initialization */
    DMA_Init();

    /* ADC1 initialization */
    ADC_Init();

    while (1)
    {
        if (DMA_ReadStatusFlag(DMA1_FLAG_TC1) == SET)
        {
            /* ADC1 register data */
            temp0 = (DMA_DualConvertedValue[0] & 0XFFFF);

            /* ADC2 register data */
            temp1 = (DMA_DualConvertedValue[0] & 0XFFFF0000) >> 16;

            ADC1_CH0_ConvertedValue = (float) temp0 / 4095 * 3.3;
            ADC2_CH1_ConvertedValue = (float) temp1 / 4095 * 3.3;

            printf("\r\n");
            printf("ADC register data          = 0x%04X, 0x%04X\r\n", temp0, temp1);

            printf("ADC1 Channel0(PA0) Voltage = %.03f V \r\n", ADC1_CH0_ConvertedValue);
            printf("ADC2 Channel1(PA1) Voltage = %.03f V \r\n", ADC2_CH1_ConvertedValue);

            Delay(200);
            DMA_ClearStatusFlag(DMA1_FLAG_TC1);
        }
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
    dmaConfig.memoryBaseAddr = (uint32_t)DMA_DualConvertedValue;
    dmaConfig.dir = DMA_DIR_PERIPHERAL_SRC;
    dmaConfig.bufferSize = 1;
    dmaConfig.peripheralInc = DMA_PERIPHERAL_INC_DISABLE;
    dmaConfig.memoryInc = DMA_MEMORY_INC_ENABLE;
    dmaConfig.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_WORD;
    dmaConfig.memoryDataSize = DMA_MEMORY_DATA_SIZE_WORD;
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

    /* Configure PA0, PA1 (ADC Channel0, ADC Channel1) as analog input */
    GPIO_ConfigStructInit(&gpioConfig);
    gpioConfig.mode    = GPIO_MODE_ANALOG;
    gpioConfig.pin     = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_Config(GPIOA, &gpioConfig);

    /* ADCCLK = PCLK2/4 */
    RCM_ConfigADCCLK(RCM_PCLK2_DIV_4);

    /* Enable ADC clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC1 | RCM_APB2_PERIPH_ADC2);

    ADC_ConfigStructInit(&adcConfig);
    adcConfig.mode                  = ADC_MODE_REG_SIMULT;
    adcConfig.scanConvMode          = ENABLE;
    adcConfig.continuosConvMode     = ENABLE;
    adcConfig.externalTrigConv      = ADC_EXT_TRIG_CONV_None;
    adcConfig.dataAlign             = ADC_DATA_ALIGN_RIGHT;
    adcConfig.nbrOfChannel          = 1;

    /* ADC1 configuration */
    ADC_Reset(ADC1);
    ADC_Config(ADC1, &adcConfig);
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_0, 1, ADC_SAMPLETIME_13CYCLES5);

    /* Enable ADC DMA */
    ADC_EnableDMA(ADC1);

    /* ADC2 configuration */
    ADC_Reset(ADC2);
    ADC_Config(ADC2, &adcConfig);
    ADC_ConfigRegularChannel(ADC2, ADC_CHANNEL_1, 1, ADC_SAMPLETIME_13CYCLES5);

    /* Enable ADC1 */
    ADC_Enable(ADC1);

    /* Enable ADC1 reset calibration register */
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while (ADC_ReadResetCalibrationStatus(ADC1));

    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while (ADC_ReadCalibrationStartFlag(ADC1));

    /* Enable ADC2 */
    ADC_Enable(ADC2);

    /* Enable ADC2 reset calibration register */
    ADC_ResetCalibration(ADC2);
    /* Check the end of ADC2 reset calibration register */
    while (ADC_ReadResetCalibrationStatus(ADC2));

    /* Start ADC2 calibration */
    ADC_StartCalibration(ADC2);
    /* Check the end of ADC2 calibration */
    while (ADC_ReadCalibrationStartFlag(ADC2));

    /* Start ADC2 Software Conversion */
    ADC_EnableSoftwareStartConv(ADC2);

    /* Start ADC1 Software Conversion */
    ADC_EnableSoftwareStartConv(ADC1);
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

/**@} end of group ADC_DualRegulSimulMode_Functions */
/**@} end of group ADC_DualRegulSimulMode */
/**@} end of group Examples */
