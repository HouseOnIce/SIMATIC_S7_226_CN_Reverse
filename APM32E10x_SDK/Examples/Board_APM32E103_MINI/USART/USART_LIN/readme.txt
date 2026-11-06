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

The program shows how to  using USART LIN mode, in this case,
USART1 sends data to upper computer. You can check the data in a Serial Port Utility

  - USART1 configured as follow:
  - BaudRate = 115200
  - Word Length = USART_WordLength_8b
  - Stop Bit = USART_StopBits_1
  - Parity = USART_Parity_No
  - Hardware flow control disabled (RTS and CTS signals)
  - Transmit enabled

USARTx configured as follow:
  - BaudRate = 9600 baud
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  - Enable the LIN mode

To use this example, the user needs to prepare two LIN modules.

   PA2(USART2_TX)  <---> PB11(USART3_RX)
   PA3(USART2_RX)  <---> PB10(USART3_TX)

&par Directory contents

  - USART/USART_LIN/Source/apm32e10x_int.c        Interrupt handlers
  - USART/USART_LIN/Source/main.c                 Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103_MINI Devices.