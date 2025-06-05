#pragma once
#include <stdio.h>

/* Hardware & calibration macros */
#define DC_GAIN                   (0.8f)
#define ZERO_POINT_VOLTAGE        (3.28f/DC_GAIN) 

#define REACTION_VOLTAGE          (0.03f)          
#define ADC_COUNTS                (65535.0f)  /* 12-bit */
#define VREF                      (3.3f)     /* volts */

static float CO2Curve[3] = {
    2.602f, 
    ZERO_POINT_VOLTAGE, 
    (REACTION_VOLTAGE / (2.602f - 3.0f))
};

float MG_ReadVoltage();
int MG_GetPercentage(float);
int32_t getTemp();
int32_t getHum();
int32_t getCO2();
