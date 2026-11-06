/*!
 * @file       apm32e10x_bakpr.c
 *
 * @brief      This file provides all the BAKPR firmware functions.
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
#include "apm32e10x_bakpr.h"
#include "apm32e10x_rcm.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/
#define BAKPR_CLKCAL_CALVALUE_MASK  (0x7FU)
#define BAKPR_CLKCAL_CALCOEN        (1U << 7)
#define BAKPR_CLKCAL_ASPOEN         (1U << 8)
#define BAKPR_CLKCAL_ASPOSEL        (1U << 9)
#define BAKPR_CLKCAL_OUT_MASK       (BAKPR_CLKCAL_CALCOEN | BAKPR_CLKCAL_ASPOEN | BAKPR_CLKCAL_ASPOSEL)

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief      Reset the BAKPR peripheral registers to their default reset values.
 *
 * @param      None
 *
 * @retval     None
 */
void BAKPR_Reset(void)
{
    RCM_EnableBackupReset();
    RCM_DisableBackupReset();
}

/*!
 * @brief      Deinitializes the BAKPR peripheral registers to their default reset values.
 *
 * @param      value: specifies the RTC output source.
 *                    This parameter can be one of the following values:
 *                    @arg BAKPR_TAMPER_PIN_LEVEL_HIGH: Tamper pin active on high level
 *                    @arg BAKPR_TAMPER_PIN_LEVEL_LOW: Tamper pin active on low level
 *
 * @retval     None
 */
void BAKPR_ConfigTamperPinLevel(BAKPR_TAMPER_PIN_LEVEL_T value)
{
    BAKPR->CTRL_B.TPFCFG = DISABLE;
    BAKPR->CTRL_B.TPALCFG = value;
}

/*!
 * @brief      Enables the Tamper Pin activation.
 *
 * @param      None
 *
 * @retval     None
 */
void BAKPR_EnableTamperPin(void)
{
    BAKPR->CTRL_B.TPFCFG = ENABLE ;
}

/*!
 * @brief      Disables the Tamper Pin activation.
 *
 * @param      None
 *
 * @retval     None
 */
void BAKPR_DisableTamperPin(void)
{
    BAKPR->CTRL_B.TPFCFG = DISABLE ;
}

/*!
 * @brief      Enables the Tamper Pin Interrupt.
 *
 * @param      None
 *
 * @retval     None
 */
void BAKPR_EnableInterrupt(void)
{
    BAKPR->CSTS_B.TPIEN = ENABLE ;
}

/*!
 * @brief      Disables the Tamper Pin Interrupt.
 *
 * @param      None
 *
 * @retval     None
 */
void BAKPR_DisableInterrupt(void)
{
    BAKPR->CSTS_B.TPIEN = DISABLE ;
}

/*!
 * @brief   Select the RTC output source to output on the Tamper pin.
 *
 * @param   source: specifies the RTC output source.
 *            This parameter can be one of the following values:
 *            @arg BAKPR_RTC_OUTPUT_SOURCE_NONE             : no RTC output on the Tamper pin.
 *            @arg BAKPR_RTC_OUTPUT_SOURCE_CALIBRATION_CLOCK: output the RTC clock with frequency divided by 64 on the Tamper pin.
 *            @arg BAKPR_RTC_OUTPUT_SOURCE_ALARM            : output the RTC Alarm pulse signal on the Tamper pin.
 *            @arg BAKPR_RTC_OUTPUT_SOURCE_SECOND           : output the RTC Second pulse signal on the Tamper pin.
 *
 * @retval  None
 */
void BAKPR_ConfigRTCOutput(BAKPR_RTC_OUTPUT_SOURCE_T source)
{
    uint32_t tmp;

    tmp = BAKPR->CLKCAL & BAKPR_CLKCAL_CALVALUE_MASK;
    tmp &= ~BAKPR_CLKCAL_OUT_MASK;

    if (source == BAKPR_RTC_OUTPUT_SOURCE_CALIBRATION_CLOCK)
    {
        tmp |= BAKPR_CLKCAL_CALCOEN;
    }
    else if (source == BAKPR_RTC_OUTPUT_SOURCE_ALARM)
    {
        tmp |= BAKPR_CLKCAL_ASPOEN;
    }
    else if (source == BAKPR_RTC_OUTPUT_SOURCE_SECOND)
    {
        tmp |= (BAKPR_CLKCAL_ASPOEN | BAKPR_CLKCAL_ASPOSEL);
    }

    BAKPR->CLKCAL = tmp;
}

/*!
 * @brief      Sets RTC Clock Calibration value.
 *
 * @param      calibrationValue: Specifies the calibration value.
 *                               This parameter must be a number between 0 and 0x7F.
 *
 * @retval     None
 */
void BAKPR_ConfigRTCCalibrationValue(uint8_t calibrationValue)
{
    BAKPR->CLKCAL_B.CALVALUE = (uint32_t)(calibrationValue & BAKPR_CLKCAL_CALVALUE_MASK);
}

/*!
 * @brief      Set user data to the specified Data Backup Register.
 *
 * @param      bakrData : specifies the Data Backup Register.
 *                        This parameter can be BAKPR_DATAx where x is between 1 and 42.
 *
 * @param      data : data to set
 *                    This parameter can be a 16bit value.
 *
 * @retval     None
 */
void BAKPR_ConfigBackupRegister(BAKPR_DATA_T bakrData, uint16_t data)
{
    __IOM uint32_t tmp = 0;

    tmp = (uint32_t)BAKPR_BASE;
    tmp += bakrData;

    *(__IOM uint32_t *) tmp = data;
}

/*!
 * @brief      Reads user data from the specified Data Backup Register.
 *
 * @param      bakrData : specifies the Data Backup Register.
 *                        This parameter can be BAKPR_DATAx where x is between 1 and 42.
 *
 * @retval     The content of the specified Data Backup Register
 */
uint16_t BAKPR_ReadBackupRegister(BAKPR_DATA_T bakrData)
{
    __IOM uint32_t tmp = 0;

    tmp = (uint32_t)BAKPR_BASE;
    tmp += bakrData;

    return (*(__IOM uint32_t *) tmp);
}

/*!
 * @brief      Read whether the Tamper Pin Event flag is set or not.
 *
 * @param      None
 *
 * @retval     Tamper Pin Event flag state
 */
uint8_t BAKPR_ReadStatusFlag(void)
{
    return BAKPR->CSTS_B.TEFLG;
}

/*!
 * @brief      Clears Tamper Pin Event pending flag.
 *
 * @param      None
 *
 * @retval     None
 */
void BAKPR_ClearStatusFlag(void)
{
    BAKPR->CSTS_B.TECLR = BIT_SET;
}

/*!
 * @brief      Get whether the Tamper Pin Interrupt has occurred or not.
 *
 * @param      None
 *
 * @retval     Tamper Pin Interrupt State
 */
uint8_t BAKPR_ReadIntFlag(void)
{
    return BAKPR->CSTS_B.TIFLG;
}

/*!
 * @brief      Clears Tamper Pin Interrupt pending bit.
 *
 * @param      None
 *
 * @retval     None
 */
void BAKPR_ClearIntFlag(void)
{
    BAKPR->CSTS_B.TICLR = BIT_SET;
}

