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
        
        /*
        uint16_t packageID = 0xF5;
        uint8_t sender_DG = 0x07;
        uint8_t sender_SN = 0x00;
        int32_t data = 0;
        
        CANPacket p; 
        
        p.id = (1|7|0);
        
        p.data[0] = 0xF5;
        p.data[1] = 0x07;
        p.data[2] = 0x00;
        p.data[3] = 0x16;
        
        if(SendCANPacket(&p) == 0){
           Print("Sent");
        }else{
            Print("Not Sent");
        }
        */
        if (!PollAndReceiveCANPacket(&can_recieve)) {
            err = ProcessCAN(&can_recieve, &can_send);
        }
        
        
        /*
        if (UART_SpiUartGetRxBufferSize()) {
            DebugPrint(UART_UartGetByte());
        }
        */
        CyDelay(100);
    }
}

void Initialize(void) {
    uint32 err;
    CyGlobalIntEnable; /* Enable global interrupts. LED arrays need this first */
    
    UART_Start();
    
    sprintf(txData, "\r\nHello\r\n");
    Print(txData);
    
    address = 0; // TODO replace with science sensor address
    
    SHT31_Init();
    InitCAN(DEVICE_GROUP_SCIENCE, (int) address);
    
    sprintf(txData, "Address: %x \r\n", address);
    Print(txData);
    
    CyDelay(30); // ensure sensors are initialized
}


/* [] END OF FILE */