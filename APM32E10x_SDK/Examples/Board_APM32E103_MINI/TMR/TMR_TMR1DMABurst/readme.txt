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

The program to show how to configure the TMR1 channel period 
and the duty cycle by DMA burst to generate 6 PWM 
with 6 different duty cycles (80%, 70%, 60%, 40%, 30% and 20%).

On the DMA update request, the DMA will do 5 transfers of half words into TMR1
registers (AUTORLD, REPCNT, CC1, CC2, CC3).
999 will be transferred into AUTORLD
0 will be transferred into REPCNT
800 will be transferred into CC1.
700 will be transferred into CC2.
600 will be transferred into CC3.

The objective is to generate 7 PWM signal at 1 KHz:
  - TMR1 Repetition counter =   6
  - SystemCoreClock         =   72 MHz.
  - TMR1CLK                 =   SystemCoreClock
  - Prescaler               =   71
  - TMR1 counter clock      =   1 MHz
  - TMR1_Period             =   (1000000 / 1000) - 1
  - Channel 1 duty cycle = 80%
  - Channel 2 duty cycle = 70%
  - Channel 3 duty cycle = 60%
  - Channel 3N duty cycle = 40%
  - Channel 2N duty cycle = 30%
  - Channel 1N duty cycle = 20%

Display TMR1 waveform by oscilloscope.

&par Hardware Description

  - using TMR1 Channel 1 (PA8) to output PWM
  - using TMR1 Channel 2 (PA9) to output PWM
  - using TMR1 Channel 3 (PA10) to output PWM
  - using TMR1 Channel 3N (PB15) to output PWM
  - using TMR1 Channel 2N (PB14) to output PWM
  - using TMR1 Channel 1N (PB13) to output PWM

&par Directory contents

  - TMR/TMR_TMR1DMABurst/Source/apm32e10x_int.c     Interrupt handlers
  - TMR/TMR_TMR1DMABurst/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103_MINI Devices.
