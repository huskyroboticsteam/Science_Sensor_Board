#include <project.h>
#include <math.h>
#include <TempHum_Lib.h>
#include <HumTemp_I2C.h>

// CRC-8 calculation for SHT31 (Polynomial 0x31)
static uint8_t crc8(const uint8_t *data, int len) {
    const uint8_t POLYNOMIAL = 0x31;
    uint8_t crc = 0xFF;

    for (int j = 0; j < len; ++j) {
        crc ^= data[j];
        for (int i = 0; i < 8; ++i) {
            crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
        }
    }
    return crc;
}

bool SHT31_Init() {
    uint8_t status = 0;
    HumTemp_I2C_Start();
    
    writeCommand(SHT31_SOFTRESET);
    
    CyDelay(10);

    return (SHT31_ReadStatus() != 0xFFFF);
}

static bool writeCommand(uint16_t command) {
    
    uint8_t cmd[2];
    cmd[0] = command >> 8;
    cmd[1] = command & 0xFF;
    
    uint8_t err = 0;
    uint8_t err2 = 0;
    
    // Ensure bus is free (clear any old errors)
    HumTemp_I2C_I2CMasterClearStatus();

    // 1) Send a true START for the first byte
    err = HumTemp_I2C_I2CMasterSendStart(SHT31_I2C_ADDR, HumTemp_I2C_I2C_WRITE_XFER_MODE, TIMEOUT);
    
    char buffer[64];
    
    if(err != HumTemp_I2C_I2C_MSTR_NO_ERROR){
        UART_UartPutString("Error with write Command sendStart\r\n");
        sprintf(buffer, "err = %d\n", err);
        UART_UartPutString(buffer);
        
    }
    
    err = HumTemp_I2C_I2CMasterWriteByte(cmd[0], TIMEOUT);
    
    
    err2 = HumTemp_I2C_I2CMasterWriteByte(cmd[1], TIMEOUT);
    

    if(err != HumTemp_I2C_I2C_MSTR_NO_ERROR){
        UART_UartPutString("Error with writeCommand: writeByte\r\n");
        sprintf(buffer, "err = %d\n", err);
        UART_UartPutString(buffer);
    }
        if(err2 != HumTemp_I2C_I2C_MSTR_NO_ERROR){
        UART_UartPutString("Error with writeCommand: writeByte2\r\n");
        sprintf(buffer, "err = %d\n", err);
        UART_UartPutString(buffer);
    }
        
    if (err != HumTemp_I2C_I2C_MSTR_NO_ERROR)
        return false;
    
    HumTemp_I2C_I2CMasterSendStop(TIMEOUT);
    
    return true;
}

uint16_t SHT31_ReadStatus(void) {
    uint8_t err = 0;
    
    if(!writeCommand(SHT31_READSTATUS)){
        UART_UartPutString("Error with Read Status: writeCommand\r\n");
    }

    HumTemp_I2C_I2CMasterClearStatus();
    
    err = HumTemp_I2C_I2CMasterSendStart(SHT31_I2C_ADDR, HumTemp_I2C_I2C_READ_XFER_MODE, TIMEOUT);
    
    uint8_t data[3] = {0};
    
    if(err != HumTemp_I2C_I2C_MSTR_NO_ERROR){
        UART_UartPutString("Error with Read Status: SendStart\r\n");
    }
    
    err = HumTemp_I2C_I2CMasterReadByte(HumTemp_I2C_I2C_ACK_DATA, &data[0], TIMEOUT);
    CyDelay(50);
    err = HumTemp_I2C_I2CMasterReadByte(HumTemp_I2C_I2C_ACK_DATA, &data[1], TIMEOUT);
    CyDelay(50);
    err = HumTemp_I2C_I2CMasterReadByte(HumTemp_I2C_I2C_NAK_DATA, &data[2], TIMEOUT);
    CyDelay(50);
    
    HumTemp_I2C_I2CMasterSendStop(TIMEOUT);
    
    if(err != HumTemp_I2C_I2C_MSTR_NO_ERROR){
         UART_UartPutString("Error with Read Status: ReadBuf\r\n");
    }
    
    uint16_t status = ((uint16_t)data[0] << 8) | data[1];
    
    return status;
}

void SHT31_Reset(void) {
    writeCommand(SHT31_SOFTRESET);
    CyDelay(10);
}

void SHT31_Heater(bool enable) {
    writeCommand(enable ? SHT31_HEATEREN : SHT31_HEATERDIS);
    CyDelay(1);
}

bool SHT31_IsHeaterEnabled(void) {
    uint16_t reg = SHT31_ReadStatus();
    return (reg != 0xFFFF) && (reg & (1 << SHT31_REG_HEATER_BIT));
}

bool SHT31_ReadTempHum(float *outTemp, float *outHum) {
    uint8_t readbuffer[6] = {0};
    uint8_t err;
    char buffer[64];
    
    CyDelay(50);
    
    if (!writeCommand(SHT31_MEAS_HIGHREP)){ //preps sensor by sending command
        CyDelay(50);
        UART_UartPutString("Error with Read Temp Hum: writeCommand\r\n");
        return false;
    }
    
    CyDelay(50);
    
    HumTemp_I2C_I2CMasterClearStatus();
    
    err = HumTemp_I2C_I2CMasterSendStart(SHT31_I2C_ADDR, HumTemp_I2C_I2C_READ_XFER_MODE, TIMEOUT);
    
    CyDelay(50);
    
        if(err != HumTemp_I2C_I2C_MSTR_NO_ERROR){
            UART_UartPutString("Error with Read Temp Hum: SendStart\r\n");
            HumTemp_I2C_I2CMasterSendStop(TIMEOUT);
           }
        
    for(int i = 0; i < 6; i++){ //get data byte by byte
        CyDelay(50);
        err = HumTemp_I2C_I2CMasterReadByte((i<5) ? HumTemp_I2C_I2C_ACK_DATA : HumTemp_I2C_I2C_NAK_DATA, &readbuffer[i], TIMEOUT);
        if(err != HumTemp_I2C_I2C_MSTR_NO_ERROR){
            UART_UartPutString("Error with Read Temp Hum: readByte\r\n");
            sprintf(buffer, "i = %d\t", i);
            UART_UartPutString(buffer);
        }
    }
    
    if (readbuffer[2] != crc8(readbuffer, 2) || readbuffer[5] != crc8(readbuffer + 3, 2))
        return false;

    int32_t stemp = ((uint32_t)readbuffer[0] << 8) | readbuffer[1]; //handle temp data
    stemp = ((4375 * stemp) >> 14) - 4500;
    float temp;
    temp = (float)stemp / 100.0f;
    
    uint32_t shum = ((uint32_t)readbuffer[3] << 8) | readbuffer[4]; //handle humidity data
    shum = (625 * shum) >> 12;
    float humidity;
    humidity = (float)shum / 100.0f;
    
    
    *outTemp = temp; //assign variables to og temp var
    *outHum = humidity; //assign variables to og temp var
    
    HumTemp_I2C_I2CMasterSendStop(TIMEOUT);

    return true; 
}
 

