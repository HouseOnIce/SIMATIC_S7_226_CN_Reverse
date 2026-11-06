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
  * @brief SDIO SDCard examples
  @{
  */

/** @addtogroup SDIO_SDCard
  @{
  */

/** @defgroup SDIO_SDCard_Macros Macros
  @{
*/

/* SDIO read or write file path */
#define PATH_TEST   "0:SDIO_Test.txt"

/**@} end of group SDIO_SDCard_Macros*/

/** @defgroup SDIO_SDCard_Variables Variables
  @{
  */

/* Record SD card command and data status */
FRESULT g_status = FR_OK;
/* Number of the file that creat by soft */
/* Filesystem object */
FATFS g_fatFs;
/* Use to Create the File system */
BYTE g_work[FF_MAX_SS];
/* Number of bytes written */
UINT g_fnum;
/* File object */
FIL g_fileNew;
/* Data to Write to the file */
BYTE g_writeBuffer[] = "Geehy SDIO Example File System Test\r\n";
/* Use to Scan the file */
char g_fileScanPath[256] = "0:";

/* Menu title string */
const char* menuTitleStr[] =
{
    " SDIO SDCard Example  ",
};

/* Menu bottom column string */
const char* menuBottomColStr[] =
{
    "   GEEHY SEMICONDUCTOR   ",
};

/**@} end of group SDIO_SDCard_Variables*/

/** @defgroup SDIO_SDCard_Functions Functions
  @{
  */

void APM_Init(void);
FRESULT FATFS_ScanFiles(char* path);

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
    uint8_t keyType;

    SystemClockConfig();

    APM_Init();

    printf("This is a SDIO SDCard Demo\r\n");

    /* Wait board and module power on */
    Delay_ms(200);

    /* Init SD Card */
    if (SD_Init() == SD_OK)
    {
        APM_MINI_LEDOff(LED3);
        printf("SD Init Success!\r\n");
        g_status = FR_OK;

        /* Show SD card informations */
        SD_ShowCardInfo();
    }
    else
    {
        APM_MINI_LEDOn(LED3);
        printf("SD Init Fail!\r\n");
        g_status = FR_DISK_ERR;
    }

    if (g_status == FR_OK)
    {
        /* mount the file system */
        g_status = f_mount(&g_fatFs, "0:", 1);

        if (g_status == FR_OK)
        {
            printf("File System mount Success!\r\n");
        }
        else
        {
            if (g_status == FR_NO_FILESYSTEM)
            {
                printf("Can not mount file system,\r\n");
                printf("You can try to press KEY1 twice to Create it. \r\n");
                printf("NOTICE: The data in SD card maybe lost!!!\r\n");

                while (Key_Scan(KEY_MODE_SINGLE) != KEY1_PRESSED);
                while (Key_Scan(KEY_MODE_SINGLE) != KEY1_PRESSED);

                g_status = f_mkfs("0:", NULL, g_work, sizeof(g_work));
                if (g_status == FR_OK)
                {
                    printf("Create the File System Success!\r\n");

                    g_status = f_mount(&g_fatFs, "0:", 1);
                    if (g_status == FR_OK)
                    {
                        printf("File System mount Success!\r\n");
                    }
                    else
                    {
                        printf("File System mount Fail!\r\n");
                    }
                }
                else
                {
                    printf("Create the File System Fail!\r\n");
                }
            }
            else
            {
                printf("File System mount Fail!\r\n");
            }
        }
    }
    printf("\r\n");

    while (1)
    {
        keyType = Key_Scan(KEY_MODE_SINGLE);

        /* Scan the files in file system  */
        if (keyType == KEY1_PRESSED)
        {
            printf(">> Scan the files in file system :\r\n");

            if (g_status == FR_OK)
            {
                g_status = FATFS_ScanFiles(g_fileScanPath);
                if (g_status != FR_OK)
                {
                    printf("Scan files Fail!\r\n");
                }
            }
            while (KEY1 == 0);
        }
        /* Write a file to SD card */
        else if (keyType == KEY2_PRESSED)
        {
            printf(">> Write a file to SD card\r\n");

            if (g_status == FR_OK)
            {
                g_status = f_open(&g_fileNew, PATH_TEST, FA_CREATE_ALWAYS | FA_WRITE);
                if (g_status == FR_OK)
                {
                    g_status = f_write(&g_fileNew, g_writeBuffer, sizeof(g_writeBuffer), &g_fnum);
                }
                /* Show the status */
                if (g_status == FR_OK)
                {
                    printf("Write the file to SD Card Success!\r\n");
                }
                else
                {
                    printf("Write the file to SD Card Fail!\r\n");
                }
            }
            f_close(&g_fileNew);
            while (KEY2 == 0);
        }
    }
}

