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

This example shows how to synchronize TMR1 and TMR peripherals in cascade mode
with an external trigger

  - TMR1 Master for TMR2 Configure Toggle Mode and Enable event is used as Trigger Output
  - TMR2 is slave for TMR1, And master for TMR3,
  - Configure Toggle Mode and Enable event is used as Trigger Output
  - The ITR0(TMR1) is used as input trigger
  - TMR3 is slave for TMR2, Configure Toggle Mode
  - The ITR1(TMR2) is used as input trigger

  - The TMR1 peripherals clock is 72MHz.

  - The Master Timer TMR1 frequency :
  - TMR1 frequency = (72M) / (71 + 1) /2 = 500KHz

  - The TMR2 is running at:
  - TMR2 frequency = (72M)/ (95 + 1) /2 = 375KHz

  - The TMR3 is running at:
  - TMR3 frequency = (72M)/ (191 + 1) /2 = 187KHz

The starts and stops of the TMR1 counters are controlled by the external trigger.
The TMR2 starts and stops are controlled by the TMR1, and the TMR3 starts and
stops are controlled by the TMR2.

&par Hardware Description

  - using TMR1 CHANNEL1(PA8) to output PWM
  - using TMR2 CHANNEL1(PA0) to output PWM
  - using TMR3 CHANNEL1(PA6) to output PWM

  - using TMR1 CHANNEL2(PA9) connect the external signal

&par Directory contents

  - TMR/TMR_ExtTriggerSynchro/Source/apm32e10x_int.c        Interrupt handlers
  - TMR/TMR_ExtTriggerSynchro/Source/main.c                       Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103 MINI Devices.
