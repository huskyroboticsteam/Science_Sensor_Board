/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#pragma once

#include <project.h>

int32 ReadSensorTemperature();
int32 ReadSensorHumidity();
int32 ReadSensorCO();
int32 ReadSensorCO2();
int32 ReadSensorCH4();
int32 ReadSensorO2();

uint8 readReg16(uint8 addr, uint8 reg, uint16* val);
uint8 writeReg16(uint8 addr, uint8 reg, uint16 val);

#define TIMEOUT 20

#define SCD41_ADDR 0


/* [] END OF FILE */
