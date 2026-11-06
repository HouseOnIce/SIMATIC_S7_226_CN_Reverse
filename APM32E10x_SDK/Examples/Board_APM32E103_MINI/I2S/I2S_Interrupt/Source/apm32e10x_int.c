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

/** @addtogroup I2S_Interrupt
  @{
  */

/** @defgroup I2S_Interrupt_INT_Functions INT_Functions
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
 * @brief   This function handles SPI3 Handler
 *
 * @param   None
 *
 * @retval  None
 *
 */
void SPI3_IRQHandler(void)
{
    /* Check the interrupt source */
    if (SPI_I2S_ReadIntFlag(SPI3, SPI_I2S_INT_TXBE) == SET)
    {
        /* Send a data from I2S3 */
        SPI_I2S_TxData(SPI3, I2S3_Buffer_Tx[txCnt++]);
    }

    /* Check the end of buffer transfer */
    if (rxCnt == 32)
    {
        /* Disable the I2S3 TXE interrupt to end the communication */
        SPI_I2S_DisableInterrupt(SPI3, SPI_I2S_INT_TXBE);
    }
}

/*!
 * @brief   This function handles SPI2 Handler
 *
 * @param   None
 *
 * @retval  None
 *
 */
void SPI2_IRQHandler(void)
{
    /* Check the interrupt source */
    if (SPI_I2S_ReadIntFlag(SPI2, SPI_I2S_INT_RXBNE) == SET)
    {
        /* Store the I2S2 received data in the relative data table */
        I2S2_Buffer_Rx[rxCnt++] = SPI_I2S_RxData(SPI2);
    }
}

/**@} end of group I2S_Interrupt_INT_Functions */
/**@} end of group I2S_Interrupt */
/**@} end of group Examples */
