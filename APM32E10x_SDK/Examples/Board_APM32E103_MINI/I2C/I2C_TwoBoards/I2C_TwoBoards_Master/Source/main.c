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

/** @addtogroup I2C_TwoBoards_Master
  @{
  */

/** @defgroup I2C_TwoBoards_Master_Macros Macros
  @{
*/

/* printf function configs to USART1*/
#define DEBUG_USART  USART1

/**@} end of group I2C_TwoBoards_Master_Macros*/

/** @defgroup I2C_TwoBoards_Master_Enumerations Enumerations
  @{
  */

enum
{
    FAILED,
    PASSED
};

/**@} end of group I2C_TwoBoards_Master_Enumerations*/

/** @defgroup I2C_TwoBoards_Master_Macros Macros
  @{
*/

#define TEMPERATURE_THYS    31
#define TEMPERATURE_TOS     32

/* Waiting TIMEOUT */
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/**@} end of group I2C_TwoBoards_Master_Macros*/

/** @defgroup I2C_TwoBoards_Master_Functions Functions
  @{
  */

void Delay(void);
void USART_Init(void);
void USART_Write(uint8_t* dat, uint8_t len);

/* I2C Init */
void I2CInit(void);
/* I2C Write */
uint8_t I2CWrite(char* pBuffer);


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

    APM_MINI_PBInit(BUTTON_KEY1, BUTTON_MODE_EINT);
    APM_MINI_LEDInit(LED2);
    APM_MINI_LEDInit(LED3);
    I2CInit();
    USART_Init();

    while (1)
    {
        APM_MINI_LEDToggle(LED3);
        Delay();
    }
}

/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 */
void Delay(void)
{
    volatile uint32_t delay = 0xffff3;

    while (delay--);
}

/*!
 * @brief       USART init
 *
 * @param       None
 *
 * @retval      None
 *
 */
void USART_Init(void)
{
    GPIO_Config_T GPIO_ConfigStruct;
    USART_Config_T USART_ConfigStruct;

    RCM_EnableAPB2PeriphClock((RCM_APB2_PERIPH_T)(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_USART1));

    GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
    GPIO_ConfigStruct.pin = GPIO_PIN_9;
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);

    GPIO_ConfigStruct.mode = GPIO_MODE_IN_PU;
    GPIO_ConfigStruct.pin = GPIO_PIN_10;
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);

    USART_ConfigStruct.baudRate = 115200;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode = USART_MODE_TX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;
    USART_Config(USART1, &USART_ConfigStruct);

    USART_Enable(USART1);
}

/*!
 * @brief       USART write
 *
 * @param       dat:    data
 *
 * @param       len:    Data length
 *
 * @retval      None
 *
 */
void USART_Write(uint8_t* dat, uint8_t len)
{
    uint8_t i;

    for (i = 0; i < len; i++)
    {
        while (USART_ReadStatusFlag(USART1, USART_FLAG_TXBE) == RESET);
        USART_TxData(USART1, dat[i]);
    }
}

/*!
 * @brief       I2C Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void I2CInit(void)
{
    GPIO_Config_T gpioConfigStruct;
    I2C_Config_T i2cConfigStruct;
    /* Enable I2C related Clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOB | RCM_APB2_PERIPH_AFIO);
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_I2C1);

    /* Free I2C_SCL and I2C_SDA */
    gpioConfigStruct.mode = GPIO_MODE_AF_OD;
    gpioConfigStruct.speed = GPIO_SPEED_50MHz;
    gpioConfigStruct.pin = GPIO_PIN_6;
    GPIO_Config(GPIOB, &gpioConfigStruct);

    gpioConfigStruct.mode = GPIO_MODE_AF_OD;
    gpioConfigStruct.speed = GPIO_SPEED_50MHz;
    gpioConfigStruct.pin = GPIO_PIN_7;
    GPIO_Config(GPIOB, &gpioConfigStruct);

    /*  Config I2C1 */
    I2C_Reset(I2C1);
    i2cConfigStruct.mode = I2C_MODE_I2C;
    i2cConfigStruct.dutyCycle = I2C_DUTYCYCLE_2;
    i2cConfigStruct.ackAddress = I2C_ACK_ADDRESS_7BIT;
    i2cConfigStruct.ownAddress1 = 0XA0;
    i2cConfigStruct.ack = I2C_ACK_ENABLE;
    i2cConfigStruct.clockSpeed = 100000;
    I2C_Config(I2C1, &i2cConfigStruct);

    /* NVIC coniguration */
    NVIC_EnableIRQRequest(I2C1_EV_IRQn, 1, 0);
    /* Enable the I2C1 Interrupt */
    I2C_EnableInterrupt(I2C1, I2C_INT_EVT);

    /* Enable I2Cx */
    I2C_Enable(I2C1);
}

