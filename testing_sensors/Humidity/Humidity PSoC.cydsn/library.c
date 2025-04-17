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
    uint8_t cmd[2] = { command >> 8, command & 0xFF };
    
    uint8_t err = 0;
    
    I2C_I2CMasterClearStatus();
    
    err = I2C_I2CMasterSendStart(SHT31_I2C_ADDR, I2C_I2C_WRITE_XFER_MODE, TIMEOUT);
    if (err)
        return false;
    
    return true;
}


bool SHT31_Init() {
    uint8_t status = 0;
    I2C_Start();
    
    I2C_I2CMasterClearStatus();
    
    status = I2C_I2CMasterSendStart(SHT31_I2C_ADDR, I2C_I2C_READ_XFER_MODE, TIMEOUT);
    
    I2C_I2CMasterSendStop(TIMEOUT);
    
    return (status == I2C_I2C_MSTR_NO_ERROR);
}
 //if (I2C_MasterReadBuf(SHT31_I2C_ADDR, buffer, len, I2C_MODE_COMPLETE_XFER) != I2C_MSTR_NO_ERROR)
       // return false;

uint16_t SHT31_ReadStatus(void) {
    uint8_t err = 0;
    writeCommand(SHT31_READSTATUS);

    I2C_I2CMasterClearStatus();
    
    I2C_I2CMasterSendStart(SHT31_I2C_ADDR, I2C_I2C_READ_XFER_MODE, TIMEOUT);
    uint8_t data[3] = {0};

    CyDelay(2);
    
    err = I2C_I2CMasterReadBuf(SHT31_I2C_ADDR, data, 3, I2C_I2C_MODE_COMPLETE_XFER);
    
    if(err){
        I2C_I2CMasterSendStop(TIMEOUT);
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
    if (!writeCommand(SHT31_MEAS_HIGHREP))
        return false;
    
    CyDelay(20);

    //read status
    I2C_I2CMasterClearStatus();
    
    err = I2C_I2CMasterSendStart(SHT31_I2C_ADDR, I2C_I2C_READ_XFER_MODE, TIMEOUT);
    uint8_t data[3] = {0};

    CyDelay(2);

    err = I2C_I2CMasterReadBuf(SHT31_I2C_ADDR, readbuffer, 6, I2C_I2C_MODE_COMPLETE_XFER);
    
    if(err){
        I2C_I2CMasterSendStop(TIMEOUT);
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
