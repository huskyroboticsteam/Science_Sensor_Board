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
    return (int32_t) temp;
}
