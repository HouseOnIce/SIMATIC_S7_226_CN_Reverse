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

/** @addtogroup FMC_Program
  @{
  */

/** @defgroup FMC_Program_Macros Macros
  @{
*/
#define FLASH_PAGE_SIZE    ((uint16_t)0x800)

#define BANK1_WRITE_START_ADDR  ((uint32_t)0x08008000)
#define BANK1_WRITE_END_ADDR    ((uint32_t)0x0800C000)

/**@} end of group FMC_Program_Macros */

/** @defgroup FMC_Program_Enumerations Enumerations
  @{
*/

enum
{
    FAILED,
    PASSED
};

/**@} end of group FMC_Program_Enumerations */

/** @defgroup FMC_Program_Variables Variables
  @{
*/

uint32_t EraseCounter = 0x00, Address = 0x00;
uint32_t Data = 0x3210ABCD;
uint32_t NbrOfPage = 0x00;
volatile FMC_STATUS_T FLASHStatus = FMC_STATUS_COMPLETE;
volatile uint8_t MemoryProgramStatus = PASSED;

/**@} end of group FMC_Program_Variables */

/** @defgroup FMC_Program_Functions Functions
  @{
*/

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

    APM_MINI_LEDInit(LED2);
    APM_MINI_LEDInit(LED3);
    APM_MINI_LEDOff(LED2);
    APM_MINI_LEDOff(LED3);

    FMC_Unlock();

    NbrOfPage = (BANK1_WRITE_END_ADDR - BANK1_WRITE_START_ADDR) / FLASH_PAGE_SIZE;
    FMC_ClearStatusFlag((FMC_FLAG_T)(FMC_FLAG_OC | FMC_FLAG_PE | FMC_FLAG_WPE));
    for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FMC_STATUS_COMPLETE); EraseCounter++)
    {
        FLASHStatus = FMC_ErasePage(BANK1_WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
    }

    Address = BANK1_WRITE_START_ADDR;
    while ((Address < BANK1_WRITE_END_ADDR) && (FLASHStatus == FMC_STATUS_COMPLETE))
    {
        FLASHStatus = FMC_ProgramWord(Address, Data);
        Address = Address + 4;
    }

    FMC_Lock();

    Address = BANK1_WRITE_START_ADDR;

    while ((Address < BANK1_WRITE_END_ADDR) && (MemoryProgramStatus != FAILED))
    {
        if ((*(__IO uint32_t*) Address) != Data)
        {
            MemoryProgramStatus = FAILED;
        }
        Address += 4;
    }
    if (MemoryProgramStatus == FAILED)
    {
        APM_MINI_LEDOn(LED3);
    }
    else
    {
        APM_MINI_LEDOn(LED2);
    }

    while (1)
    {

    }
}

/**@} end of group FMC_Program_Functions */
/**@} end of group FMC_Program */
/**@} end of group Examples */


