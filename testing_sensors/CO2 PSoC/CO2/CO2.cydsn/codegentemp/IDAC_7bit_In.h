/*******************************************************************************
* File Name: IDAC_7bit_In.h  
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

#if !defined(CY_PINS_IDAC_7bit_In_H) /* Pins IDAC_7bit_In_H */
#define CY_PINS_IDAC_7bit_In_H

#include "cytypes.h"
#include "cyfitter.h"
#include "IDAC_7bit_In_aliases.h"


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
} IDAC_7bit_In_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   IDAC_7bit_In_Read(void);
void    IDAC_7bit_In_Write(uint8 value);
uint8   IDAC_7bit_In_ReadDataReg(void);
#if defined(IDAC_7bit_In__PC) || (CY_PSOC4_4200L) 
    void    IDAC_7bit_In_SetDriveMode(uint8 mode);
#endif
void    IDAC_7bit_In_SetInterruptMode(uint16 position, uint16 mode);
uint8   IDAC_7bit_In_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void IDAC_7bit_In_Sleep(void); 
void IDAC_7bit_In_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(IDAC_7bit_In__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define IDAC_7bit_In_DRIVE_MODE_BITS        (3)
    #define IDAC_7bit_In_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - IDAC_7bit_In_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the IDAC_7bit_In_SetDriveMode() function.
         *  @{
         */
        #define IDAC_7bit_In_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define IDAC_7bit_In_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define IDAC_7bit_In_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define IDAC_7bit_In_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define IDAC_7bit_In_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define IDAC_7bit_In_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define IDAC_7bit_In_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define IDAC_7bit_In_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define IDAC_7bit_In_MASK               IDAC_7bit_In__MASK
#define IDAC_7bit_In_SHIFT              IDAC_7bit_In__SHIFT
#define IDAC_7bit_In_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in IDAC_7bit_In_SetInterruptMode() function.
     *  @{
     */
        #define IDAC_7bit_In_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define IDAC_7bit_In_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define IDAC_7bit_In_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define IDAC_7bit_In_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(IDAC_7bit_In__SIO)
    #define IDAC_7bit_In_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(IDAC_7bit_In__PC) && (CY_PSOC4_4200L)
    #define IDAC_7bit_In_USBIO_ENABLE               ((uint32)0x80000000u)
    #define IDAC_7bit_In_USBIO_DISABLE              ((uint32)(~IDAC_7bit_In_USBIO_ENABLE))
    #define IDAC_7bit_In_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define IDAC_7bit_In_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define IDAC_7bit_In_USBIO_ENTER_SLEEP          ((uint32)((1u << IDAC_7bit_In_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << IDAC_7bit_In_USBIO_SUSPEND_DEL_SHIFT)))
    #define IDAC_7bit_In_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << IDAC_7bit_In_USBIO_SUSPEND_SHIFT)))
    #define IDAC_7bit_In_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << IDAC_7bit_In_USBIO_SUSPEND_DEL_SHIFT)))
    #define IDAC_7bit_In_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(IDAC_7bit_In__PC)
    /* Port Configuration */
    #define IDAC_7bit_In_PC                 (* (reg32 *) IDAC_7bit_In__PC)
#endif
/* Pin State */
#define IDAC_7bit_In_PS                     (* (reg32 *) IDAC_7bit_In__PS)
/* Data Register */
#define IDAC_7bit_In_DR                     (* (reg32 *) IDAC_7bit_In__DR)
/* Input Buffer Disable Override */
#define IDAC_7bit_In_INP_DIS                (* (reg32 *) IDAC_7bit_In__PC2)

/* Interrupt configuration Registers */
#define IDAC_7bit_In_INTCFG                 (* (reg32 *) IDAC_7bit_In__INTCFG)
#define IDAC_7bit_In_INTSTAT                (* (reg32 *) IDAC_7bit_In__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define IDAC_7bit_In_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(IDAC_7bit_In__SIO)
    #define IDAC_7bit_In_SIO_REG            (* (reg32 *) IDAC_7bit_In__SIO)
#endif /* (IDAC_7bit_In__SIO_CFG) */

/* USBIO registers */
#if !defined(IDAC_7bit_In__PC) && (CY_PSOC4_4200L)
    #define IDAC_7bit_In_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define IDAC_7bit_In_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define IDAC_7bit_In_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define IDAC_7bit_In_DRIVE_MODE_SHIFT       (0x00u)
#define IDAC_7bit_In_DRIVE_MODE_MASK        (0x07u << IDAC_7bit_In_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins IDAC_7bit_In_H */


/* [] END OF FILE */
