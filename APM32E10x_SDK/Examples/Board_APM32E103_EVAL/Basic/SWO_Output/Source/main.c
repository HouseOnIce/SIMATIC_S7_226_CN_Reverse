/*!
 * @file        main.c
 *
 * @brief       Main program body
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

/* Includes */
#include "main.h"
#include "apm32e10x_gpio.h"
#include "apm32e10x_rcm.h"
#include <stdio.h>
/** @addtogroup Examples
  @{
  */

/** @addtogroup SWO_Output
  @{
  */

/** @defgroup SWO_Output_Functions Functions
  @{
  */
 void delay(uint32_t count)
{
    while(count--);
}
/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
int main(void)
{
    uint32_t count = 0;
    
    SystemClockConfig();
    
    /* Enable AFIO clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_AFIO);
    
    /* Disable JTAG and enable SWD to release PB3 as SWO pin */
    GPIO_ConfigPinRemap(GPIO_REMAP_SWJ_JTAGDISABLE);

    while (1)
    {
      /* Insert delay */
      delay(0xFFFFF);
      printf("Hello APM32F103 Eval Board! count = %d\r\n", count++);
    }
}

/*!
 * @brief       Redirect C Library function printf to SWO (ITM).
 *              After Redirection, you can use printf function.
 *
 * @param       ch:  The characters that need to be send.
 *
 * @param       *f:  pointer to a FILE that can recording all information
 *              needed to control a stream
 *
 * @retval      The characters that need to be send.
 *
 * @note
 */
int fputc(int ch, FILE *f)
{
  return ITM_SendChar(ch);
}

/**@} end of group SWO_Output_Functions */
/**@} end of group SWO_Output */
/**@} end of group Examples */
