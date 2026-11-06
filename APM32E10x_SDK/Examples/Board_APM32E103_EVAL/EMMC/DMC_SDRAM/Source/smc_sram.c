/*!
 * @file        apm3210e_eval_fsmc_sram.c
 *
 * @brief       This file provides a set of functions needed to drive the 
 *              IS61WV51216BLL SRAM memory mounted on APM32E103 board
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
#include "smc_sram.h"
#include "apm32e10x_smc.h"
#include "apm32e10x_gpio.h"
#include "apm32e10x_rcm.h"
#include "apm32e10x_dmc.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/** 
  * @brief  SMC Bank 1 NOR/SRAM3  
  */
#define Bank1_SRAM3_ADDR    ((uint32_t)0x60000000)     

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief       Configures the SMC and GPIOs to interface with the SRAM memory
 *              This function must be called before any write/read operation
 *
 * @param       None
 *
 * @retval      None
 *
 * @note       
 */
void SRAM_Init(void)
{
    GPIO_Config_T GPIO_ConfigStruct;
    
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_SMC);
    RCM_EnableAPB2PeriphClock((RCM_APB2_PERIPH_T)(RCM_APB2_PERIPH_GPIOB | RCM_APB2_PERIPH_GPIOC | \
                                                  RCM_APB2_PERIPH_GPIOD | RCM_APB2_PERIPH_GPIOG | \
                                                  RCM_APB2_PERIPH_GPIOE | RCM_APB2_PERIPH_GPIOF));
    
    /*-- GPIO Configuration ------------------------------------------------------*/
    /** config gpio mode and speed */
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
    /**
     +--------------------+--------------------+--------------------+
     | PB10 <-> DMC_UDQM  | PC10 <-> DMC_DQ8   | PD2  <-> DMC_DQ10  |
     | PB11 <-> DMC_CKE   | PC11 <-> DMC_DQ9   | PD3  <-> DMC_DQ11  |
     |                    |                    | PD4  <-> DMC_DQ12  |
     |                    |                    | PD5  <-> DMC_DQ13  |
     |                    |                    | PD6  <-> DMC_DQ14  |
     |                    |                    | PD12 <-> DMC_      |
     +--------------------+--------------------+--------------------+
     |  PE3  <-> DMC_DQ4  | PF0  <-> DMC_DQ7   | PG0  <-> DMC_A3    |
     |  PE5  <-> DMC_DQ5  | PF2  <-> DMC_CS    | PG9  <-> DMC_DQ15  |
     |  PE6  <-> DMC_DQ6  | PF4  <-> DMC_RAS   | PG12 <-> DMC_DQ0   |
     |  PE8  <-> DMC_A4   | PF5  <-> DMC_CAS   | PG13 <-> DMC_DQ1   |
     |  PE9  <-> DMC_A5   | PF6  <-> DMC_WE    | PG14 <-> DMC_DQ2   |
     |  PE10 <-> DMC_A6   | PF10 <-> DMC_LDQM  | PG15 <-> DMC_DQ3   |
     |  PE11 <-> DMC_A7   | PF11 <-> DMC_BA    |                    |
     |  PE12 <-> DMC_A8   | PF12 <-> DMC_A10   |                    |
     |  PE13 <-> DMC_A9   | PF13 <-> DMC_A0    |                    |
     |  PE15 <-> DMC_CK   | PF14 <-> DMC_A1    |                    |
     |                    | PF15 <-> DMC_A2    |                    |
     +--------------------+--------------------+--------------------+
    */
    GPIO_ConfigStruct.pin = GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_Config(GPIOB, &GPIO_ConfigStruct);
    
    GPIO_ConfigStruct.pin = GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_Config(GPIOC, &GPIO_ConfigStruct);
    
    GPIO_ConfigStruct.pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_12 | GPIO_PIN_2 | \
                            GPIO_PIN_3 | GPIO_PIN_4;
    GPIO_Config(GPIOD, &GPIO_ConfigStruct);
    
    GPIO_ConfigStruct.pin = GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_8 | \
                            GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 | \
                            GPIO_PIN_10 | GPIO_PIN_15;
    GPIO_Config(GPIOE, &GPIO_ConfigStruct);
    
    GPIO_ConfigStruct.pin = GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_11 | \
                            GPIO_PIN_10 | GPIO_PIN_0 | GPIO_PIN_13 | GPIO_PIN_14 | \
                            GPIO_PIN_15 | GPIO_PIN_12 | GPIO_PIN_5;
    GPIO_Config(GPIOF, &GPIO_ConfigStruct);
    
    GPIO_ConfigStruct.pin =  GPIO_PIN_0 |GPIO_PIN_9 | GPIO_PIN_12 | \
                             GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 ;
    GPIO_Config(GPIOG, &GPIO_ConfigStruct);
    
    DMC_Config_T dmcConfig;

    dmcConfig.bankWidth = DMC_BANK_WIDTH_1;
    dmcConfig.clkPhase = DMC_CLK_PHASE_REVERSE;
    dmcConfig.colWidth = DMC_COL_WIDTH_8;
    dmcConfig.memorySize = DMC_MEMORY_SIZE_2MB;
    dmcConfig.rowWidth = DMC_ROW_WIDTH_11;

    DMC_ConfigTimingStructInit(&dmcConfig.timing);

    DMC_Config(&dmcConfig);

    DMC_Enable();
}

/**
  * @brief  Writes a Half-word buffer to the SMC SRAM memory. 
  * @param  pBuffer : pointer to buffer. 
  * @param  WriteAddr : SRAM memory internal address from which the data will be 
  *         written.
  * @param  NumHalfwordToWrite : number of half-words to write. 
  * @retval None
  */
void SRAM_Write(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
  for(; NumHalfwordToWrite != 0; NumHalfwordToWrite--) /*!< while there is data to write */
  {
    /*!< Transfer data to the memory */
    *(uint16_t *) (Bank1_SRAM3_ADDR + WriteAddr) = *pBuffer++;
    
    /*!< Increment the address*/  
    WriteAddr += 2;
  }
}

/**
  * @brief  Reads a block of data from the SMC SRAM memory.
  * @param  pBuffer : pointer to the buffer that receives the data read from the 
  *         SRAM memory.
  * @param  ReadAddr : SRAM memory internal address to read from.
  * @param  NumHalfwordToRead : number of half-words to read.
  * @retval None
  */
void SRAM_Read(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
  for(; NumHalfwordToRead != 0; NumHalfwordToRead--) /*!< while there is data to read */
  {
    /*!< Read a half-word from the memory */
    *pBuffer++ = *(__IO uint16_t*) (Bank1_SRAM3_ADDR + ReadAddr);

    /*!< Increment the address*/  
    ReadAddr += 2;
  }  
}
