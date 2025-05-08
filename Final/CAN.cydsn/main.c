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
#include <CAN.h>
#include "HindsightCAN/CANLibrary.h"
//#include "HindsightCAN/CANScience.h"

#include <project.h>
#include "HumTemp_I2C.h"
#include "main.h"
#include "TempHum_Lib.h"

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
        
        /*
        if (UART_SpiUartGetRxBufferSize()) {
            DebugPrint(UART_UartGetByte());
        }
        */
        CyDelay(10);
    }
}

void Initialize(void) {
    uint32 err;
    CyGlobalIntEnable; /* Enable global interrupts. LED arrays need this first */
    
    sprintf(txData, "\r\nHello\r\n");
    Print(txData);
    
    address = 0; // TODO replace with science sensor address
    
    UART_Start();
    SHT31_Init();
    InitCAN(DEVICE_GROUP_SCIENCE, (int) address);
    //Timer_Period_Reset_Start();
    //isr_Period_Reset_StartEx(Period_Reset_Handler);
    
    sprintf(txData, "Address: %x \r\n", address);
    Print(txData);
    
    CyDelay(30); // ensure sensors are initialized
}

/* [] END OF FILE */