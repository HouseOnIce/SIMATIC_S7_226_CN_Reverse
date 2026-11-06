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

/* printf function configures to USART1 */
#define DEBUG_USART  USART1

/** @addtogroup Examples
  @{
*/

/** @addtogroup USART_IrDA
  @{
*/

/** @defgroup USART_IrDA_Variables Variables
  @{
*/

volatile uint32_t tick = 0;

uint8_t USART2ReceivedData = 0x00;
uint8_t USART2SendData = 0x00;

uint8_t USART3ReceivedData = 0x00;
uint8_t USART3SendData = 0xFF;

/**@} end of group USART_IrDA_Variables */

/** @defgroup USART_IrDA_Functions Functions
  @{
*/

void IrDAInit(void);

void IrDA_USART2_TxData(uint8_t data);
uint8_t IrDA_USART2_RxData(void);

void IrDA_USART3_TxData(uint8_t data);
uint8_t IrDA_USART3_RxData(void);

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

    APM_MINI_LEDInit(LED2);

    /* COM1(USART1) configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */

    USART_ConfigStruct.baudRate = 115200;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode = USART_MODE_TX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;

    APM_MINI_COMInit(COM1, &USART_ConfigStruct);

    IrDAInit();

    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);

    printf("\r\nThis example provides a basic communication USARTx IrDA receive and transmit mode.\r\n");
    printf(">> SystemCoreClock: %ld\r\n", SystemCoreClock);

    while (1)
    {
        Delay();

        APM_MINI_LEDToggle(LED2);

        IrDA_USART2_TxData(USART2SendData);
        USART3ReceivedData = IrDA_USART3_RxData();
        printf(">> USART2 Send Data : 0x%02X ,USART3 Received Data : 0x%02X. \r\n", USART2SendData, USART3ReceivedData);

        Delay();

        IrDA_USART3_TxData(USART3SendData);
        USART2ReceivedData = IrDA_USART2_RxData();
        printf(">> USART3 Send Data : 0x%02X ,USART2 Received Data : 0x%02X. \r\n", USART3SendData, USART2ReceivedData);

        USART2SendData++;
        USART3SendData--;
    }
}

/*!
 * @brief       USART2 and USART3 IrDA mode initializes functions
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void IrDAInit(void)
{
    GPIO_Config_T GPIO_ConfigStruct;
    USART_Config_T USART_ConfigStruct;

    /* System Clocks Configuration */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_GPIOB | RCM_APB2_PERIPH_AFIO);
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_USART2);
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_USART3);

    /* Configure the GPIO ports */
    /* USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_ConfigStruct.pin = GPIO_PIN_2;
    GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);

    GPIO_ConfigStruct.pin = GPIO_PIN_3;
    GPIO_ConfigStruct.mode = GPIO_MODE_IN_FLOATING;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);

    /* USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_ConfigStruct.pin = GPIO_PIN_10;
    GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOB, &GPIO_ConfigStruct);

    GPIO_ConfigStruct.pin = GPIO_PIN_11;
    GPIO_ConfigStruct.mode = GPIO_MODE_IN_FLOATING;
    GPIO_Config(GPIOB, &GPIO_ConfigStruct);

    /* USARTx configuration */
    /* USARTx configured as follow:
        - BaudRate = 9600 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */
    USART_ConfigStruct.baudRate = 9600;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode = USART_MODE_TX_RX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;

    /* Configure the USARTx */
    USART_Config(USART2, &USART_ConfigStruct);
    USART_Config(USART3, &USART_ConfigStruct);

    /* Enable the USARTx */
    USART_Enable(USART2);
    USART_Enable(USART3);

    /* Set the USARTx prescaler */
    USART_ConfigPrescaler(USART2, 0x01);
    USART_ConfigPrescaler(USART3, 0x01);

    /* Configure the USARTx IrDA mode */
    USART_ConfigIrDA(USART2, USART_IRDALP_NORMAL);
    USART_ConfigIrDA(USART3, USART_IRDALP_NORMAL);

    /* Enable the USARTx IrDA mode */
    USART_EnableIrDA(USART2);
    USART_EnableIrDA(USART3);
}

/*!
 * @brief       USART2 Sends data function in IrDA mode
 *
 * @param       data : The sent data.
 *
 * @retval      None
 *
 * @note
 */
void IrDA_USART2_TxData(uint8_t data)
{
    USART_TxData(USART2, data);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(USART2, USART_FLAG_TXC) == RESET);
}

/*!
 * @brief       USART2 Receive data function in IrDA mode.
 *
 * @param       None
 *
 * @retval      The received data.
 *
 * @note
 */
uint8_t IrDA_USART2_RxData(void)
{
    while (USART_ReadStatusFlag(USART2, USART_FLAG_RXBNE) == RESET);

    return (USART_RxData(USART2));
}

/*!
 * @brief       USART3 Sends data function in IrDA mode.
 *
 * @param       data : The sent data.
 *
 * @retval      None
 *
 * @note
 */
void IrDA_USART3_TxData(uint8_t data)
{
    USART_TxData(USART3, data);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(USART3, USART_FLAG_TXC) == RESET);
}

/*!
 * @brief       USART3 Receive data function in IrDA mode.
 *
 * @param       None
 *
 * @retval      The received data.
 *
 * @note
 */
uint8_t IrDA_USART3_RxData(void)
{
    while (USART_ReadStatusFlag(USART3, USART_FLAG_RXBNE) == RESET);

    return (USART_RxData(USART3));
}

/*!
 * @brief       Delay
 *
 * @param       None
 *
 * @retval      None
 *
 */
void Delay(void)
{
    tick = 0;

    while (tick < 1000);
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

/**@} end of group USART_IrDA_INT_Functions */
/**@} end of group USART_IrDA */
/**@} end of group Examples */
