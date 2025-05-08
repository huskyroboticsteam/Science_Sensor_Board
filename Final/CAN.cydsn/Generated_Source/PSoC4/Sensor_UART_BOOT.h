/***************************************************************************//**
* \file Sensor_UART_BOOT.h
* \version 4.0
*
* \brief
*  This file provides constants and parameter values of the bootloader
*  communication APIs for the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2014-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_BOOT_Sensor_UART_H)
#define CY_SCB_BOOT_Sensor_UART_H

#include "Sensor_UART_PVT.h"

#if (Sensor_UART_SCB_MODE_I2C_INC)
    #include "Sensor_UART_I2C.h"
#endif /* (Sensor_UART_SCB_MODE_I2C_INC) */

#if (Sensor_UART_SCB_MODE_EZI2C_INC)
    #include "Sensor_UART_EZI2C.h"
#endif /* (Sensor_UART_SCB_MODE_EZI2C_INC) */

#if (Sensor_UART_SCB_MODE_SPI_INC || Sensor_UART_SCB_MODE_UART_INC)
    #include "Sensor_UART_SPI_UART.h"
#endif /* (Sensor_UART_SCB_MODE_SPI_INC || Sensor_UART_SCB_MODE_UART_INC) */


/***************************************
*  Conditional Compilation Parameters
****************************************/

/* Bootloader communication interface enable */
#define Sensor_UART_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Sensor_UART) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

/* Enable I2C bootloader communication */
#if (Sensor_UART_SCB_MODE_I2C_INC)
    #define Sensor_UART_I2C_BTLDR_COMM_ENABLED     (Sensor_UART_BTLDR_COMM_ENABLED && \
                                                            (Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             Sensor_UART_I2C_SLAVE_CONST))
#else
     #define Sensor_UART_I2C_BTLDR_COMM_ENABLED    (0u)
#endif /* (Sensor_UART_SCB_MODE_I2C_INC) */

/* EZI2C does not support bootloader communication. Provide empty APIs */
#if (Sensor_UART_SCB_MODE_EZI2C_INC)
    #define Sensor_UART_EZI2C_BTLDR_COMM_ENABLED   (Sensor_UART_BTLDR_COMM_ENABLED && \
                                                         Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG)
#else
    #define Sensor_UART_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (Sensor_UART_EZI2C_BTLDR_COMM_ENABLED) */

/* Enable SPI bootloader communication */
#if (Sensor_UART_SCB_MODE_SPI_INC)
    #define Sensor_UART_SPI_BTLDR_COMM_ENABLED     (Sensor_UART_BTLDR_COMM_ENABLED && \
                                                            (Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             Sensor_UART_SPI_SLAVE_CONST))
#else
        #define Sensor_UART_SPI_BTLDR_COMM_ENABLED (0u)
#endif /* (Sensor_UART_SPI_BTLDR_COMM_ENABLED) */

/* Enable UART bootloader communication */
#if (Sensor_UART_SCB_MODE_UART_INC)
       #define Sensor_UART_UART_BTLDR_COMM_ENABLED    (Sensor_UART_BTLDR_COMM_ENABLED && \
                                                            (Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             (Sensor_UART_UART_RX_DIRECTION && \
                                                              Sensor_UART_UART_TX_DIRECTION)))
#else
     #define Sensor_UART_UART_BTLDR_COMM_ENABLED   (0u)
#endif /* (Sensor_UART_UART_BTLDR_COMM_ENABLED) */

/* Enable bootloader communication */
#define Sensor_UART_BTLDR_COMM_MODE_ENABLED    (Sensor_UART_I2C_BTLDR_COMM_ENABLED   || \
                                                     Sensor_UART_SPI_BTLDR_COMM_ENABLED   || \
                                                     Sensor_UART_EZI2C_BTLDR_COMM_ENABLED || \
                                                     Sensor_UART_UART_BTLDR_COMM_ENABLED)


/***************************************
*        Function Prototypes
***************************************/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Sensor_UART_I2C_BTLDR_COMM_ENABLED)
    /* I2C Bootloader physical layer functions */
    void Sensor_UART_I2CCyBtldrCommStart(void);
    void Sensor_UART_I2CCyBtldrCommStop (void);
    void Sensor_UART_I2CCyBtldrCommReset(void);
    cystatus Sensor_UART_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus Sensor_UART_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map I2C specific bootloader communication APIs to SCB specific APIs */
    #if (Sensor_UART_SCB_MODE_I2C_CONST_CFG)
        #define Sensor_UART_CyBtldrCommStart   Sensor_UART_I2CCyBtldrCommStart
        #define Sensor_UART_CyBtldrCommStop    Sensor_UART_I2CCyBtldrCommStop
        #define Sensor_UART_CyBtldrCommReset   Sensor_UART_I2CCyBtldrCommReset
        #define Sensor_UART_CyBtldrCommRead    Sensor_UART_I2CCyBtldrCommRead
        #define Sensor_UART_CyBtldrCommWrite   Sensor_UART_I2CCyBtldrCommWrite
    #endif /* (Sensor_UART_SCB_MODE_I2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Sensor_UART_I2C_BTLDR_COMM_ENABLED) */


