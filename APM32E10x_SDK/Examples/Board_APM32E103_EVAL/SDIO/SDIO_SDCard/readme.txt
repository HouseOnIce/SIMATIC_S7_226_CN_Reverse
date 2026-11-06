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

The program aims to show how to DMA or polling mode to write and read SD card data by SDIO.
In this case, the application will mount the file system in SD Card. If there is no the file
System, you can Press KEY1(PF9) twice to creat a file system which could make you data lost.
When the file system is mounted success, you can press KEY1, KEY2 and KEY3 to operation the
file system.

The status of SD card can be displayed using serial assistant.
KEY1 KEY2 and KEY3 can be used as :
1. Press KEY1(PF9)  ----> Scan the files of the file system.
2. Press KEY2(PC13) ----> Write a file to file system.
3. Press KEY3(PA0)  ----> Delete the file from file system.

notes:
It is best to use SD card V2.0 to test.

&par Hardware Description

USART1_TX(PA9) ----> host_RX
USART1_RX(PA10)----> host_TX

  - USART1 configured as follow:
  - BaudRate = 115200
  - Word Length = USART_WordLength_8b
  - Stop Bit = USART_StopBits_1
  - Parity = USART_Parity_No
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled

  - SDIO_D0(PC8)     ----> SD_D0
  - SDIO_D1(PC9)     ----> SD_D1
  - SDIO_D2(PC10)    ----> SD_D2
  - SDIO_D3(PC11)    ----> SD_D3
  - SDIO_CK(PC12)    ----> SD_CK
  - SDIO_CMD(PD2)    ----> SD_CMD

&par Directory contents

  - SDIO/SDIO_SDCard/Source/apm32e10x_int.c     Interrupt handlers
  - SDIO/SDIO_SDCard/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103_EVAL Devices.
