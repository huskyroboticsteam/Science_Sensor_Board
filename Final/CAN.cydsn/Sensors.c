#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <TempHum_Lib.h>
#include <project.h>
#include <I2C.h>

bool enableHeater = false;

int main(void)
{
    
    CyGlobalIntEnable;
    UART_Start();
    
    UART_UartPutString("__________________________\r\n");
    
    CyDelay(100);
    
    if (!SHT31_Init()) {
        UART_UartPutString("Couldn't find SHT31\r\n");
    }
    
    
    //UART_UartPutString("Heater Enabled State: ");
    if (SHT31_IsHeaterEnabled()) {
        //UART_UartPutString("ENABLED\r\n");
    } else {
        //UART_UartPutString("DISABLED\r\n");
    }
    
    CyDelay(100);
    
    
    /*
    I2C_I2CMasterClearStatus();
    uint8_t err = I2C_I2CMasterSendStart(0x44, I2C_I2C_WRITE_XFER_MODE, 10);
    I2C_I2CMasterSendStop(TIMEOUT);
    
    
    CyDelay(1000);
    if(err == I2C_I2C_MSTR_NO_ERROR){
        char buffer[32];
        sprintf(buffer, "Found I2C device at 0x%02X\r\n", 0x44);
        UART_UartPutString(buffer);
    } else{
        char buffer[32];
        sprintf(buffer, "no I2C device at 0x%02X\r\n", 0x44);
        UART_UartPutString(buffer);
    }
    */
    
    CyDelay(50);
    for(;;){
        float temperature = 0;
        float humidity = 0;
        bool status = SHT31_ReadTempHum(&temperature, &humidity);

        char buffer[64];
        
        int hum = humidity;
        int temp = temperature;
        
        if (!isnan(temperature) && status) {
            snprintf(buffer, sizeof(buffer), "Temperature (degrees Celcius) = %d\t\t", temp);
            UART_UartPutString(buffer);
        } else {
            UART_UartPutString("Failed to read temperature\r\n");
        }
        
        if (!isnan(humidity) && status) {
            snprintf(buffer, sizeof(buffer), "Humidity (percentage) = %d\r\n", hum);
            UART_UartPutString(buffer);
        } else {
            UART_UartPutString("Failed to read humidity\r\n");
        }

        CyDelay(1000);
        
    }   
}