#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Sensor_UART_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void Sensor_UART_EzI2CCyBtldrCommStart(void);
    void Sensor_UART_EzI2CCyBtldrCommStop (void);
    void Sensor_UART_EzI2CCyBtldrCommReset(void);
    cystatus Sensor_UART_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus Sensor_UART_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map EZI2C specific bootloader communication APIs to SCB specific APIs */
    #if (Sensor_UART_SCB_MODE_EZI2C_CONST_CFG)
        #define Sensor_UART_CyBtldrCommStart   Sensor_UART_EzI2CCyBtldrCommStart
        #define Sensor_UART_CyBtldrCommStop    Sensor_UART_EzI2CCyBtldrCommStop
        #define Sensor_UART_CyBtldrCommReset   Sensor_UART_EzI2CCyBtldrCommReset
        #define Sensor_UART_CyBtldrCommRead    Sensor_UART_EzI2CCyBtldrCommRead
        #define Sensor_UART_CyBtldrCommWrite   Sensor_UART_EzI2CCyBtldrCommWrite
    #endif /* (Sensor_UART_SCB_MODE_EZI2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Sensor_UART_EZI2C_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Sensor_UART_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void Sensor_UART_SpiCyBtldrCommStart(void);
    void Sensor_UART_SpiCyBtldrCommStop (void);
    void Sensor_UART_SpiCyBtldrCommReset(void);
    cystatus Sensor_UART_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus Sensor_UART_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map SPI specific bootloader communication APIs to SCB specific APIs */
    #if (Sensor_UART_SCB_MODE_SPI_CONST_CFG)
        #define Sensor_UART_CyBtldrCommStart   Sensor_UART_SpiCyBtldrCommStart
        #define Sensor_UART_CyBtldrCommStop    Sensor_UART_SpiCyBtldrCommStop
        #define Sensor_UART_CyBtldrCommReset   Sensor_UART_SpiCyBtldrCommReset
        #define Sensor_UART_CyBtldrCommRead    Sensor_UART_SpiCyBtldrCommRead
        #define Sensor_UART_CyBtldrCommWrite   Sensor_UART_SpiCyBtldrCommWrite
    #endif /* (Sensor_UART_SCB_MODE_SPI_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Sensor_UART_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Sensor_UART_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void Sensor_UART_UartCyBtldrCommStart(void);
    void Sensor_UART_UartCyBtldrCommStop (void);
    void Sensor_UART_UartCyBtldrCommReset(void);
    cystatus Sensor_UART_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus Sensor_UART_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map UART specific bootloader communication APIs to SCB specific APIs */
    #if (Sensor_UART_SCB_MODE_UART_CONST_CFG)
        #define Sensor_UART_CyBtldrCommStart   Sensor_UART_UartCyBtldrCommStart
        #define Sensor_UART_CyBtldrCommStop    Sensor_UART_UartCyBtldrCommStop
        #define Sensor_UART_CyBtldrCommReset   Sensor_UART_UartCyBtldrCommReset
        #define Sensor_UART_CyBtldrCommRead    Sensor_UART_UartCyBtldrCommRead
        #define Sensor_UART_CyBtldrCommWrite   Sensor_UART_UartCyBtldrCommWrite
    #endif /* (Sensor_UART_SCB_MODE_UART_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Sensor_UART_UART_BTLDR_COMM_ENABLED) */

/**
* \addtogroup group_bootloader
* @{
*/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Sensor_UART_BTLDR_COMM_ENABLED)
    #if (Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Bootloader physical layer functions */
        void Sensor_UART_CyBtldrCommStart(void);
        void Sensor_UART_CyBtldrCommStop (void);
        void Sensor_UART_CyBtldrCommReset(void);
        cystatus Sensor_UART_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus Sensor_UART_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Map SCB specific bootloader communication APIs to common APIs */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Sensor_UART)
        #define CyBtldrCommStart    Sensor_UART_CyBtldrCommStart
        #define CyBtldrCommStop     Sensor_UART_CyBtldrCommStop
        #define CyBtldrCommReset    Sensor_UART_CyBtldrCommReset
        #define CyBtldrCommWrite    Sensor_UART_CyBtldrCommWrite
        #define CyBtldrCommRead     Sensor_UART_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Sensor_UART) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Sensor_UART_BTLDR_COMM_ENABLED) */

/** @} group_bootloader */

/***************************************
*           API Constants
***************************************/

/* Timeout unit in milliseconds */
#define Sensor_UART_WAIT_1_MS  (1u)

/* Return number of bytes to copy into bootloader buffer */
#define Sensor_UART_BYTES_TO_COPY(actBufSize, bufSize) \
                            ( ((uint32)(actBufSize) < (uint32)(bufSize)) ? \
                                ((uint32) (actBufSize)) : ((uint32) (bufSize)) )

/* Size of Read/Write buffers for I2C bootloader  */
#define Sensor_UART_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
#define Sensor_UART_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)

/* Byte to byte time interval: calculated basing on current component
* data rate configuration, can be defined in project if required.
*/
#ifndef Sensor_UART_SPI_BYTE_TO_BYTE
    #define Sensor_UART_SPI_BYTE_TO_BYTE   (160u)
#endif

/* Byte to byte time interval: calculated basing on current component
* baud rate configuration, can be defined in the project if required.
*/
#ifndef Sensor_UART_UART_BYTE_TO_BYTE
    #define Sensor_UART_UART_BYTE_TO_BYTE  (175u)
#endif /* Sensor_UART_UART_BYTE_TO_BYTE */

#endif /* (CY_SCB_BOOT_Sensor_UART_H) */


/* [] END OF FILE */
