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

This example provides a basic communication between USART1 and USART2 using DMA1
capability.

After system reset, the DMA transfers data from DMA_USART_TxBuf buffer to USART2 
Transmit data register, then this data is sent to USART1. Data received by USART1 
is transferred by DMA and stored in DMA_USART_RxBuf. Then compared with the two 
buffers, If the data of DMA_USART_TxBuf and DMA_USART_RxBuf are the same, LED2 will 
light, otherwise LED3 will light.

&par Hardware Description

USART2_TX (PA2)  --->  USART1_RX (PA10)

The USART1 and USART2 is configured as follows:
    - USART1 TX:PA9, RX:PA10, USART2 TX:PA2, RX:PA3
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

&par Directory contents

  - DMA/DMA_UsartToMemory/Source/apm32e10x_int.c     Interrupt handlers
  - DMA/DMA_UsartToMemory/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103_MINI Devices.
