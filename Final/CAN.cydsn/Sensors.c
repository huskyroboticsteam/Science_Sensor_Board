#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <TempHum_Lib.h>
#include <project.h>
#include <HumTemp_I2C.h>

int32_t getTemp(){
    float temp, hum;
    bool status;
    status = SHT31_IsHeaterEnabled();
    if(!status){
        return -1;
    }
    status = SHT31_ReadTempHum(&temp, &hum);
    if(!status){
        return -1;
    }
     char buffer[64];
    snprintf(buffer, sizeof(buffer), "Temperature (degrees Celcius) = %d\t\t", (int) temp);
    UART_UartPutString(buffer);
    return (int32_t) temp;
}

int32_t getHum(){
    float temp, hum;
    bool status;
    status = SHT31_IsHeaterEnabled();
    if(!status){
        return -1;
    }
    status = SHT31_ReadTempHum(&temp, &hum);
    if(!status){
        return -1;
    }
     char buffer[64];
    snprintf(buffer, sizeof(buffer), "Temperature (degrees Celcius) = %d\t\t", (int) hum);
    UART_UartPutString(buffer);
    return (int32_t) hum;
}
