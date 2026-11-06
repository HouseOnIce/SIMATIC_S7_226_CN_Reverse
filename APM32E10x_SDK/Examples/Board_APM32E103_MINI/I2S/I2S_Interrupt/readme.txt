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

This example describes how to use I2S peripheral.
by making a communication between the I2S2 and the I2S3.
If communication success, LED2 and LED3 will turn on.

Connect I2S2 and I2S3 pins as follows:
I2S2 WS   (PB12)   to  I2S3 WS  (PA15)
I2S2 CK    (PB13)   to  I2S3 CK   (PB3)
I2S2 SD    (PB15)   to  I2S3 SD   (PB5)

&par Directory contents 

  - I2S/I2S_Interrupt/Source/apm32e10x_int.c     Interrupt handlers
  - I2S/I2S_Interrupt/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103_MINI Devices.