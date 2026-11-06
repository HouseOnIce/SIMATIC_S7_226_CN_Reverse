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

/* Includes */
#include "main.h"
#include "apm32e10x_int.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup CAN_LoopBack
  @{
  */

/** @defgroup CAN_LoopBack_INT_Functions INT_Functions
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
 * @brief   This function handles CAN1 Handler
 *
 * @param   None
 *
 * @retval  None
 *
 */
void USBD1_LP_CAN1_RX0_IRQHandler(void)
{
    CAN_RxMessage_T RxMessage;

    /* receive */
    RxMessage.stdID = 0x00;
    RxMessage.extID = 0x00;
    RxMessage.typeID = 0;
    RxMessage.dataLengthCode = 0;
    RxMessage.data[0] = 0x00;
    RxMessage.data[1] = 0x00;
    CAN_RxMessage(CAN1, CAN_RX_FIFO_0, &RxMessage);

    if ((RxMessage.extID == 0x1234) && (RxMessage.typeID == CAN_TYPEID_EXT)
            && (RxMessage.dataLengthCode == 2) && ((RxMessage.data[1] | RxMessage.data[0] << 8) == 0xDECA))
    {
        intFlag = 1;
    }
    else
    {
        intFlag = 0;
    }
}

/**@} end of group CAN_LoopBack_INT_Functions */
/**@} end of group CAN_LoopBack */
/**@} end of group Examples */
