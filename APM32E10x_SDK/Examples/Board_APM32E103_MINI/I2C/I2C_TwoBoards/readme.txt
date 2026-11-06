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

This example shows how to control I2C devices and communicate between two different boards.

To use this example, you need to load the software into two APM32E103_MINI boards(let's call them
Board master and Board Slave) then connect these two boards through I2C lines and GND.

 - At startup, Boards master and slave are both in slave receiver mode and wait for messages 
   to be received.
 - When KEY1 is pressed on Board master, the master transmitter sents "Hello slave" to Board slave.
   The message is displayed on serial assistant through USART1 on Board slave.
 - When KEY1 is pressed on Board slave, the slave transmitter sents "Hello master" to Board master.
   The message is displayed on serial assistant through USART1 on Board master.

&par Hardware Description

   I2C     BOARD1    BOARD2
   SCL      PB6       PB6     (External Pull up resistance)
   SDA      PB7       PB7     (External Pull up resistance)

using USART1(TX:PA9、RX:PA10).
  - USART1 configured as follow:
  - BaudRate = 115200 
  - Word Length = USART_WordLength_8b
  - Stop Bit = USART_StopBits_1
  - Parity = USART_Parity_No
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled

&par Directory contents 

  - I2C/I2C_TwoBoards/I2C_TwoBoards_Master/Source/apm32e10x_int.c     Interrupt handlers
  - I2C/I2C_TwoBoards/I2C_TwoBoards_Master/Source/main.c               Main program
  - I2C/I2C_TwoBoards/I2C_TwoBoards_Slave/Source/apm32e10x_int.c       Interrupt handlers
  - I2C/I2C_TwoBoards/I2C_TwoBoards_Slave/Source/main.c                Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103_MINI Devices. 