/***************************************************************************//**
* \file .h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_Sensor_UART_H)
#define CY_SCB_PVT_Sensor_UART_H

#include "Sensor_UART.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define Sensor_UART_SetI2CExtClkInterruptMode(interruptMask) Sensor_UART_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define Sensor_UART_ClearI2CExtClkInterruptSource(interruptMask) Sensor_UART_CLEAR_INTR_I2C_EC(interruptMask)
#define Sensor_UART_GetI2CExtClkInterruptSource()                (Sensor_UART_INTR_I2C_EC_REG)
#define Sensor_UART_GetI2CExtClkInterruptMode()                  (Sensor_UART_INTR_I2C_EC_MASK_REG)
#define Sensor_UART_GetI2CExtClkInterruptSourceMasked()          (Sensor_UART_INTR_I2C_EC_MASKED_REG)

#if (!Sensor_UART_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define Sensor_UART_SetSpiExtClkInterruptMode(interruptMask) \
                                                                Sensor_UART_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define Sensor_UART_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                Sensor_UART_CLEAR_INTR_SPI_EC(interruptMask)
    #define Sensor_UART_GetExtSpiClkInterruptSource()                 (Sensor_UART_INTR_SPI_EC_REG)
    #define Sensor_UART_GetExtSpiClkInterruptMode()                   (Sensor_UART_INTR_SPI_EC_MASK_REG)
    #define Sensor_UART_GetExtSpiClkInterruptSourceMasked()           (Sensor_UART_INTR_SPI_EC_MASKED_REG)
#endif /* (!Sensor_UART_CY_SCBIP_V1) */

#if(Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void Sensor_UART_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (Sensor_UART_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_Sensor_UART_CUSTOM_INTR_HANDLER)
    extern cyisraddress Sensor_UART_customIntrHandler;
#endif /* !defined (CY_REMOVE_Sensor_UART_CUSTOM_INTR_HANDLER) */
#endif /* (Sensor_UART_SCB_IRQ_INTERNAL) */

extern Sensor_UART_BACKUP_STRUCT Sensor_UART_backup;

#if(Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 Sensor_UART_scbMode;
    extern uint8 Sensor_UART_scbEnableWake;
    extern uint8 Sensor_UART_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 Sensor_UART_mode;
    extern uint8 Sensor_UART_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * Sensor_UART_rxBuffer;
    extern uint8   Sensor_UART_rxDataBits;
    extern uint32  Sensor_UART_rxBufferSize;

    extern volatile uint8 * Sensor_UART_txBuffer;
    extern uint8   Sensor_UART_txDataBits;
    extern uint32  Sensor_UART_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 Sensor_UART_numberOfAddr;
    extern uint8 Sensor_UART_subAddrSize;
#endif /* (Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (Sensor_UART_SCB_MODE_I2C_CONST_CFG || \
        Sensor_UART_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 Sensor_UART_IntrTxMask;
#endif /* (! (Sensor_UART_SCB_MODE_I2C_CONST_CFG || \
              Sensor_UART_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define Sensor_UART_SCB_MODE_I2C_RUNTM_CFG     (Sensor_UART_SCB_MODE_I2C      == Sensor_UART_scbMode)
    #define Sensor_UART_SCB_MODE_SPI_RUNTM_CFG     (Sensor_UART_SCB_MODE_SPI      == Sensor_UART_scbMode)
    #define Sensor_UART_SCB_MODE_UART_RUNTM_CFG    (Sensor_UART_SCB_MODE_UART     == Sensor_UART_scbMode)
    #define Sensor_UART_SCB_MODE_EZI2C_RUNTM_CFG   (Sensor_UART_SCB_MODE_EZI2C    == Sensor_UART_scbMode)
    #define Sensor_UART_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (Sensor_UART_SCB_MODE_UNCONFIG == Sensor_UART_scbMode)

    /* Defines wakeup enable */
    #define Sensor_UART_SCB_WAKE_ENABLE_CHECK       (0u != Sensor_UART_scbEnableWake)
#endif /* (Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!Sensor_UART_CY_SCBIP_V1)
    #define Sensor_UART_SCB_PINS_NUMBER    (7u)
#else
    #define Sensor_UART_SCB_PINS_NUMBER    (2u)
#endif /* (!Sensor_UART_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_Sensor_UART_H) */


/* [] END OF FILE */
