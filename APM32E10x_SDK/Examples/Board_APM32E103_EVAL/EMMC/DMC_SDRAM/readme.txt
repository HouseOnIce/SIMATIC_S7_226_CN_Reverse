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

The program aims to show how to use DMC to read and write data to external SDRAM.
The test results will be displayed on the LCD screen, and printed by USART1.

The USART1 is configured as follows:
  - TX:PA9, RX:PA10
  - BaudRate = 115200 baud
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled

&par Directory contents 

  - EMMC/SDRAM/src/apm32e10x_int.c      Interrupt handlers
  - EMMC/SDRAM/src/main.c               Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0
  
&par Hardware and Software environment

  - This example runs on APM32E103 EVAL Devices.
