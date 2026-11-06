/*!
 * @file        bsp_delay.c
 *
 * @brief       Delay board support package program body
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
#include "bsp_delay.h"
#include "apm32e10x_misc.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* The count of microseconds*/
uint32_t cntUs = 0;
/* The count of milliseconds*/
uint32_t cntMs = 0;

/* External functions *****************************************************/

/*!
 * @brief       Configures Delay.
 *
 * @param       None
 *
 * @retval      None
 */
void APM_EVAL_DelayInit(void)
{
    SystemCoreClockUpdate();

    cntUs = SystemCoreClock / SystemCoreClock;
    cntMs = (uint16_t)cntUs * 1000;
}

/*!
 * @brief       Configures Delay ms.
 *
 * @param       nms: Specifies the delay to be configured.
 *              This parameter can be one of following parameters:
 *              @arg nms
 *
 * @retval      None
 */
void APM_EVAL_DelayMs(__IO uint32_t nms)
{
    uint32_t temp;
    
    SysTick->LOAD = (uint32_t)nms * cntMs;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
}

/*!
 * @brief       Configures Delay us.
 *
 * @param       nus: Specifies the delay to be configured.
 *              This parameter can be one of following parameters:
 *              @arg nus
 *
 * @retval      None
 */
void APM_EVAL_DelayUs(__IO uint32_t nus)
{
    uint32_t temp;
    
    SysTick->LOAD = (uint32_t)nus * cntUs;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
}

