/*!
 * @file        usbd_descriptor.c
 *
 * @brief       usb device descriptor configuration
 *
 * @version     V1.0.3
 *
 * @date        2025-06-17
 *
 * @attention
 *
 *  Copyright (C) 2023-2025 Geehy Semiconductor
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
#include "usbd_descriptor.h"
#include "usbd_composite.h"
#include <stdio.h>
#include <string.h>

/* Private includes *******************************************************/

/* Private macro **********************************************************/

#define USBD_GEEHY_VID              12619
#define USBD_FS_PID                 0x010E
#define USBD_LANGID_STR             0x0409
#define USBD_MANUFACTURER_STR       "Geehy"
#define USBD_PRODUCT_STR            "APM32 Composite"
#define USBD_CONFIGURATION_STR      "Composite Config"
#define USBD_INTERFACE_STR          "Composite Interface"

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

static USBD_DESC_INFO_T USBD_FS_DeviceDescHandler(uint8_t usbSpeed);
static USBD_DESC_INFO_T USBD_FS_ConfigDescHandler(uint8_t usbSpeed);
static USBD_DESC_INFO_T USBD_FS_ConfigStrDescHandler(uint8_t usbSpeed);
static USBD_DESC_INFO_T USBD_FS_InterfaceStrDescHandler(uint8_t usbSpeed);
static USBD_DESC_INFO_T USBD_FS_LangIdStrDescHandler(uint8_t usbSpeed);
static USBD_DESC_INFO_T USBD_FS_ManufacturerStrDescHandler(uint8_t usbSpeed);
static USBD_DESC_INFO_T USBD_FS_ProductStrDescHandler(uint8_t usbSpeed);
static USBD_DESC_INFO_T USBD_FS_SerialStrDescHandler(uint8_t usbSpeed);
#if USBD_SUP_LPM
static USBD_DESC_INFO_T USBD_FS_BosDescHandler(uint8_t usbSpeed);
#endif
static USBD_DESC_INFO_T USBD_FS_WinUsbOsStrDescHandler(uint8_t usbSpeed);
static USBD_DESC_INFO_T USBD_OtherSpeedConfigDescHandler(uint8_t usbSpeed);
static USBD_DESC_INFO_T USBD_DevQualifierDescHandler(uint8_t usbSpeed);

/* USB device descripotr handler */
USBD_DESC_T USBD_DESC_FS =
{
    "Composite Descriptor",
    USBD_FS_DeviceDescHandler,
    USBD_FS_ConfigDescHandler,
    USBD_FS_ConfigStrDescHandler,
    USBD_FS_InterfaceStrDescHandler,
    USBD_FS_LangIdStrDescHandler,
    USBD_FS_ManufacturerStrDescHandler,
    USBD_FS_ProductStrDescHandler,
    USBD_FS_SerialStrDescHandler,
#if USBD_SUP_LPM
    USBD_FS_BosDescHandler,
#endif
    USBD_FS_WinUsbOsStrDescHandler,
    USBD_OtherSpeedConfigDescHandler,
    USBD_DevQualifierDescHandler,
};

/**
 * @brief   Device descriptor
 */
uint8_t USBD_DeviceDesc[USBD_DEVICE_DESCRIPTOR_SIZE] =
{
    /* bLength */
    0x12,
    /* bDescriptorType */
    USBD_DESC_DEVICE,
    /* bcdUSB */
#if USBD_SUP_LPM
    0x01,            /*<! For resume test of USBCV3.0. Only support LPM USB device */
#else
    0x00,
#endif
    0x02,
    /* bDeviceClass */
    0xEF,
    /* bDeviceSubClass */
    0x02,
    /* bDeviceProtocol */
    0x01,
    /* bMaxPacketSize */
    USBD_EP0_PACKET_MAX_SIZE,
    /* idVendor */
    USBD_GEEHY_VID & 0xFF, USBD_GEEHY_VID >> 8,
    /* idProduct */
    USBD_FS_PID & 0xFF, USBD_FS_PID >> 8,
    /* bcdDevice = 2.00 */
    0x00, 0x02,
    /* Index of string descriptor describing manufacturer */
    USBD_DESC_STR_MFC,
    /* Index of string descriptor describing product */
    USBD_DESC_STR_PRODUCT,
    /* Index of string descriptor describing the device serial number */
    USBD_DESC_STR_SERIAL,
    /* bNumConfigurations */
    USBD_SUP_CONFIGURATION_MAX_NUM,
};

