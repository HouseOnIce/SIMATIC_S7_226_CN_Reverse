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

/* Includes ***************************************************************/
#include "main.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

typedef enum {FALSE, TRUE} BOOL;

/* printf using USART1  */
#define DEBUG_USART           USART1

/* Buffer Size */
#define DATA_BUF_SIZE         (56)

/* Private variables ******************************************************/

W25Q16_INFO_T w25q16Info;

/* Timer tick */
uint16_t cntTick = 0;

uint16_t txDataBufSPI1[DATA_BUF_SIZE] = {0};
uint16_t rxDataBufSPI1[DATA_BUF_SIZE] = {0};

/** SPI TX Buffer*/
const uint16_t SPI_Data_TX[DATA_BUF_SIZE] =
{
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0X07,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0X07,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0X07,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0X07,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0X07,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0X07,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0X07,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0X07,
};

/* Menu title string*/
const char *menuTitleStr[] = {
    "   SPI Flash Example    ",
};

/* Menu item string*/
const char *menuItemStr[] = {
    "Press KEY1 to start test",
    "       Test OK          ",
    "       Test Error       ",
    "       Testing          ",
    "Can not detect W25Q16 ID",
};

/* Menu bottom column string*/
const char *menuBottomColStr[] = {
    "   GEEHY SEMICONDUCTOR   ",
};

/* Private function prototypes ********************************************/

void APM_EVAL_Init(void);
void W25Q16_SPI_Test(void);
void ADC1_Init(void);
void Menu_DisplayInit(void);

void APM_EVAL_TMR7_Init(uint16_t period, uint16_t div);

void Print_Buffer(uint16_t* pBuffer, uint16_t BufferLength);
uint8_t BufferCompare(uint16_t* buf1, uint16_t* buf2, uint8_t size);

/* External variables *****************************************************/

/* External functions *****************************************************/

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
    SystemClockConfig();

    char str1[20];
    char str2[20];
    
    APM_EVAL_Init();
    printf("This is an example of W25Q16 flash operation \r\n");
    
    /* Get Flash Device ID */
    w25q16Info.deviceID = W25Q16_ReadFlashDeviceID();
    
    APM_EVAL_DelayMs(200);
    
    /* Get Flash ID */
    w25q16Info.flashID = W25Q16_ReadFlashID();
    
    sprintf(str1, "Flash  ID:0x%lX",w25q16Info.flashID);
    sprintf(str2, "Device ID:0x%lX",w25q16Info.deviceID);
    
    LCD_DisplayString(10,LCD_LINE_0,str1,LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    LCD_DisplayString(10,LCD_LINE_1,str2,LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    
    if(w25q16Info.flashID == W25Q16_FLASH_ID)
    {
        printf("SPI FLASH W25Q16 detected!\r\n");
        printf("Press KEY1 to start test.\r\n");
        
        while (1)
        {
            if(APM_EVAL_PBGetState(BUTTON_KEY1) == 0)
            {
                W25Q16_SPI_Test();
                
                while(APM_EVAL_PBGetState(BUTTON_KEY1) == 0);
            }
        }
    }
    else
    {
        APM_EVAL_LEDOn(LED3);
        LCD_DisplayString(10,LCD_LINE_2,menuItemStr[4],LCD_TEXT_FC,LCD_TEXT_BC,16,0);
        printf("Can not detect W25Q16 ID!\n\r");
    }
}

/*!
 * @brief       Init EVAL board
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void APM_EVAL_Init(void)
{
    USART_Config_T usartConfigStruct;

    /* Init delay function */
    APM_EVAL_DelayInit();

    APM_EVAL_LEDInit(LED1);
    APM_EVAL_LEDInit(LED2);
    APM_EVAL_LEDInit(LED3);

    APM_EVAL_PBInit(BUTTON_KEY1, BUTTON_MODE_GPIO);
    APM_EVAL_PBInit(BUTTON_KEY2, BUTTON_MODE_GPIO);
    APM_EVAL_PBInit(BUTTON_KEY3, BUTTON_MODE_GPIO);

    usartConfigStruct.baudRate = 115200;
    usartConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    usartConfigStruct.mode = USART_MODE_TX_RX;
    usartConfigStruct.parity = USART_PARITY_NONE;
    usartConfigStruct.stopBits = USART_STOP_BIT_1;
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;

    APM_EVAL_COMInit(COM1, &usartConfigStruct);

    APM_EVAL_TMR7_Init(1000, 72);
    APM_LCDInit();
    LCD_Clear(0, 0, LCD_WIDTH, LCD_HEIGHT, LCD_BCOLOR);

    APM_EVAL_DelayMs(100);

    Menu_DisplayInit();
    
    /* SPI init*/
    W25Q16_SPI_Init();
}
/*!
 * @brief       Using SPI operate W25Q16 flash
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void W25Q16_SPI_Test(void)
{
    uint16_t i = 0;
    
    LCD_DisplayString(10,LCD_LINE_2,menuItemStr[3],LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    APM_EVAL_LEDOff(LED2);
    APM_EVAL_LEDOff(LED3);

    /* initialization  Buffer*/
    for (i = 0; i < DATA_BUF_SIZE; i++)
    {
        txDataBufSPI1[i] = SPI_Data_TX[i] + 0x1010;
        rxDataBufSPI1[i] = 0;
    }

    printf("SPI write Data to flash \r\n");

    /* Erase SPI FLASH Sector */
    W25Q16_EraseSector(W25Q16_FLASH_WRITE_ADDR);

    /* Write data to flash */
    W25Q16_WriteBuffer((uint8_t*)txDataBufSPI1, W25Q16_FLASH_WRITE_ADDR, DATA_BUF_SIZE * 2);
    printf("\r\nWrite data: \r\n");
    Print_Buffer(txDataBufSPI1, DATA_BUF_SIZE);

    /* Read data from flash */
    W25Q16_ReadBuffer((uint8_t*)rxDataBufSPI1, W25Q16_FLASH_READ_ADDR, DATA_BUF_SIZE * 2);
    printf("\r\nRead data: \r\n");
    Print_Buffer(rxDataBufSPI1, DATA_BUF_SIZE);

    /* Compare receive Buffer */
    /* Data is ok then turn on LED2 */
    if (BufferCompare(txDataBufSPI1, rxDataBufSPI1, DATA_BUF_SIZE) == TRUE)
    {
        APM_EVAL_LEDOn(LED2);
        APM_EVAL_LEDOff(LED3);
        LCD_DisplayString(10,LCD_LINE_2,menuItemStr[1],LCD_TEXT_FC,LCD_TEXT_BC,16,0);
        printf("\r\n16M flash(W25Q16) test OK! LED2 on\n\r");
    }
    else
    {
        APM_EVAL_LEDOff(LED2);
        APM_EVAL_LEDOn(LED3);
        LCD_DisplayString(10,LCD_LINE_2,menuItemStr[2],LCD_TEXT_FC,LCD_TEXT_BC,16,0);
        printf("\r\n16M flash(W25Q16) test fail! LED3 on\n\r");
    }
}

