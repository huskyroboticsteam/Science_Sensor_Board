/*******************************************************************************
* File Name: Sensor_CAN_HFCLK.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_Sensor_CAN_HFCLK_H)
#define CY_CLOCK_Sensor_CAN_HFCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Sensor_CAN_HFCLK_StartEx(uint32 alignClkDiv);
#define Sensor_CAN_HFCLK_Start() \
    Sensor_CAN_HFCLK_StartEx(Sensor_CAN_HFCLK__PA_DIV_ID)

#else

void Sensor_CAN_HFCLK_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Sensor_CAN_HFCLK_Stop(void);

void Sensor_CAN_HFCLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Sensor_CAN_HFCLK_GetDividerRegister(void);
uint8  Sensor_CAN_HFCLK_GetFractionalDividerRegister(void);

#define Sensor_CAN_HFCLK_Enable()                         Sensor_CAN_HFCLK_Start()
#define Sensor_CAN_HFCLK_Disable()                        Sensor_CAN_HFCLK_Stop()
#define Sensor_CAN_HFCLK_SetDividerRegister(clkDivider, reset)  \
    Sensor_CAN_HFCLK_SetFractionalDividerRegister((clkDivider), 0u)
#define Sensor_CAN_HFCLK_SetDivider(clkDivider)           Sensor_CAN_HFCLK_SetDividerRegister((clkDivider), 1u)
#define Sensor_CAN_HFCLK_SetDividerValue(clkDivider)      Sensor_CAN_HFCLK_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Sensor_CAN_HFCLK_DIV_ID     Sensor_CAN_HFCLK__DIV_ID

#define Sensor_CAN_HFCLK_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Sensor_CAN_HFCLK_CTRL_REG   (*(reg32 *)Sensor_CAN_HFCLK__CTRL_REGISTER)
#define Sensor_CAN_HFCLK_DIV_REG    (*(reg32 *)Sensor_CAN_HFCLK__DIV_REGISTER)

#define Sensor_CAN_HFCLK_CMD_DIV_SHIFT          (0u)
#define Sensor_CAN_HFCLK_CMD_PA_DIV_SHIFT       (8u)
#define Sensor_CAN_HFCLK_CMD_DISABLE_SHIFT      (30u)
#define Sensor_CAN_HFCLK_CMD_ENABLE_SHIFT       (31u)

#define Sensor_CAN_HFCLK_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Sensor_CAN_HFCLK_CMD_DISABLE_SHIFT))
#define Sensor_CAN_HFCLK_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Sensor_CAN_HFCLK_CMD_ENABLE_SHIFT))

#define Sensor_CAN_HFCLK_DIV_FRAC_MASK  (0x000000F8u)
#define Sensor_CAN_HFCLK_DIV_FRAC_SHIFT (3u)
#define Sensor_CAN_HFCLK_DIV_INT_MASK   (0xFFFFFF00u)
#define Sensor_CAN_HFCLK_DIV_INT_SHIFT  (8u)

#else 

#define Sensor_CAN_HFCLK_DIV_REG        (*(reg32 *)Sensor_CAN_HFCLK__REGISTER)
#define Sensor_CAN_HFCLK_ENABLE_REG     Sensor_CAN_HFCLK_DIV_REG
#define Sensor_CAN_HFCLK_DIV_FRAC_MASK  Sensor_CAN_HFCLK__FRAC_MASK
#define Sensor_CAN_HFCLK_DIV_FRAC_SHIFT (16u)
#define Sensor_CAN_HFCLK_DIV_INT_MASK   Sensor_CAN_HFCLK__DIVIDER_MASK
#define Sensor_CAN_HFCLK_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Sensor_CAN_HFCLK_H) */

/* [] END OF FILE */