/**
 * @brief   WinUSB OS descriptor
 */
uint8_t USBD_WinUsbOsString[USBD_WINUSB_OS_STRING_SIZE] =
{
    /* bLength */
    0x12,
    /* bDescriptorType */
    USBD_DESC_STRING,
    /* wcChar0 */
    'M', 0x00,
    /* wcChar1 */
    'S', 0x00,
    /* wcChar2 */
    'F', 0x00,
    /* wcChar3 */
    'T', 0x00,
    /* wcChar4 */
    '1', 0x00,
    /* wcChar5 */
    '0', 0x00,
    /* wcChar6 */
    '0', 0x00,
    /* bVendorCode */
    USBD_VEN_REQ_MS_CODE, 
    /* bReserved */
    0x00,
};

/**
 * @brief   Configuration descriptor
 */
uint8_t USBD_ConfigDesc[USBD_CONFIG_DESCRIPTOR_SIZE] =
{
    /* bLength */
    0x09,
    /* bDescriptorType */
    USBD_DESC_CONFIGURATION,
    /* wTotalLength */
    USBD_CONFIG_DESCRIPTOR_SIZE & 0xFF,
    USBD_CONFIG_DESCRIPTOR_SIZE >> 8,

    /* bNumInterfaces */
    0x02,
    /* bConfigurationValue */
    0x01,
    /* iConfiguration */
    USBD_DESC_STR_PRODUCT,
    /* bmAttributes */
#if USBD_SUP_SELF_PWR
    0xC0,
#else
    0x80,
#endif
    /* MaxPower */
    0x32,

    /********************  Custom HID ********************/
    /********************  Standard Interface Association Descriptor ********************/
    0x08,                                               /* bLength: Interface Descriptor size */
    USBD_DESC_IAD,                                      /* bDescriptorType: */
    USBD_CUSTOM_HID_INTERFACE_NUM,                      /* bFirstInterface */
    USBD_CUSTOM_HID_INTERFACE_CNT,                      /* bInterfaceCount */
    USBD_HID_ITF_CLASS_ID,                              /* bFunctionClass: HID */
    USBD_HID_SUB_CLASS_BOOT,                            /* bFunctionSubClass */
    USBD_HID_ITF_PORTOCOL_KEYBOARD,                     /* bInterfaceProtocol */
    USBD_DESC_STR_PRODUCT,                              /* iFunction */

    /******************** CUSTOM HID Interface ********************/
    0x09,                                               /* bLength: Interface Descriptor size */
    USBD_DESC_INTERFACE,                                /* bDescriptorType: */
    USBD_CUSTOM_HID_INTERFACE_NUM,                      /* bInterfaceNumber: Number of Interface */
    0x00,                                               /* bAlternateSetting: Alternate setting */
    0x02,                                               /* bNumEndpoints */
    USBD_HID_ITF_CLASS_ID,                              /* bInterfaceClass: HID */
    USBD_HID_SUB_CLASS_BOOT,                            /* bInterfaceSubClass */
    USBD_HID_ITF_PORTOCOL_KEYBOARD,                     /* nInterfaceProtocol */
    0x00,                                               /* iInterface: */

    /********************  HID Descriptor ********************/
    /* bLength */
    0x09,
    /* bDescriptorType: HID */
    USBD_DESC_HID,
    /* bcdHID */
    0x11, 0x01,
    /* bCountryCode */
    0x00,
    /* bNumDescriptors */
    0x01,
    /* bDescriptorType */
    USBD_DESC_HID_REPORT,
    /* wItemLength */
    USBD_CUSTOM_HID_REPORT_DESC_SIZE & 0xFF,
    USBD_CUSTOM_HID_REPORT_DESC_SIZE >> 8,

    /********************  CUSTOM HID Endpoints ********************/
    0x07,                                               /* Endpoint descriptor length = 7 */
    USBD_DESC_ENDPOINT,                                 /* Endpoint descriptor type */
    USBD_CUSTOM_HID_OUT_EP_ADDR,                        /* Endpoint address (OUT, address 1) */
    0x03,                                               /* bmAttributes */
    USBD_CUSTOM_HID_OUT_EP_SIZE & 0xFF,
    USBD_CUSTOM_HID_OUT_EP_SIZE >> 8,
    USBD_CUSTOM_HID_FS_INTERVAL,                        /* Polling interval in milliseconds */
    
    0x07,                                               /* Endpoint descriptor length = 7 */
    USBD_DESC_ENDPOINT,                                 /* Endpoint descriptor type */
    USBD_CUSTOM_HID_IN_EP_ADDR,                         /* Endpoint address (IN, address 1) */
    0x03,                                               /* bmAttributes */
    USBD_CUSTOM_HID_IN_EP_SIZE & 0xFF,
    USBD_CUSTOM_HID_IN_EP_SIZE >> 8,
    USBD_CUSTOM_HID_FS_INTERVAL,                        /* Polling interval in milliseconds */
    
    /********************  WINUSB ********************/
    /********************  Standard Interface Association Descriptor ********************/
    0x08,                                               /* bLength: Interface Descriptor size */
    USBD_DESC_IAD,                                      /* bDescriptorType: */
    USBD_WINUSB_INTERFACE_NUM,                          /* bFirstInterface */
    USBD_WINUSB_INTERFACE_CNT,                          /* bInterfaceCount */
    0xFFU,                                              /* bFunctionClass: Vendor Specific */
    0x00U,                                              /* bFunctionSubClass */
    0x00U,                                              /* bInterfaceProtocol */
    USBD_DESC_STR_PRODUCT,                              /* iFunction */

    /********************  WINUSB Interface ********************/
    0x09,                                               /* bLength: Interface Descriptor size */
    USBD_DESC_INTERFACE,                                /* bDescriptorType: */
    USBD_WINUSB_INTERFACE_NUM,                          /* bInterfaceNumber: Number of Interface */
    0x00,                                               /* bAlternateSetting: Alternate setting */
    0x02,                                               /* bNumEndpoints */
    0xFF,                                               /* bInterfaceClass: Vendor Specific */
    0x00,                                               /* bInterfaceSubClass : SCSI transparent*/
    0x00,                                               /* nInterfaceProtocol */
    0x00,                                               /* iInterface: */

    /********************  WINUSB Endpoints ********************/
    0x07,                                               /* Endpoint descriptor length = 7 */
    USBD_DESC_ENDPOINT,                                 /* Endpoint descriptor type */
    USBD_WINUSB_OUT_EP_ADDR,                            /* Endpoint address (OUT, address 1) */
    0x02,                                               /* Bulk endpoint type */
    WINUSB_FS_MAX_PACKET_SIZE & 0xFF,
    WINUSB_FS_MAX_PACKET_SIZE >> 8,
    0x00,                                               /* Polling interval in milliseconds */
    
    0x07,                                               /* Endpoint descriptor length = 7 */
    USBD_DESC_ENDPOINT,                                 /* Endpoint descriptor type */
    USBD_WINUSB_IN_EP_ADDR,                             /* Endpoint address (IN, address 1) */
    0x02,                                               /* Bulk endpoint type */
    WINUSB_FS_MAX_PACKET_SIZE & 0xFF,
    WINUSB_FS_MAX_PACKET_SIZE >> 8,
    0x00,                                               /* Polling interval in milliseconds */
};