/*!
 * @brief       Initialize the Menu UI
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void Menu_DisplayInit(void)
{
    /* Menu title*/
    LCD_Clear(0,0,LCD_WIDTH,40,LCD_FCOLOR);
    LCD_DisplayString(10,LCD_LINE_TITLE,menuTitleStr[0],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,16,0);
    
    /* Menu item*/
    LCD_DisplayString(10,LCD_LINE_2,menuItemStr[0],LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    
    /* Menu bottom*/
    LCD_Clear(0,LCD_LINE_BOTTOM,LCD_WIDTH,LCD_HEIGHT,LCD_FCOLOR);
    LCD_DisplayString(60,LCD_LINE_BOTTOM,menuBottomColStr[0],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,16,0);
}

/*!
 * @brief       TMR7 init
 *
 * @param       None
 *
 * @retval      None
 *
 */
void APM_EVAL_TMR7_Init(uint16_t period, uint16_t div)
{
    TMR_BaseConfig_T  timeBaseConfig;

    /* Enable Clock*/
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR7);

    /* Set clockDivision = 1 */
    timeBaseConfig.clockDivision =  TMR_CLOCK_DIV_1;
    /* Up-counter */
    timeBaseConfig.countMode =  TMR_COUNTER_MODE_UP;
    /* Set divider */
    timeBaseConfig.division = div - 1 ;
    /* Set counter = 0xffff */
    timeBaseConfig.period = period - 1;
    /* Repetition counter = 0x0 */
    timeBaseConfig.repetitionCounter =  0;

    TMR_ConfigTimeBase(TMR7, &timeBaseConfig);

    /* Enable update interrupt*/
    TMR_EnableInterrupt(TMR7, TMR_INT_UPDATE);
    NVIC_EnableIRQRequest(TMR7_IRQn, 2, 2);

    /*  Enable TMR7  */
    TMR_Enable(TMR7);
}

/*!
 * @brief       TMR4 interrupt service runtine
 *
 * @param       None
 *
 * @retval      None
 *
 * @note        This function need to put into TMR7_IRQHandler() in apm32f0xx_int.c
 */
void TMR7_Isr(void)
{
    if (TMR_ReadIntFlag(TMR7, TMR_INT_UPDATE) == SET)
    {
        TMR_ClearIntFlag(TMR7, TMR_INT_UPDATE);
        cntTick++;

        if (cntTick >= 500)
        {
            cntTick = 0;
            APM_EVAL_LEDToggle(LED1);
        }
    }
}

/*!
 * @brief       Compares two buffers
 *
 * @param       buf1:    First buffer to be compared
 *
 * @param       buf1:    Second buffer to be compared
 *
 * @param       size:    Buffer size
 *
 * @retval      Return TRUE if buf1 = buf2. If not then return FALSE
 */
uint8_t BufferCompare(uint16_t* buf1, uint16_t* buf2, uint8_t size)
{
    uint8_t i;

    for (i = 0; i < size; i++)
    {
        if (buf1[i] != buf2[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*!
 * @brief       Print Buffer Data
 *
 * @param       pBuffer:buffer
 *
 * @param       length : length of the Buffer
 *
 * @retval      None
 */
void Print_Buffer(uint16_t* pBuffer, uint16_t BufferLength)
{
    uint16_t i;

    for (i=0; i  < BufferLength; i++)
    {
        printf("0x%04X  ", pBuffer[i]);

        if ((i+1)%8 == 0)
        {
            printf("\r\n");
        }
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
	UNUSED(file);
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

