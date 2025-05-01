/*******************************************************************************
* File Name: ComponentPin.h  
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

#if !defined(CY_PINS_ComponentPin_ALIASES_H) /* Pins ComponentPin_ALIASES_H */
#define CY_PINS_ComponentPin_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define ComponentPin_0			(ComponentPin__0__PC)
#define ComponentPin_0_PS		(ComponentPin__0__PS)
#define ComponentPin_0_PC		(ComponentPin__0__PC)
#define ComponentPin_0_DR		(ComponentPin__0__DR)
#define ComponentPin_0_SHIFT	(ComponentPin__0__SHIFT)
#define ComponentPin_0_INTR	((uint16)((uint16)0x0003u << (ComponentPin__0__SHIFT*2u)))

#define ComponentPin_INTR_ALL	 ((uint16)(ComponentPin_0_INTR))


#endif /* End Pins ComponentPin_ALIASES_H */


/* [] END OF FILE */
