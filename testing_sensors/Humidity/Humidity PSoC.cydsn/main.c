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
    
    
     UART_UartPutString("SHT31 test\r\n");
    
    
    if (!SHT31_Init()) {
        UART_UartPutString("Couldn't find SHT31\r\n");
        for(;;) CyDelay(1000);
    }
    
    UART_UartPutString("Heater Enabled State: ");
    if (SHT31_IsHeaterEnabled()) {
        UART_UartPutString("ENABLED\r\n");
    } else {
        UART_UartPutString("DISABLED\r\n");
    }
/*
    for(;;)
    {
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

    }
    */
    
}

