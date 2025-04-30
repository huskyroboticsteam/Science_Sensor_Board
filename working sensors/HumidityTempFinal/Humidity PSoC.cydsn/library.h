#pragma once

#include <project.h>
#include <math.h>
#include <stdbool.h>
#include <I2C.h>

#define SHT31_I2C_ADDR 0x44
#define TIMEOUT 20
#define SHT31_MEAS_HIGHREP_STRETCH 0x2C06 // Measurement High Repeatability with Clock Stretch Enabled /
#define SHT31_MEAS_MEDREP_STRETCH 0x2C0D // Measurement Medium Repeatability with Clock Stretch Enabled/
#define SHT31_MEAS_LOWREP_STRETCH 0x2C10 // Measurement Low Repeatability with Clock Stretch Enabled*/                                              \
  
#define SHT31_MEAS_HIGHREP 0x2400 // Measurement High Repeatability with Clock Stretch Disabled /                                                     \
  
#define SHT31_MEAS_MEDREP 0x240B // Measurement Medium Repeatability with Clock Stretch Disabled//
#define SHT31_MEAS_LOWREP 0x2416 // Measurement Low Repeatability with Clock Stretch Disabled */

#define SHT31_READSTATUS 0xF32D   // Read Out of Status Register //
#define SHT31_CLEARSTATUS 0x3041  /**< Clear Status**/
#define SHT31_SOFTRESET 0x30A2    // Soft Reset */
#define SHT31_HEATEREN 0x306D     // Heater Enable /
#define SHT31_HEATERDIS 0x3066    // Heater Disable//
#define SHT31_REG_HEATER_BIT 0x0d /* Status Register Heater Bit/ **/

static uint8_t crc8(const uint8_t *data, int len);

static bool writeCommand(uint16_t command);

static bool readData(uint8_t *buffer, uint8_t len);

bool SHT31_Init(void);

uint16_t SHT31_ReadStatus(void);

void SHT31_Reset(void);

void SHT31_Heater(bool enable);

bool SHT31_IsHeaterEnabled(void);

bool SHT31_ReadTempHum(float *outTemp, float *outHum);

float SHT31_ReadTemperature(void);

float SHT31_ReadHumidity(void);
