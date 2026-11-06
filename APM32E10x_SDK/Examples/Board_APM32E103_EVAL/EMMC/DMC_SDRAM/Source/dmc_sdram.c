/*!
 * @file        mmc_sdram.c
 *
 * @brief       This file contains all the functions for the APM32F103xCS SDRAM
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
#include "dmc_sdram.h"
#include "apm32e10x_dmc.h"
#include "apm32e10x_gpio.h"
#include "apm32e10x_rcm.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

extern volatile uint32_t tick;

static void SDRAM_GPIOConfig(void);
static void SDRAM_DMCConfig(void);

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief       SDRAM Init
 *
 * @param       None
 *
 * @retval      None
 *
 */
void SDRAM_Init(void)
{
    SDRAM_GPIOConfig();
    SDRAM_DMCConfig();
}

/*!
 * @brief       SDRAM Word Test
 *
 * @param       None
 *
 * @retval      i : W/R Err Count
 *
 */
uint32_t SDRAM_Word_Test(void)
{
    uint32_t Addr = 0;
    uint32_t i = 0;

    for (Addr = SDRAM_START_ADDR; Addr < SDRAM_END_ADDR; Addr += 0x4)
    {
        SDRAM_WriteWord(Addr, (uint32_t)Addr);
    }

    for (Addr = SDRAM_START_ADDR; Addr < SDRAM_END_ADDR; Addr += 0x4)
    {
        if (*(__IO uint32_t*)(Addr) != (uint32_t)Addr)
        {
            i++;
        }
    }

    return i;
}

/*!
 * @brief       SDRAM HalfWord Test
 *
 * @param       None
 *
 * @retval      i : W/R Err Count
 *
 */
uint32_t SDRAM_HalfWord_Test(void)
{
    uint32_t Addr = 0;
    uint32_t i= 0;

    for (Addr = SDRAM_START_ADDR; Addr < SDRAM_END_ADDR; Addr += 0x2)
    {
        SDRAM_WriteHalfWord(Addr, (uint16_t)Addr);
    }

    for (Addr = SDRAM_START_ADDR; Addr < SDRAM_END_ADDR; Addr += 0x2)
    {
        if (*(__IO uint16_t*)(Addr) != (uint16_t)Addr)
        {
            i++;
        }
    }

    return i;
}

/*!
 * @brief       SDRAM Byte Test
 *
 * @param       None
 *
 * @retval      i : W/R Err Count
 *
 */
uint32_t SDRAM_Byte_Test(void)
{
    uint32_t Addr = 0;
    uint32_t i= 0;

    for (Addr = SDRAM_START_ADDR; Addr < SDRAM_END_ADDR; Addr += 0x1)
    {
        SDRAM_WriteByte(Addr, (uint8_t)Addr);
    }

    for (Addr = SDRAM_START_ADDR; Addr < SDRAM_END_ADDR; Addr += 0x1)
    {
        if (*(__IO uint8_t*)(Addr) != (uint8_t)Addr)
        {
            i++;
        }
    }

    return i;
}

/*!
 * @brief       SDRAM Write Word
 *
 * @param       address:the address to be programmed.
 *
 * @param       data: the data to be programmed.
 *
 * @retval      None
 *
 */
void SDRAM_WriteWord(uint32_t address, uint32_t data)
{
    *(__IOM uint32_t*)address = data;
}

/*!
 * @brief       SDRAM Write Half Word
 *
 * @param       address:the address to be programmed.
 *
 * @param       data: the data to be programmed.
 *
 * @retval      None
 *
 */
void SDRAM_WriteHalfWord(uint32_t address, uint16_t data)
{
    *(uint16_t*)address = (uint16_t)data;
}

/*!
 * @brief       SDRAM Write Byte
 *
 * @param       address:the address to be programmed.
 *
 * @param       data: the data to be programmed.
 *
 * @retval      None
 *
 */
void SDRAM_WriteByte(uint32_t address, uint8_t data)
{
    *(uint8_t*)address = data;
}

/*!
 * @brief       SDRAM GPIO Config
 *
 * @param       None
 *
 * @retval      None
 *
 */
