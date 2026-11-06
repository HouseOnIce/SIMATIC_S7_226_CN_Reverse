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

This example shows how to output debug information by SWO (ITM).
The program disables JTAG and keeps SWD, then redirects printf to ITM.
You can check the data in the IDE Debug (printf) Viewer.

  - SWO pin: PB3 (JTDO / TRACESWO)
  - JTAG-DP Disabled and SW-DP Enabled
  - printf redirected to ITM_SendChar
  - Connect a debugger that supports SWO in SWD mode
  - Enable SWO/ITM trace in the IDE debug settings
  - Set the Core Clock to the system clock frequency

&par Directory contents 

  - Basic/SWO_Output/Source/apm32e10x_int.c     Interrupt handlers
  - Basic/SWO_Output/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103_MINI Devices.
