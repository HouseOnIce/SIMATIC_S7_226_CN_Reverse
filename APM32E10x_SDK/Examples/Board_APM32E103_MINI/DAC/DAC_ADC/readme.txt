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

This example provides an example of how to use DAC channel 1(PA4) to output
voltage to ADC channel 0(PA0). The converted voltage of PA4 is detected by
ADC  channel 0 and displayed on serial assistant through USART1.

 -DAC generates voltage from 0V to 3.3V

 -when the voltage is greater than 2.0V, then LED2 turns on and LED3 turns off.
 -when the voltage is less than 0.8V, then LED2 turns off and LED3 turns on.
 -when the voltage is greater than 0.8V but less than 2.0V, then LED2 and LED3 turn on.

&par Directory contents 

  - DAC/DAC_ADC/Source/apm32e10x_int.c     Interrupt handlers
  - DAC/DAC_ADC/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.29
  - EWARM V8.50.4.26131
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32E103_MINI Devices. 