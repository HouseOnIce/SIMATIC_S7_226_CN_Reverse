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

/** @addtogroup DMA_ADCMultiChannel
  @{
  */

/** @defgroup DMA_ADCMultiChannel_Macros Macros
  @{
*/

/* printf function configs to USART1*/
#define DEBUG_USART  USART1

#define DMA_BUFF_SIZE  4
#define ADC_CHANNELS  DMA_BUFF_SIZE

/* If the current macro is not enabled, the TMR2_CC2 event will be triggered.*/
//#define ADC_EXT_TRIG_CONV_TMR3_TRGO_ENABLE 

/**@} end of group DMA_ADCMultiChannel_Macros*/

/** @defgroup DMA_ADCMultiChannel_Variables Variables
  @{
*/
/* DMA value from ADC*/
uint16_t DMA_ConvertedValue[DMA_BUFF_SIZE];
/* ADC convert to volatage*/
float ADC_ConvertedValue = 0;

uint32_t systick = 0;
/**@} end of group DMA_ADCMultiChannel_Variables */

/** @defgroup DMA_ADCMultiChannel_Functions Functions
  @{
*/

/* Delay */
void Delay(uint32_t count);
/* DMA init */
void DMA_Init(void);
/* ADC init */
void ADC_Init(void);
/* TIMER init */
void TIMER_Init(void);

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
    APM_MINI_LEDOn(LED2);
    APM_MINI_LEDInit(LED3);
    APM_MINI_LEDOff(LED3);

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

    /* DMA init*/
    DMA_Init();

    /* ADC init*/
    ADC_Init();

    /* Timer init*/
    TIMER_Init();

    /* Configure the SysTick to generate a time base equal to 1 ms */
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        while (1);
    }
    while (1)
    {
        if (DMA_ReadStatusFlag(DMA1_FLAG_TC1) == SET)
        {
            for (uint32_t i = 0; i < DMA_BUFF_SIZE; i++)
            {
                ADC_ConvertedValue = 3.3 * DMA_ConvertedValue[i] / 4096;
                printf("\r\n");
                printf("ADC REGDATA[%ld] = 0x%04X \r\n", i, DMA_ConvertedValue[i]);
                printf("Volatage    = %f V \r\n", ADC_ConvertedValue);
            }
            printf("*************************************************** \r\n");
            DMA_ClearStatusFlag(DMA1_FLAG_TC1);
        }

        Delay(1000);
        APM_MINI_LEDToggle(LED2);
        APM_MINI_LEDToggle(LED3);
    }
}
/*!
 * @brief     DMA Init
 *
 * @param     Buf:   memory addr
 *
 * @retval    None
 */
void DMA_Init(void)
{
    DMA_Config_T dmaconfig;

    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA1);

    dmaconfig.bufferSize         = DMA_BUFF_SIZE;
    dmaconfig.dir                = DMA_DIR_PERIPHERAL_SRC;
    dmaconfig.loopMode           = DMA_MODE_CIRCULAR;
    dmaconfig.memoryBaseAddr     = (uint32_t) & (DMA_ConvertedValue[0]);
    dmaconfig.memoryDataSize     = DMA_MEMORY_DATA_SIZE_HALFWORD;
    dmaconfig.peripheralBaseAddr = (uint32_t) & (ADC1->REGDATA);
    dmaconfig.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_HALFWORD;
    dmaconfig.memoryInc          = DMA_MEMORY_INC_ENABLE;
    dmaconfig.peripheralInc      = DMA_PERIPHERAL_INC_DISABLE;
    dmaconfig.priority           = DMA_PRIORITY_HIGH;
    dmaconfig.M2M                = DMA_M2MEN_DISABLE;

    DMA_Config(DMA1_Channel1, &dmaconfig);

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
    GPIO_Config_T   gpioConfig;
    ADC_Config_T    adcConfig;

    /* Enable GPIOA clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    /* ADC channel 0-3 configuration */
    GPIO_ConfigStructInit(&gpioConfig);
    gpioConfig.mode    = GPIO_MODE_ANALOG;
    gpioConfig.speed     = GPIO_SPEED_50MHz;
    gpioConfig.pin     = GPIO_PIN_0;
    GPIO_Config(GPIOA, &gpioConfig);

    gpioConfig.pin     = GPIO_PIN_1;
    GPIO_Config(GPIOA, &gpioConfig);

    gpioConfig.pin     = GPIO_PIN_2;
    GPIO_Config(GPIOA, &gpioConfig);

    gpioConfig.pin     = GPIO_PIN_3;
    GPIO_Config(GPIOA, &gpioConfig);

    /* Enable ADC clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC1);
    RCM_ConfigADCCLK(RCM_PCLK2_DIV_6);

    /* ADC configuration */
    ADC_Reset(ADC1);

    adcConfig.mode = ADC_MODE_INDEPENDENT;
    adcConfig.scanConvMode = ENABLE;
    adcConfig.continuosConvMode = DISABLE;

