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
#include <project.h>
#include "cyapicallbacks.h"
#include "Sensor_Stuff.h"

#define TIMEOUT 20

int32 ReadSensorTemperature() {
    // TODO
    return 0;
}

int32 ReadSensorHumidity() {
    // TODO
    return 0;
}

int32 ReadSensorCO() {
    // TODO
    return 0;
}

int32 ReadSensorCO2() {
    // TODO
    return 0;
}

int32 ReadSensorCH4() {
    // TODO
    return 0;
}

int32 ReadSensorO2(uint8 reg, uint16* val) {
    uint8 b1, b2;
    I2C_I2CMasterClearStatus(); //clear the garbage

	I2C_I2CMasterSendStart(DEVICE_ADDR, I2C_I2C_WRITE_XFER_MODE, TIMEOUT);
	I2C_I2CMasterWriteByte(reg, TIMEOUT);
	I2C_I2CMasterSendStop(TIMEOUT);
	
	I2C_I2CMasterSendStart(DEVICE_ADDR, I2C_I2C_READ_XFER_MODE, TIMEOUT);
	I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA, &b2, TIMEOUT);
   I2C_I2CMasterReadByte(I2C_I2C_NAK_DATA, &b1, TIMEOUT);
    
    int err = I2C_I2CMasterSendStop(TIMEOUT);
    *val = ((uint16) b2 << 8) | b1;
	return err;
}
/* [] END OF FILE */
