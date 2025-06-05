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
#pragma once
   
#include "cyapicallbacks.h"
#include <stdint.h>
#include <stdlib.h>
#include <CAN.h>
#include "HindsightCAN/CANLibrary.h"

#define TX_DATA_SIZE            (100u)

#define Print(message) UART_UartPutString(message)
#define PrintChar(character) UART_UartPutChar(character)
#define PrintInt(integer) UART_UartPutString(itoa(integer, txData, 10))
#define PrintIntBin(integer) UART_UartPutString(itoa(integer, txData, 2))

void Initialize(void);
int getSerialAddress();
void DebugPrint(char input);
void DisplayErrorCode(uint8_t code);
void InitCAN();

/* [] END OF FILE */