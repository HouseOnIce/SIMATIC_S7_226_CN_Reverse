/*!
 * @file       readme.txt
 *
 * @brief      This file is routine instruction
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

This example describes how to use RT-Thread for APM32E10x.
The IO of LED2 and LED3 is configured to toggle constantly.
The phenomenon of LED2 and LED3 constantly flickered alternately.

&par Software Description

Please install the following dependencies beforehand.
  - Pack/RealThread.RT-Thread.3.1.5.pack    RT-Thread Software Components

&par Hardware Description

LED2 and LED3

The USART1 is configured as follows:
    - TX:PA9, RX:PA10
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

&par Directory contents

  - Source/main.c                       Main program
  - Source/apm32e10x_int.c              Interrupt handlers
  - MDK_Project/RTE/RTOS/board.c        RT-Thread firmware functions
  - MDK_Project/RTE/RTOS/finsh_port.c   RT-Thread finsh port functions
  - MDK_Project/RTE/RTOS/rtconfig.h     RT-Thread config functions

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103 MINI Devices.