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

/** @addtogroup CAN_LoopBack
  @{
  */

/** @defgroup CAN_LoopBack_Macros Macros
  @{
*/

#define CANx        CAN1

/**@} end of group CAN_LoopBack_Macros*/

/** @defgroup CAN_LoopBack_Functions Functions
  @{
  */

uint8_t intFlag = 0;

enum {FAILED = 0, PASSED = 1};

void NVIC_Configuration(void);
uint8_t CAN_Polling(void);
uint8_t CAN_Interrupt(void);

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
    uint8_t state;

    SystemClockConfig();

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_CAN1);
    NVIC_Configuration();

    /* Configures LED2 and LED3 */
    APM_MINI_LEDInit(LED2);
    APM_MINI_LEDInit(LED3);

    /* Turns selected LED Off */
    APM_MINI_LEDOff(LED2);
    APM_MINI_LEDOff(LED3);

    state = CAN_Polling();
    if (state == FAILED)
    {
        /* Turn off LED2 */
        APM_MINI_LEDOff(LED2);
    }
    else
    {
        /* Turn on LED2 */
        APM_MINI_LEDOn(LED2);
    }

    /* CAN transmit at 500Kb/s and receive by interrupt in loopback mode */
    state = CAN_Interrupt();

    if (state == FAILED)
    {
        /* Turn off LED3 */
        APM_MINI_LEDOff(LED3);
    }
    else
    {
        /* Turn on LED3 */
        APM_MINI_LEDOn(LED3);
    }
    while (1)
    {
    }
}

/*!
 * @brief       Configures the CAN, transmit and receive by polling
 *
 * @param       None
 *
 * @retval      PASSED if the reception is well done, FAILED in other case
 *
 */
uint8_t CAN_Polling(void)
{
    CAN_Config_T    CAN_ConfigStructure;
    CAN_FilterConfig_T CAN_FilterStruct;
    CAN_TxMessage_T TxMessage;
    CAN_RxMessage_T RxMessage;
    uint32_t i = 0;
    CAN_TX_MAILBIX_T TransmitMailbox = CAN_TX_MAILBIX_1;

    /* CAN register init */
    CAN_Reset(CANx);

    CAN_ConfigStructInit(&CAN_ConfigStructure);

    /* CAN cell init */

    CAN_ConfigStructure.autoBusOffManage = DISABLE;
    CAN_ConfigStructure.autoWakeUpMode = DISABLE;
    CAN_ConfigStructure.nonAutoRetran = DISABLE;
    CAN_ConfigStructure.rxFIFOLockMode = DISABLE;
    CAN_ConfigStructure.txFIFOPriority = DISABLE;
    CAN_ConfigStructure.mode = CAN_MODE_LOOPBACK;

    /* Baudrate = 125kbps*/
    CAN_ConfigStructure.syncJumpWidth = CAN_SJW_1;
    CAN_ConfigStructure.timeSegment1 = CAN_TIME_SEGMENT1_2;
    CAN_ConfigStructure.timeSegment2 = CAN_TIME_SEGMENT2_3;
    CAN_ConfigStructure.prescaler = 48;
    CAN_Config(CANx, &CAN_ConfigStructure);

    /* CAN filter init */
    CAN_FilterStruct.filterNumber = 0;
    CAN_FilterStruct.filterMode = CAN_FILTER_MODE_IDMASK;
    CAN_FilterStruct.filterScale = CAN_FILTER_SCALE_32BIT;
    CAN_FilterStruct.filterIdHigh = 0x0000;
    CAN_FilterStruct.filterIdLow = 0x0000;
    CAN_FilterStruct.filterMaskIdHigh = 0x0000;
    CAN_FilterStruct.filterMaskIdLow = 0x0000;
    CAN_FilterStruct.filterFIFO = CAN_FILTER_FIFO_0;
    CAN_FilterStruct.filterActivation = ENABLE;
    CAN_ConfigFilter(CANx, &CAN_FilterStruct);

    /* transmit */
    TxMessage.stdID = 0x11;
    TxMessage.remoteTxReq = CAN_RTXR_DATA;
    TxMessage.typeID = CAN_TYPEID_STD;
    TxMessage.dataLengthCode = 2;
    TxMessage.data[0] = 0xCA;
    TxMessage.data[1] = 0xFE;

    TransmitMailbox = (CAN_TX_MAILBIX_T)CAN_TxMessage(CANx, &TxMessage);
    i = 0;
    while ((!CAN_TxMessageStatus(CANx, TransmitMailbox)) && (i != 0xFFFF))
    {
        i++;
    }

    i = 0;
    while ((CAN_PendingMessage(CANx, CAN_RX_FIFO_0) < 1) && (i != 0xFFFF))
    {
        i++;
    }

    /* receive */
    RxMessage.stdID = 0x00;
    RxMessage.typeID = CAN_TYPEID_STD;
    RxMessage.dataLengthCode = 0;
    RxMessage.data[0] = 0x00;
    RxMessage.data[1] = 0x00;
    CAN_RxMessage(CANx, CAN_RX_FIFO_0, &RxMessage);

    if (RxMessage.stdID != 0x11)
    {
        return FAILED;
    }

    if (RxMessage.typeID != CAN_TYPEID_STD)
    {
        return FAILED;
    }

    if (RxMessage.dataLengthCode != 2)
    {
        return FAILED;
    }

    if ((RxMessage.data[0] << 8 | RxMessage.data[1]) != 0xCAFE)
    {
        return FAILED;
    }

    return PASSED; /* Test Passed */
}

