/*****************************************************************************
 * File: ntc.h
 * Author: DigitalLivesock
 * Date: 2026-02-07
 * Description: NTC temperature sensor related definintions and methods.
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

#ifndef NTC_H
#define NTC_H

#include <avr/io.h>
#include <Arduino.h>
#include <stdint.h>
#include <math.h>

/* Fixed resistor value: 10kOhm */
static const float series_resistor = 10000.0;

/* Steinhart-Hart coefficients found: */
static const float A = 87.504;
static const float B = -31.518;
static const float C = 1.807;

/* Pin connected to the voltage divider: */
static uint8_t ntc_analog_pin;

/* Private: */
static float ntc_get_resistance(const int adc_value);
static float ntc_calculate_temperature(float resistance);

/* Public: */
void ntc_init(const uint8_t pin);
float ntc_get_temperature_c(void);

#endif