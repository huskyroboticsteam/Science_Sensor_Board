/*******************************************************************************
* File Name: HumTemp_I2C_sda.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_HumTemp_I2C_sda_ALIASES_H) /* Pins HumTemp_I2C_sda_ALIASES_H */
#define CY_PINS_HumTemp_I2C_sda_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define HumTemp_I2C_sda_0			(HumTemp_I2C_sda__0__PC)
#define HumTemp_I2C_sda_0_PS		(HumTemp_I2C_sda__0__PS)
#define HumTemp_I2C_sda_0_PC		(HumTemp_I2C_sda__0__PC)
#define HumTemp_I2C_sda_0_DR		(HumTemp_I2C_sda__0__DR)
#define HumTemp_I2C_sda_0_SHIFT	(HumTemp_I2C_sda__0__SHIFT)
#define HumTemp_I2C_sda_0_INTR	((uint16)((uint16)0x0003u << (HumTemp_I2C_sda__0__SHIFT*2u)))

#define HumTemp_I2C_sda_INTR_ALL	 ((uint16)(HumTemp_I2C_sda_0_INTR))


#endif /* End Pins HumTemp_I2C_sda_ALIASES_H */


/* [] END OF FILE */