/*!
 * @brief       I2C interrupt service routine
 *
 * @param       None
 *
 * @retval      None
 *
 * @note        This function need to put into I2C1_IRQHandler() in apm32e10x_int.c
 */
void  I2C_Isr(void)
{
    uint8_t det;
    char dat;
    if (I2C_ReadIntFlag(I2C1, I2C_INT_FLAG_ADDR) == SET)
    {
        det = I2C1->STS2;
    }

    if (I2C_ReadStatusFlag(I2C1, I2C_FLAG_RXBNE) == SET)
    {
        dat = I2C_RxData(I2C1);
        printf("%c", dat);
    }

    if (I2C_ReadIntFlag(I2C1, I2C_INT_FLAG_STOP) == SET)
    {
        det = I2C1->CTRL1;
        I2C1->CTRL1 = det;
    }

}

/*!
 * @brief       Used to locate the wrong location of the
 *              I2C_Write()
 *
 * @param       errorCode
 *
 * @retval      Error code location information
 *
 * @note
 */
uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
    printf("I2C Wait TIMEOUT! errorCode = %d\r\n", errorCode);
    return 0;
}

uint8_t I2CWrite(char* pBuffer)
{
    uint16_t I2CTimeout = I2CT_LONG_TIMEOUT;

    while (I2C_ReadStatusFlag(I2C1, I2C_FLAG_BUSBSY))
    {
        I2CInit();
        if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
    }

    I2C_DisableInterrupt(I2C1, I2C_INT_EVT);
    /* Send START condition */
    I2C_EnableGenerateStart(I2C1);

    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_ReadEventStatus(I2C1, I2C_EVENT_MASTER_MODE_SELECT))  //EV5
    {
        if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(5);
    }

    /* Clear ADDR */


    /* Send address for write */
    I2C_Tx7BitAddress(I2C1, 0xB0, I2C_DIRECTION_TX);

    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_ReadEventStatus(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))  //EV6
    {
        if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(6);
    }

    /* While there is data to be written */
    while (*pBuffer != '\0')
    {
        /* Send the current byte */
        I2C_TxData(I2C1, *pBuffer);

        /* Point to the next byte to be written */
        pBuffer++;

        I2CTimeout = I2CT_LONG_TIMEOUT;
        while (!I2C_ReadEventStatus(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))   //EV8
        {
            if ((I2CTimeout--) == 0)
            {
                return I2C_TIMEOUT_UserCallback(8);
            }
        }
    }

//    I2C_EnableGenerateStop(I2C1);
//    I2C_EnableInterrupt(I2C1, I2C_INT_EVT);
    return 1;
}

/*!
 * @brief       BUTTON_KEY interrupt service routine
 *
 * @param       None
 *
 * @retval      None
 *
 * @note        This function need to put into EINT0_1_IRQHandler()
 *              in apm32e10x_int.c
 */
void APM_MINI_PB_I2C_Isr()
{
    if (EINT_ReadStatusFlag(EINT_LINE_1) == SET)
    {
        I2CWrite("Hello slave\r\n");
        EINT_ClearStatusFlag(EINT_LINE_1);
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

/**@} end of group I2C_TwoBoards_Master_Functions */
/**@} end of group I2C_TwoBoards_Master */
/**@} end of group Examples */
