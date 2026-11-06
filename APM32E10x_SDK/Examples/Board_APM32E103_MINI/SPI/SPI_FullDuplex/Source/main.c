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

/** @addtogroup SPI_FullDuplex
  @{
  */

/** @defgroup SPI_FullDuplex_Macros Macros
  @{
  */

/* Buffer Size*/
#define DATA_BUF_SIZE       (30)

/**@} end of group SPI_FullDuplex_Macros*/

/** @defgroup SPI_FullDuplex_Enumerations Enumerations
  @{
  */

enum
{
    FALSE,
    TRUE
};

/**@} end of group SPI_FullDuplex_Enumerations*/

/** @defgroup SPI_FullDuplex_Functions Functions
  @{
  */

/* FullDuplex Init*/
void SPI_FullDuplex_Init(void);
void USART_FullDuplex_Init(void);
/* Delay */
void Delay(uint32_t count);
/* SPI Write uint8_t dat*/
void SPI_WriteByte(uint8_t dat);
/* compare Buffer */
uint8_t BufferCompare(uint8_t* buf1, uint8_t* buf2, uint8_t size);

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
    /* Buffer Compare state */
    uint8_t state = FALSE;
    /* SPI receive Buf*/
    uint8_t rxDataBufSPI[DATA_BUF_SIZE]   = {0x00};
    /* USART receive Buf*/
    uint8_t rxDataBufUSART[DATA_BUF_SIZE] = {0x00};

    SystemClockConfig();

    APM_MINI_LEDInit(LED2);
    APM_MINI_LEDInit(LED3);
    APM_MINI_LEDOff(LED2);
    APM_MINI_LEDOff(LED3);

    USART_FullDuplex_Init();
    SPI_FullDuplex_Init();

    Delay(0x5000);

    for (int i = 0; i < DATA_BUF_SIZE; i++)
    {
        /* Tx send data*/
        USART_TxData(USART1, i);

        /* SPI send data*/
        SPI_WriteByte(i);

        /* Wait data is received by SPI */
        while (SPI_I2S_ReadStatusFlag(SPI1, SPI_FLAG_RXBNE) == RESET);

        rxDataBufSPI[i] = SPI_I2S_RxData(SPI1);

        /* Wait data is received by UART1 */
        while (USART_ReadStatusFlag(USART1, USART_FLAG_RXBNE) == RESET);

        rxDataBufUSART[i] = USART_RxData(USART1);
    }

    /* Compare receive Buffer*/
    state = BufferCompare(rxDataBufUSART, rxDataBufSPI, DATA_BUF_SIZE);

    /* Data is ok then turn on LED2 */
    if (state == 1)
    {
        APM_MINI_LEDOn(LED2);
    }

    while (1)
    {
        APM_MINI_LEDToggle(LED3);
        Delay(0x7ffff);
    }
}

/*!
 * @brief       SPI Write uint8_t data
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void SPI_WriteByte(uint8_t dat)
{

    while (SPI_I2S_ReadStatusFlag(SPI1, SPI_FLAG_TXBE) == RESET);

    SPI_I2S_TxData(SPI1, dat);

}

/*!
 * @brief       SPI Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void SPI_FullDuplex_Init()
{
    GPIO_Config_T gpioConfig;
    SPI_Config_T spiConfig;

    /* Enable related Clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SPI1);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    /* config PIN_6  MISO*/
    gpioConfig.pin =  GPIO_PIN_6 ;
    gpioConfig.mode = GPIO_MODE_AF_PP;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &gpioConfig);

    /* config PIN_5->SCK , PIN_7->MOSI */
    gpioConfig.pin = GPIO_PIN_5 | GPIO_PIN_7;
    gpioConfig.mode = GPIO_MODE_IN_PU;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &gpioConfig);

    SPI_ConfigStructInit(&spiConfig);

    spiConfig.length = SPI_DATA_LENGTH_8B;

    spiConfig.baudrateDiv = SPI_BAUDRATE_DIV_32;
    /*  2 line full duplex  */
    spiConfig.direction = SPI_DIRECTION_2LINES_FULLDUPLEX;
    /*  LSB first  */
    spiConfig.firstBit = SPI_FIRSTBIT_LSB;
    /*  Slave mode  */
    spiConfig.mode = SPI_MODE_SLAVE;
    /*  Polarity is low  */
    spiConfig.polarity = SPI_CLKPOL_LOW;
    /*  Software select slave enable  */
    spiConfig.nss = SPI_NSS_SOFT;
    /*  Phase is 1 edge  */
    spiConfig.phase = SPI_CLKPHA_1EDGE;
    /*  SPI config  */
    SPI_Config(SPI1, &spiConfig);

    SPI_ConfigDataSize(SPI1, SPI_DATA_LENGTH_8B);

    SPI_Enable(SPI1);
}

