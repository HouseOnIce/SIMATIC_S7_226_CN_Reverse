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

This example shows how to generate a APP firmware to IAP.

LED2 are toggled with a timing defined by the Delay function.

note:

Before using IAP function, you must download the bootloader program to the target board.
Then compile the "Application1" or "Application2" project, and update the generated bin 
firmware file to the target board with the function of IAP through the HyperTerminal tool.

&par Hardware Description

Delays exactly one second, and the LED2 on-off state changes.

&par Directory contents

  - IAP/IAP_Application1/Source/apm32e10x_int.c     Interrupt handlers
  - IAP/IAP_Application1/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103 MINI Devices.
