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

/** @addtogroup Examples
  @{
  */

/** @addtogroup FPU_Math
  @{
  */

/** @defgroup FPU_Math_Macros Macros
  @{
*/

/* printf function configs to USART1*/
#define DEBUG_USART  USART1

/**@} end of group FPU_Math_Macros*/

/** @defgroup FPU_Math_Enumerations Enumerations
  @{
*/

enum
{
    FAILED,
    PASSED
};

/**@} end of group FPU_Math_Enumerations*/

/** @defgroup FPU_Math_Variables Variables
  @{
*/

float a, b, s, x, y, z, ans[100], X[100] = {0, 0, 0,};
float PI = 3.14159;

uint8_t databuff;
uint32_t m = 0;

volatile uint8_t MemoryProgramStatus = PASSED;

/**@} end of group FPU_Math_Variables*/

/** @defgroup FPU_Math_Functions Functions
  @{
*/

/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 */
int main(void)
{
    SystemClockConfig();

    GPIO_Config_T GPIO_ConfigStruct;
    USART_Config_T USART_ConfigStruct;

    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_FPU);
    RCM->CFG |= BIT27;

    RCM_EnableAPB2PeriphClock((RCM_APB2_PERIPH_T)(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_USART1));

    GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
    GPIO_ConfigStruct.pin = GPIO_PIN_9;
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);

    USART_ConfigStruct.baudRate = 115200;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode = USART_MODE_TX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;
    USART_Config(USART1, &USART_ConfigStruct);

    USART_Enable(USART1);

    printf("SIN:");

    for (s = 0; s < 100; s++)
    {
        ans[databuff] = 0x00;
        databuff++;
    }

    databuff = 0;

    for (s = -PI; s < 0; s = s + 0.1)
    {
        ans[databuff] = sc_math_sin(s);
        databuff++;
    }

    ans[databuff] = sc_math_sin(0);
    databuff++;

    for (s = PI; s > 0; s = s - 0.1)
    {
        ans[databuff] = sc_math_sin(s);
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        printf("%5.11f\n", ans[databuff]);
        databuff++;
    }

    printf("\n");
    printf("\n");
    printf("COS:\n");

    for (s = 0; s < 100; s++)
    {
        ans[databuff] = 0x00;
        databuff++;
    }

    databuff = 0;

    for (s = -PI; s < 0; s = s + 0.1)
    {
        ans[databuff] = sc_math_cos(s);
        databuff++;
    }

    ans[databuff] = sc_math_cos(0);
    databuff++;

    for (s = PI; s > 0; s = s - 0.1)
    {
        ans[databuff] = sc_math_cos(s);
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        printf("%5.11f\n", ans[databuff]);
        databuff++;
    }

    printf("\n");
    printf("\n");
    printf("TAN:\n");

    for (s = 0; s < 100; s++)
    {
        ans[databuff] = 0x00;
        databuff++;
    }

    databuff = 0;

    for (s = -PI; s < 0; s = s + 0.1)
    {
        ans[databuff] = sc_math_tan(s);
        databuff++;
    }

    ans[databuff] = sc_math_tan(0);
    databuff++;

    for (s = PI; s > 0; s = s - 0.1)
    {
        ans[databuff] = sc_math_tan(s);
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        printf("%5.11f\n", ans[databuff]);
        databuff++;
    }

    printf("\n");
    printf("\n");
    printf("ATAN:\n");

    for (s = 0; s < 100; s++)
    {
        ans[databuff] = 0x00;
        databuff++;
    }

    databuff = 0;

    for (s = -PI; s < 0; s = s + 0.1)
    {
        ans[databuff] = sc_math_atan(s);
        databuff++;
    }

    ans[databuff] = sc_math_atan(0);
    databuff++;

    for (s = PI; s > 0; s = s - 0.1)
    {
        ans[databuff] = sc_math_atan(s);
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        printf("%5.11f\n", ans[databuff]);
        databuff++;
    }

    printf("\n");
    printf("\n");
    printf("ASIN:\n");

    for (s = 0; s < 100; s++)
    {
        ans[databuff] = 0x00;
        databuff++;
    }

    databuff = 0;

    for (s = -1; s <= 1; s = s + 0.1)
    {
        ans[databuff] = sc_math_asin(s);
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        printf("%5.11f\n", ans[databuff]);
        databuff++;
    }

    printf("\n");
    printf("\n");
    printf("ACOS:\n");

    for (s = 0; s < 100; s++)
    {
        ans[databuff] = 0x00;
        databuff++;
    }

    databuff = 0;

    for (s = -1; s <= 1; s = s + 0.1)
    {
        ans[databuff] = sc_math_acos(s);
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        printf("%5.11f\n", ans[databuff]);
        databuff++;
    }

    printf("\n");
    printf("\n");
    printf("ATAN2:\n");

    for (s = 0; s < 100; s++)
    {
        ans[databuff] = 0x00;
        databuff++;
    }

    databuff = 0;


    for (s = -PI; s < 0; s = s + 0.1)
    {
        a = s;
        b = s + 0.1;
        ans[databuff] = sc_math_atan2(b, a);
        databuff++;
    }

    a = s;
    b = s + 0.1;
    ans[databuff] = sc_math_atan2(0, 0);
    databuff++;


    for (s = PI; s > 0; s = s - 0.1)
    {
        a = s;
        b = s - 0.1;
        ans[databuff] = sc_math_atan2(b, a);
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        printf("%5.11f\n", ans[databuff]);
        databuff++;
    }

    printf("\n");
    printf("\n");
    printf("invsqrt:\n");
    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        ans[databuff] = sc_math_invsqrt(databuff);
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        printf("%5.11f\n", ans[databuff]);
        databuff++;
    }

    printf("\n");
    printf("\n");
    printf("mac:\n");

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        x = s;
        y = s + 1;
        z = s + 2;
        ans[databuff] = sc_math_mac(x, y, z);
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        printf("%f\n", ans[databuff]);
        databuff++;
    }

    printf("\n");
    printf("\n");
    printf("sum_N:\n");
    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        X[databuff] = s;
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        ans[databuff] = sc_math_sum_N(X, databuff);
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        printf("%5.11f\n", ans[databuff]);
        databuff++;
    }

    printf("\n");
    printf("\n");
    printf("sub_N:\n");

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        X[databuff] = 100 - s;
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        ans[databuff] = sc_math_sub_N(X, databuff);
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        printf("%5.11f\n", ans[databuff]);
        databuff++;
    }

    printf("\n");
    printf("\n");
    printf("prdct:\n");

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        X[databuff] = s + 1;
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        ans[databuff] = sc_math_prdct(X, databuff);
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 100; s++)
    {
        printf("%5.11f\n", ans[databuff]);
        databuff++;
    }

    printf("\n");
    printf("\n");
    printf("sumsq:\n");
    databuff = 0;

    for (s = 0; s < 16; s++)
    {
        X[databuff] = 16 - s;
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 16; s++)
    {
        ans[databuff] = sc_math_sumsq(X, databuff);
        databuff++;
    }

    databuff = 0;

    for (s = 0; s < 16; s++)
    {
        printf("%5.11f\n", ans[databuff]);
        databuff++;
    }

    while (1)
    {
    }
}

#if defined (__CC_ARM) || defined (__ICCARM__) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))

/*!
* @brief       Redirect C Library function printf to serial port.
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
int fputc(int ch, FILE* f)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /* wait for the data to be send */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return (ch);
}

#elif defined (__GNUC__)

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       ch:  The characters that need to be send.
*
* @retval      The characters that need to be send.
*
* @note
*/
int __io_putchar(int ch)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, ch);

    /* wait for the data to be send */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return ch;
}

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       file:  Meaningless in this function.
*
* @param       *ptr:  Buffer pointer for data to be sent.
*
* @param       len:  Length of data to be sent.
*
* @retval      The characters that need to be send.
*
* @note
*/
int _write(int file, char* ptr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        __io_putchar(*ptr++);
    }

    return len;
}

#else
#warning Not supported compiler type
#endif

/**@} end of group FPU_Math_Functions */
/**@} end of group FPU_Math */
/**@} end of group Examples */