/*!
 * @brief       USART Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void USART_FullDuplex_Init()
{
    GPIO_Config_T gpioConfig;
    USART_Config_T usartConfig;
    USART_ClockConfig_T usartSyncClockConfig;

    /* Enable GPIO clock */
    RCM_EnableAPB2PeriphClock(MINI_COM1_TX_GPIO_CLK);

    /* Enable COM1 or COM2 clock */
    RCM_EnableAPB2PeriphClock(MINI_COM1_CLK);

    /* Configure USART Tx as alternate function push-pull */
    gpioConfig.mode = GPIO_MODE_AF_PP;
    gpioConfig.pin = MINI_COM1_TX_PIN;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    GPIO_Config(MINI_COM1_TX_GPIO_PORT, &gpioConfig);

    /* Configure USART Rx as input floating */
    gpioConfig.mode = GPIO_MODE_IN_PU;
    gpioConfig.pin  = MINI_COM1_RX_PIN;
    GPIO_Config(MINI_COM1_RX_GPIO_PORT, &gpioConfig);

    /* Configure USART CLK  */
    gpioConfig.mode = GPIO_MODE_AF_PP;
    gpioConfig.pin  = GPIO_PIN_8;
    GPIO_Config(GPIOA, &gpioConfig);

    /*  Enable USART Clock  */
    usartSyncClockConfig.clock = USART_CLKEN_ENABLE;
    /*  Enable last bit clock  */
    usartSyncClockConfig.lastBit = USART_LBCP_ENABLE;
    /*  Clock phase is 1 edge */
    usartSyncClockConfig.phase = USART_CLKPHA_1EDGE;
    /*  Clock polarity is Low */
    usartSyncClockConfig.polarity = USART_CLKPOL_LOW;
    /*  Synchronous communication clock configuration */
    USART_ConfigClock(USART1, &usartSyncClockConfig);

    /*  BaudRate is 115200 */
    usartConfig.baudRate = 115200;
    /*  Enable RX and TX */
    usartConfig.mode = USART_MODE_TX_RX;
    /*  Parity disable */
    usartConfig.parity = USART_PARITY_NONE;
    /*  One stop bit */
    usartConfig.stopBits = USART_STOP_BIT_1;
    /*  Word length is 8bit */
    usartConfig.wordLength = USART_WORD_LEN_8B;

    usartConfig.hardwareFlow = USART_HARDWARE_FLOW_NONE;

    /*  USART1 configuration */
    USART_Config(USART1, &usartConfig);

    /*  Enable USART1 */
    USART_Enable(USART1);
}

/*!
 * @brief       Delay
 *
 * @param       count:  delay count
 *
 * @retval      None
 *
 * @note
 */
void Delay(uint32_t count)
{
    volatile uint32_t delay = count;

    while (delay--);
}

/*!
 * @brief       Compares two buffers
 *
 * @param       buf1:    First buffer to be compared
 *
 * @param       buf1:    Second buffer to be compared
 *
 * @param       size:    Buffer size
 *
 * @retval      Return TRUE if buf1 = buf2. If not then return FALSE
 *
 * @note
 */
uint8_t BufferCompare(uint8_t* buf1, uint8_t* buf2, uint8_t size)
{
    uint8_t i;

    for (i = 0; i < size; i++)
    {
        if (buf1[i] != buf2[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

/**@} end of group SPI_FullDuplex_Functions */
/**@} end of group SPI_FullDuplex */
/**@} end of group Examples */
