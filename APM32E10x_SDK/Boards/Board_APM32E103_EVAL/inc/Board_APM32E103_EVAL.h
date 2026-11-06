/*!
 * @file        Board_APM32E103_EVAL.h
 *
 * @brief       This file contains definitions for APM32E103_EVAL's Leds, push-buttons hardware resources    
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

/* Define to prevent recursive inclusion */
#ifndef __BOARD_APM32E103_EVAL_H
#define __BOARD_APM32E103_EVAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ***************************************************************/
#include "apm32e10x.h"
#include "apm32e10x_gpio.h"
#include "apm32e10x_rcm.h"
#include "apm32e10x_eint.h"
#include "apm32e10x_usart.h"
#include "apm32e10x_misc.h"

/* Exported typedef *******************************************************/

typedef enum
{
    LED1 = 0,
    LED2 = 1,
    LED3 = 2
} Led_TypeDef;

typedef enum
{
    BUTTON_KEY1 = 0,
    BUTTON_KEY2 = 1,
    BUTTON_KEY3 = 2
} Button_TypeDef;

typedef enum
{
    BUTTON_MODE_GPIO = 0,
    BUTTON_MODE_EINT = 1
} ButtonMode_TypeDef;

typedef enum
{
    COM1 = 0,
    COM2 = 1,
    COM3 = 2,
    COM4 = 3,
    COM5 = 4,
} COM_TypeDef;

/* Exported macro *********************************************************/

#define LEDn                             3

#define LED1_PIN                         GPIO_PIN_13
#define LED1_GPIO_PORT                   GPIOD
#define LED1_GPIO_CLK                    RCM_APB2_PERIPH_GPIOD

#define LED2_PIN                         GPIO_PIN_14
#define LED2_GPIO_PORT                   GPIOD
#define LED2_GPIO_CLK                    RCM_APB2_PERIPH_GPIOD

#define LED3_PIN                         GPIO_PIN_15
#define LED3_GPIO_PORT                   GPIOD
#define LED3_GPIO_CLK                    RCM_APB2_PERIPH_GPIOD

#define BUTTONn                          3

/**
 * @brief Key1 push-button
 */
#define KEY1_BUTTON_PIN                   GPIO_PIN_9
#define KEY1_BUTTON_GPIO_PORT             GPIOF
#define KEY1_BUTTON_GPIO_CLK              RCM_APB2_PERIPH_GPIOF
#define KEY1_BUTTON_EINT_LINE             EINT_LINE_9
#define KEY1_BUTTON_EINT_PORT_SOURCE      GPIO_PORT_SOURCE_F
#define KEY1_BUTTON_EINT_PIN_SOURCE       GPIO_PIN_SOURCE_9
#define KEY1_BUTTON_EINT_IRQn             EINT9_5_IRQn

/**
 * @brief Key2 push-button
 */
#define KEY2_BUTTON_PIN                   GPIO_PIN_13
#define KEY2_BUTTON_GPIO_PORT             GPIOC
#define KEY2_BUTTON_GPIO_CLK              RCM_APB2_PERIPH_GPIOC
#define KEY2_BUTTON_EINT_LINE             EINT_LINE_13
#define KEY2_BUTTON_EINT_PORT_SOURCE      GPIO_PORT_SOURCE_C
#define KEY2_BUTTON_EINT_PIN_SOURCE       GPIO_PIN_SOURCE_13
#define KEY2_BUTTON_EINT_IRQn             EINT15_10_IRQn

/**
 * @brief Key3 push-button
 */
#define KEY3_BUTTON_PIN                   GPIO_PIN_0
#define KEY3_BUTTON_GPIO_PORT             GPIOA
#define KEY3_BUTTON_GPIO_CLK              RCM_APB2_PERIPH_GPIOA
#define KEY3_BUTTON_EINT_LINE             EINT_LINE_0
#define KEY3_BUTTON_EINT_PORT_SOURCE      GPIO_PORT_SOURCE_A
#define KEY3_BUTTON_EINT_PIN_SOURCE       GPIO_PIN_SOURCE_0
#define KEY3_BUTTON_EINT_IRQn             EINT0_IRQn

#define COMn                            5

/**
 * @brief Definition for COM port1, connected to USART1
 */
