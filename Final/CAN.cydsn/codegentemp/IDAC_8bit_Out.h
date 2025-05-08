/*******************************************************************************
* File Name: IDAC_8bit_Out.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_IDAC_8bit_Out_H) /* Pins IDAC_8bit_Out_H */
#define CY_PINS_IDAC_8bit_Out_H

#include "cytypes.h"
#include "cyfitter.h"
#include "IDAC_8bit_Out_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} IDAC_8bit_Out_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   IDAC_8bit_Out_Read(void);
void    IDAC_8bit_Out_Write(uint8 value);
uint8   IDAC_8bit_Out_ReadDataReg(void);
#if defined(IDAC_8bit_Out__PC) || (CY_PSOC4_4200L) 
    void    IDAC_8bit_Out_SetDriveMode(uint8 mode);
#endif
void    IDAC_8bit_Out_SetInterruptMode(uint16 position, uint16 mode);
uint8   IDAC_8bit_Out_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void IDAC_8bit_Out_Sleep(void); 
void IDAC_8bit_Out_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(IDAC_8bit_Out__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define IDAC_8bit_Out_DRIVE_MODE_BITS        (3)
    #define IDAC_8bit_Out_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - IDAC_8bit_Out_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the IDAC_8bit_Out_SetDriveMode() function.
         *  @{
         */
        #define IDAC_8bit_Out_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define IDAC_8bit_Out_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define IDAC_8bit_Out_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define IDAC_8bit_Out_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define IDAC_8bit_Out_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define IDAC_8bit_Out_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define IDAC_8bit_Out_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define IDAC_8bit_Out_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define IDAC_8bit_Out_MASK               IDAC_8bit_Out__MASK
#define IDAC_8bit_Out_SHIFT              IDAC_8bit_Out__SHIFT
#define IDAC_8bit_Out_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in IDAC_8bit_Out_SetInterruptMode() function.
     *  @{
     */
        #define IDAC_8bit_Out_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define IDAC_8bit_Out_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define IDAC_8bit_Out_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define IDAC_8bit_Out_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(IDAC_8bit_Out__SIO)
    #define IDAC_8bit_Out_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(IDAC_8bit_Out__PC) && (CY_PSOC4_4200L)
    #define IDAC_8bit_Out_USBIO_ENABLE               ((uint32)0x80000000u)
    #define IDAC_8bit_Out_USBIO_DISABLE              ((uint32)(~IDAC_8bit_Out_USBIO_ENABLE))
    #define IDAC_8bit_Out_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define IDAC_8bit_Out_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define IDAC_8bit_Out_USBIO_ENTER_SLEEP          ((uint32)((1u << IDAC_8bit_Out_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << IDAC_8bit_Out_USBIO_SUSPEND_DEL_SHIFT)))
    #define IDAC_8bit_Out_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << IDAC_8bit_Out_USBIO_SUSPEND_SHIFT)))
    #define IDAC_8bit_Out_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << IDAC_8bit_Out_USBIO_SUSPEND_DEL_SHIFT)))
    #define IDAC_8bit_Out_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(IDAC_8bit_Out__PC)
    /* Port Configuration */
    #define IDAC_8bit_Out_PC                 (* (reg32 *) IDAC_8bit_Out__PC)
#endif
/* Pin State */
#define IDAC_8bit_Out_PS                     (* (reg32 *) IDAC_8bit_Out__PS)
/* Data Register */
#define IDAC_8bit_Out_DR                     (* (reg32 *) IDAC_8bit_Out__DR)
/* Input Buffer Disable Override */
#define IDAC_8bit_Out_INP_DIS                (* (reg32 *) IDAC_8bit_Out__PC2)

/* Interrupt configuration Registers */
#define IDAC_8bit_Out_INTCFG                 (* (reg32 *) IDAC_8bit_Out__INTCFG)
#define IDAC_8bit_Out_INTSTAT                (* (reg32 *) IDAC_8bit_Out__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define IDAC_8bit_Out_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(IDAC_8bit_Out__SIO)
    #define IDAC_8bit_Out_SIO_REG            (* (reg32 *) IDAC_8bit_Out__SIO)
#endif /* (IDAC_8bit_Out__SIO_CFG) */

/* USBIO registers */
#if !defined(IDAC_8bit_Out__PC) && (CY_PSOC4_4200L)
    #define IDAC_8bit_Out_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define IDAC_8bit_Out_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define IDAC_8bit_Out_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define IDAC_8bit_Out_DRIVE_MODE_SHIFT       (0x00u)
#define IDAC_8bit_Out_DRIVE_MODE_MASK        (0x07u << IDAC_8bit_Out_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins IDAC_8bit_Out_H */


/* [] END OF FILE */
