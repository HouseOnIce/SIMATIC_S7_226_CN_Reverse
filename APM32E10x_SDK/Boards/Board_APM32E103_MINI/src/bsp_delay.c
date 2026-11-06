/*!
 * @file        bsp_delay.c
 *
 * @brief       Delay board support package body
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

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* public variables */
/* The count of microseconds */
__IO uint32_t cntUs = 0;
/* The count of milliseconds */
__IO uint32_t cntMs = 0;

/* External functions *****************************************************/

/*!
 * @brief       Update SystemCoreClock variable according to Clock Register Values
 *              The SystemCoreClock variable contains the core clock (HCLK)
 *
 * @param       None
 *
 * @retval      None
 *
 */
void Delay_Init(void)
{
    SystemCoreClockUpdate();
}

/*!
 * @brief       Configures Delay ms.
 *
 * @param       nms: Specifies the delay to be configured.
 *              This parameter can be one of following parameters:
 *              @arg nms
 *
 *
 * @retval      None
 */
void Delay_ms(__IO u32 nms)
{
    SysTick_Config(SystemCoreClock / 1000);

    cntMs = nms;
    while (cntMs != 0);
}

/*!
 * @brief       Configures Delay us.
 *
 * @param       nus: Specifies the delay to be configured.
 *              This parameter can be one of following parameters:
 *              @arg nus
 *
 *
 * @retval      None
 */
void Delay_us(__IO u32 nus)
{
    SysTick_Config(SystemCoreClock / 1000000);

    cntUs = nus;
    while (cntUs != 0);
}

