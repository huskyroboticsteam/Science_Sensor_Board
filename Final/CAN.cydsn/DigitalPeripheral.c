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
#include "DigitalPeripheral.h"

void setLaser(uint8_t setValue) {
    Laser_Write(setValue ? ON : OFF);
}

void setWaterPump1(uint8_t setValue) {
    WaterPump1_Write(setValue ? ON : OFF);
}

void setWaterPump2(uint8_t setValue) {
    WaterPump2_Write(setValue ? ON : OFF);
}

/* [] END OF FILE */