/*!
 * @brief       Configures the CAN, transmit and receive using interrupt
 *
 * @param       None
 *
 * @retval      PASSED if the reception is well done, FAILED in other case
 *
 */
uint8_t CAN_Interrupt(void)
{
    CAN_Config_T    CAN_ConfigStructure;
    CAN_FilterConfig_T CAN_FilterStruct;
    CAN_TxMessage_T TxMessage;
    uint32_t i = 0;

    /* CAN register init */
    CAN_Reset(CANx);


    CAN_ConfigStructInit(&CAN_ConfigStructure);

    /* CAN cell init */

    CAN_ConfigStructure.autoBusOffManage = DISABLE;
    CAN_ConfigStructure.autoWakeUpMode = DISABLE;
    CAN_ConfigStructure.nonAutoRetran = DISABLE;
    CAN_ConfigStructure.rxFIFOLockMode = DISABLE;
    CAN_ConfigStructure.txFIFOPriority = DISABLE;
    CAN_ConfigStructure.mode = CAN_MODE_LOOPBACK;
    /* Baudrate = 500 Kbps */
    CAN_ConfigStructure.syncJumpWidth = CAN_SJW_1;
    CAN_ConfigStructure.timeSegment1 = CAN_TIME_SEGMENT1_2;
    CAN_ConfigStructure.timeSegment2 = CAN_TIME_SEGMENT2_3;
    CAN_ConfigStructure.prescaler = 12;
    CAN_Config(CANx, &CAN_ConfigStructure);

    /* CAN filter init */
    CAN_FilterStruct.filterNumber = 1;
    CAN_FilterStruct.filterMode = CAN_FILTER_MODE_IDMASK;
    CAN_FilterStruct.filterScale = CAN_FILTER_SCALE_32BIT;
    CAN_FilterStruct.filterIdHigh = 0x0000;
    CAN_FilterStruct.filterIdLow = 0x0000;
    CAN_FilterStruct.filterMaskIdHigh = 0x0000;
    CAN_FilterStruct.filterMaskIdLow = 0x0000;
    CAN_FilterStruct.filterFIFO = CAN_FILTER_FIFO_0;
    CAN_FilterStruct.filterActivation = ENABLE;
    CAN_ConfigFilter(CANx, &CAN_FilterStruct);

    /* CAN FIFO0 message pending interrupt enable */
    CAN_EnableInterrupt(CANx, CAN_INT_F0MP);

    /* transmit 1 message */
    TxMessage.stdID = 0x0;
    TxMessage.extID = 0x1234;
    TxMessage.remoteTxReq = CAN_RTXR_DATA;
    TxMessage.typeID = CAN_TYPEID_EXT;
    TxMessage.dataLengthCode = 2;
    TxMessage.data[0] = 0xDE;
    TxMessage.data[1] = 0xCA;

    CAN_TxMessage(CANx, &TxMessage);

    /* initialize the value that will be returned */
    intFlag = 0xFF;

    /* receive message with interrupt handling */
    i = 0;
    while ((intFlag == 0xFF) && (i < 0xFFF))
    {
        i++;
    }

    if (i == 0xFFF)
    {
        intFlag = 0;
    }

    /* disable interrupt handling */
    CAN_DisableInterrupt(CANx, CAN_INT_F0MP);

    return intFlag;
}

/*!
 * @brief       Configures NVIC and Vector Table base location
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void NVIC_Configuration(void)
{
    NVIC_EnableIRQRequest(USBD1_LP_CAN1_RX0_IRQn, 0, 0);
}

/**@} end of group CAN_LoopBack_Functions */
/**@} end of group CAN_LoopBack */
/**@} end of group Examples */
