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
 *  Copyright (C) 2025 Geehy Semiconductor
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

This example describes how to uses ADC for the injection channel and regular channel sampling.

PC0   ------> ADC12_IN10 ------> VBUS_CUR   (Injection Channel)
PC1   ------> ADC12_IN11 ------> VBAT_VOL   (Injection Channel)
PC3   ------> ADC12_IN13 ------> U_VOL         (Regular Channel)
PA0   ------> ADC12_IN4  ------> V_VOL         (Regular Channel)
PA4   ------> ADC12_IN5  ------> W_VOL         (Regular Channel)
PC4   ------> ADC12_IN14 ------> U_CUR         (Regular Channel)
PC5   ------> ADC12_IN15 ------> V_CUR         (Regular Channel)

&par Hardware Description

using ADC1_Channel0(PA0)

The USART1 is configured as follows:
  - TX:PA9, RX:PA10
  - BaudRate = 115200 baud
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled

&par Directory contents

  - ADC/ADC_InjectionRegularSampling/src/apm32f10x_it.c     Interrupt handlers
  - ADC/ADC_InjectionRegularSampling/src/main.c             Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F103_MINI Devices.
