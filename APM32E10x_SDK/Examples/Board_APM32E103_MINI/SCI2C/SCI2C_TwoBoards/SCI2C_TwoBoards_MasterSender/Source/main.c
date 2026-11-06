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

/** @addtogroup SCI2C_TwoBoards_MasterSender
  @{
  */

/** @defgroup SCI2C_TwoBoards_MasterSender_Macros Macros
  @{
*/

#define SCI2C_GPIO_CLK              RCM_APB2_PERIPH_GPIOB
#define SCI2C_GPIO_PORT             GPIOB
#define SCI2C_GPIO_SCL_PIN          GPIO_PIN_6
#define SCI2C_GPIO_SDA_PIN          GPIO_PIN_7

#define SCI2C_CLK_HIGH_PERIOD       10   /* set sci2c clk high period, min = 6 */
#define SCI2C_CLK_LOW_PERIOD        8    /* set sci2c clk low period, min = 8 */
#define SCI2C_SDA_DELAY_TIME        0    /* set sci2c sda delay */
#define SCI2C_SDA_TX_HOLD_TIME      0    /* set sci2c sda Tx hold time */
#define SCI2C_SDA_RX_HOLD_TIME      0    /* set sci2c sda Rx hold time */
#define SCI2C_SDA_TX_FIFO_THRESHOLD 0    /* set sci2c Tx fifo threshold */
#define SCI2C_SDA_RX_FIFO_THRESHOLD 0    /* set sci2c Rx fifo threshold */

#define SCI2C_MASTER_ADDRESS        0x4
#define SCI2C_SLAVE_ADDRESS         0x2B

#define BUF_LENGTH                  20
#define TX_LENGTH                   13

/**@} end of group SCI2C_TwoBoards_MasterSender_Macros*/

/** @defgroup SCI2C_TwoBoards_MasterSender_Variables Variables
  @{
  */

uint8_t txBuf[BUF_LENGTH] = {"hello world\r\n"};

/**@} end of group SCI2C_TwoBoards_MasterSender_Variables*/

/** @defgroup SCI2C_TwoBoards_MasterSender_Functions Functions
  @{
  */

/* I2C Init */
void GPIO_Init(void);
void I2C_Init(void);

/* I2C Handler */
void SCI2C_Master_Write_Process(void);
void SCI2C_Master_WriteHandler(const uint8_t *txBuf, uint32_t length);

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

    APM_MINI_LEDInit(LED3);
    APM_MINI_LEDOff(LED3);
    Delay_Init();

    GPIO_Init();
    I2C_Init();

    while (1)
    {
        APM_MINI_LEDToggle(LED3);
        SCI2C_Master_Write_Process();
        Delay_ms(1000);
    }
}

/*!
 * @brief       GPIO Init
 * @param       None
 * @retval      None
 */
void GPIO_Init(void)
{
    GPIO_Config_T gpioConfigStruct;

    RCM_EnableAPB2PeriphClock(SCI2C_GPIO_CLK | RCM_APB2_PERIPH_AFIO);

    gpioConfigStruct.mode = GPIO_MODE_AF_OD;
    gpioConfigStruct.speed = GPIO_SPEED_50MHz;
    gpioConfigStruct.pin = SCI2C_GPIO_SDA_PIN | SCI2C_GPIO_SCL_PIN;
    GPIO_Config(SCI2C_GPIO_PORT, &gpioConfigStruct);
}

/*!
 * @brief       I2C Init
 * @param       None
 * @retval      None
 */
void I2C_Init(void)
{
    SCI2C_Config_T sci2cConfigStruct;
    /* Enable I2C related Clock (I2C1 and I2C3 use the same bus) */
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_I2C1);

    SCI2C_ConfigStructInit(&sci2cConfigStruct);

    /* Config I2C3 */
    SCI2C_Reset(I2C3);
    SCI2C_ConfigMasterCode(I2C3, SCI2C_MASTER_ADDRESS);

    sci2cConfigStruct.mode = SCI2C_MODE_MASTER;
    sci2cConfigStruct.speed = SCI2C_SPEED_HIGH;
    sci2cConfigStruct.clkLowPeriod = SCI2C_CLK_LOW_PERIOD;
    sci2cConfigStruct.clkHighPeriod = SCI2C_CLK_HIGH_PERIOD;
    sci2cConfigStruct.rxFifoThreshold = SCI2C_SDA_TX_FIFO_THRESHOLD;
    sci2cConfigStruct.txFifoThreshold = SCI2C_SDA_RX_FIFO_THRESHOLD;
    sci2cConfigStruct.restart = SCI2C_RESTART_ENABLE;
    sci2cConfigStruct.addrMode = SCI2C_ADDR_MODE_7BIT;
    SCI2C_Config(I2C3, &sci2cConfigStruct);

    SCI2C_EnableStopDetectAddressed(I2C3);

    /* setup sci2c sda tx/rx hold time */
    SCI2C_ConfigSDAHoldTime(I2C3, SCI2C_SDA_TX_HOLD_TIME, SCI2C_SDA_RX_HOLD_TIME);
    /* set sci2c sda delay time */
    SCI2C_ConfigSDADelayTime(I2C3, SCI2C_SDA_DELAY_TIME);

    /* Enable I2C3 */
    SCI2C_Enable(I2C3);
}

/*!
 * @brief       SCI2C write data process as the master
 * @param       None
 * @retval      None
 */
void SCI2C_Master_Write_Process(void)
{
    /* Waiting for SCI2C to return the idle state */
    while (SCI2C_ReadStatusFlag(I2C3, SCI2C_FLAG_ACT));

    /* Set target address */
    SCI2C_ConfigMasterAddr(I2C3, SCI2C_ADDR_MODE_7BIT, SCI2C_SLAVE_ADDRESS);
    SCI2C_Master_WriteHandler(txBuf, TX_LENGTH);
}

/*!
 * @brief       SCI2C read data handler as the master
 * @param       pBuffer: pointer to the array sending
 * @param       length: the length of the data to be sent
 * @retval      None
 */
void SCI2C_Master_WriteHandler(const uint8_t *txBuf, uint32_t length)
{
    uint32_t txNum = 0;
    SCI2C_ClearIntFlag(I2C3, SCI2C_INT_TA);

    for(txNum = 0; txNum < length; txNum++)
    {
        /* Waiting for transmit FIFO not to be full */
        while(!SCI2C_ReadStatusFlag(I2C3, SCI2C_FLAG_TFNF));
        SCI2C_TxData(I2C3, txBuf[txNum]);
    }

    /* Waiting for SCI2C transmission to end */
    while(!SCI2C_ReadRawIntFlag(I2C3, SCI2C_INT_TFE));

    SCI2C_Abort(I2C3);
}

/**@} end of group SCI2C_TwoBoards_MasterSender_Functions */
/**@} end of group SCI2C_TwoBoards_MasterSender */
/**@} end of group Examples */
