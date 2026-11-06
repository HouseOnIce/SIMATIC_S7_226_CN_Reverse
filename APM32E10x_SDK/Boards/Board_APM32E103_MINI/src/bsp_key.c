/*!
 * @file        bsp_key.c
 *
 * @brief       KEY board support package program body
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

/* Includes ***************************************************************/
#include "bsp_key.h"
#include "bsp_delay.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
* @brief    Scan KEY1 and KEY2
*
* @param    None
*
* @retval   key value
*
*/
uint8_t Key_Scan(uint8_t mode)
{
    /* The flag of button is release */
    static uint8_t s_keyRelease = 1;

    if (mode == KEY_MODE_CONTINUOUS)
    {
        s_keyRelease = 1;
    }

    if (s_keyRelease && (KEY1 == 0 || KEY2 == 0))
    {
        Delay_ms(10);

        s_keyRelease = 0;

        if (KEY1 == 0)
        {
            return KEY1_PRESSED;
        }
        else if (KEY2 == 0)
        {
            return KEY2_PRESSED;
        }

    }
    else if (KEY1 == 1 && KEY2 == 1)
    {
        s_keyRelease = 1;
    }

    return KEY_ALL_REALEASED;
}

