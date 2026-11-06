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

This example describes how to use SPI peripheral to drive LCD screen.
The LCD screen displays multi-level menus, you can press KEY1, KEY2
and KEY3 to operation the multi-level.

KEY1 KEY2 and KEY3 can be used as :
1. Press KEY1(PF9)  ----> Select menu.
2. Press KEY2(PC13) ----> Enter menu.
3. Press KEY3(PA0)  ----> Return to the previous menu.

The USART1 is configured as follows:
  - TX:PA9, RX:PA10
  - BaudRate = 115200 baud
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled

&par Directory contents 

  - SPI/FullDuplex/src/apm32e10x_int.c     Interrupt handlers
  - SPI/FullDuplex/src/main.c              Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0
  
&par Hardware and Software environment

  - This example runs on APM32E103_EVAL Devices.
