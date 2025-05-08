/*******************************************************************************
* File Name: HumTemp_I2C_sda.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "HumTemp_I2C_sda.h"

static HumTemp_I2C_sda_BACKUP_STRUCT  HumTemp_I2C_sda_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: HumTemp_I2C_sda_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet HumTemp_I2C_sda_SUT.c usage_HumTemp_I2C_sda_Sleep_Wakeup
*******************************************************************************/
void HumTemp_I2C_sda_Sleep(void)
{
    #if defined(HumTemp_I2C_sda__PC)
        HumTemp_I2C_sda_backup.pcState = HumTemp_I2C_sda_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            HumTemp_I2C_sda_backup.usbState = HumTemp_I2C_sda_CR1_REG;
            HumTemp_I2C_sda_USB_POWER_REG |= HumTemp_I2C_sda_USBIO_ENTER_SLEEP;
            HumTemp_I2C_sda_CR1_REG &= HumTemp_I2C_sda_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(HumTemp_I2C_sda__SIO)
        HumTemp_I2C_sda_backup.sioState = HumTemp_I2C_sda_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        HumTemp_I2C_sda_SIO_REG &= (uint32)(~HumTemp_I2C_sda_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: HumTemp_I2C_sda_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to HumTemp_I2C_sda_Sleep() for an example usage.
*******************************************************************************/
void HumTemp_I2C_sda_Wakeup(void)
{
    #if defined(HumTemp_I2C_sda__PC)
        HumTemp_I2C_sda_PC = HumTemp_I2C_sda_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            HumTemp_I2C_sda_USB_POWER_REG &= HumTemp_I2C_sda_USBIO_EXIT_SLEEP_PH1;
            HumTemp_I2C_sda_CR1_REG = HumTemp_I2C_sda_backup.usbState;
            HumTemp_I2C_sda_USB_POWER_REG &= HumTemp_I2C_sda_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(HumTemp_I2C_sda__SIO)
        HumTemp_I2C_sda_SIO_REG = HumTemp_I2C_sda_backup.sioState;
    #endif
}


/* [] END OF FILE */