#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK                    RCM_APB2_PERIPH_USART1
#define EVAL_COM1_TX_PIN                 GPIO_PIN_9
#define EVAL_COM1_TX_GPIO_PORT           GPIOA
#define EVAL_COM1_TX_GPIO_CLK            RCM_APB2_PERIPH_GPIOA
#define EVAL_COM1_RX_PIN                 GPIO_PIN_10
#define EVAL_COM1_RX_GPIO_PORT           GPIOA
#define EVAL_COM1_RX_GPIO_CLK            RCM_APB2_PERIPH_GPIOA
#define EVAL_COM1_IRQn                   USART1_IRQn

/**
 * @brief Definition for COM port2, connected to USART2
 */
#define EVAL_COM2                        USART2
#define EVAL_COM2_CLK                    RCM_APB1_PERIPH_USART2
#define EVAL_COM2_TX_PIN                 GPIO_PIN_2
#define EVAL_COM2_TX_GPIO_PORT           GPIOA
#define EVAL_COM2_TX_GPIO_CLK            RCM_APB2_PERIPH_GPIOA
#define EVAL_COM2_RX_PIN                 GPIO_PIN_3
#define EVAL_COM2_RX_GPIO_PORT           GPIOA
#define EVAL_COM2_RX_GPIO_CLK            RCM_APB2_PERIPH_GPIOA
#define EVAL_COM2_IRQn                   USART2_IRQn

/**
 * @brief Definition for COM port3, connected to USART2
 */
#define EVAL_COM3                        USART3
#define EVAL_COM3_CLK                    RCM_APB1_PERIPH_USART3
#define EVAL_COM3_TX_PIN                 GPIO_PIN_8
#define EVAL_COM3_TX_GPIO_PORT           GPIOD
#define EVAL_COM3_TX_GPIO_CLK            RCM_APB2_PERIPH_GPIOD
#define EVAL_COM3_RX_PIN                 GPIO_PIN_9
#define EVAL_COM3_RX_GPIO_PORT           GPIOD
#define EVAL_COM3_RX_GPIO_CLK            RCM_APB2_PERIPH_GPIOD
#define EVAL_COM3_IRQn                   USART3_IRQn

/**
 * @brief Definition for COM port4, connected to USART2
 */
#define EVAL_COM4                        UART4
#define EVAL_COM4_CLK                    RCM_APB1_PERIPH_UART4
#define EVAL_COM4_TX_PIN                 GPIO_PIN_2
#define EVAL_COM4_TX_GPIO_PORT           GPIOA
#define EVAL_COM4_TX_GPIO_CLK            RCM_APB2_PERIPH_GPIOA
#define EVAL_COM4_RX_PIN                 GPIO_PIN_3
#define EVAL_COM4_RX_GPIO_PORT           GPIOA
#define EVAL_COM4_RX_GPIO_CLK            RCM_APB2_PERIPH_GPIOA
#define EVAL_COM4_IRQn                   UART4_IRQn

/**
 * @brief Definition for COM port5, connected to USART2
 */
#define EVAL_COM5                        UART5
#define EVAL_COM5_CLK                    RCM_APB1_PERIPH_UART5
#define EVAL_COM5_TX_PIN                 GPIO_PIN_12
#define EVAL_COM5_TX_GPIO_PORT           GPIOC
#define EVAL_COM5_TX_GPIO_CLK            RCM_APB2_PERIPH_GPIOC
#define EVAL_COM5_RX_PIN                 GPIO_PIN_2
#define EVAL_COM5_RX_GPIO_PORT           GPIOD
#define EVAL_COM5_RX_GPIO_CLK            RCM_APB2_PERIPH_GPIOD
#define EVAL_COM5_IRQn                   UART5_IRQn

/* Exported function prototypes *******************************************/

void APM_EVAL_LEDInit(Led_TypeDef Led);
void APM_EVAL_LEDOn(Led_TypeDef Led);
void APM_EVAL_LEDOff(Led_TypeDef Led);
void APM_EVAL_LEDToggle(Led_TypeDef Led);

void APM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t APM_EVAL_PBGetState(Button_TypeDef Button);

void APM_EVAL_COMInit(COM_TypeDef COM, USART_Config_T* configStruct);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_APM32E103_EVAL_H */

