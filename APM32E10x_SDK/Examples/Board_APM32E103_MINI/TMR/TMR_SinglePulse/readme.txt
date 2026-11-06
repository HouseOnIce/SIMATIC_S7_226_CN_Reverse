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

This example shows how to config TMR peripherals to generate a Single Pulse
with an external trigger

  - TMR1 Configure Single Pulse Mode and Enable event is used as Trigger Output
  - The TMR1 peripherals clock is 72MHz.
  - Configure Timer TMR1 counter clock at 36 MHz:
  - TMR1 counter clock = (72M)/ (1 + 1) = 36MHz

  - TMR1->AUTORLD is 32767, so the trigger input max frequency value
  - is 36MHz/32767 = 1.098KHz .
  - TMR1 pulse = 7200
  - Single Pulse delay value = 7200/(72MHz/2) = 200us
  - And the pulse value is fixed to single pulse value
  - Pulse value = (period - pulse)/counter clock
  - = (32767- 7200)/ 36MHz = 710us

&par Hardware Description

  - using TMR1 CHANNEL1(PA8) to output Pulse by TMR1 CHANNEL2(PA9)
  - using TMR1 CHANNEL2(PA9) connect the external signal

&par Directory contents

  - TMR/TMR_SinglePulse/Source/apm32e10x_int.c        Interrupt handlers
  - TMR/TMR_SinglePulse/Source/main.c                 Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103 MINI Devices.