/**
 * @brief   Other speed configuration descriptor
 */
uint8_t USBD_OtherSpeedCfgDesc[USBD_CONFIG_DESCRIPTOR_SIZE] =
{
    /* bLength */
    0x09,
    /* bDescriptorType */
    USBD_DESC_OTHER_SPEED,
    /* wTotalLength */
    USBD_CONFIG_DESCRIPTOR_SIZE & 0xFF,
    USBD_CONFIG_DESCRIPTOR_SIZE >> 8,

    /* bNumInterfaces */
    0x02,
    /* bConfigurationValue */
    0x01,
    /* iConfiguration */
    USBD_DESC_STR_PRODUCT,
    /* bmAttributes */
#if USBD_SUP_SELF_PWR
    0xC0,
#else
    0x80,
#endif
    /* MaxPower */
    0x32,

    /********************  Custom HID ********************/
    /********************  Standard Interface Association Descriptor ********************/
    0x08,                                               /* bLength: Interface Descriptor size */
    USBD_DESC_IAD,                                      /* bDescriptorType: */
    USBD_CUSTOM_HID_INTERFACE_NUM,                      /* bFirstInterface */
    USBD_CUSTOM_HID_INTERFACE_CNT,                      /* bInterfaceCount */
    USBD_HID_ITF_CLASS_ID,                              /* bFunctionClass: HID */
    USBD_HID_SUB_CLASS_BOOT,                            /* bFunctionSubClass */
    USBD_HID_ITF_PORTOCOL_KEYBOARD,                     /* bInterfaceProtocol */
    USBD_DESC_STR_PRODUCT,                              /* iFunction */

    /******************** CUSTOM HID Interface ********************/
    0x09,                                               /* bLength: Interface Descriptor size */
    USBD_DESC_INTERFACE,                                /* bDescriptorType: */
    USBD_CUSTOM_HID_INTERFACE_NUM,                      /* bInterfaceNumber: Number of Interface */
    0x00,                                               /* bAlternateSetting: Alternate setting */
    0x02,                                               /* bNumEndpoints */
    USBD_HID_ITF_CLASS_ID,                              /* bInterfaceClass: HID */
    USBD_HID_SUB_CLASS_BOOT,                            /* bInterfaceSubClass */
    USBD_HID_ITF_PORTOCOL_KEYBOARD,                     /* nInterfaceProtocol */
    0x00,                                               /* iInterface: */

    /********************  HID Descriptor ********************/
    /* bLength */
    0x09,
    /* bDescriptorType: HID */
    USBD_DESC_HID,
    /* bcdHID */
    0x11, 0x01,
    /* bCountryCode */
    0x00,
    /* bNumDescriptors */
    0x01,
    /* bDescriptorType */
    USBD_DESC_HID_REPORT,
    /* wItemLength */
    USBD_CUSTOM_HID_REPORT_DESC_SIZE & 0xFF,
    USBD_CUSTOM_HID_REPORT_DESC_SIZE >> 8,

    /********************  CUSTOM HID Endpoints ********************/
    0x07,                                               /* Endpoint descriptor length = 7 */
    USBD_DESC_ENDPOINT,                                 /* Endpoint descriptor type */
    USBD_CUSTOM_HID_OUT_EP_ADDR,                        /* Endpoint address (OUT, address 1) */
    0x03,                                               /* bmAttributes */
    USBD_CUSTOM_HID_OUT_EP_SIZE & 0xFF,
    USBD_CUSTOM_HID_OUT_EP_SIZE >> 8,
    USBD_CUSTOM_HID_FS_INTERVAL,                        /* Polling interval in milliseconds */
    
    0x07,                                               /* Endpoint descriptor length = 7 */
    USBD_DESC_ENDPOINT,                                 /* Endpoint descriptor type */
    USBD_CUSTOM_HID_IN_EP_ADDR,                         /* Endpoint address (IN, address 1) */
    0x03,                                               /* bmAttributes */
    USBD_CUSTOM_HID_IN_EP_SIZE & 0xFF,
    USBD_CUSTOM_HID_IN_EP_SIZE >> 8,
    USBD_CUSTOM_HID_FS_INTERVAL,                        /* Polling interval in milliseconds */
    
    /********************  WINUSB ********************/
    /********************  Standard Interface Association Descriptor ********************/
    0x08,                                               /* bLength: Interface Descriptor size */
    USBD_DESC_IAD,                                      /* bDescriptorType: */
    USBD_WINUSB_INTERFACE_NUM,                          /* bFirstInterface */
    USBD_WINUSB_INTERFACE_CNT,                          /* bInterfaceCount */
    0xFFU,                                              /* bFunctionClass: Vendor Specific */
    0x00U,                                              /* bFunctionSubClass */
    0x00U,                                              /* bInterfaceProtocol */
    USBD_DESC_STR_PRODUCT,                              /* iFunction */

    /********************  WINUSB Interface ********************/
    0x09,                                               /* bLength: Interface Descriptor size */
    USBD_DESC_INTERFACE,                                /* bDescriptorType: */
    USBD_WINUSB_INTERFACE_NUM,                          /* bInterfaceNumber: Number of Interface */
    0x00,                                               /* bAlternateSetting: Alternate setting */
    0x02,                                               /* bNumEndpoints */
    0xFF,                                               /* bInterfaceClass: Vendor Specific */
    0x00,                                               /* bInterfaceSubClass : SCSI transparent*/
    0x00,                                               /* nInterfaceProtocol */
    0x00,                                               /* iInterface: */

    /********************  WINUSB Endpoints ********************/
    0x07,                                               /* Endpoint descriptor length = 7 */
    USBD_DESC_ENDPOINT,                                 /* Endpoint descriptor type */
    USBD_WINUSB_OUT_EP_ADDR,                            /* Endpoint address (OUT, address 1) */
    0x02,                                               /* Bulk endpoint type */
    WINUSB_FS_MAX_PACKET_SIZE & 0xFF,
    WINUSB_FS_MAX_PACKET_SIZE >> 8,
    0x00,                                               /* Polling interval in milliseconds */
    
    0x07,                                               /* Endpoint descriptor length = 7 */
    USBD_DESC_ENDPOINT,                                 /* Endpoint descriptor type */
    USBD_WINUSB_IN_EP_ADDR,                             /* Endpoint address (IN, address 1) */
    0x02,                                               /* Bulk endpoint type */
    WINUSB_FS_MAX_PACKET_SIZE & 0xFF,
    WINUSB_FS_MAX_PACKET_SIZE >> 8,
    0x00,                                               /* Polling interval in milliseconds */
};

