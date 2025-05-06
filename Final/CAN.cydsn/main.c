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

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "cyapicallbacks.h"
#include <Science_CAN.h>
#include "./HindsightCAN/CANLibrary.h"

// UART stuff
char txData[TX_DATA_SIZE];

// CAN stuff
CANPacket can_recieve;
CANPacket can_send;
uint8 address = 0;

int main(void)
{ 
    Initialize();
    
    int err;
    
    for(;;)
    {
        err = 0;
        
        if (!PollAndReceiveCANPacket(&can_recieve)) {
            err = ProcessCAN(&can_recieve, &can_send);
        }
        
        if (err) DisplayErrorCode(err);
        
        if (DBG_UART_SpiUartGetRxBufferSize()) {
            DebugPrint(DBG_UART_UartGetByte());
        }
        
        CyDelay(10);
    }
}

void Initialize(void) {
    uint32 err;
    CyGlobalIntEnable; /* Enable global interrupts. LED arrays need this first */
    
    sprintf(txData, "\r\nHello\r\n");
    Print(txData);
    
    address = 0; // TODO replace with science sensor address
    
    Sensor_UART_Start();
    Sensor_I2C_Start();
    InitCAN(DEVICE_GROUP_SCIENCE, (int) address);
    //Timer_Period_Reset_Start();
    //isr_Period_Reset_StartEx(Period_Reset_Handler);
    
    sprintf(txData, "Address: %x \r\n", address);
    Print(txData);
    
    CyDelay(30); // ensure sensors are initialized
    err = initializeSensors();
    if (err) {
        Print("Failed sensor init: ");
        PrintInt(err);
        Print("\r\n");
    }
}

void DebugPrint(char input) {
    uint16 val = -1;
    switch(input) {
        case 't':
            sprintf(txData, "Temp: %li", ReadSensorTemperature());
            break;
        case 'h':
            sprintf(txData, "Moist: %li", ReadSensorHumidity());
            break;
        case 'c':
            sprintf(txData, "CO: %li", ReadSensorCO());
            break;
        case 'd':
            sprintf(txData, "CO2: %li", ReadSensorCO2());
            break;
        case 'o':
            sprintf(txData, "O2: %li", ReadSensorO2());
            break;
        case 'm':
            sprintf(txData, "CH4: %li", ReadSensorCH4());
            break;
        case ' ':
            readReg16crc(SCD41_ADDR, SCD41_REG_get_data_ready_status, &val);
            sprintf(txData, "Data Ready: %X", val);
            // writeReg0(SCD41_ADDR, SCD41_REG_perform_forced_recalibration);
            // sprintf(txData, "Cal");
            break;
        default:
            sprintf(txData, "what");
            break;
    }
    Print(txData);
    Print("\r\n");
}

void DisplayErrorCode(uint8_t code) {    
    
    sprintf(txData, "Error %X\r\n", code);
    Print(txData);

    switch(code)
    {
        case ERROR_INVALID_TTC:
            Print("Cannot Send That Data Type!\n\r");
            break;
        default:
            //some error
            break;
    }
}

/* [] END OF FILE */