#if defined (ADC_EXT_TRIG_CONV_TMR3_TRGO_ENABLE)
    adcConfig.externalTrigConv = ADC_EXT_TRIG_CONV_TMR3_TRGO;
#else
    adcConfig.externalTrigConv = ADC_EXT_TRIG_CONV_TMR2_CC2;
#endif

    adcConfig.dataAlign = ADC_DATA_ALIGN_RIGHT;
    adcConfig.nbrOfChannel = ADC_CHANNELS;
    ADC_Config(ADC1, &adcConfig);

    /* ADC channel 0-3 Convert configuration */
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_0, 1, ADC_SAMPLETIME_239CYCLES5);
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_1, 2, ADC_SAMPLETIME_239CYCLES5);
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_2, 3, ADC_SAMPLETIME_239CYCLES5);
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_3, 4, ADC_SAMPLETIME_239CYCLES5);

    ADC_EnableDMA(ADC1);

    /* Enable ADC */
    ADC_Enable(ADC1);

    ADC_ResetCalibration(ADC1);
    while (ADC_ReadResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_ReadCalibrationStartFlag(ADC1));

    /* ADC start conversion */
    ADC_EnableSoftwareStartConv(ADC1);

}


void TIMER_Init(void)
{
#if defined (ADC_EXT_TRIG_CONV_TMR3_TRGO_ENABLE)
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR3);
    TMR_BaseConfig_T baseConfig;

    baseConfig.period = 1000-1;
    baseConfig.division = 72-1;
    baseConfig.clockDivision = TMR_CLOCK_DIV_1;
    baseConfig.countMode = TMR_COUNTER_MODE_UP;
    baseConfig.repetitionCounter = 0xFF;
    TMR_ConfigTimeBase(TMR3, &baseConfig);

    TMR_Enable(TMR3);

    TMR_SelectOutputTrigger(TMR3, TMR_TRGO_SOURCE_UPDATE);
#else
    TMR_BaseConfig_T TMR_BaseConfigStruct;
    TMR_OCConfig_T OCcongigStruct;

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2);

    TMR_BaseConfigStruct.clockDivision = TMR_CLOCK_DIV_1;
    TMR_BaseConfigStruct.countMode = TMR_COUNTER_MODE_UP;
    TMR_BaseConfigStruct.division = 719;
    TMR_BaseConfigStruct.period = (10000);
    TMR_BaseConfigStruct.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR2, &TMR_BaseConfigStruct);

    OCcongigStruct.mode = TMR_OC_MODE_PWM1;
    OCcongigStruct.outputState = TMR_OC_STATE_ENABLE;
    OCcongigStruct.polarity = TMR_OC_POLARITY_LOW;
    OCcongigStruct.pulse = 5000;
    TMR_ConfigOC2(TMR2, &OCcongigStruct);

    TMR_Enable(TMR2);
    TMR_ConfigInternalClock(TMR2);
    TMR_ConfigOC2Preload(TMR2, TMR_OC_PRELOAD_ENABLE);
    TMR_DisableUpdate(TMR2);
#endif

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
    systick = count;

    while (systick);
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
int fputc(int ch, FILE *f)
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
int _write(int file, char *ptr, int len)
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

/**@} end of group DMA_ADCMultiChannel_Functions */
/**@} end of group DMA_ADCMultiChannel */
/**@} end of group Examples */
