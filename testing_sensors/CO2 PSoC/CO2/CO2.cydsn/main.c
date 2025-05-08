//based on arduino code by Tiequan Shao: tiequan.shao@sandboxelectronics.com
    //Peng Wei: peng.wei@sandboxelectronics.com


#include <project.h>
#include <math.h>
#include <stdio.h>

/* Hardware & calibration macros */
#define DC_GAIN                   (8.5f)
#define ZERO_POINT_VOLTAGE        (3.328f/DC_GAIN) 

#define REACTION_VOLTAGE          (0.030f)          
#define ADC_COUNTS                (4095.0f)  /* 12-bit */
#define VREF                      (5.0f)     /* volts */

/* CO2 curve: { x = log10(ppm), y = sensor voltage @ that ppm, slope } */
static float CO2Curve[3] = {
    2.602f, 
    ZERO_POINT_VOLTAGE, 
    (REACTION_VOLTAGE / (2.602f - 3.0f))
};

float MG_ReadVoltage(void);
int   MG_GetPercentage(float volts);

int main(void)
{
    char buf[64];
    float volts;
    int   ppm;
    
    CyGlobalIntEnable;          /* Enable global interrupts */
    
    UART_Start();             /* Initialize UART at 9600bps */ 
    ADC_Start();          /* Initialize SAR ADC */ 
    
    UART_PutString("MG-811 Demonstration\r\n");
    
    for (;;)
    {
        /* 1) Read sensor voltage */
        volts = MG_ReadVoltage();
        
        /* 2) Compute CO2 concentration */
        ppm = MG_GetPercentage(volts);
        
        /* 3) Print readings */
        sprintf(buf, "SEN0159: %.3f V    CO2: %s%d ppm\r\n",
                volts,
                (ppm < 0) ? "<" : "",
                (ppm < 0) ? 400 : ppm);
        UART_PutString(buf);
        
        /* 4) Read BOOL_PIN and print its state */
        if (BOOL_PIN_Read() == 1)
        {
            UART_PutString("=====BOOL is HIGH======\r\n");
        }
        else
        {
            UART_PutString("=====BOOL is LOW=======\r\n");
        }
        
        CyDelay(500);
    }
}


/**
 * @brief  Take multiple ADC readings, average, and convert to voltage.
 * @return Sensor voltage in volts.
 */
float MG_ReadVoltage(void)
{
    uint32_t sum = 0;
    uint16_t sample;
    const int READ_SAMPLE_TIMES     = 5;
    const int READ_SAMPLE_INTERVAL  = 50;  /* ms */
    int i;
    
    for (i = 0; i < READ_SAMPLE_TIMES; i++)
    {
        ADC_StartConvert();
        ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
        sample = ADC_GetResult16();
        sum   += sample;
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
    if ((volts / DC_GAIN) >= ZERO_POINT_VOLTAGE)
        return -1;
    
    /* Logarithmic interpolation */
    float x = ((volts / DC_GAIN) - CO2Curve[1]) / CO2Curve[2] + CO2Curve[0];
    return (int)powf(10.0f, x);
}