#if USBD_SUP_LPM
/**
 * @brief   BOS descriptor
 */
uint8_t USBD_BosDesc[USBD_BOS_DESCRIPTOR_SIZE] =
{
    /* bLength */
    0x05,
    /* bDescriptorType */
    USBD_DESC_BOS,
    /* wtotalLength */
    0x0C, 0x00,
    /* bNumDeviceCaps */
    0x01,

    /* Device Capability */
    /* bLength */
    0x07,
    /* bDescriptorType */
    USBD_DEVICE_CAPABILITY_TYPE,
    /* bDevCapabilityType */
    USBD_20_EXTENSION_TYPE,
    /* bmAttributes */
    0x02, 0x00, 0x00, 0x00,
};
#endif

/**
 * @brief   Serial string descriptor
 */
uint8_t USBD_SerialStrDesc[USBD_SERIAL_STRING_SIZE] =
{
    USBD_SERIAL_STRING_SIZE,
    USBD_DESC_STRING,

    '0', 0,
    '0', 0,
    '0', 0,
    '0', 0,
    '0', 0,
    '0', 0,
    '0', 0,
    '0', 0,
    '0', 0,
    '0', 0,
    '0', 0,
    '1', 0
};

/**
 * @brief   Language ID string descriptor
 */
uint8_t USBD_LandIDStrDesc[USBD_LANGID_STRING_SIZE] =
{
    /* Size */
    USBD_LANGID_STRING_SIZE,
    /* bDescriptorType */
    USBD_DESC_STRING,
    USBD_LANGID_STR & 0xFF, USBD_LANGID_STR >> 8
};

