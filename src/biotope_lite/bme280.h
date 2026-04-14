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

#ifndef BME280_H
#define BME280_H

#include <stdint.h>
#include <Arduino.h>
#include "twi.h"

/* BME280 registers: */
#define HUM_LSB     0xFE
#define HUM_MSB     0xFD
#define TEMP_XLSB   0xFC
#define TEMP_LSB    0xFB
#define TEMP_MSB    0xFA
#define PRESS_XLSB  0xF9
#define PRESS_LSB   0xF8
#define PRESS_MSB   0xF7
#define CONFIG      0xF5
#define CTRL_MEAS   0xF4
#define STATUS      0xF3
#define CTRL_HUM    0xF2
#define RESET       0xE0
#define ID          0xD0

#define BMEADDR 0x76

static uint16_t t_calibration[6];
static uint16_t p_calibration[18];
static uint16_t h_calibration[6];
static int32_t t_fine;

/* BME280 related functions: */
void bme_init();
void bme_cmd(uint8_t addr, uint8_t reg, uint8_t cmd, bool rs);
void bme_get_calibration(uint16_t *t, uint16_t *p, uint16_t *h);
uint8_t bme_get_id();

void bme_force_temp();
float bme_get_temp();

void bme_force_pres();
float bme_get_pres();

void bme_force_hum();
float bme_get_hum();

#endif