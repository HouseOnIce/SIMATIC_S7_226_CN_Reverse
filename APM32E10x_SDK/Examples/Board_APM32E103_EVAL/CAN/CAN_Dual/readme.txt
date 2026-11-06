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

This example shows how to configure the CAN1 and CAN2 peripherals to send and
receive CAN frames in normal mode. The sent frames are used to control LCD by
pressing KEY1 or KEY2 push buttons:
  - When KEY1 button is pressed, CAN1 sends a message to CAN2.
  - When KEY2 button is pressed, CAN2 sends a message to CAN1.

notes:
For this example, the board must be powered with 5V.

The CAN connection as follows:
     _____________                                     ________________
    |             |                                   |                |
    |             |-PB8(CAN1_RX)  <--------->  CAN_RX-| CAN Transceiver|-CANH  <--------
    |             |-PB9(CAN1_TX)  <--------->  CAN_TX-|                |-CANL  <--     |
    |     MCU     |                                   |________________|          |    |
    |             |                                    ________________           |    |
    |             |                                   |                |          |    |
    |             |-PB12(CAN2_RX) <--------->  CAN_RX-| CAN Transceiver|-CANL  <--     |
    |             |-PB13(CAN2_TX) <--------->  CAN_TX-|                |-CANH  <--------
     _____________                                    |________________|
                                               

The USART1 is configured as follows:
  - TX:PA9, RX:PA10
  - BaudRate = 115200 baud
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled

&par Directory contents

  - CAN/CAN_Dual/src/apm32e10x_int.c      Interrupt handlers
  - CAN/CAN_Dual/src/main.c               Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0
  
&par Hardware and Software environment

  - This example runs on APM32E103_EVAL Devices.
