/*!
 * @file        apm32e10x_int.c
 *
 * @brief       Main Interrupt Service Routines
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

#include "main.h"
#include "apm32e10x_int.h"

/** @addtogroup Examples
  @{
*/

/** @addtogroup TMR_PWMInput
  @{
*/

/** @defgroup TMR_PWMInput_INT_Functions INT_Functions
  @{
*/

/*!
 * @brief   This function handles NMI exception
 *
 * @param   None
 *
 * @retval  None
 *
 */
void NMI_Handler(void)
{
}

/*!
 * @brief   This function handles Hard Fault exception
 *
 * @param   None
 *
 * @retval  None
 *
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/*!
 * @brief   This function handles Memory Manage exception
 *
 * @param   None
 *
 * @retval  None
 *
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/*!
 * @brief   This function handles Bus Fault exception
 *
 * @param   None
 *
 * @retval  None
 *
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/*!
 * @brief   This function handles Usage Fault exception
 *
 * @param   None
 *
 * @retval  None
 *
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/*!
 * @brief   This function handles SVCall exception
 *
 * @param   None
 *
 * @retval  None
 *
 */
void SVC_Handler(void)
{
}

/*!
 * @brief   This function handles Debug Monitor exception
 *
 * @param   None
 *
 * @retval  None
 *
 */
void DebugMon_Handler(void)
{
}

/*!
 * @brief   This function handles PendSV_Handler exception
 *
 * @param   None
 *
 * @retval  None
 *
 */
void PendSV_Handler(void)
{
}

/*!
 * @brief   This function handles SysTick Handler
 *
 * @param   None
 *
 * @retval  None
 *
 */
void SysTick_Handler(void)
{
}

/*!
 * @brief   This function handles TMR1 Update Handler
 *
 * @param   None
 *
 * @retval  None
 *
 */
void TMR1_UP_IRQHandler(void)
{
    if (TMR_ReadIntFlag(TMR1, TMR_INT_UPDATE) == SET)
    {
        tick++;
        TMR_ClearIntFlag(TMR1, TMR_INT_UPDATE);
    }
}

/*!
 * @brief   This function handles TMR5 Handler
 *
 * @param   None
 *
 * @retval  None
 *
 */
void TMR3_IRQHandler(void)
{
    uint32_t tempValue = 0;
    if (TMR_ReadIntFlag(TMR3, TMR_INT_UPDATE) == SET)
    {

        if (irqFlag == 1)
        {
            Updatecnt++;
        }
        TMR_ClearIntFlag(TMR3, TMR_INT_UPDATE);
    }

    if (TMR_ReadIntFlag(TMR3, TMR_INT_CC2) == SET)
    {
        /* Clear TMR5 Capture compare 2 interrupt pending bit */
        TMR_ClearIntFlag(TMR3, TMR_INT_CC2);

        /* Get the Input Capture value */
        IC2Value[irqFlag] = TMR_ReadCaputer2(TMR3);

        if (irqFlag != 0)
        {
            irqFlag = 0;

            /* Capture computation */
            tempValue = IC2Value[1] + (Updatecnt * 65536) - IC2Value[0];

            /* Frequency computation */
            Frequency = 1.0 * pclk1 * 2 / tempValue;

            dutycycle = 100.0 * hightime / tempValue;
        }
        else
        {
            Updatecnt = 0;
            irqFlag++;
        }
    }
    if (TMR_ReadIntFlag(TMR3, TMR_INT_CC1) == SET && irqFlag == 1)
    {
        hightime = TMR_ReadCaputer1(TMR3);

        hightime = hightime + (Updatecnt * 65536) - IC2Value[0];
    }
}

/**@} end of group TMR_PWMInput_INT_Functions */
/**@} end of group TMR_PWMInput */
/**@} end of group Examples */
