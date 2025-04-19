#include <project.h>
#include <math.h>
#include <library.h>
#include <I2C.h>

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

static bool writeCommand(uint16_t command) {
    
    uint8_t cmd[2];
    cmd[0] = command >> 8;
    cmd[1] = command & 0xFF;
    
    uint8_t err = 0;
    uint8_t err2 = 0;
    
    I2C_I2CMasterClearStatus();
    
    err = I2C_I2CMasterSendStart(SHT31_I2C_ADDR, I2C_I2C_WRITE_XFER_MODE, TIMEOUT);
    
    char buffer[64];
    
    if(err != I2C_I2C_MSTR_NO_ERROR){
        UART_UartPutString("Error with write Command\r\n");
        sprintf(buffer, "err = %d\n", err);
        UART_UartPutString(buffer);
        I2C_I2CMasterSendStop(TIMEOUT);
        
    }
    
    CyDelay(50);
    
    err = I2C_I2CMasterWriteByte(cmd[0], TIMEOUT);
    
    CyDelay(50);
    err2 = I2C_I2CMasterWriteByte(cmd[1], TIMEOUT);
    

    if(err != I2C_I2C_MSTR_NO_ERROR){
        UART_UartPutString("Error with writeCommand: writeByte\r\n");
        sprintf(buffer, "err = %d\n", err);
        UART_UartPutString(buffer);
        I2C_I2CMasterSendStop(TIMEOUT);
    }
        if(err2 != I2C_I2C_MSTR_NO_ERROR){
        UART_UartPutString("Error with writeCommand: writeByte\r\n");
        sprintf(buffer, "err = %d\n", err);
        UART_UartPutString(buffer);
        I2C_I2CMasterSendStop(TIMEOUT);
    }
    
    I2C_I2CMasterSendStop(TIMEOUT);
        
    if (err != I2C_I2C_MSTR_NO_ERROR)
        return false;
    
    return true;
}


bool SHT31_Init() {
    uint8_t status = 0;
    I2C_Start();
    
    status = I2C_I2CMasterSendStart(SHT31_I2C_ADDR, I2C_I2C_READ_XFER_MODE, TIMEOUT);
    
    I2C_I2CMasterSendStop(TIMEOUT);
    
    return (status == I2C_I2C_MSTR_NO_ERROR);
}
 //if (I2C_MasterReadBuf(SHT31_I2C_ADDR, buffer, len, I2C_MODE_COMPLETE_XFER) != I2C_MSTR_NO_ERROR)
       // return false;

uint16_t SHT31_ReadStatus(void) {
    uint8_t err = 0;
    if(!writeCommand(SHT31_READSTATUS)){
        I2C_I2CMasterSendStop(TIMEOUT);
        UART_UartPutString("Error with Read Status: writeCommand\r\n");
    }

    I2C_I2CMasterClearStatus();
    
    err = I2C_I2CMasterSendStart(SHT31_I2C_ADDR, I2C_I2C_READ_XFER_MODE, TIMEOUT);
    
    
    //I2C_I2CMasterSendRestart(SHT31_I2C_ADDR, I2C_I2C_READ_XFER_MODE, TIMEOUT);
    uint8_t data[3] = {0};
    
    if(err != I2C_I2C_MSTR_NO_ERROR){
        UART_UartPutString("Error with Read Status: SendStart\r\n");
        I2C_I2CMasterSendStop(TIMEOUT);
    }

    CyDelay(50);
    
    //err = I2C_I2CMasterReadBuf(SHT31_I2C_ADDR, data, 3, I2C_I2C_MODE_COMPLETE_XFER);
    err = I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA, &data[0], TIMEOUT);
    CyDelay(50);
    err = I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA, &data[1], TIMEOUT);
    CyDelay(50);
    err = I2C_I2CMasterReadByte(I2C_I2C_NAK_DATA, &data[2], TIMEOUT);
    CyDelay(50);
    
    if(err != I2C_I2C_MSTR_NO_ERROR){
        I2C_I2CMasterSendStop(TIMEOUT);
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
    uint8_t readbuffer[6];
    uint8_t err;
    if (!writeCommand(SHT31_MEAS_HIGHREP)){
        UART_UartPutString("Error with Read Temp Hum: writeCommand\r\n");
        return false;
    }
    
    //CyDelay(20);
/*
    err = I2C_I2CMasterClearStatus();
    if(err){
        UART_UartPutString("Error with Read Temp Hum: ClearStatus\r\n");
        I2C_I2CMasterSendStop(TIMEOUT);
    }
    */
    I2C_I2CMasterClearStatus();
    
    err = I2C_I2CMasterSendStart(SHT31_I2C_ADDR, I2C_I2C_READ_XFER_MODE, TIMEOUT);
    
    if(err){
        UART_UartPutString("Error with Read Temp Hum: SendStart\r\n");
        I2C_I2CMasterSendStop(TIMEOUT);
    }
   /*
    I2C_I2CMasterSendRestart(SHT31_I2C_ADDR, I2C_I2C_READ_XFER_MODE, TIMEOUT);
    if(err){
        UART_UartPutString("Error with Read Status: SendRestart\r\n");
        I2C_I2CMasterSendStop(TIMEOUT);
    }
    */
    uint8_t data[3] = {0};

    CyDelay(50);
    
    //err = I2C_I2CMasterReadBuf(SHT31_I2C_ADDR, readbuffer, 6, I2C_I2C_MODE_COMPLETE_XFER);
    char buffer[64];
    for(int i = 0; i < 6; i++){
        CyDelay(50);
        err = I2C_I2CMasterReadByte((i<5) ? I2C_I2C_ACK_DATA : I2C_I2C_NAK_DATA, &readbuffer[i], TIMEOUT);
        if(err != I2C_I2C_MSTR_NO_ERROR){
            UART_UartPutString("Error with Read Temp Hum: readByte\r\n");
            sprintf(buffer, "i = %d\t", i);
            UART_UartPutString(buffer);
            I2C_I2CMasterSendStop(TIMEOUT);
        }
    }
    
    
    
    if (readbuffer[2] != crc8(readbuffer, 2) || readbuffer[5] != crc8(readbuffer + 3, 2))
        return false;

    int32_t stemp = ((uint32_t)readbuffer[0] << 8) | readbuffer[1];
    stemp = ((4375 * stemp) >> 14) - 4500;
    float temp;
    temp = (float)stemp / 100.0f;

    uint32_t shum = ((uint32_t)readbuffer[3] << 8) | readbuffer[4];
    shum = (625 * shum) >> 12;
    float humidity;
    humidity = (float)shum / 100.0f;
    
    *outTemp = temp;
    *outHum = humidity;
    
    //I2C_I2CMasterSendStop(TIMEOUT);
    
    return true;
}

float SHT31_ReadTemperature(void) {
    float t;
    return SHT31_ReadTempHum(&t, NULL) ? t : NAN;
}

float SHT31_ReadHumidity(void) {
    float h;
    return SHT31_ReadTempHum(NULL, &h) ? h : NAN;
}
