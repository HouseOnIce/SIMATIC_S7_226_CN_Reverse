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

The program shows how to using USART Smartcard mode, in this case,
USART1 sends data to upper computer. You can check the data in a Serial Port Utility.

  - USART1 configured as follow:
  - BaudRate    = 115200
  - Word Length = USART_WordLength_8b
  - Stop Bit    = USART_StopBits_1
  - Parity      = USART_Parity_No
  - Hardware flow control disabled (RTS and CTS signals)
  - Transmit enabled

Smartcard USART (USART3) configured as follow:
  - BaudRate    = 12096 baud
  - Word Length = 9 Bits
  - 1.5 Stop Bit
  - Even parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Tx and Rx enabled
  - USART Clock enabled
  - USART CPOL Low
  - USART CPHA on first edge
  - USART Last Bit Clock Enabled

To use this example, the user needs to prepare a smart card module.
  PB10 (USART3_TX) ---> Smartcard Tx Pin
  PB11 (Pin)       ---> Smartcard Reset Pin
  PB12 (USART3_CK) ---> Smartcard CK Pin

&par Directory contents

  - USART/USART_Smartcard/Source/apm32e10x_int.c        Interrupt handlers
  - USART/USART_Smartcard/Source/main.c                 Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103 MINI Devices.
