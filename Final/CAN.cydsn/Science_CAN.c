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

#include <stdio.h>
#include <math.h>
#include <CAN.h>
#include <stdbool.h>
#include "main.h"
#include "TempHum_Lib.h"
#include "Sensors.h"
#include "Science_CAN.h"
#include "HindsightCAN/Port.h"


extern char txData[TX_DATA_SIZE];
extern uint8 address;

//Reads from CAN FIFO and changes the state and mode accordingly
int ProcessCAN(CANPacket* receivedPacket, CANPacket* packetToSend) {
    uint16_t packageID = GetPacketID(receivedPacket);
    uint8_t sender_DG = GetSenderDeviceGroupCode(receivedPacket);
    uint8_t sender_SN = GetSenderDeviceSerialNumber(receivedPacket);
    int32_t data = 0;
    int err = 0;
    float temp, hum;
    bool status;
    
    switch(packageID){            
        // Common Packets
        
        case(ID_ESTOP):
            Print("\r\n\r\nSTOP\r\n\r\n");
            err = ESTOP_ERR_GENERAL;
            break;
        
        case(ID_TELEMETRY_PULL):            
            switch(DecodeTelemetryType(receivedPacket))
            {                
                case(PACKET_TELEMETRY_SENSOR1):
                    data = getTemp();
                    break;
                case(PACKET_TELEMETRY_SENSOR2):
                    data = 0; // TODO
                    break;
                case(PACKET_TELEMETRY_SENSOR3):
                    data = getHum();
                    break;
                case(PACKET_TELEMETRY_SENSOR4):
                    data = 0; // TODO
                    break;
                case(PACKET_TELEMETRY_SENSOR5):
                    data = 0; // TODO
                    break;
                case(PACKET_TELEMETRY_SENSOR6):
                    data = 0; // TODO
                    break;
                default:
                    err = ERROR_INVALID_TTC;
                    break;
            }
            
            // Assemble and send packet
            AssembleTelemetryReportPacket(packetToSend, sender_DG, sender_SN, receivedPacket->data[3], data);
            
            if (err == 0 && data != -1)
                SendCANPacket(packetToSend);
            
            break;
            
        default: //recieved Packet with non-valid ID
            return ERROR_INVALID_PACKET;
    }
    
    return err;
}

void PrintCanPacket(CANPacket packet){
    for(int i = 0; i < packet.dlc; i++ ) {
        sprintf(txData,"Byte%d %x   ", i+1, packet.data[i]);
        Print(txData);
    }

    sprintf(txData,"ID:%x %x %x\r\n",packet.id >> 10, 
        (packet.id >> 6) & 0xF , packet.id & 0x3F);
    Print(txData);
}


/* [] END OF FILE */