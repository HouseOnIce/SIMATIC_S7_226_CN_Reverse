/*!
 * @file        readme.txt
 *
 * @brief       This file is routine instruction
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

&par Example Description

This example describes how to use ADC1 and ADC2 in regular simultaneous dual mode.
The ADC1 are configured to convert ADC Channel 0(PA0).
The ADC2 are configured to convert ADC Channel 1(PA1).

A DMA request is generated each time 2 data items are available.
1st request: ADC->CDATA[31:0] = (ADC2_CH1_REGDATA[15:0] << 16) | ADC1_CH0_REGDATA[15:0]

&par Hardware Description

using ADC_Channel0(PA0)
using ADC_Channel1(PA1)

The USART1 is configured as follows:
    - TX:PA9, RX:PA10
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

&par Directory contents

  - ADC/ADC_DualRegulSimulMode/Source/apm32e10x_int.c     Interrupt handlers
  - ADC/ADC_DualRegulSimulMode/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103_MINI Devices.