/**
 * @brief   Device qualifier descriptor
 */
uint8_t USBD_DevQualifierDesc[USBD_DEVICE_QUALIFIER_DESCRIPTOR_SIZE] =
{
    /* Size */
    USBD_DEVICE_QUALIFIER_DESCRIPTOR_SIZE,
    /* bDescriptorType */
    USBD_DESC_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0xEF,
    0x02,
    0x01,
    0x40, /* In FS device*/
    0x01,
    0x00,
};

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief     USB device convert ascii string descriptor to unicode format
 *
 * @param     desc : descriptor string
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_DESC_Ascii2Unicode(uint8_t* desc)
{
    USBD_DESC_INFO_T descInfo;
    uint8_t* buffer;
    uint8_t str[USBD_SUP_STR_DESC_MAX_NUM];

    uint8_t* unicode = str;
    uint16_t length;
    uint8_t index = 0;

    if (desc == NULL)
    {
        descInfo.desc = NULL;
        descInfo.size = 0;
    }
    else
    {
        buffer = desc;
        length = (strlen((char*)buffer) * 2) + 2;
        /* Get unicode descriptor */
        unicode[index] = length;

        index++;
        unicode[index] = USBD_DESC_STRING;
        index++;

        while (*buffer != '\0')
        {
            unicode[index] = *buffer;
            buffer++;
            index++;

            unicode[index] = 0x00;
            index++;
        }
    }

    descInfo.desc = unicode;
    descInfo.size = length;

    return descInfo;
}

