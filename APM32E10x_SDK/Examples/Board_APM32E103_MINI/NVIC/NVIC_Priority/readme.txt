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

This example describes how to use NVIC priority.
At startup, press KEY1(PA1) to occur enter EINT1 Interrupt, and device will enter Infinite loop mode.
The device will enter higher priority EINT0 Interrupt if press KEY2.
Now press KEY1 again will not enter EINT1 Interrupt.
The status of device is displayed on serial assistant through USART1.

&par Hardware Description

EINT1 is connected to PA1(KEY1)
EINT0 is connected to PA0(KEY2)

&par Directory contents

  - NVIC/NVIC_Priority/Source/apm32e10x_int.c     Interrupt handlers
  - NVIC/NVIC_Priority/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103 MINI Devices.
