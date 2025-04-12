#include "project.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <library.h>

/*
bool enableHeater = false;
uint8_t loopCnt = 0;
char buffer[64];

// Send command to sensor
void SHT31_SendCommand(uint16_t cmd) {
    uint8_t data[2] = { (uint8_t)(cmd >> 8), (uint8_t)(cmd & 0xFF) };
    I2C_MasterClearStatus();
    I2C_MasterWriteBuf(SHT31_ADDR, data, 2, I2C_MODE_COMPLETE_XFER);
    while (I2C_MasterStatus() & I2C_MSTAT_XFER_INP);
}

// Read temperature and humidity
bool SHT31_ReadTempHum(float* temperature, float* humidity) {
    uint8_t readBuffer[6];

    SHT31_SendCommand(0x2400); // Single shot measurement
    CyDelay(15); // Wait for measurement

    I2C_MasterClearStatus();
    I2C_MasterReadBuf(SHT31_ADDR, readBuffer, 6, I2C_MODE_COMPLETE_XFER);
    while (I2C_MasterStatus() & I2C_MSTAT_XFER_INP);

    uint16_t rawT = (readBuffer[0] << 8) | readBuffer[1];
    uint16_t rawH = (readBuffer[3] << 8) | readBuffer[4];

    *temperature = -45.0 + 175.0 * ((float)rawT / 65535.0);
    *humidity = 100.0 * ((float)rawH / 65535.0);
    
    return true;
}

void toggleHeater(bool enable) {
    if (enable)
        SHT31_SendCommand(0x306D); // Heater on
    else
        SHT31_SendCommand(0x3066); // Heater off
}
*/
int main(void) {
    CyGlobalIntEnable;
    /*
    I2C_Start();
    UART_Start();

    UART_PutString("SHT31 Test\r\n");

    SHT31_SendCommand(0x2400); // Initial dummy read

    for (;;) {
        float t, h;
        if (SHT31_ReadTempHum(&t, &h)) {
            snprintf(buffer, sizeof(buffer), "TempC = %.2f\tHum. %% = %.2f\r\n", t, h);
            UART_PutString(buffer);
        } else {
            UART_PutString("Failed to read data\r\n");
        }

        CyDelay(1000);

        if (++loopCnt >= 30) {
            enableHeater = !enableHeater;
            toggleHeater(enableHeater);
            UART_PutString("Heater Enabled State: ");
            UART_PutString(enableHeater ? "ENABLED\r\n" : "DISABLED\r\n");
            loopCnt = 0;
        
        }
    }
    */
}

