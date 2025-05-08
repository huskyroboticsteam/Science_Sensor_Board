/*******************************************************************************
* File Name: HumTemp_I2C_SCBCLK.h
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

#if !defined(CY_CLOCK_HumTemp_I2C_SCBCLK_H)
#define CY_CLOCK_HumTemp_I2C_SCBCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void HumTemp_I2C_SCBCLK_StartEx(uint32 alignClkDiv);
#define HumTemp_I2C_SCBCLK_Start() \
    HumTemp_I2C_SCBCLK_StartEx(HumTemp_I2C_SCBCLK__PA_DIV_ID)

#else

void HumTemp_I2C_SCBCLK_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void HumTemp_I2C_SCBCLK_Stop(void);

void HumTemp_I2C_SCBCLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 HumTemp_I2C_SCBCLK_GetDividerRegister(void);
uint8  HumTemp_I2C_SCBCLK_GetFractionalDividerRegister(void);

#define HumTemp_I2C_SCBCLK_Enable()                         HumTemp_I2C_SCBCLK_Start()
#define HumTemp_I2C_SCBCLK_Disable()                        HumTemp_I2C_SCBCLK_Stop()
#define HumTemp_I2C_SCBCLK_SetDividerRegister(clkDivider, reset)  \
    HumTemp_I2C_SCBCLK_SetFractionalDividerRegister((clkDivider), 0u)
#define HumTemp_I2C_SCBCLK_SetDivider(clkDivider)           HumTemp_I2C_SCBCLK_SetDividerRegister((clkDivider), 1u)
#define HumTemp_I2C_SCBCLK_SetDividerValue(clkDivider)      HumTemp_I2C_SCBCLK_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define HumTemp_I2C_SCBCLK_DIV_ID     HumTemp_I2C_SCBCLK__DIV_ID

#define HumTemp_I2C_SCBCLK_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define HumTemp_I2C_SCBCLK_CTRL_REG   (*(reg32 *)HumTemp_I2C_SCBCLK__CTRL_REGISTER)
#define HumTemp_I2C_SCBCLK_DIV_REG    (*(reg32 *)HumTemp_I2C_SCBCLK__DIV_REGISTER)

#define HumTemp_I2C_SCBCLK_CMD_DIV_SHIFT          (0u)
#define HumTemp_I2C_SCBCLK_CMD_PA_DIV_SHIFT       (8u)
#define HumTemp_I2C_SCBCLK_CMD_DISABLE_SHIFT      (30u)
#define HumTemp_I2C_SCBCLK_CMD_ENABLE_SHIFT       (31u)

#define HumTemp_I2C_SCBCLK_CMD_DISABLE_MASK       ((uint32)((uint32)1u << HumTemp_I2C_SCBCLK_CMD_DISABLE_SHIFT))
#define HumTemp_I2C_SCBCLK_CMD_ENABLE_MASK        ((uint32)((uint32)1u << HumTemp_I2C_SCBCLK_CMD_ENABLE_SHIFT))

#define HumTemp_I2C_SCBCLK_DIV_FRAC_MASK  (0x000000F8u)
#define HumTemp_I2C_SCBCLK_DIV_FRAC_SHIFT (3u)
#define HumTemp_I2C_SCBCLK_DIV_INT_MASK   (0xFFFFFF00u)
#define HumTemp_I2C_SCBCLK_DIV_INT_SHIFT  (8u)

#else 

#define HumTemp_I2C_SCBCLK_DIV_REG        (*(reg32 *)HumTemp_I2C_SCBCLK__REGISTER)
#define HumTemp_I2C_SCBCLK_ENABLE_REG     HumTemp_I2C_SCBCLK_DIV_REG
#define HumTemp_I2C_SCBCLK_DIV_FRAC_MASK  HumTemp_I2C_SCBCLK__FRAC_MASK
#define HumTemp_I2C_SCBCLK_DIV_FRAC_SHIFT (16u)
#define HumTemp_I2C_SCBCLK_DIV_INT_MASK   HumTemp_I2C_SCBCLK__DIVIDER_MASK
#define HumTemp_I2C_SCBCLK_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_HumTemp_I2C_SCBCLK_H) */

/* [] END OF FILE */