/*!
 * @brief       Init board
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void APM_Init(void)
{
    USART_Config_T usartConfigStruct;

    /* Set system interrupt priority grouping */
    NVIC_ConfigPriorityGroup(NVIC_PRIORITY_GROUP_2);

    /* Init delay function */
    Delay_Init();

    /* Init LED */
    APM_MINI_LEDInit(LED2);
    APM_MINI_LEDInit(LED3);

    /* Init USART1 */
    usartConfigStruct.baudRate = 115200;
    usartConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    usartConfigStruct.mode = USART_MODE_TX_RX;
    usartConfigStruct.parity = USART_PARITY_NONE;
    usartConfigStruct.stopBits = USART_STOP_BIT_1;
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;
    APM_MINI_COMInit(COM1, &usartConfigStruct);

    /* Init KEY */
    APM_MINI_PBInit(BUTTON_KEY2, BUTTON_MODE_GPIO);
    APM_MINI_PBInit(BUTTON_KEY1, BUTTON_MODE_GPIO);
}

/*!
 * @brief       Scan files in file system
 *
 * @param       path: Path of the scan file
 *
 * @retval      File function return code
 *
 * @note
 */
FRESULT FATFS_ScanFiles(char* path)
{
    FRESULT status;
    /* file informations */
    FILINFO fileInfo;
    /* file directory */
    DIR dir;
    /* file name */
    char* fileName;
    uint32_t lengthScanPath;
    fileInfo.fsize = sizeof(fileInfo.fname);

    /* open directory */
    status = f_opendir(&dir, (const TCHAR*)path);
    if (status == FR_OK)
    {
        lengthScanPath = strlen(path);
        while (1)
        {
            /* Read the contents of the directory */
            status = f_readdir(&dir, &fileInfo);

            /* Error or read all items complete */
            if ((status != FR_OK) || (fileInfo.fname[0] == 0))
            {
                break;
            }
#if FF_USE_LFN
            fileName = *fileInfo.fname ? fileInfo.fname : fileInfo.altname;
#else
            fileName = fileInfo.fname;
#endif
            /* Ignore parent directory */
            if (*fileName == '.')
            {
                continue;
            }
            /* It is a directory */
            if (fileInfo.fattrib & AM_DIR)
            {
                sprintf(&path[lengthScanPath], "/%s", fileName);

                /* recursive */
                status = FATFS_ScanFiles(path);
                path[lengthScanPath] = 0;
                if (status != FR_OK)
                {
                    break;
                }
            }
            /* It is a file */
            else
            {
                printf("%s/%s\r\n", path, fileName);
            }
        }
    }

    return status;
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
    USART_TxData(USART1, (uint8_t)ch);

    /* wait for the data to be send */
    while (USART_ReadStatusFlag(USART1, USART_FLAG_TXBE) == RESET);

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
    USART_TxData(USART1, ch);

    /* wait for the data to be send */
    while (USART_ReadStatusFlag(USART1, USART_FLAG_TXBE) == RESET);

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

/**@} end of group SDIO_SDCard_Functions */
/**@} end of group SDIO_SDCard */
/**@} end of group Examples */
