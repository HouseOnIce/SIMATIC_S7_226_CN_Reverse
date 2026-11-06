/*!
 * @file        readme.txt
 *
 * @brief       This file is routine instruction
 *
 * @version     V1.0.3
 *
 * @date        2024-11-05
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

This example shows how to control SCI2C devices and communicate between two different boards.

To use this example, you need to load the software into two APM32E103_MINI boards(let's call them
Board master and Board Slave) then connect these two boards through I2C lines and GND.

 - In the routine "MasterSender", SCI2C periodically sends data as the master and toggle LED2;
 - In the routine "MasterReceiver", SCI2C periodically receives data as the master and toggle LED2;
 - In the routine "SlaveReceiver", SCI2C periodically receives data as the slave and toggle LED2;

Note: when the system clock is at 72MHz, one time unit is approximately 30ns.

&par Hardware Description

   I2C     BOARD1    BOARD2
   SCL      PB6       PB6     (External Pull up resistance)
   SDA      PB7       PB7     (External Pull up resistance)

&par Directory contents

  - I2C/I2C_TwoBoards/I2C_TwoBoards_MasterSender/Source/apm32e10x_int.c      Interrupt handlers
  - I2C/I2C_TwoBoards/I2C_TwoBoards_MasterSender/Source/main.c               Main program
  - I2C/I2C_TwoBoards/I2C_TwoBoards_MasterReceiver/Source/apm32e10x_int.c    Interrupt handlers
  - I2C/I2C_TwoBoards/I2C_TwoBoards_MasterReceiver/Source/main.c             Main program
  - I2C/I2C_TwoBoards/I2C_TwoBoards_SlaveReceiver/Source/apm32e10x_int.c     Interrupt handlers
  - I2C/I2C_TwoBoards/I2C_TwoBoards_SlaveReceiver/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103_MINI Devices.
