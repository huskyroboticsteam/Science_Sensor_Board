/***************************************************************************//**
* \file Sensor_UART_PM.c
* \version 4.0
*
* \brief
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Sensor_UART.h"
#include "Sensor_UART_PVT.h"

#if(Sensor_UART_SCB_MODE_I2C_INC)
    #include "Sensor_UART_I2C_PVT.h"
#endif /* (Sensor_UART_SCB_MODE_I2C_INC) */

#if(Sensor_UART_SCB_MODE_EZI2C_INC)
    #include "Sensor_UART_EZI2C_PVT.h"
#endif /* (Sensor_UART_SCB_MODE_EZI2C_INC) */

#if(Sensor_UART_SCB_MODE_SPI_INC || Sensor_UART_SCB_MODE_UART_INC)
    #include "Sensor_UART_SPI_UART_PVT.h"
#endif /* (Sensor_UART_SCB_MODE_SPI_INC || Sensor_UART_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG || \
   (Sensor_UART_SCB_MODE_I2C_CONST_CFG   && (!Sensor_UART_I2C_WAKE_ENABLE_CONST))   || \
   (Sensor_UART_SCB_MODE_EZI2C_CONST_CFG && (!Sensor_UART_EZI2C_WAKE_ENABLE_CONST)) || \
   (Sensor_UART_SCB_MODE_SPI_CONST_CFG   && (!Sensor_UART_SPI_WAKE_ENABLE_CONST))   || \
   (Sensor_UART_SCB_MODE_UART_CONST_CFG  && (!Sensor_UART_UART_WAKE_ENABLE_CONST)))

    Sensor_UART_BACKUP_STRUCT Sensor_UART_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: Sensor_UART_Sleep
****************************************************************************//**
*
*  Prepares the Sensor_UART component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the Sensor_UART_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void Sensor_UART_Sleep(void)
{
#if(Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG)

    if(Sensor_UART_SCB_WAKE_ENABLE_CHECK)
    {
        if(Sensor_UART_SCB_MODE_I2C_RUNTM_CFG)
        {
            Sensor_UART_I2CSaveConfig();
        }
        else if(Sensor_UART_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            Sensor_UART_EzI2CSaveConfig();
        }
    #if(!Sensor_UART_CY_SCBIP_V1)
        else if(Sensor_UART_SCB_MODE_SPI_RUNTM_CFG)
        {
            Sensor_UART_SpiSaveConfig();
        }
        else if(Sensor_UART_SCB_MODE_UART_RUNTM_CFG)
        {
            Sensor_UART_UartSaveConfig();
        }
    #endif /* (!Sensor_UART_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        Sensor_UART_backup.enableState = (uint8) Sensor_UART_GET_CTRL_ENABLED;

        if(0u != Sensor_UART_backup.enableState)
        {
            Sensor_UART_Stop();
        }
    }

#else

    #if (Sensor_UART_SCB_MODE_I2C_CONST_CFG && Sensor_UART_I2C_WAKE_ENABLE_CONST)
        Sensor_UART_I2CSaveConfig();

    #elif (Sensor_UART_SCB_MODE_EZI2C_CONST_CFG && Sensor_UART_EZI2C_WAKE_ENABLE_CONST)
        Sensor_UART_EzI2CSaveConfig();

    #elif (Sensor_UART_SCB_MODE_SPI_CONST_CFG && Sensor_UART_SPI_WAKE_ENABLE_CONST)
        Sensor_UART_SpiSaveConfig();

    #elif (Sensor_UART_SCB_MODE_UART_CONST_CFG && Sensor_UART_UART_WAKE_ENABLE_CONST)
        Sensor_UART_UartSaveConfig();

    #else

        Sensor_UART_backup.enableState = (uint8) Sensor_UART_GET_CTRL_ENABLED;

        if(0u != Sensor_UART_backup.enableState)
        {
            Sensor_UART_Stop();
        }

    #endif /* defined (Sensor_UART_SCB_MODE_I2C_CONST_CFG) && (Sensor_UART_I2C_WAKE_ENABLE_CONST) */

#endif /* (Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: Sensor_UART_Wakeup
****************************************************************************//**
*
*  Prepares the Sensor_UART component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the Sensor_UART_Wakeup() function without first calling the 
*   Sensor_UART_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void Sensor_UART_Wakeup(void)
{
#if(Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG)

    if(Sensor_UART_SCB_WAKE_ENABLE_CHECK)
    {
        if(Sensor_UART_SCB_MODE_I2C_RUNTM_CFG)
        {
            Sensor_UART_I2CRestoreConfig();
        }
        else if(Sensor_UART_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            Sensor_UART_EzI2CRestoreConfig();
        }
    #if(!Sensor_UART_CY_SCBIP_V1)
        else if(Sensor_UART_SCB_MODE_SPI_RUNTM_CFG)
        {
            Sensor_UART_SpiRestoreConfig();
        }
        else if(Sensor_UART_SCB_MODE_UART_RUNTM_CFG)
        {
            Sensor_UART_UartRestoreConfig();
        }
    #endif /* (!Sensor_UART_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != Sensor_UART_backup.enableState)
        {
            Sensor_UART_Enable();
        }
    }

#else

    #if (Sensor_UART_SCB_MODE_I2C_CONST_CFG  && Sensor_UART_I2C_WAKE_ENABLE_CONST)
        Sensor_UART_I2CRestoreConfig();

    #elif (Sensor_UART_SCB_MODE_EZI2C_CONST_CFG && Sensor_UART_EZI2C_WAKE_ENABLE_CONST)
        Sensor_UART_EzI2CRestoreConfig();

    #elif (Sensor_UART_SCB_MODE_SPI_CONST_CFG && Sensor_UART_SPI_WAKE_ENABLE_CONST)
        Sensor_UART_SpiRestoreConfig();

    #elif (Sensor_UART_SCB_MODE_UART_CONST_CFG && Sensor_UART_UART_WAKE_ENABLE_CONST)
        Sensor_UART_UartRestoreConfig();

    #else

        if(0u != Sensor_UART_backup.enableState)
        {
            Sensor_UART_Enable();
        }

    #endif /* (Sensor_UART_I2C_WAKE_ENABLE_CONST) */

#endif /* (Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