static void SDRAM_GPIOConfig(void)
{
    GPIO_Config_T gpioConfig;

    RCM_EnableAPB2PeriphClock(RCM_SDRAM_GPIO_PERIPH);

    /* SDRAM pins assignment */
    /*
    +-------------------------+--------------------------+--------------------------+
    | PB10 <-> MMC_SDRAM_UNQM | PC10 <-> MMC_SDRAM_D8    | PD2 <-> MMC_SDRAM_D10    |
    | PB11 <-> MMC_SDRAM_CKE  | PC11 <-> MMC_SDRAM_D9    | PD3 <-> MMC_SDRAM_D11    |
    |                         |                          | PD4 <-> MMC_SDRAM_D12    |
    |                         |                          | PD5 <-> MMC_SDRAM_D13    |
    |                         |                          | PD6 <-> MMC_SDRAM_D14    |
    +-------------------------+--------------------------+--------------------------+
    | PE3  <-> MMC_SDRAM_D4   | PF0  <-> MMC_SDRAM_D7    | PG0  <-> MMC_SDRAM_A3    |
    | PE5  <-> MMC_SDRAM_D5   | PF2  <-> MMC_SDRAM_NCS   | PG9  <-> MMC_SDRAM_D15   |
    | PE6  <-> MMC_SDRAM_D6   | PF4  <-> MMC_SDRAM_NRAS  | PG12 <-> MMC_SDRAM_D0    |
    | PE8  <-> MMC_SDRAM_A4   | PF5  <-> MMC_SDRAM_NCAS  | PG13 <-> MMC_SDRAM_D1    |
    | PE9  <-> MMC_SDRAM_A5   | PF6  <-> MMC_SDRAM_NWE   | PG14 <-> MMC_SDRAM_D2    |
    | PE10 <-> MMC_SDRAM_A6   | PF10 <-> MMC_SDRAM_LDQM  | PG15 <-> MMC_SDRAM_D3    |
    | PE11 <-> MMC_SDRAM_A7   | PF11 <-> MMC_SDRAM_BA    |                          |
    | PE12 <-> MMC_SDRAM_A8   | PF12 <-> MMC_SDRAM_A10   |                          |
    | PE13 <-> MMC_SDRAM_A9   | PF13 <-> MMC_SDRAM_A0    |                          |
    | PE15 <-> MMC_SDRAM_CLK  | PF14 <-> MMC_SDRAM_A1    |                          |
    |                         | PF15 <-> MMC_SDRAM_A2    |                          |
    +-------------------------+--------------------------+--------------------------+
    */
    
    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.mode = GPIO_MODE_AF_PP;
    gpioConfig.pin = GPIO_PIN_10 |GPIO_PIN_11;
    GPIO_Config(GPIOB, &gpioConfig);
    
    gpioConfig.pin = GPIO_PIN_10 |GPIO_PIN_11;
    GPIO_Config(GPIOC, &gpioConfig);
    
    gpioConfig.pin = GPIO_PIN_2 | GPIO_PIN_3 |
                    GPIO_PIN_4 | GPIO_PIN_5 |
                    GPIO_PIN_6;
    GPIO_Config(GPIOD, &gpioConfig);
    
    gpioConfig.pin = GPIO_PIN_3 | GPIO_PIN_5 |
                    GPIO_PIN_6 | GPIO_PIN_8 |
                    GPIO_PIN_9 | GPIO_PIN_10 |
                    GPIO_PIN_11 | GPIO_PIN_12 |
                    GPIO_PIN_13 | GPIO_PIN_15 ;
    GPIO_Config(GPIOE, &gpioConfig);
    
    gpioConfig.pin = GPIO_PIN_0 | GPIO_PIN_2 |
                    GPIO_PIN_4 | GPIO_PIN_5 |
                    GPIO_PIN_6 | GPIO_PIN_10 |
                    GPIO_PIN_11 | GPIO_PIN_12 |
                    GPIO_PIN_13 | GPIO_PIN_14 |
                    GPIO_PIN_15;
    GPIO_Config(GPIOF, &gpioConfig);
    
    gpioConfig.pin = GPIO_PIN_0 | GPIO_PIN_9 |
                    GPIO_PIN_12 | GPIO_PIN_13 |
                    GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_Config(GPIOG, &gpioConfig);
}

/*!
 * @brief       SDRAM DMC Config
 *
 * @param       None
 *
 * @retval      None
 *
 */
static void SDRAM_DMCConfig(void)
{
    uint32_t sdramCapacity;
    DMC_Config_T dmcConfig;
    DMC_TimingConfig_T timingConfig;

    RCM_EnableAHBPeriphClock(RCM_SDRAM_PERIPH);

    timingConfig.latencyCAS = DMC_CAS_LATENCY_3;        //!< Configure CAS latency period
    timingConfig.tARP       = DMC_AUTO_REFRESH_10;      //!< Configure auto refresh period
    timingConfig.tRAS       = DMC_RAS_MINIMUM_5;        //!< Configure line activation and precharging minimum time
    timingConfig.tCMD       = DMC_ATA_CMD_7;            //!< Configure active to active period
    timingConfig.tRCD       = DMC_DELAY_TIME_2;         //!< Configure RAS To CAS delay Time
    timingConfig.tRP        = DMC_PRECHARGE_2;          //!< Configure precharge period
    timingConfig.tWR        = DMC_NEXT_PRECHARGE_2;     //!< Configure time between the Last Data and The Next Precharge for write
    timingConfig.tXSR       = 6;                        //!< Configure XSR0
    timingConfig.tRFP       = 0xC3;                     //!< Configure refresh Cycle

    dmcConfig.bankWidth     = DMC_BANK_WIDTH_1;         //!< Configure bank address width
    dmcConfig.clkPhase      = DMC_CLK_PHASE_REVERSE;    //!< Configure clock phase
    dmcConfig.rowWidth      = DMC_ROW_WIDTH_11;         //!< Configure row address width
    dmcConfig.colWidth      = DMC_COL_WIDTH_8;          //!< Configure column address width
    dmcConfig.memorySize    = DMC_MEMORY_SIZE_2MB;
    dmcConfig.timing        = timingConfig;

    DMC_Config(&dmcConfig);
    DMC_ConfigOpenBank(DMC_BANK_NUMBER_2);
    DMC_EnableAccelerateModule();

    DMC_Enable();

    /* sdramCapacity = row * col * 16bit * bank (16bit == 2Bytes) */
    sdramCapacity = (1 << (dmcConfig.rowWidth + 1)) * (1 << (dmcConfig.colWidth + 1)) * 2 * (1 << (dmcConfig.bankWidth + 1));

    printf("This is an example of DMC SDRAM \r\n");
    printf("> Row Address Width    :%d bit\r\n", dmcConfig.rowWidth + 1);
    printf("> Column Address Width :%d bit\r\n", dmcConfig.colWidth + 1);
    printf("> Bank Address Width   :%d bit band addr\r\n", dmcConfig.bankWidth + 1);
    printf("> capacity             :%ld MByte\r\n\r\n", sdramCapacity / 1024 / 1024);
}

