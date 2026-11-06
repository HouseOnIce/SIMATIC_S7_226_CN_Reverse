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

This example shows how to use I2C Peripheral to read and write EEPROM.
Press KEY1 to write data to EEPROM and read data form EEPROM. If read and
write success, LED2 will turn on.  The data will be shown on serial assistant
trough usart1. If error, LED3 will turn on. And LCD will display the test information.

The USART1 is configured as follows:
  - TX:PA9, RX:PA10
  - BaudRate = 115200 baud
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled

&par Hardware Description

  - Config I2C1
  - EEPROM I2C SCL PIN  PB6
  - EEPROM I2C SDA PIN  PB7

&par Directory contents

  - I2C/I2C_EEPROM/Source/apm32e10x_int.c     Interrupt handlers
  - I2C/I2C_EEPROM/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103_EVAL Devices.
