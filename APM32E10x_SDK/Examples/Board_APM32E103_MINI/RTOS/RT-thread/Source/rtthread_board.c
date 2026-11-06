/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-24                  the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include "Board.h"

#define _SCB_BASE       (0xE000E010UL)
#define _SYSTICK_CTRL   (*(rt_uint32_t *)(_SCB_BASE + 0x0))
#define _SYSTICK_LOAD   (*(rt_uint32_t *)(_SCB_BASE + 0x4))
#define _SYSTICK_VAL    (*(rt_uint32_t *)(_SCB_BASE + 0x8))
#define _SYSTICK_CALIB  (*(rt_uint32_t *)(_SCB_BASE + 0xC))
#define _SYSTICK_PRI    (*(rt_uint8_t  *)(0xE000ED23UL))

// Updates the variable SystemCoreClock and must be called
// whenever the core clock is changed during program execution.
extern void SystemCoreClockUpdate(void);

// Holds the system core clock, which is the system clock
// frequency supplied to the SysTick timer and the processor
// core clock.
extern uint32_t SystemCoreClock;

static rt_uint32_t _SysTick_Config(rt_uint32_t ticks)
{
    if ((ticks - 1) > 0xFFFFFF)
    {
        return 1;
    }

    _SYSTICK_LOAD = ticks - 1;
    _SYSTICK_PRI = 0xFF;
    _SYSTICK_VAL  = 0;
    _SYSTICK_CTRL = 0x07;

    return 0;
}

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
/*
 * Please modify RT_HEAP_SIZE if you enable RT_USING_HEAP
 * the RT_HEAP_SIZE max value = (sram size - ZI size), 1024 means 1024 bytes
 */

/* Internal SRAM memory size[Kbytes] */
#define APM32_SRAM_SIZE      128
#define APM32_SRAM_END       (0x20000000 + APM32_SRAM_SIZE * 1024)

#if defined(__ARMCC_VERSION)
    extern int Image$$RW_IRAM1$$ZI$$Limit;
    #define HEAP_BEGIN      ((void *)&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
    #pragma section="CSTACK"
    #define HEAP_BEGIN      (__segment_end("CSTACK"))
#else
    extern int __bss_end;
    #define HEAP_BEGIN      ((void *)&__bss_end)
#endif

#define HEAP_END        APM32_SRAM_END


RT_WEAK void* rt_heap_begin_get(void)
{
    return HEAP_BEGIN;
}

RT_WEAK void* rt_heap_end_get(void)
{
    return (void*)HEAP_END;
}


/*
#define RT_HEAP_SIZE (30*1024)
static rt_uint8_t rt_heap[RT_HEAP_SIZE];

RT_WEAK void* rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void* rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}*/
#endif

void rt_os_tick_callback(void)
{
    rt_interrupt_enter();

    rt_tick_increase();

    rt_interrupt_leave();
}
static int uart_init(void);
/**
 * This function will initial your board.
 */
void rt_hw_board_init(void)
{
//#error "TODO 1: OS Tick Configuration."
    /*
     * TODO 1: OS Tick Configuration
     * Enable the hardware timer and call the rt_os_tick_callback function
     * periodically with the frequency RT_TICK_PER_SECOND.
     */
    SystemCoreClockUpdate();
    _SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
    #ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
    #endif

    #if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
    #endif
}

void SysTick_Handler(void)
{
    rt_os_tick_callback();
}

static int uart_init(void)
{
    static USART_Config_T UartHandle;
    GPIO_Config_T GPIO_configStruct;
    GPIO_ConfigStructInit(&GPIO_configStruct);

    /* Enable GPIO clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    /* Configure USART Tx as alternate function push-pull */
    GPIO_configStruct.mode = GPIO_MODE_AF_PP;
    GPIO_configStruct.pin = GPIO_PIN_9;
    GPIO_configStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &GPIO_configStruct);

    /* Configure USART Rx as input floating */
    GPIO_configStruct.mode = GPIO_MODE_IN_FLOATING;
    GPIO_configStruct.pin = GPIO_PIN_10;
    GPIO_Config(GPIOA, &GPIO_configStruct);

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_USART1);

    /* USARTx configuration */
    /* USARTx configured as follow:
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled
    */

    UartHandle.baudRate = 115200;
    UartHandle.mode = USART_MODE_TX_RX;
    UartHandle.parity = USART_PARITY_NONE;
    UartHandle.stopBits = USART_STOP_BIT_1;
    UartHandle.wordLength = USART_WORD_LEN_8B;
    UartHandle.hardwareFlow = USART_HARDWARE_FLOW_NONE;

    USART_Config(USART1, &UartHandle);
    USART_Enable(USART1);
    return 0;
}
INIT_BOARD_EXPORT(uart_init);

void rt_hw_console_output( const char* str )
{

    rt_enter_critical();

    while (*str != '\0')
    {
        if (*str == '\n')
        {
            while (USART_ReadStatusFlag( USART1, USART_FLAG_TXBE)==RESET);

            USART_TxData(USART1, '\r');
        }

        while (USART_ReadStatusFlag( USART1, USART_FLAG_TXBE)==RESET);

        USART_TxData(USART1, *str++ );
    }

    rt_exit_critical();
}

char rt_hw_console_getchar(void)
{
    int ch = -1;

    if (USART_ReadStatusFlag(USART1, USART_FLAG_RXBNE) != RESET)
    {
        ch = (int)USART_RxData(USART1);
        USART_ClearStatusFlag(USART1, USART_FLAG_RXBNE);
    }
    else
    {
        if (USART_ReadStatusFlag(USART1, USART_FLAG_OVRE) != RESET)
        {
            USART_ClearStatusFlag(USART1, USART_FLAG_OVRE);
        }

        rt_thread_mdelay(10);
    }

    return ch;
}

