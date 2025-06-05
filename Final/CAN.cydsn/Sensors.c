#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <TempHum_Lib.h>
#include <project.h>
#include <HumTemp_I2C.h>
#include "main.h"
#include "Sensors.h"

int32_t getTemp(){
    float temp, hum;
    bool status;
    status = SHT31_IsHeaterEnabled(); //ignore result cus heater doesn't rlly for our implementation

    status = SHT31_ReadTempHum(&temp, &hum);  //Get data from sensor using I2C
    
    if(!status){
        Print("readTempHum fail");
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
    status = SHT31_ReadTempHum(&temp, &hum);
    if(!status){
        return -1;
    }
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Temperature (degrees Celcius) = %d\t\t", (int) hum);
    UART_UartPutString(buffer);
    return (int32_t) hum;
}

int32_t getCO2(){
    char buf[64];
    float volts;
    int   ppm;
    
    /* 1) Read sensor voltage */
    volts = MG_ReadVoltage();
        
    /* 2) Compute CO2 concentration */
    return MG_GetPercentage(volts);

    }


float MG_ReadVoltage(){
    char txData[200];
    uint32_t sum = 0;
    uint16_t sample;
    const int READ_SAMPLE_TIMES     = 5;
    const int READ_SAMPLE_INTERVAL  = 50;  /* ms */
    int i;
    
    for (i = 0; i < READ_SAMPLE_TIMES; i++)
    {
        ADC_StartConvert();
        ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
        sample = ADC_GetResult16(0);
        sum   += sample;
        sprintf(txData, "Sample: %d\n\r", sample);
        UART_UartPutString(txData);
        CyDelay(READ_SAMPLE_INTERVAL);
    }
    
    /* Average and scale to voltage */
    float avgCounts = (float)sum / READ_SAMPLE_TIMES;
    return (avgCounts * VREF / ADC_COUNTS);
}


/**
 * @brief  Convert voltage reading to CO2 ppm using log-linear approximation.
 * @param  volts  Sensor voltage (V).
 * @return CO2 in ppm, or -1 if below zero point (i.e. <400 ppm).
 */
int MG_GetPercentage(float volts)
{
    /* If sensor voltage (after gain) is above zero point → low CO2 */
    //if ((volts / DC_GAIN) >= ZERO_POINT_VOLTAGE)
        //return -1;
    
    /* Logarithmic interpolation */
    float x = ((volts / DC_GAIN) - CO2Curve[1]) / CO2Curve[2] + CO2Curve[0];
    return (int)powf(10.0f, x);
}