/*!
 * @brief     USB device FS device descriptor
 *
 * @param     usbSpeed : usb speed
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_FS_DeviceDescHandler(uint8_t usbSpeed)
{
    USBD_DESC_INFO_T descInfo;

    descInfo.desc = USBD_DeviceDesc;
    descInfo.size = sizeof(USBD_DeviceDesc);

    return descInfo;
}

/*!
 * @brief     USB device FS configuration descriptor
 *
 * @param     usbSpeed : usb speed
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_FS_ConfigDescHandler(uint8_t usbSpeed)
{
    USBD_DESC_INFO_T descInfo;

    descInfo.desc = USBD_ConfigDesc;
    descInfo.size = sizeof(USBD_ConfigDesc);

    return descInfo;
}

#if USBD_SUP_LPM
/*!
 * @brief     USB device FS BOS descriptor
 *
 * @param     usbSpeed : usb speed
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_FS_BosDescHandler(uint8_t usbSpeed)
{
    USBD_DESC_INFO_T descInfo;

    descInfo.desc = USBD_BosDesc;
    descInfo.size = sizeof(USBD_BosDesc);

    return descInfo;
}
#endif

/*!
 * @brief     USB device FS configuration string descriptor
 *
 * @param     usbSpeed : usb speed
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_FS_ConfigStrDescHandler(uint8_t usbSpeed)
{
    USBD_DESC_INFO_T descInfo;

    descInfo = USBD_DESC_Ascii2Unicode((uint8_t*)USBD_CONFIGURATION_STR);

    return descInfo;
}

/*!
 * @brief     USB device FS interface string descriptor
 *
 * @param     usbSpeed : usb speed
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_FS_InterfaceStrDescHandler(uint8_t usbSpeed)
{
    USBD_DESC_INFO_T descInfo;

    descInfo = USBD_DESC_Ascii2Unicode((uint8_t*)USBD_INTERFACE_STR);

    return descInfo;
}

/*!
 * @brief     USB device FS WINUSB OS string descriptor
 *
 * @param     usbSpeed : usb speed
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_FS_WinUsbOsStrDescHandler(uint8_t usbSpeed)
{
    USBD_DESC_INFO_T descInfo;

    descInfo.desc = USBD_WinUsbOsString;
    descInfo.size = sizeof(USBD_WinUsbOsString);

    return descInfo;
}

/*!
 * @brief     USB device FS LANG ID string descriptor
 *
 * @param     usbSpeed : usb speed
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_FS_LangIdStrDescHandler(uint8_t usbSpeed)
{
    USBD_DESC_INFO_T descInfo;

    descInfo.desc = USBD_LandIDStrDesc;
    descInfo.size = sizeof(USBD_LandIDStrDesc);

    return descInfo;
}

/*!
 * @brief     USB device FS manufacturer string descriptor
 *
 * @param     usbSpeed : usb speed
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_FS_ManufacturerStrDescHandler(uint8_t usbSpeed)
{
    USBD_DESC_INFO_T descInfo;

    descInfo = USBD_DESC_Ascii2Unicode((uint8_t*)USBD_MANUFACTURER_STR);

    return descInfo;
}

/*!
 * @brief     USB device FS product string descriptor
 *
 * @param     usbSpeed : usb speed
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_FS_ProductStrDescHandler(uint8_t usbSpeed)
{
    USBD_DESC_INFO_T descInfo;

    descInfo = USBD_DESC_Ascii2Unicode((uint8_t*)USBD_PRODUCT_STR);

    return descInfo;
}

/*!
 * @brief     USB device FS serial string descriptor
 *
 * @param     usbSpeed : usb speed
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_FS_SerialStrDescHandler(uint8_t usbSpeed)
{
    USBD_DESC_INFO_T descInfo;

    descInfo.desc = USBD_SerialStrDesc;
    descInfo.size = sizeof(USBD_SerialStrDesc);

    return descInfo;
}

/*!
 * @brief     USB device other speed configuration descriptor
 *
 * @param     usbSpeed : usb speed
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_OtherSpeedConfigDescHandler(uint8_t usbSpeed)
{
    USBD_DESC_INFO_T descInfo;

    /* Use FS configuration */
    descInfo.desc = USBD_OtherSpeedCfgDesc;
    descInfo.size = sizeof(USBD_OtherSpeedCfgDesc);

    return descInfo;
}

/*!
 * @brief     USB device device qualifier descriptor
 *
 * @param     usbSpeed : usb speed
 *
 * @retval    usb descriptor information
 */
static USBD_DESC_INFO_T USBD_DevQualifierDescHandler(uint8_t usbSpeed)
{
    USBD_DESC_INFO_T descInfo;

    /* Use FS configuration */
    descInfo.desc = USBD_DevQualifierDesc;
    descInfo.size = sizeof(USBD_DevQualifierDesc);

    return descInfo;
}

