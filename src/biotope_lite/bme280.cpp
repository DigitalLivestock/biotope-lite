/*****************************************************************************
 * File: bme280.cpp
 * Author: DigitalLivesock
 * Date: 2026-02-07
 * Description: Temperature, Humidity and Pressure sensor BME280 related 
 *              definitions and methods.
/*****************************************************************************
 * Copyright (C) 2026 DigitalLivestock
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *****************************************************************************/

#include "bme280.h"

void bme_init()
{
  /*Get calibration values: */
  bme_get_calibration(&t_calibration[0], &p_calibration[0], &h_calibration[0]);
}

void bme_cmd(uint8_t addr, uint8_t reg, uint8_t cmd, bool rs)
{
  twi_write(addr, reg, cmd);
}

float bme_get_temp(void)
{
  int32_t var1, var2;

  int32_t adc_T = (uint32_t)twi_read(BMEADDR, TEMP_MSB) << 16 | (uint32_t)twi_read(BMEADDR, TEMP_LSB) << 8;
  
  adc_T >>= 4;

  var1 = (int32_t)((adc_T / 8) - ((int32_t)t_calibration[0] * 2));
  var1 = (var1 * (int32_t)t_calibration[1]) / 2048;
  
  var2 = (int32_t)((adc_T / 16) - ((int32_t)t_calibration[0]));
  var2 = (((var2 * var2) / 4096) * (int32_t)t_calibration[2]) / 16384;

  t_fine = var1 + var2;

  int32_t T = (t_fine * 5 + 128) / 256;

  return (float)T / 100;
}

float bme_get_pres(void)
{
  int64_t var1, var2, var3, var4;

  int32_t adc_P = (uint32_t)twi_read(BMEADDR, PRESS_MSB) << 16 | 
                  (uint32_t)twi_read(BMEADDR, PRESS_LSB) << 8 |
                  (uint32_t)twi_read(BMEADDR, PRESS_XLSB);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)p_calibration[5];
  var2 = var2 + ((var1 * (int64_t)p_calibration[4]) * 131072);
  var2 = var2 + (int64_t)p_calibration[3] * 34359738368;
  var1 = ((var1 * var1 * (int64_t)p_calibration[2]) / 256) +
         ((var1 * (int64_t)p_calibration[1] * 4096));
  var3 = ((int64_t)1) * 140737488355328;
  var1 = (var3 + var1) * (int64_t)p_calibration[0] / 8589934592;

  if (var1 == 0) {
    return 0; // avoid exception caused by division by zero
  }

  var4 = 1048576 - adc_P;
  var4 = (((var4 * 2147483648) - var2) * 3125) / var1;
  var1 = ((int64_t)p_calibration[8] * (var4 / 8192) * (var4 / 8192)) / 33554432;
  var2 = ((int64_t)p_calibration[7] * var4) / 524288;
  var4 = ((var4 + var1 + var2) / 256) + ((int64_t)p_calibration[6] * 16);

  float P = var4 / 256.0;

  return P;
}

float bme_get_hum()
{
  int32_t var1, var2, var3, var4, var5;

  int32_t adc_H = (uint32_t)twi_read(BMEADDR, HUM_MSB) << 8 | (uint32_t)twi_read(BMEADDR, HUM_LSB);

  var1 = t_fine - ((int32_t)76800);
  var2 = (int32_t)(adc_H * 16384);
  var3 = (int32_t)(((int32_t)h_calibration[3]) * 1048576);
  var4 = ((int32_t)h_calibration[4]) * var1;
  var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
  var2 = (var1 * ((int32_t)h_calibration[5])) / 1024;
  var3 = (var1 * ((int32_t)h_calibration[2])) / 2048;
  var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
  var2 = ((var4 * ((int32_t)h_calibration[1])) + 8192) / 16384;
  var3 = var5 * var2;
  var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
  var5 = var3 - ((var4 * ((int32_t)h_calibration[0])) / 16);
  var5 = (var5 < 0 ? 0 : var5);
  var5 = (var5 > 419430400 ? 419430400 : var5);

  uint32_t H = (uint32_t)(var5 / 4096);

  return (float)H / 1024.0;
}

void bme_get_calibration(uint16_t *t, uint16_t *p, uint16_t *h)
{
  t[0] = ((uint16_t)twi_read(BMEADDR, 0x89) << 8) | (uint16_t)twi_read(BMEADDR, 0x88);
  t[1] = ((uint16_t)twi_read(BMEADDR, 0x8B) << 8) | (uint16_t)twi_read(BMEADDR, 0x8A);
  t[2] = ((uint16_t)twi_read(BMEADDR, 0x8D) << 8) | (uint16_t)twi_read(BMEADDR, 0x8C);

  p[0] = ((uint16_t)twi_read(BMEADDR, 0x8F) << 8) | (uint16_t)twi_read(BMEADDR, 0x8E);
  p[1] = ((uint16_t)twi_read(BMEADDR, 0x91) << 8) | (uint16_t)twi_read(BMEADDR, 0x90);
  p[2] = ((uint16_t)twi_read(BMEADDR, 0x93) << 8) | (uint16_t)twi_read(BMEADDR, 0x92);
  p[3] = ((uint16_t)twi_read(BMEADDR, 0x95) << 8) | (uint16_t)twi_read(BMEADDR, 0x94);
  p[4] = ((uint16_t)twi_read(BMEADDR, 0x97) << 8) | (uint16_t)twi_read(BMEADDR, 0x96);
  p[5] = ((uint16_t)twi_read(BMEADDR, 0x99) << 8) | (uint16_t)twi_read(BMEADDR, 0x98);
  p[6] = ((uint16_t)twi_read(BMEADDR, 0x9B) << 8) | (uint16_t)twi_read(BMEADDR, 0x9A);
  p[7] = ((uint16_t)twi_read(BMEADDR, 0x9D) << 8) | (uint16_t)twi_read(BMEADDR, 0x9C);
  p[8] = ((uint16_t)twi_read(BMEADDR, 0x9F) << 8) | (uint16_t)twi_read(BMEADDR, 0x9E);

  h[0]  = (uint16_t)twi_read(BMEADDR, 0xA1);
  h[1]  = (uint16_t)twi_read(BMEADDR, 0xE2)<<8;
  h[1] |= (uint16_t)twi_read(BMEADDR, 0xE1);
  h[2]  = (uint16_t)twi_read(BMEADDR, 0xE3);
  h[3]  = (uint16_t)twi_read(BMEADDR, 0xE4)<<4;
  h[3] |= (uint16_t)twi_read(BMEADDR, 0xE5) & 0b00001111;
  h[4]  = (uint16_t)twi_read(BMEADDR, 0xE6)<<4;
  h[4] |= (uint16_t)twi_read(BMEADDR, 0xE5)>>4;
  h[5]  = (uint16_t)twi_read(BMEADDR, 0xE7)>>4;
}

uint8_t bme_get_id()
{
  return twi_read(BMEADDR, ID); 
}

void bme_force_temp()
{
  twi_write(BMEADDR, CTRL_MEAS, 0b00100001);
}

void bme_force_pres()
{
  twi_write(BMEADDR, CTRL_MEAS, 0b00000101);
}

void bme_force_hum()
{
  twi_write(BMEADDR, CTRL_HUM, 0b00000001);
  twi_write(BMEADDR, CTRL_MEAS, 0b00000001);
}

