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
#include "HindsightCAN/CANScience.h"
#include "HindsightCAN/CANCommon.h"
#include "DigitalPeripheral.h"

CAN_RX_CFG rxMailbox;

CY_ISR_PROTO(CAN_FLAG_ISR);

extern char txData[TX_DATA_SIZE];
extern uint8 address;

//Reads from CAN FIFO and changes the state and mode accordingly
int ProcessCAN(CANPacket* receivedPacket, CANPacket* packetToSend) {
    uint16_t packageID = GetPacketID(receivedPacket);
    uint8_t sender_DG = GetSenderDeviceGroupCode(receivedPacket);
    uint8_t sender_SN = GetSenderDeviceSerialNumber(receivedPacket);
    int32_t data = 0;
    uint8_t digitalSetValue = 0;
    int err = 0;
    float temp, hum;
    bool status;
    
    Print("\r\n\r\nBeginning of Science_CAN\r\n\r\n");
    
    switch(packageID){            
        // Common Packets
        
        case(ID_ESTOP):
            Print("\r\n\r\nSTOP\r\n\r\n");
            err = ESTOP_ERR_GENERAL;
            break;
        
        case(ID_TELEMETRY_PULL):            
            switch(DecodeTelemetryType(receivedPacket))
            {                
                case(1): //PACKET_TELEMETRY_SENSOR1): //
                    data = getTemp();
                    Print("temp"); 
                    break;
                case(2)://PACKET_TELEMETRY_SENSOR2):
                    Print("CO2");
                    CyDelay(500);
                    data = getCO2();
                    CyDelay(500);
                    break;
                case(3)://PACKET_TELEMETRY_SENSOR3):
                    Print("hum");
                    data = getHum();
                    break;
                case(PACKET_TELEMETRY_SENSOR4):
                    Print("sensor 4");
                    data = 0; // TODO
                    break;
                case(PACKET_TELEMETRY_SENSOR5):
                    data = 0; // TODO
                    Print("sensor 5");
                    break;
                case(PACKET_TELEMETRY_SENSOR6):
                    data = 0; // TODO
                    Print("sensor 6");
                    break;
                default:
                    Print("Default");
                    err = ERROR_INVALID_TTC;
                    break;
            }
            
            // Assemble and send packet
            AssembleTelemetryReportPacket(packetToSend, sender_DG, sender_SN, receivedPacket->data[3], data);
            
            if (err == 0 ){
                SendCANPacket(packetToSend);
            }
            break;
        
        case(ID_DIGITAL_SET): //CAN format is 1 07 00 FA XX 0/1
            digitalSetValue = GetDigitalSetValueFromPacket(receivedPacket);
            switch (GetDigitalSetAddrFromPacket(receivedPacket))
            {
                case(SCIENCE_SENSOR_LASER): 
                    Print("laser");
                    setLaser(digitalSetValue);
                    break;
                case(SCIENCE_SENSOR_WATER_PUMP_1):
                    Print("water pump 1");
                    setWaterPump1(digitalSetValue);
                    break;
                case(SCIENCE_SENSOR_WATER_PUMP_2):
                    Print("water pump 2");
                    setWaterPump2(digitalSetValue);
                    break;
                default:
                    Print("Default");
                    err = ERROR_INVALID_ARG;
                    break;
            }

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
