#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <library.h>
#include <project.h>
#include <I2C.h>

bool enableHeater = false;

int main(void)
{
    
    CyGlobalIntEnable;
    UART_Start();
    
    UART_UartPutString("__________________________\r\n");
    UART_UartPutString("SHT31 test\r\n");
    
    CyDelay(100);
    
    if (!SHT31_Init()) {
        UART_UartPutString("Couldn't find SHT31\r\n");
    }
    
    UART_UartPutString("Heater Enabled State: ");
    if (SHT31_IsHeaterEnabled()) {
        UART_UartPutString("ENABLED\r\n");
    } else {
        UART_UartPutString("DISABLED\r\n");
    }
    
    CyDelay(1000);
    
    I2C_I2CMasterClearStatus();
    uint8_t err = I2C_I2CMasterSendStart(0x44, I2C_I2C_WRITE_XFER_MODE, 10);
    I2C_I2CMasterSendStop(TIMEOUT);
    
    CyDelay(1000);
    if(err == I2C_I2C_MSTR_NO_ERROR){
        char buffer[32];
        sprintf(buffer, "Found I2C device at 0x%02X\r\n", 0x44);
        UART_UartPutString(buffer);
    } 
    
    CyDelay(50);
    //for(;;){
        float t = SHT31_ReadTemperature();
        float h = SHT31_ReadHumidity();

        char buffer[64];
        
        if (!isnan(t)) {
            snprintf(buffer, sizeof(buffer), "TempC = %.2f\t\t", t);
            UART_UartPutString(buffer);
        } else {
            UART_UartPutString("Failed to read temperature\r\n");
        }

        if (!isnan(h)) {
            snprintf(buffer, sizeof(buffer), "Hum. %% = %.2f\r\n", h);
            UART_UartPutString(buffer);
        } else {
            UART_UartPutString("Failed to read humidity\r\n");
        }

        CyDelay(1000);

    //}
    
   
    
}

